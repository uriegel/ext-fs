#include "exif_reader.h"
#include <sstream>
#include <iomanip>
#include <chrono>
#include <algorithm>
using namespace std;

uint64_t get_exif_date(const wstring& file) {
    Exif_reader er(file);
	auto res = er.initialize();
	if (!res)
        return 0;
	bool success;
	string result;
	tie(success, result) = er.get_tag_string(Exif_tag::DateTimeOriginal);
    if (!success) 
        tie(success, result) = er.get_tag_string(Exif_tag::DateTime);
    if (!success)
        return 0;

	tm tm = {};
	stringstream ss(result.c_str());
	ss >> get_time(&tm, "%Y:%m:%d %H:%M:%S");
	auto time = mktime(&tm);

	auto secs = static_cast<chrono::seconds>(time).count();
	auto unix_time = static_cast<uint64_t>(secs)  * 1000;
    return unix_time;
}

char get_tiff_field_length(uint16_t tiff_data_type);

static uint16_t swap16(uint16_t x) {
	return (uint16_t)((((x) >> 8) & 0xff) | (((x) & 0xff) << 8));
}

static uint32_t swap32(uint32_t x) {
	return (((x) & 0xff000000u) >> 24) | (((x) & 0x00ff0000u) >> 8) | (((x) & 0x0000ff00u) << 8) | (((x) & 0x000000ffu) << 24);
}

bool Exif_reader::initialize()
{
	if (!exif_stream)
		return false;
	if (read_ushort() != 0xFFD8)
		//throw new Exception("File is not a valid JPEG");
		return false;
	if (!read_to_exif_start())
		return false;

	auto sizeOfExifData = read_ushort();

	auto exif = read_string(4);
	if (exif != "Exif")
		return false;

	if (read_ushort() != 0)
		//throw new Exception("Malformed Exif data");
		return false;

	tiff_header_start = static_cast<int>(exif_stream.tellg());

	// II for Intel, MM for Motorola
	auto ii = read_string(2);
	is_little_endian = ii == "II";

	if (read_ushort() != 0x002A)
		//throw new Exception("Error in TIFF data");
		return false;
	
	// offset image file directory
	auto ifd_offset = read_uint();

	exif_stream.seekg(ifd_offset + tiff_header_start);
	
	catalogue_ifd();

	bool success;
	int offset;
	tie(success, offset) = get_tag_int(Exif_tag::SubIFD);
	if (!success)
		// throw new Exception("Unable to locate Exif data");
		return false;

	exif_stream.seekg(offset + tiff_header_start);
	catalogue_ifd();

	tie(success, offset) = get_tag_int(Exif_tag::GpsIFD);
	if (success) {
		exif_stream.seekg(offset + tiff_header_start);
		catalogue_ifd();
	}
	
	return true;
}

bool Exif_reader::read_to_exif_start() {
	uint8_t marker_start;
	uint8_t marker_number{ 0 };

	while (((marker_start = read_byte()) == 0xFF) && (marker_number = read_byte()) != 0xE1)	{
		auto data_length = read_ushort();
		exif_stream.seekg(data_length - 2, istream::cur);
	}

	if (marker_start != 0xFF || marker_number != 0xE1)
		//throw new Exception("Could not find Exif data block");
		return false;
	return true;
}

void Exif_reader::catalogue_ifd() {
	auto count = read_ushort();

	for (auto current_entry = 0; current_entry < count; current_entry++) {
		auto current_tag_number = static_cast<Exif_tag>(read_ushort());

		catalogue[current_tag_number] = static_cast<int>(exif_stream.tellg()) - 2;
		exif_stream.seekg(10, ios_base::cur);
	}
}

uint8_t Exif_reader::read_byte() {
	char ret{ 0 };
	exif_stream.read(&ret, 1);
	return static_cast<uint8_t>(ret);
}

uint16_t Exif_reader::read_ushort() {
	uint16_t ret{ 0 };
	exif_stream.read(reinterpret_cast<char*>(&ret), 2);
	return is_little_endian ? ret : swap16(ret);
}

uint32_t Exif_reader::read_uint() {
	uint32_t  ret{ 0 };
	exif_stream.read(reinterpret_cast<char*>(&ret), 4);
	return is_little_endian ? ret : swap32(ret);
}

string Exif_reader::read_string(int length) {
	vector<char> bytes(length);
	exif_stream.read(bytes.data(), bytes.size());
	return string(bytes.data(), bytes.size());
}

tuple<bool, int> Exif_reader::get_tag_int(Exif_tag tag) {
	bool success;
	string result;
	std::tie(success, result) = get_tag_bytes(tag);
	if (!success)
		return make_tuple(false, 0);
	return make_tuple(true, to_int(result));
}

tuple<bool, string> Exif_reader::get_tag_bytes(Exif_tag tag) {
	if (catalogue.find(tag) == catalogue.end())
		return make_tuple(false, "");

	int offset = static_cast<int>(catalogue[tag]);
	exif_stream.seekg(offset);

	auto current_tag_id = static_cast<Exif_tag>(read_ushort());
	if (current_tag_id != tag)
		throw "Tag number not at expected offset";

	auto tiff_data_type = read_ushort();
	auto number_of_components = read_uint();
	auto tag_data = read_string(4);

	auto data_size = (int)(number_of_components * get_tiff_field_length(tiff_data_type));
	if (data_size > 4) {
		auto offset_address = to_ushort(tag_data);
		return make_tuple(true, read_string(offset_address, data_size));
	}
	return make_tuple(true, move(tag_data));
}

string Exif_reader::read_string(uint16_t tiff_offset, int length) {
	auto originalOffset = static_cast<int>(exif_stream.tellg());

	exif_stream.seekg(tiff_offset + tiff_header_start);
	auto result = read_string(length);

	exif_stream.seekg(originalOffset);

	return result;
}


uint32_t Exif_reader::to_int(string data) {
	if (!is_little_endian)
		reverse(data.begin(), data.end());

	return static_cast<uint32_t>(*(reinterpret_cast<const uint32_t*>(data.c_str())));
}

uint16_t Exif_reader::to_ushort(std::string data) {
	if (!is_little_endian)
		reverse(data.begin(), data.end());

	return static_cast<uint16_t>(*(reinterpret_cast<const uint16_t*>(data.c_str())));
}

char get_tiff_field_length(uint16_t tiff_data_type)
{
	switch (tiff_data_type)
	{
	case 1:
	case 2:
	case 6:
		return 1;
	case 3:
	case 8:
		return 2;
	case 4:
	case 7:
	case 9:
	case 11:
		return 4;
	case 5:
	case 10:
	case 12:
		return 8;
	default:
		throw "Unknown TIFF datatype";
	}
}

