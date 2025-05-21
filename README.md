[![pipeline status](https://gitlab.com/iniparser/iniparser/badges/main/pipeline.svg)](https://gitlab.com/iniparser/iniparser/-/commits/main)
[![latest release](https://gitlab.com/iniparser/iniparser/-/badges/release.svg)](https://gitlab.com/iniparser/iniparser/-/releases)

# Ammunution Inventory Manager (ammo-mgr) 4 #

## Overview

This is an interactive program to manage a personal stock of ammunition.

Key features:

 - Small : around 1500 sloc inside 4 files (2 .c and 2 .h)
 - Portable : no dependancies, written in `-pedantic` C
 - Core functions embodied in a library
 - CLI/TUI/GUI user-facing component use the core library

## Building

This project uses CMake as build system. Use these command at the root or the
project to get the static (i.e. `libiniparser.a`) and shared (i.e.
`libiniparser.so`) libraries compiled:

```
mkdir build
cd build
cmake ..
make all
```


## Tests

_TODO_


## Examples

_TODO_

## Documentation

The library is completely documented in its header file.

_TODO_

## License

This software is released under MIT License.
See [LICENSE](LICENSE) for more details


## Versions

[![latest release]](https://github.com/lueckenhoff/ammo-mgr/)


## FAQ

_TODO_

