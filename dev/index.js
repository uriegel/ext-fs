var addon = require('../build/Debug/extension-fs')
// var addon = require('../build/Release/extension-fs')

const run = async function() {


    let items = await addon.getFiles("A:\\Aff👌")
    await addon.addExtendedInfos("A:\\Aff👌", items)

    let hrstart = process.hrtime()
    for (let i = 0; i < 1000; i++) {
        let exifDatum = await addon.getExifDate("A:\\Bilder\\2015\\Canon\\IMG_8420.JPG")
    }
    let diff = process.hrtime(hrstart)
    console.info(`Execution time drives (hr): ${(diff[1] / 1000000.0)}`)

    let exifDatum = await addon.getExifDate("A:\\Bilder\\2015\\Canon\\IMG_8420gg.JPG")
    if (exifDatum)
        console.log(exifDatum)

    hrstart = process.hrtime()
    drives = await addon.getDrives() 
    diff = process.hrtime(hrstart)
    console.info(`Execution time drives (hr): ${(diff[1] / 1000000.0)}`)
    console.log(drives)



    hrstart = process.hrtime()
    let files = await addon.getFiles("d:/Test😎😎😎")
    diff = process.hrtime(hrstart)
    console.info(`Execution time drives (hr): ${(diff[1] / 1000000.0)}`)
    
    hrstart = process.hrtime()
    files = await addon.getFiles("c:/windows/system32")
    diff = process.hrtime(hrstart)
    console.info(`Execution time files (hr): ${(diff[1] / 1000000.0)}`)

    hrstart = process.hrtime()
    let icon = await addon.getIcon(".exe")
    diff = process.hrtime(hrstart)
    console.info(`Execution time files (hr): ${(diff[1] / 1000000.0)}`)

    console.log("Finished")

    hrstart = process.hrtime()
    let version = await addon.getFileVersion(`C:\\Program Files\\internet explorer\\iexplore.exe`)
    diff = process.hrtime(hrstart)
    console.info(`Execution time version: ${(diff[1] / 1000000.0)}`)

    hrstart = process.hrtime()
    files = await addon.getFiles("c:/windows/system32")
    diff = process.hrtime(hrstart)
    console.info(`Execution time files 2 (hr): ${(diff[1] / 1000000.0)}`)

    hrstart = process.hrtime()
    version = await addon.getFileVersion(`C:\\Program Files\\internet explorer\\iexplore.exe`)
    diff = process.hrtime(hrstart)
    console.info(`Execution time version 2: ${(diff[1] / 1000000.0)}`)

    // hrstart = process.hrtime()
    // files.forEach(async n => {
    //     const version = await addon.getFileVersion("c:\\windows\\system32\\" + n.name)
    // })
    // diff = process.hrtime(hrstart)
    // console.info(`Execution time version iteration: ${(diff[1] / 1000000.0)}`)

}
run()


