declare enum DriveType {
	UNKNOWN,
	HARDDRIVE,
	ROM,
	REMOVABLE,
	NETWORK
}

declare enum ServiceStatus {
    DEFAULT,
    STOPPED,
    IS_STARTING,
    IS_STOPPING,
    STARTED
}

declare enum NetShareType {
    DISKTREE,
	PRINTQ,
	DEVICE,
	IPC
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

interface Service {
    name: string,
    displayName: string,
    status: ServiceStatus
}

interface Version {
    major: number
    minor: number
    patch: number
    build: number
}

interface Conflict {
    name: string
    sourceSize: number,
    sourceTime: Date,
    sourceVersion?: Version
    targetSize: number,
    targetTime: Date,
    targetVersion?: Version
}

interface NetShare {
    name: string,
    description: string
    type: NetShareType
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
    function createDirectory(path: string): Promise<any>
    function rename(path: string, name: string, newName: string): Promise<any>
    function deleteFiles(files: string[]): Promise<any>
    function getConflicts(sourcePath: string, targetPath: string, items: string[]): Promise<Conflict[]>
    function copyFiles(sourcePath: string, targetPath: string, files: string[], conflictFiles: string[]): Promise<void>
    function moveFiles(sourcePath: string, targetPath: string, files: string[], conflictFiles: string[]): Promise<void>
    function existsFile(path: string): boolean
    function getServices(): Promise<Service[]>
    function registerServiceEvents(callback: (changeServices: Service[]) => void): number
    function unregisterServiceEvents(handle: number): void
    function startService(name: string): void
    function stopService(name: string): void
    function startElevated(name: string): void
    function getNetShares(server: string): Promise<NetShare[]>
    function getTest(): number
}