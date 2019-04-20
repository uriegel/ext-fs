//const addon = require('./build/Debug/extension-fs')
const addonchen = require('./build/Release/extension-fs')

const work = s => new Promise((res, rej) => {
    addonchen.work(s, r => res(r))    
})

module.exports.work = work