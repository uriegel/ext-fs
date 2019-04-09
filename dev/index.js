var addon = require('../build/Debug/extension-fs')

const run = async function() {
    //let files = await addon.getFiles("c:/windows/system32")
    let files = await addon.getFiles("d:/Test😎😎😎")
    
    const hrstart = process.hrtime()
    files = await addon.getFiles("c:/windows/system32")
    const diff = process.hrtime(hrstart)
    console.info(`Execution time files (hr): ${(diff[1] / 1000000.0)}`)

    const hrstart1 = process.hrtime()
    let icon = await addon.getIcon(".exe")
    const diff1 = process.hrtime(hrstart1)
    console.info(`Execution time files (hr): ${(diff1[1] / 1000000.0)}`)

    console.log("Finished")
}
run()

