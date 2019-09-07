const addon = require('bindings')('extension-fs')

let exists = addon.existsFile("A:\\Bilder\\2017\\Abu Dabbab\\M0015912.JPG")
exists = addon.existsFile("A:\\Bilder\\2017\\Abu Dabbab\\nichda.jpg")

try {
    addon.startService("AxInstSV")
}
catch (err) {
    console.error("Fehler", err)
}

// const services = new Services(n => {
//     console.log("Event", n)
// })
const items = addon.getServices()
console.log(items)

const id = addon.registerServiceEvents(n => {
    console.log("Event", n)
})

setTimeout(() => addon.unregisterServiceEvents(id), 6000)

return

const run = async () => {

    
    addon.copyFiles("C:\\Users\\uwe.CASERIS\\Pictures", "h:\\",
    [ "Bild02.jpg" ], [])

    addon.copyFiles("C:\\Users\\uwe.CASERIS\\Pictures", "h:\\weck",
    [ "Bild02.jpg" ], [])

    const result = await addon.getConflicts("C:\\Users\\urieg\\Sources\\test\\test1", "C:\\Users\\urieg\\Sources\\test\\test2", ["2019", "dartdoc_options.yaml", "wabmig.exe", "flutter_console.bat"])

    // const f1 = await addon.getExifDate("A:\\Bilder\\2017\\Abu Dabbab\\M0015912.JPG")
    // const f2 = await addon.getExifDate("A:\\Bilder\\1968-1969\\GummiReiff.jpg")
    // const f3 = await addon.getExifDate("C:\\Users\\urieg\\Desktop\\Affe\\1968.jpg")
    
    // TODO: Version 2.0.0
    // TODO copyFiles(sourcePath, targetPaths, items, exceptions)

    // addon.copyFiles("C:\\Users\\urieg\\Sources\\test\\Test1", "C:\\Users\\urieg\\Sources\\test\\Test2",
    //     [ "Sub" ], [ "Sub\\SubSub\\Text1.txt", "Sub\\SubSub\\Text2.txt", "Sub\\text.txt" ])
    //     //[ "Sub" ], [ "Sub\\SubSub\\Text2.txt" ])
    //     //[ "Sub" ], undefined)
    // //(copyItems.items, copyItems.targetPath, false)
    

    addon.copyFiles("C:\\Users\\urieg\\Sources\\test\\Test1\\Sub", "C:\\Users\\urieg\\Sources\\test\\Test3\\Sub",
    [ "Text.txt", "SubSub" ], ["text.txt", "SubSub\\Text1.txt", "SubSub\\Text2.txt"])
    //[ "Sub" ], [ "Sub\\SubSub\\Text2.txt" ])
    //[ "Sub" ], undefined)


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
    

    let start = process.hrtime.bigint()
    for (let i = 0; i < 1000; i++) {
        let exifDatum = await addon.getExifDate("A:\\Bilder\\2015\\Canon\\IMG_8420.JPG")
    }
    let end = process.hrtime.bigint()
    console.info(`Execution time getExifDate (hr): ${((hrend - hrstart) / BigInt(1000000.0))} ms`)

    let exifDatum = await addon.getExifDate("A:\\Bilder\\2015\\Canon\\IMG_8420.JPG")
    if (exifDatum)
        console.log(exifDatum)

    start = process.hrtime.bigint()
    drives = await addon.getDrives() 
    end = process.hrtime.bigint()
    console.info(`Execution time drives (hr): ${((hrend - hrstart) / BigInt(1000000.0))} ms`)
    console.log(drives)

    start = process.hrtime.bigint()
    let files = await addon.getFiles("d:/Test😎😎😎")
    end = process.hrtime.bigint()
    console.info(`Execution time get files d:/test: ${((hrend - hrstart) / BigInt(1000000.0))} ms`)
    
    start = process.hrtime.bigint()
    files = await addon.getFiles("c:/windows/system32")
    end = process.hrtime.bigint()
    console.info(`Execution time files c:/windows/system32: ${((hrend - hrstart) / BigInt(1000000.0))} ms`)

    start = process.hrtime.bigint()
    let icon = await addon.getIcon(".exe")
    end = process.hrtime.bigint()
    console.info(`Execution time geticon .exe: ${((hrend - hrstart) / BigInt(1000000.0))} ms`)

    start = process.hrtime.bigint()
    icon = await addon.getIcon(".dll")
    end = process.hrtime.bigint()
    console.info(`Execution time geticon .dll: ${((hrend - hrstart) / BigInt(1000000.0))} ms`)

    console.log("Finished")

    start = process.hrtime.bigint()
    let version = await addon.getFileVersion(`C:\\Program Files\\internet explorer\\iexplore.exe`)
    end = process.hrtime.bigint()
    console.info(`Execution getFileVersion: ${((hrend - hrstart) / BigInt(1000000.0))} ms`)

    start = process.hrtime.bigint()
    files = await addon.getFiles("c:/windows/system32")
    end = process.hrtime.bigint()
    console.info(`Execution c:/windows/system32: ${((hrend - hrstart) / BigInt(1000000.0))} ms`)

    start = process.hrtime.bigint()
    version = await addon.getFileVersion(`C:\\Program Files\\internet explorer\\iexplore.exe`)
    end = process.hrtime.bigint()
    console.info(`Execution getFileVersion: ${((hrend - hrstart) / BigInt(1000000.0))} ms`)

    start = process.hrtime.bigint()
    files.forEach(async n => {
        const version = await addon.getFileVersion("c:\\windows\\system32\\" + n.name)
    })
    end = process.hrtime.bigint()
    console.info(`Execution time version iteration: ${((hrend - hrstart) / BigInt(1000000.0))} ms`)
}
run()

// addon.showInfo("C:\\Program Files\\nodejs\\node.exe")
// addon.open("C:\\Windows\\notepad.exe")
// addon.open("C:\\Windows\\system.ini")
// addon.openAs("C:\\Windows\\system.ini")

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




