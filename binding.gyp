{
    "targets": [{
            "target_name": "extension-fs",
            "sources": [ 
                "addon.cpp",
                "GetFilesWorker.cpp",
                './windows/win.cpp',
            ],
            "cflags": ["-Wall", "-std=c++1z"],
            "include_dirs" : [
                "<!(node -e \"require('nan')\")"
            ],
        "conditions": [
            ['OS=="linux"', {
                    'sources!': [
                        './windows/win.cpp',  
                    ]
            }
            ],
        ]          
    }]
}