# Copyright (c) 2013 Kazumasa Kohtaka. All rights reserved.
# This file is available under the MIT license.

{
  'includes': [
    '/home/kkohtaka/repos/dart-repo/dart/runtime/tools/gyp/runtime-configurations.gypi',
  ],
  'targets': [
    {
      'target_name': 'dart_gtk',
      'type': 'shared_library',
      'include_dirs': [
        '/home/kkohtaka/repos/dart-repo/dart/runtime',
      ],
      'sources': [
        'dart_gtk.cc',
      ],
      'defines': [
        'DART_SHARED_LIB',
      ],
      'conditions': [
        ['OS=="linux"', {
          'cflags': [
            '-Werror',
            '-Wall',
            '-O2',
            '-rdynamic',
            '-fPIC',
            '-std=c++11',
            '<!(pkg-config --cflags gtk+-3.0)',
          ],
          'libraries': [
            '<!(pkg-config --libs gtk+-3.0)',
          ],
          'ldflags': [
            '-shared',
          ]
        }],
      ],
    },
  ],
}

