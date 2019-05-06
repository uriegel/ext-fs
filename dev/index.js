const addon = require('bindings')('extension-fs')

const run = async () => {


    const f1 = await addon.getExifDate("A:\\Bilder\\2017\\Abu Dabbab\\M0015912.JPG")
    const f2 = await addon.getExifDate("A:\\Bilder\\1968-1969\\GummiReiff.jpg")
    const f3 = await addon.getExifDate("C:\\Users\\urieg\\Desktop\\Affe\\1968.jpg")
    
    

    

    const path = "A:/Bilder/2017/Abu Dabbab"
    let exifFiles = await addon.getFiles(path)
    let exifs  = await exifFiles.map(async n => {
        const item = path + '/' + n.name
        
    })
    




    try {
        await addon.createDirectory("d:\\test\\affe")
        await addon.createDirectory("d:\\test\\affe2")
    } catch (err) {
         console.log("Ging nich", err)
    }


    try {
        await addon.deleteFiles(["d:\\test\\affe", "d:\\test\\affe2"])
    } catch (err) {
         console.log("Ging nich", err)
    }

    try {
        await addon.deleteFiles(["c:\\windows\\affe"])
    } catch (err) {
         console.log("Ging nich", err)
    }

    try {
        await addon.rename("C:\\Users\\urieg\\Desktop", "name.txt", "neuerName.txt")
    } catch (err) {
         console.log("Ging nich", err)
    }
    

    let hrstart = process.hrtime()
    for (let i = 0; i < 1000; i++) {
        let exifDatum = await addon.getExifDate("A:\\Bilder\\2015\\Canon\\IMG_8420.JPG")
    }
    let diff = process.hrtime(hrstart)
    console.info(`Execution time getExifDate (hr): ${(diff[1] / 1000000.0)}`)

    let exifDatum = await addon.getExifDate("A:\\Bilder\\2015\\Canon\\IMG_8420.JPG")
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
    console.info(`Execution time get files d:/test: ${(diff[1] / 1000000.0)}`)
    
    hrstart = process.hrtime()
    files = await addon.getFiles("c:/windows/system32")
    diff = process.hrtime(hrstart)
    console.info(`Execution time files c:/windows/system32: ${(diff[1] / 1000000.0)}`)

    hrstart = process.hrtime()
    let icon = await addon.getIcon(".exe")
    diff = process.hrtime(hrstart)
    console.info(`Execution time geticon .exe (hr): ${(diff[1] / 1000000.0)}`)

    console.log("Finished")

    hrstart = process.hrtime()
    let version = await addon.getFileVersion(`C:\\Program Files\\internet explorer\\iexplore.exe`)
    diff = process.hrtime(hrstart)
    console.info(`Execution getFileVersion: ${(diff[1] / 1000000.0)}`)

    hrstart = process.hrtime()
    files = await addon.getFiles("c:/windows/system32")
    diff = process.hrtime(hrstart)
    console.info(`Execution time files c:/windows/system32 (hr): ${(diff[1] / 1000000.0)}`)

    hrstart = process.hrtime()
    version = await addon.getFileVersion(`C:\\Program Files\\internet explorer\\iexplore.exe`)
    diff = process.hrtime(hrstart)
    console.info(`Execution getFileVersion: ${(diff[1] / 1000000.0)}`)

    hrstart = process.hrtime()
    files.forEach(async n => {
        const version = await addon.getFileVersion("c:\\windows\\system32\\" + n.name)
    })
    diff = process.hrtime(hrstart)
    console.info(`Execution time version iteration: ${(diff[1] / 1000000.0)}`)
}
run()

addon.showInfo("C:\\Program Files\\nodejs\\node.exe")
addon.open("C:\\Windows\\notepad.exe")
addon.open("C:\\Windows\\system.ini")
addon.openAs("C:\\Windows\\system.ini")

let i = 1

const renn = async  () => {
    let files = await addon.getFiles("d:/Test😎😎😎")
    console.log(i++)
}

// renn()
// renn()
// renn()
// renn()
// renn()
// renn()
// renn()
// renn()
// renn()
// renn()
// renn()
// renn()
// renn()
// renn()
// renn()
// renn()




