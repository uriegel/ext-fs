var addon = require('../build/Debug/extension-fs')

const run = async function() {
    let files = await addon.getFiles("c:/windows/system32")
    files = await addon.getFiles("d:/Test😎😎😎")
    
    console.log("Finished")
}
run()

