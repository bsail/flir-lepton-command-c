/*  Arduino Library for the Lepton FLiR Thermal Camera Module.
    Copyright (c) 2016 NachtRaveVL      <nachtravevl@gmail.com>

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use,
    copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following
    conditions:

    This permission notice shall be included in all copies or
    substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.

    Lepton-FLiR-Arduino - Version 0.9.91
*/

#ifndef LeptonFLiR_H
#define LeptonFLiR_H

#ifdef __cplusplus
extern "C" {
#endif

// Library Setup

#ifndef TEST
// Uncomment this define if wanting to exclude extended i2c functions from compilation.
#define LEPFLIR_EXCLUDE_EXT_I2C_FUNCS   1

// Uncomment this define if wanting to exclude SYS functions from compilation.
// #define LEPFLIR_EXCLUDE_SYS_FUNCS   1

// Uncomment this define if wanting to exclude VID functions from compilation.
#define LEPFLIR_EXCLUDE_VID_FUNCS   1

// Uncomment this define if wanting to exclude AGC functions from compilation.
// #define LEPFLIR_EXCLUDE_AGC_FUNCS   1

// Uncomment this define if wanting to exclude misc. functions from compilation.
#define LEPFLIR_EXCLUDE_MISC_FUNCS   1

// Explicitly suppress warning about LEPFLIR_EXCLUDE_MISC_FUNCS
#define LEPFLIR_EXCLUDE_MISC_FUNCS_SUPPRESS_WARNING 1

// Uncomment this define if wanting to exclude image statistics functions from compilation.
#define LEPFLIR_EXCLUDE_IMAGE_FUNCS   1

// Uncomment this define to enable debug output functions.
// #define LEPFLIR_ENABLE_DEBUG_OUTPUT     1

/* Do not use locking instruction in case of single threading */
// #define LEPFLIR_EXCLUDE_LOCKING

#ifndef LEPFLIR_EXCLUDE_MISC_FUNCS
#ifdef LEPFLIR_EXCLUDE_IMAGE_FUNCS
#error "LEPFLIR_EXCLUDE_IMAGE_FUNCS should be undefined"
#endif
#endif

#ifndef LEPFLIR_EXCLUDE_SYS_FUNCS
#ifdef LEPFLIR_EXCLUDE_MISC_FUNCS
#ifndef LEPFLIR_EXCLUDE_MISC_FUNCS_SUPPRESS_WARNING
#pragma message "It is recommended to undefine LEPFLIR_EXCLUDE_MISC_FUNCS"
#endif
#endif
#endif

#endif

#include "lepton-flir-defs.h"
#include <inttypes.h>
#include <stdlib.h>

  struct lepton_callbacks {
    void (*i2cWire_beginTransmission) (struct lepton_callbacks * this,
                                       uint8_t addr);
     uint8_t(*i2cWire_endTransmission) (struct lepton_callbacks * this);
     uint8_t(*i2cWire_requestFrom) (struct lepton_callbacks * this,
                                    uint8_t addr, uint8_t len);
     size_t(*i2cWire_write16) (struct lepton_callbacks * this, uint16_t data);
     uint8_t(*i2cWire_read) (struct lepton_callbacks * this);
     uint16_t(*i2cWire_read16) (struct lepton_callbacks * this);
    unsigned long (*millis_callback) (void);
    void (*delay_callback) (unsigned long);
#ifndef LEPFLIR_EXCLUDE_LOCKING
    void (*mutex_lock) (void);
    void (*mutex_unlock) (void);
#endif
    uint8_t _lastI2CError;      // Last i2c error
  };

  struct lepton_communication {
    struct lepton_callbacks callbacks;
    int8_t _lastLepResult;      // Last lep result
    int buffer_length;
  };

  struct lepton_driver;

  // SYS module commands vtable

  struct lepton_sys {
    LEP_SYS_CAM_STATUS_STATES(*getCameraStatus) (struct lepton_driver * driver);
    void (*getFlirSerialNumber) (struct lepton_driver * driver, char *buffer, int maxLength /* = 16 */ ); // maxLength must at least be 16, recommended 20
    void (*getCustomerSerialNumber) (struct lepton_driver * driver, char *buffer, int maxLength /* = 64 */ ); // maxLength must at least be 64, recommended 80
     uint32_t(*getCameraUptime) (struct lepton_driver * driver); // (milliseconds)
    float (*getAuxTemperature) (struct lepton_driver * driver); // min:-273.15C max:382.20C (celsius), min:-459.67F max:719.96F (fahrenheit), min:0.00K max:655.35K (kelvin)
    float (*getFPATemperature) (struct lepton_driver * driver); // min:-273.15C max:382.20C (celsius), min:-459.67F max:719.96F (fahrenheit), min:0.00K max:655.35K (kelvin)
    void (*setTelemetryEnabled) (struct lepton_driver * driver, uint8_t enabled); // def:enabled
     uint8_t(*getTelemetryEnabled) (struct lepton_driver * driver);
    void (*runFFCNormalization) (struct lepton_driver * driver);
#ifndef LEPFLIR_EXCLUDE_EXT_I2C_FUNCS
    void (*runPingCamera) (struct lepton_driver * driver); // return put into lastLepResult
    void (*setTelemetryLocation) (struct lepton_driver * driver, LEP_SYS_TELEMETRY_LOCATION location); // def:LEP_TELEMETRY_LOCATION_FOOTER
     LEP_SYS_TELEMETRY_LOCATION(*getTelemetryLocation) (struct lepton_driver *
                                                        driver);
    void (*runFrameAveraging) (struct lepton_driver * driver);
    void (*setNumFramesToAverage) (struct lepton_driver * driver, LEP_SYS_FRAME_AVERAGE average); // def:LEP_SYS_FA_DIV_8
     LEP_SYS_FRAME_AVERAGE(*getNumFramesToAverage) (struct lepton_driver *
                                                    driver);
    void (*getSceneStatistics) (struct lepton_driver * driver,
                                LEP_SYS_SCENE_STATISTICS * statistics);
    void (*setSceneRegion) (struct lepton_driver * driver, LEP_SYS_SCENE_ROI * region); // min:0,0/end>beg, max:79,59/beg<end def:{0,0,79,59} (pixels)
    void (*getSceneRegion) (struct lepton_driver * driver,
                            LEP_SYS_SCENE_ROI * region);
     uint16_t(*getThermalShutdownCount) (struct lepton_driver * driver); // min:0 max:65535 default:270 (pixels)
    void (*setShutterPosition) (struct lepton_driver * driver, LEP_SYS_SHUTTER_POSITION position); // def:LEP_SYS_SHUTTER_POSITION_UNKNOWN
     LEP_SYS_SHUTTER_POSITION(*getShutterPosition) (struct lepton_driver *
                                                    driver);
    void (*setFFCShutterMode) (struct lepton_driver * driver, LEP_SYS_FFC_SHUTTER_MODE * mode); // see LEP_SYS_FFC_SHUTTER_MODE for defs
    void (*getFFCShutterMode) (struct lepton_driver * driver,
                               LEP_SYS_FFC_SHUTTER_MODE * mode);
     LEP_SYS_FFC_STATUS(*getFFCNormalizationStatus) (struct lepton_driver * driver); // def:LEP_SYS_FFC_STATUS_READY
#endif
  };

  // AGC module commands vtable

  struct lepton_agc {
    void (*setAGCEnabled) (struct lepton_driver * driver, uint8_t enabled); // def:disabled
     uint8_t(*getAGCEnabled) (struct lepton_driver * driver);
    void (*setAGCPolicy) (struct lepton_driver * driver, LEP_AGC_POLICY policy); // def:LEP_AGC_HEQ
     LEP_AGC_POLICY(*getAGCPolicy) (struct lepton_driver * driver);
    void (*setHEQScaleFactor) (struct lepton_driver * driver, LEP_AGC_HEQ_SCALE_FACTOR factor); // def:LEP_AGC_SCALE_TO_8_BITS
     LEP_AGC_HEQ_SCALE_FACTOR(*getHEQScaleFactor) (struct lepton_driver *
                                                   driver);
    void (*setAGCCalcEnabled) (struct lepton_driver * driver, uint8_t enabled); // def:disabled
     uint8_t(*getAGCCalcEnabled) (struct lepton_driver * driver);
#ifndef LEPFLIR_EXCLUDE_EXT_I2C_FUNCS
    void (*setHistogramRegion) (struct lepton_driver * driver, LEP_AGC_HISTOGRAM_ROI * region); // min:0,0/end>beg, max:79,59/beg<end def:{0,0,79,59} (pixels)
    void (*getHistogramRegion) (struct lepton_driver * driver,
                                LEP_AGC_HISTOGRAM_ROI * region);
    void (*getHistogramStatistics) (struct lepton_driver * driver, LEP_AGC_HISTOGRAM_STATISTICS * statistics); // min:{0,0,0,0} max:{0x3FFF,0x3FFF,0x3FFF,4800} (pixels)
    void (*setHistogramClipPercent) (struct lepton_driver * driver, uint16_t percent); // def:0
     uint16_t(*getHistogramClipPercent) (struct lepton_driver * driver);
    void (*setHistogramTailSize) (struct lepton_driver * driver, uint16_t size); // def:0
     uint16_t(*getHistogramTailSize) (struct lepton_driver * driver);
    void (*setLinearMaxGain) (struct lepton_driver * driver, uint16_t gain); // def:1
     uint16_t(*getLinearMaxGain) (struct lepton_driver * driver);
    void (*setLinearMidpoint) (struct lepton_driver * driver, uint16_t midpoint); // min:0 max:256 def:128
     uint16_t(*getLinearMidpoint) (struct lepton_driver * driver);
    void (*setLinearDampeningFactor) (struct lepton_driver * driver, uint16_t factor); // def:1
     uint16_t(*getLinearDampeningFactor) (struct lepton_driver * driver);
    void (*setHEQDampeningFactor) (struct lepton_driver * driver, uint16_t factor); // min:0 max:256 def:64
     uint16_t(*getHEQDampeningFactor) (struct lepton_driver * driver);
    void (*setHEQMaxGain) (struct lepton_driver * driver, uint16_t gain); // def:1
     uint16_t(*getHEQMaxGain) (struct lepton_driver * driver);
    void (*setHEQClipLimitHigh) (struct lepton_driver * driver, uint16_t limit); // min:0 max:4800 def:4800 (pixels)
     uint16_t(*getHEQClipLimitHigh) (struct lepton_driver * driver);
    void (*setHEQClipLimitLow) (struct lepton_driver * driver, uint16_t limit); // min:0 max:1024 def:512 (pixels)
     uint16_t(*getHEQClipLimitLow) (struct lepton_driver * driver);
    void (*setHEQBinExtension) (struct lepton_driver * driver, uint16_t extension); // def:0
     uint16_t(*getHEQBinExtension) (struct lepton_driver * driver);
    void (*setHEQMidpoint) (struct lepton_driver * driver, uint16_t midpoint); // min:0 max:256 def:128
     uint16_t(*getHEQMidpoint) (struct lepton_driver * driver);
    void (*setHEQEmptyCounts) (struct lepton_driver * driver, uint16_t counts); // min:0 max:0x3FFF def:2
     uint16_t(*getHEQEmptyCounts) (struct lepton_driver * driver);
    void (*setHEQNormalizationFactor) (struct lepton_driver * driver, uint16_t factor); // def:1
     uint16_t(*getHEQNormalizationFactor) (struct lepton_driver * driver);
#endif
  };

  // VID module commands vtable

  struct lepton_vid {
    void (*setPolarity) (struct lepton_driver * driver, LEP_VID_POLARITY polarity); // def:LEP_VID_WHITE_HOT
     LEP_VID_POLARITY(*getPolarity) (struct lepton_driver * driver);
    void (*setPseudoColorLUT) (struct lepton_driver * driver, LEP_VID_PCOLOR_LUT table); // def:LEP_VID_FUSION_LUT
     LEP_VID_PCOLOR_LUT(*getPseudoColorLUT) (struct lepton_driver * driver);
    void (*setFocusCalcEnabled) (struct lepton_driver * driver, uint8_t enabled); // def:disabled
     uint8_t(*getFocusCalcEnabled) (struct lepton_driver * driver);
    void (*setFreezeEnabled) (struct lepton_driver * driver, uint8_t enabled); // def:disabled
     uint8_t(*getFreezeEnabled) (struct lepton_driver * driver);
#ifndef LEPFLIR_EXCLUDE_EXT_I2C_FUNCS
    void (*setUserColorLUT) (struct lepton_driver * driver, LEP_VID_LUT_BUFFER * table); // These two methods may not work as intended, possibly leaving the I2C bus on the
    void (*getUserColorLUT) (struct lepton_driver * driver, LEP_VID_LUT_BUFFER * table); // FLiR in a non-responding state. A full power cycle may be needed to reset.
    void (*setFocusRegion) (struct lepton_driver * driver, LEP_VID_FOCUS_ROI * region); // min:1,1/end>beg+1, max:78,58/beg<end-1 def:{1,1,78,58} (pixels)
    void (*getFocusRegion) (struct lepton_driver * driver,
                            LEP_VID_FOCUS_ROI * region);
    void (*setFocusThreshold) (struct lepton_driver * driver, uint32_t threshold); // def:30
     uint32_t(*getFocusThreshold) (struct lepton_driver * driver);
     uint32_t(*getFocusMetric) (struct lepton_driver * driver);
    void (*setSceneBasedNUCEnabled) (struct lepton_driver * driver, uint8_t enabled); // def:enabled
     uint8_t(*getSceneBasedNUCEnabled) (struct lepton_driver * driver);
    void (*setGamma) (struct lepton_driver * driver, uint32_t gamma); // def:58
     uint32_t(*getGamma) (struct lepton_driver * driver);
#endif
  };

  struct lepton_misc {
    // Module represents temperatures as kelvin x 100 (in integer format). These methods
    // convert to and from the selected temperature mode.
    float (*kelvin100ToTemperature) (struct lepton_driver * driver,
                                     uint16_t kelvin100);
     uint16_t(*temperatureToKelvin100) (struct lepton_driver * driver,
                                        float temperature);
    const char *(*getTemperatureSymbol) (struct lepton_driver * driver);

    void (*wordsToHexString) (uint16_t * dataWords, int dataLength,
                              char *buffer, int maxLength);
    float (*kelvin100ToCelsius) (uint16_t kelvin100);
    float (*kelvin100ToFahrenheit) (uint16_t kelvin100);
    float (*kelvin100ToKelvin) (uint16_t kelvin100);
     uint16_t(*celsiusToKelvin100) (float celsius);
     uint16_t(*fahrenheitToKelvin100) (float fahrenheit);
     uint16_t(*kelvinToKelvin100) (float kelvin);
  };

  struct lepton_init {
    void (*lepton_i2cWire_beginTransmission_set_callback) (struct lepton_driver
                                                           * driver,
                                                           void (*callback)
                                                           (struct
                                                            lepton_callbacks *,
                                                            uint8_t));
    void (*lepton_i2cWire_endTransmission_set_callback) (struct lepton_driver *
                                                         driver,
                                                         uint8_t(*callback)
                                                         (struct
                                                          lepton_callbacks *));
    void (*lepton_i2cWire_requestFrom_set_callback) (struct lepton_driver *
                                                     driver,
                                                     uint8_t(*callback) (struct
                                                                         lepton_callbacks
                                                                         *,
                                                                         uint8_t,
                                                                         uint8_t));
    void (*lepton_i2cWire_write16_set_callback) (struct lepton_driver * driver,
                                                 size_t(*callback) (struct
                                                                    lepton_callbacks
                                                                    *,
                                                                    uint16_t));
    void (*lepton_i2cWire_read_set_callback) (struct lepton_driver * driver,
                                              uint8_t(*callback) (struct
                                                                  lepton_callbacks
                                                                  *));
    void (*lepton_i2cWire_read16_set_callback) (struct lepton_driver * driver,
                                                uint16_t(*callback) (struct
                                                                     lepton_callbacks
                                                                     *));
    void (*lepton_i2cWire_set_buffer_length) (struct lepton_driver * driver,
                                              int length);
    void (*lepton_millis_set_callback) (struct lepton_driver * driver,
                                        unsigned long (*callback) (void));
    void (*lepton_delay_set_callback) (struct lepton_driver * driver,
                                       void (*callback) (unsigned long));
#ifndef LEPFLIR_EXCLUDE_LOCKING
    void (*mutex_lock_set_callback) (struct lepton_driver * driver,
                                       void (*callback) (void));
    void (*mutex_unlock_set_callback) (struct lepton_driver * driver,
                                       void (*callback) (void));
#endif
  };

  struct lepton_driver {
    uint16_t(*getStatusRegister) (struct lepton_driver * driver);
    uint8_t(*getLastI2CError) (struct lepton_driver * driver);
    LEP_RESULT(*getLastLepResult) (struct lepton_driver * driver);
#ifndef LEPFLIR_EXCLUDE_IMAGE_FUNCS
    LeptonFLiR_ImageStorageMode _storageMode; // Image data storage mode
    LeptonFLiR_TemperatureMode _tempMode; // Temperature display mode
     LeptonFLiR_ImageStorageMode(*getImageStorageMode) (struct lepton_driver *
                                                        driver);
     LeptonFLiR_TemperatureMode(*getTemperatureMode) (struct lepton_driver *
                                                      driver);
    /* Image descriptors */
    int (*getImageWidth) (struct lepton_driver * driver);
    int (*getImageHeight) (struct lepton_driver * driver);
    int (*getImageBpp) (struct lepton_driver * driver); // Bytes per pixel
#endif
    /* VTables */
    struct lepton_communication communication;
#ifndef LEPFLIR_EXCLUDE_SYS_FUNCS
    struct lepton_sys sys;
#endif
#ifndef LEPFLIR_EXCLUDE_AGC_FUNCS
    struct lepton_agc agc;
#endif
#ifndef LEPFLIR_EXCLUDE_VID_FUNCS
    struct lepton_vid vid;
#endif
#ifndef LEPFLIR_EXCLUDE_MISC_FUNCS
    struct lepton_misc misc;
#endif
    struct lepton_init init;
  };

#ifndef ENABLED
#define ENABLED  0x1
#endif
#ifndef DISABLED
#define DISABLED 0x0
#endif

#define LEPFLIR_GEN_CMD_TIMEOUT         5000 // Timeout for commands to be processed
#define LEPFLIR_SPI_MAX_SPEED           20000000 // Maximum SPI speed for FLiR module
#define LEPFLIR_SPI_MIN_SPEED           2200000 // Minimum SPI speed for FLiR module
#define LEPFLIR_SPI_FRAME_PACKET_SIZE           164 // 2B ID + 2B CRC + 160B for 80x1 14bpp/8bppAGC thermal image data or telemetry data
#define LEPFLIR_SPI_FRAME_PACKET_SIZE16         82

  void LeptonFLiR_init(
#ifndef LEPFLIR_EXCLUDE_IMAGE_FUNCS
                        LeptonFLiR_ImageStorageMode storageMode,
                        LeptonFLiR_TemperatureMode tempMode,
#endif
                        struct lepton_driver *driver);

#ifdef LEPTON_FLIR_INTERNAL
  enum transmission_mode {
    u16 = 0,
    u32,
    array
  };
#endif

#ifdef TEST
uint16_t getStatusRegister(struct lepton_driver *driver);
#endif

#ifdef __cplusplus
}
#endif
#endif
