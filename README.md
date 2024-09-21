# libscanmem-PINCE
This is a fork of [scanmem](https://github.com/scanmem/scanmem) mainly for stripping anything that's not library focused (***libscanmem***) and add features that are needed by [PINCE](https://github.com/korcankaraokcu/PINCE).

## Requirements
- `/proc` needs to be mounted.

## Build Requirements
- CMake 3.21 or higher
- Make
- A PC that won't crash and burn when compiling code

## Building
```
cmake -DCMAKE_BUILD_TYPE=Release .
make
```

## Licence:
LGPLv3 for libscanmem
