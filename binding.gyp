{
  "variables": {
    "use_libpng": '<!(pkg-config --exists libpng || echo $?)',
  },
  "targets": [
    {
      "target_name": "spritify",
      "sources": [ "src/spritify.cpp", "src/sprite_job.cpp", "src/imagebox.cpp", "src/layout.cpp", "src/box.cpp" ],
      "conditions": [
          [ 'OS=="linux" or OS=="freebsd" or OS=="openbsd" or OS=="solaris"', {
            'cflags_cc!': [ '-fexceptions' ],
            "libraries": [ "-lpng" ]
          }],
          ['OS=="mac"', {
            'xcode_settings': {
              'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
            },

          }],
          ["use_libpng==''", {
            "defines": ["cimg_use_png" ],
            "libraries": ["<!(pkg-config --libs libpng)"]
          }]
        ],
    }
  ]
}