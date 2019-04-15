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
    displayName: string
    size: number
    time: Date
    isDirectory: boolean
    isHidden: boolean
}

declare module 'extension-fs' {
    function getDrives(): Promise<DriveItem[]>
    function getFiles(path: string): Promise<FileItem[]>
    function getIcon(ext: string): Promise<Buffer>
}