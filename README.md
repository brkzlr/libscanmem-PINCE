# scanmem-PINCE

scanmem is a debugging utility designed to isolate the address of an arbitrary
variable in an executing process. scanmem simply needs to be told the pid of
the process and the value of the variable at several different times.

After several scans of the process, scanmem isolates the position of the
variable and allows you to modify its value.

This fork is to add features that are needed by PINCE:
https://github.com/korcankaraokcu/PINCE

## Requirements

scanmem requires libreadline to read commands interactively, and `/proc` must be
mounted.

## Documentation

To read documentation:
  * `man scanmem`
  * `scanmem --help`
  * enter `help` at the scanmem prompt

## Build Requirements

The build requires autotools-dev, libtool, libreadline-dev and python.

## Build and Install

To generate files required for the build:

    ./autogen.sh

To build:

    ./configure --prefix=/usr && make
    sudo make install

scanmem use static paths to libscanmem. So executing
`ldconfig` is not required. Consider setting `--libdir=/usr/lib/scanmem` or
`--libdir=/usr/lib64/scanmem` to avoid that libscanmem is in a library
search path.

Run `./configure --help` for more details.

## Android Build

You need a
[standalone toolchain of Android NDK](https://developer.android.com/ndk/guides/standalone_toolchain.html#itc)
(Advanced method) to build interactive capabilities for Android.
For more information, run:

    ./build_for_android.sh help

## License: 

GPLv3, LGPLv3 for libscanmem
