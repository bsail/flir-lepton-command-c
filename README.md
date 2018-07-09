C Library for the Lepton FLiR Thermal Camera Module.
===============

[![Build Status](https://img.shields.io/travis/bsail/flir-lepton-command-c/master.svg)](https://travis-ci.org/bsail/flir-lepton-command-c)
[![Coverage Status](https://img.shields.io/coveralls/github/bsail/flir-lepton-command-c/master.svg)](https://coveralls.io/github/bsail/flir-lepton-command-c?branch=master)

Fork of original library by [NachtRaveVL](https://github.com/NachtRaveVL/Lepton-FLiR-Arduino).

Library to control a Lepton FLiR (forward looking infrared) thermal camera module.
Licensed under the non-restrictive MIT license.

Created by NachtRaveVL, August 1st, 2016.

Forked by Nikolay Nerovny, BMSTU, 2018.

This library allows communication with boards running a Lepton FLiR thermal camera module. It provides a wide range of functionality from adjustable memory footprint size, adjustable temperature display mode, fast chip select enable/disable routines, to exposing the full functionality of the thermal camera itself.

Parts of this library are derived from the Lepton FLiR software development SDK, Copyright 2011,2012,2013,2014 FLIR Systems - Commercial Vision Systems.

This version differs from original version by [NachtRaveVL](https://github.com/NachtRaveVL/Lepton-FLiR-Arduino): it was ported to plain C to allow usage in wider range of architectures. We also removed the support of VoISP communication, it has only command and control interface.

Latest version of library:

  https://github.com/bsail/flir-lepton-command-c

# Library Setup

There are several defines inside of the library's header file that allows for more fine-tuned control.

```C
// Uncomment this define if wanting to exclude extended i2c functions from compilation.
// #define LEPFLIR_EXCLUDE_EXT_I2C_FUNCS   1

// Uncomment this define to enable debug output functions.
// #define LEPFLIR_ENABLE_DEBUG_OUTPUT     1
```

# Development
For developers looking to extend, bug fix, build, and test this library with dependencies and test infrastructure included in the source tree.


Setup Environment - Ubuntu 16.04/18.04
---------------------------------
```bash
sudo apt install build-essential git ruby
sudo gem install ceedling
```


Get Code
-----------------
```bash
mkdir lepton
git clone https://github.com/bsail/flir-lepton-command-c lepton
cd lepton
```

# Tests
## Unit Tests

Build & Run Unit Tests
```bash
ceedling test:all
```
You may use and create additional tasks for Ceedling build system. Please refer to the documentation in the `vendor/ceedling/docs`.


# Project Structure
## Source Directories
* /src - library source directory
  * lepton-flir.h - main header
  * lepton-flir-defs.h - additional defines
  * lepton-flir.c - main implementation

* /test
  * test_lepton-flir.c - Unity test file for lepton-flir.c module (use `ceedling test:lepton-flir` to run tests only in this source file)

* /
  * LICENSE - The MIT License
  * README.md   - this file
  * project.yml - Ceedling project file including parameters of test compiler and linker
  * .gitignore  - definitions of files and directories to be ignored by git


## Third Party Source Directories
* /vendor  - Ceedling source with documentation and plugins

## Build Directories
* /build - Ceedling library build directory


## Additional Directories
* /reference - Reference documentation about Lepton FLiR camera
