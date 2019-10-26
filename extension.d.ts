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

interface FileItem {
    name: string
    size: number
    time: Date
    isDirectory: boolean
    isHidden: boolean
}

interface DriveItem {
    name: string
    description: string
    size: number
    type: DriveType
    isMounted: boolean
}

interface Version
{
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

interface Service {
    name: string,
    displayName: string,
    status: ServiceStatus
}

interface NetShare {
    name: string,
    description: string
    type: NetShareType
}

declare module 'extension-fs' {
    function getFiles(path: string): Promise<FileItem[]>
    function getDrives(): Promise<DriveItem[]>
    function getIcon(extension: string): Promise<Buffer>
    function getExifDate(path: string): Promise<Date|null>
    function getConflicts(sourcePath: string, targetPath: string, items: string[]): Promise<Conflict[]>
    function getFileVersion(path: string): Promise<Version>
    function showInfo(path: string): void
    function open(path: string): void
    function openAs(path: string): void
    function createDirectory(path: string): Promise<void>
    function rename(path: string, name: string, newName: string): Promise<void>
    function deleteFiles(pathes: string[]): Promise<void>
    function copyFiles(sourcePath: string, targetPAth: string, files: string[]): Promise<void>
    function moveFiles(sourcePath: string, targetPAth: string, files: string[]): Promise<void>
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
