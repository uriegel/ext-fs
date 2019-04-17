#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <map>

enum class Exif_tag {
	// IFD0 items
	ImageWidth = 0x100,
	ImageLength = 0x101,
	BitsPerSample = 0x102,
	Compression = 0x103,
	PhotometricInterpretation = 0x106,
	ImageDescription = 0x10E,
	Make = 0x10F,
	Model = 0x110,
	StripOffsets = 0x111,
	Orientation = 0x112,
	SamplesPerPixel = 0x115,
	RowsPerStrip = 0x116,
	StripByteCounts = 0x117,
	XResolution = 0x11A,
	YResolution = 0x11B,
	PlanarConfiguration = 0x11C,
	ResolutionUnit = 0x128,
	TransferFunction = 0x12D,
	Software = 0x131,
	DateTime = 0x132,
	Artist = 0x13B,
	WhitePoint = 0x13E,
	PrimaryChromaticities = 0x13F,
	JPEGInterchangeFormat = 0x201,
	JPEGInterchangeFormatLength = 0x202,
	YCbCrCoefficients = 0x211,
	YCbCrSubSampling = 0x212,
	YCbCrPositioning = 0x213,
	ReferenceBlackWhite = 0x214,
	Copyright = 0x8298,

	// SubIFD items
	ExposureTime = 0x829A,
	FNumber = 0x829D,
	SubIFD = 0x8769,
	ExposureProgram = 0x8822,
	SpectralSensitivity = 0x8824,
	GpsIFD = 0x8825,
	ISOSpeedRatings = 0x8827,
	OECF = 0x8828,
	ExifVersion = 0x9000,
	DateTimeOriginal = 0x9003,
	DateTimeDigitized = 0x9004,
	ComponentsConfiguration = 0x9101,
	CompressedBitsPerPixel = 0x9102,
	ShutterSpeedValue = 0x9201,
	ApertureValue = 0x9202,
	BrightnessValue = 0x9203,
	ExposureBiasValue = 0x9204,
	MaxApertureValue = 0x9205,
	SubjectDistance = 0x9206,
	MeteringMode = 0x9207,
	LightSource = 0x9208,
	Flash = 0x9209,
	FocalLength = 0x920A,
	SubjectArea = 0x9214,
	MakerNote = 0x927C,
	UserComment = 0x9286,
	SubsecTime = 0x9290,
	SubsecTimeOriginal = 0x9291,
	SubsecTimeDigitized = 0x9292,
	FlashpixVersion = 0xA000,
	ColorSpace = 0xA001,
	PixelXDimension = 0xA002,
	PixelYDimension = 0xA003,
	RelatedSoundFile = 0xA004,
	FlashEnergy = 0xA20B,
	SpatialFrequencyResponse = 0xA20C,
	FocalPlaneXResolution = 0xA20E,
	FocalPlaneYResolution = 0xA20F,
	FocalPlaneResolutionUnit = 0xA210,
	SubjectLocation = 0xA214,
	ExposureIndex = 0xA215,
	SensingMethod = 0xA217,
	FileSource = 0xA300,
	SceneType = 0xA301,
	CFAPattern = 0xA302,
	CustomRendered = 0xA401,
	ExposureMode = 0xA402,
	WhiteBalance = 0xA403,
	DigitalZoomRatio = 0xA404,
	FocalLengthIn35mmFilm = 0xA405,
	SceneCaptureType = 0xA406,
	GainControl = 0xA407,
	Contrast = 0xA408,
	Saturation = 0xA409,
	Sharpness = 0xA40A,
	DeviceSettingDescription = 0xA40B,
	SubjectDistanceRange = 0xA40C,
	ImageUniqueID = 0xA420,

	// GPS subifd items
	GPSVersionID = 0x0,
	GPSLatitudeRef = 0x1,
	GPSLatitude = 0x2,
	GPSLongitudeRef = 0x3,
	GPSLongitude = 0x4,
	GPSAltitudeRef = 0x5,
	GPSAltitude = 0x6,
	GPSTimeStamp = 0x7,
	GPSSatellites = 0x8,
	GPSStatus = 0x9,
	GPSMeasureMode = 0xA,
	GPSDOP = 0xB,
	GPSSpeedRef = 0xC,
	GPSSpeed = 0xD,
	GPSTrackRef = 0xE,
	GPSTrack = 0xF,
	GPSImgDirectionRef = 0x10,
	GPSImgDirection = 0x11,
	GPSMapDatum = 0x12,
	GPSDestLatitudeRef = 0x13,
	GPSDestLatitude = 0x14,
	GPSDestLongitudeRef = 0x15,
	GPSDestLongitude = 0x16,
	GPSDestBearingRef = 0x17,
	GPSDestBearing = 0x18,
	GPSDestDistanceRef = 0x19,
	GPSDestDistance = 0x1A,
	GPSProcessingMethod = 0x1B,
	GPSAreaInformation = 0x1C,
	GPSDateStamp = 0x1D,
	GPSDifferential = 0x1E
};

class Exif_reader
{
public:
	Exif_reader(const std::wstring& path)
	: exif_stream(path, std::ofstream::binary) { }

	bool initialize();

	std::tuple<bool, int> get_tag_int(Exif_tag tag);
	std::tuple<bool, std::string> get_tag_string(Exif_tag tag) {
		return get_tag_bytes(tag);
	}
private:
	bool read_to_exif_start();
	void catalogue_ifd();

	uint8_t read_byte();
	uint16_t read_ushort();
	uint32_t read_uint();
	uint32_t to_int(std::string data);
	uint16_t to_ushort(std::string data);
	std::string read_string(int length);
	std::string read_string(uint16_t tiff_offset, int length);
	std::tuple<bool, std::string> get_tag_bytes(Exif_tag tag);

	std::ifstream exif_stream;
	std::map<Exif_tag, int64_t> catalogue;
	int tiff_header_start{ 0 };
	bool is_little_endian{ false };
};

