{
    "targets": [{
            "target_name": "extension-fs",
            "sources": [ 
                './addon.cpp',
                './windows/utils.cpp',
                "./Get_files_worker.cpp"
            ],
            "cflags": ["-Wall", "-std=c++14"],
            "include_dirs" : [
                "<!(node -e \"require('nan')\")"
            ],
        "conditions": [
            ['OS=="win"', {
                'defines': ['WINDOWS']   
                }
            ],
            ['OS=="linux"', {
                'defines': ['LINUX'],
                'sources!': [
                    './windows/win.cpp'  
                ]}
            ],
        ]          
    }]
}

