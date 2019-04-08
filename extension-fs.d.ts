interface FileItem {
    displayName: string
    size: number
    time: Date
    isDirectory: boolean
    isHidden: boolean
}

declare module 'extension-fs' {
    function getFiles(path: string): Promise<FileItem[]>
}