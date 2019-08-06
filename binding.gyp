{
    "targets": [{
        "target_name": "extension-fs",
        "sources": [ 
            'addon.cpp',
            'get_files_worker.cpp',
            'get_drives_worker.cpp',
            'get_icon_worker.cpp',
            'get_exif_date_worker.cpp',
            'get_file_version_worker.cpp',
            'create_directory_worker.cpp',
            'get_conflicts_worker.cpp',
            'rename_worker.cpp',
            'delete_files_worker.cpp',
            'copy_worker.cpp',
            'exif_reader.cpp',
            'std_utils.cpp',
            'nullfunction.cpp',
            'windows/utils.cpp',
            'windows/get_icon.cpp',
            'windows/shell.cpp',
            'windows/get_conflicts.cpp',
        ],
        'include_dirs': ["<!@(node -p \"require('node-addon-api').include\")"],
        'dependencies': ["<!(node -p \"require('node-addon-api').gyp\")"],
        "cflags": ["-Wall", "-std=c++17"],
        'cflags!': [ '-fno-exceptions' ],
        'cflags_cc!': [ '-fno-exceptions' ],
        'link_settings': {
            "libraries": [ 
                "gdiplus.lib",
                "Mincore.lib"
            ]
        },            
        'conditions': [
            ['OS=="win"', {
                'defines': ['WINDOWS'],
                "msvs_settings": {
                    "VCCLCompilerTool": {
                        "ExceptionHandling": 1
                    }
                }                
            }],
            ['OS=="linux"', {
                'defines': ['LINUX'],
                'sources!': [
                    'get_files_worker.cpp',
                    'get_drives_worker.cpp',
                    'get_icon_worker.cpp',
                    'get_file_version_worker.cpp',
                    'create_directory_worker.cpp',
                    'rename_worker.cpp',
                    'delete_files_worker.cpp',
                    'copy_worker.cpp',
                    'windows/utils.cpp',
                    'windows/get_icon.cpp',
                    'windows/shell.cpp'
                ],
                'libraries!': [ 
                    "gdiplus.lib",
                    "Mincore.lib"
                ]
            }],
        ]          
    }]
}

