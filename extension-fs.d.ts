declare enum DriveType {
	UNKNOWN,
	HARDDRIVE,
	ROM,
	REMOVABLE,
	NETWORK
}

interface DriveItem {
    name: string
    description: string
    size: number
    type: DriveType 
    isMounted: boolean
}

interface FileItem {
    name: string
    size: number
    time: Date
    isDirectory: boolean
    isHidden: boolean
}

interface VersionInfo {
    major: number
    minor: number
    build: number
    patch: number
}

declare module 'extension-fs' {
    function getDrives(): Promise<DriveItem[]>
    function getFiles(path: string): Promise<FileItem[]>
    function getIcon(ext: string): Promise<Buffer>
    function getFileVersion(file: string): Promise<VersionInfo>
    function getExifDate(file: string): Promise<Date>
    function open(path: string): any
    function openAs(path: string): any
    function showInfo(path: string): any
}