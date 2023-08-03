# libscanmem-PINCE

scanmem is a debugging utility designed to isolate the address of an arbitrary
variable in an executing process. scanmem simply needs to be told the pid of
the process and the value of the variable at several different times.

After several scans of the process, scanmem isolates the position of the
variable and allows you to modify its value.

This fork is to strip anything that's not libscanmem and add features that are needed by PINCE:
https://github.com/korcankaraokcu/PINCE

## Requirements

scanmem requires `/proc` to be mounted.

## Documentation

Code is documentation. This fork is only to be used for developing PINCE.

## Build Requirements

The build requires autotools-dev, libtool and python.

## Build and Install

To generate files required for the build:

    ./autogen.sh

To build:

    ./configure && make

Run `./configure --help` for more details.

## Licence: 

LGPLv3 for libscanmem
