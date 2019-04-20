const addon = require('../extension-fs')

const runDowork = async s => console.log(await addon.work(s))

runDowork("1")
runDowork("2")
runDowork("3")
runDowork("4")
runDowork("5")
runDowork("6")
runDowork("7")
runDowork("8")

// // addon.work("1", s => console.log(s))
// // addon.work("2", s => console.log(s))
// // addon.work("3", s => console.log(s))
// // addon.work("4", s => console.log(s))
// // addon.work("5", s => console.log(s))
// // addon.work("6", s => console.log(s))
// // addon.work("7", s => console.log(s))
// // addon.work("8", s => console.log(s))


//const run = async () => {
    // const startRun = async p => console.log(await addon.work1(p))
    // startRun("1")
    // startRun("2")
    // startRun("3")
    // startRun("4")
    // startRun("5")
    // startRun("6")
    // startRun("7")
    // startRun("8")
//}

// const run = async function() {

    

//     let items = await addon.getFiles("A:\\Aff👌")
//     await addon.addExtendedInfos("A:\\Aff👌", items)

//     let hrstart = process.hrtime()
//     for (let i = 0; i < 1000; i++) {
//         let exifDatum = await addon.getExifDate("A:\\Bilder\\2015\\Canon\\IMG_8420.JPG")
//     }
//     let diff = process.hrtime(hrstart)
//     console.info(`Execution time drives (hr): ${(diff[1] / 1000000.0)}`)

//     let exifDatum = await addon.getExifDate("A:\\Bilder\\2015\\Canon\\IMG_8420gg.JPG")
//     if (exifDatum)
//         console.log(exifDatum)

//     hrstart = process.hrtime()
//     drives = await addon.getDrives() 
//     diff = process.hrtime(hrstart)
//     console.info(`Execution time drives (hr): ${(diff[1] / 1000000.0)}`)
//     console.log(drives)



//     hrstart = process.hrtime()
//     let files = await addon.getFiles("d:/Test😎😎😎")
//     diff = process.hrtime(hrstart)
//     console.info(`Execution time drives (hr): ${(diff[1] / 1000000.0)}`)
    
//     hrstart = process.hrtime()
//     files = await addon.getFiles("c:/windows/system32")
//     diff = process.hrtime(hrstart)
//     console.info(`Execution time files (hr): ${(diff[1] / 1000000.0)}`)

//     hrstart = process.hrtime()
//     let icon = await addon.getIcon(".exe")
//     diff = process.hrtime(hrstart)
//     console.info(`Execution time files (hr): ${(diff[1] / 1000000.0)}`)

//     console.log("Finished")

//     hrstart = process.hrtime()
//     let version = await addon.getFileVersion(`C:\\Program Files\\internet explorer\\iexplore.exe`)
//     diff = process.hrtime(hrstart)
//     console.info(`Execution time version: ${(diff[1] / 1000000.0)}`)

//     hrstart = process.hrtime()
//     files = await addon.getFiles("c:/windows/system32")
//     diff = process.hrtime(hrstart)
//     console.info(`Execution time files 2 (hr): ${(diff[1] / 1000000.0)}`)

//     hrstart = process.hrtime()
//     version = await addon.getFileVersion(`C:\\Program Files\\internet explorer\\iexplore.exe`)
//     diff = process.hrtime(hrstart)
//     console.info(`Execution time version 2: ${(diff[1] / 1000000.0)}`)

//     // hrstart = process.hrtime()
//     // files.forEach(async n => {
//     //     const version = await addon.getFileVersion("c:\\windows\\system32\\" + n.name)
//     // })
//     // diff = process.hrtime(hrstart)
//     // console.info(`Execution time version iteration: ${(diff[1] / 1000000.0)}`)

// }
//run()




