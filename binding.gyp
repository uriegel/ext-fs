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
            'get_net_shares_worker.cpp',
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
            'windows/services.cpp',
        ],
        'include_dirs': [
            "<!@(node -p \"require('node-addon-api').include\")",
            "<!@(node -p \"var a = require('node-addon-api').include; var b = a.substr(0, a.length - 15); b + 'event-source-base' + a[a.length-1]\")"
        ],
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
                'defines': [
                    'WINDOWS', 
                    '_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING' 
                ],
                "msvs_settings": {
                    "VCCLCompilerTool": {
                        "ExceptionHandling": 1,
    					'AdditionalOptions': [
						    '-std:c++17',
						]
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

