{
    "targets": [{
        "target_name": "extension-fs",
        "sources": [ 
            './addon.cpp',
            './std_utils.cpp',
            './exif_reader.cpp',
            './windows/utils.cpp',
            './windows/get_icon.cpp',
        ],
        "cflags": ["-Wall", "-std=c++14" ],
        "cflags!": [ "-fno-exceptions" ],
        "cflags_cc!": [ "-fno-exceptions" ],
        'include_dirs': [
            "<!@(node -p \"require('node-addon-api').include\")"
        ],   
        'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
        'dependencies': [
            "<!(node -p \"require('node-addon-api').gyp\")"
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
                    './windows/utils.cpp',
                    './windows/get_icon.cpp'  
                ],
                'libraries!': [ 
                        "gdiplus.lib",
                        "Mincore.lib"
                ]}
            ],
        ]          
    }]
}

