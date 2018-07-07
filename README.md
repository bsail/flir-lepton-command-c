C Library for the Lepton FLiR Thermal Camera Module.
===============

Library to control a Lepton FLiR (forward looking infrared) thermal camera module.
Licensed under the non-restrictive MIT license.

Created by NachtRaveVL, August 1st, 2016.
Forked by Nikolay Nerovny, BMSTU, 2018.

This library allows communication with boards running a Lepton FLiR thermal camera module. It provides a wide range of functionality from adjustable memory footprint size, adjustable temperature display mode, fast chip select enable/disable routines, to exposing the full functionality of the thermal camera itself.

Parts of this library are derived from the Lepton FLiR software development SDK, Copyright 2011,2012,2013,2014 FLIR Systems - Commercial Vision Systems.

This version differs from original version by [NachtRaveVL](https://github.com/NachtRaveVL/Lepton-FLiR-Arduino): it was ported to plain C to allow usage in wider range of architectures. We also removed the support of VoISP communication, it has only command and control interface.

## Library Setup

There are several defines inside of the library's header file that allows for more fine-tuned control.

```Arduino
// Uncomment this define to enable use of the software i2c library (min 4MHz+ processor required).
//#define LEPFLIR_ENABLE_SOFTWARE_I2C     1   // http://playground.arduino.cc/Main/SoftwareI2CLibrary

// Uncomment this define to disable usage of the Scheduler library on SAM/SAMD architecures.
//#define LEPFLIR_DISABLE_SCHEDULER       1   // https://github.com/arduino-libraries/Scheduler

// Uncomment this define to disable 16 byte aligned memory allocations (may hinder performance).
//#define LEPFLIR_DISABLE_ALIGNED_MALLOC  1

// Uncomment this define if wanting to exclude extended i2c functions from compilation.
//#define LEPFLIR_EXCLUDE_EXT_I2C_FUNCS   1

// Uncomment this define to enable debug output.
//#define LEPFLIR_ENABLE_DEBUG_OUTPUT     1
```

