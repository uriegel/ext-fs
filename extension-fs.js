//const addon = require('./build/Debug/extension-fs')
const addon = require('./build/Release/extension-fs')

const getDrives = () => new Promise((res, rej) => {
    addon.getDrives(r => res(r))    
})

const getFiles = (path) => new Promise((res, rej) => {
    addon.getFiles(path, r => res(r))    
})

const getIcon = (url) => new Promise((res, rej) => {
    addon.getIcon(url, r => res(r))    
})

const getFileVersion = (file) => new Promise((res, rej) => {
    addon.getFileVersion(file, r => res(r))    
})

const getExifDate = (file) => new Promise((res, rej) => {
    addon.getExifDate(file, r => res(r))    
})

module.exports.getDrives = getDrives
module.exports.getFiles = getFiles
module.exports.getIcon = getIcon
module.exports.getFileVersion = getFileVersion
module.exports.getExifDate = getExifDate
