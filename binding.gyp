{
    "targets": [{
            "target_name": "extension-fs",
            "sources": [ 
                './addon.cpp',
                "./get_drives_worker.cpp",
                "./get_files_worker.cpp",
                "./get_icon_worker.cpp",
                "./get_file_version_worker.cpp",
                './std_utils.cpp',
                './exif_reader.cpp',
                './windows/utils.cpp',
                './windows/get_icon.cpp',
            ],
            "cflags": ["-Wall", "-std=c++14"],
            "include_dirs" : [
                "<!(node -e \"require('nan')\")"
            ],
        'link_settings': {
            "libraries": [ 
                "gdiplus.lib",
                "Mincore.lib"
            ]
        },            
        "conditions": [
            ['OS=="win"', {
                'defines': ['WINDOWS']   
                }
            ],
            ['OS=="linux"', {
                'defines': ['LINUX'],
                'sources!': [
                    './windows/utils.cpp'  
                ],
                'libraries!': [ 
                        "gdiplus.lib",
                        "Mincore.lib"
                ]}
            ],
        ]          
    }]
}

