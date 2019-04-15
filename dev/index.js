//var addon = require('../build/Debug/extension-fs')
var addon = require('../build/Release/extension-fs')

const run = async function() {

    let hrstart = process.hrtime()
    let drives = await addon.getDrives() 
    let diff = process.hrtime(hrstart)
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

    const hrstart1 = process.hrtime()
    let icon = await addon.getIcon(".exe")
    const diff1 = process.hrtime(hrstart1)
    console.info(`Execution time files (hr): ${(diff1[1] / 1000000.0)}`)

    console.log("Finished")
}
run()

