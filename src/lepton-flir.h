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

// Library Setup

// Uncomment this define if wanting to exclude extended i2c functions from compilation.
// #define LEPFLIR_EXCLUDE_EXT_I2C_FUNCS   1

// Uncomment this define to enable debug output functions.
// #define LEPFLIR_ENABLE_DEBUG_OUTPUT     1

#include "lepton-flir-defs.h"
#include <inttypes.h>
#include <stdlib.h>

struct lepton_callbacks {
  void (*i2cWire_beginTransmission) (uint8_t addr, struct lepton_callbacks * this);
  uint8_t(*i2cWire_endTransmission) (struct lepton_callbacks * this);
  uint8_t(*i2cWire_requestFrom) (uint8_t addr, uint8_t len, struct lepton_callbacks * this);
  size_t(*i2cWire_write) (uint8_t data, struct lepton_callbacks * this);
  size_t(*i2cWire_write16) (uint16_t data, struct lepton_callbacks * this);
  uint8_t(*i2cWire_read) (struct lepton_callbacks * this);
  uint16_t(*i2cWire_read16) (struct lepton_callbacks * this);
  unsigned long (*millis_callback) (void);
  void (*delay_callback) (unsigned long);
  uint8_t _lastI2CError;          // Last i2c error
};

struct lepton_communication {
  struct lepton_callbacks callbacks;
  LeptonFLiR_ImageStorageMode _storageMode; // Image data storage mode
  LeptonFLiR_TemperatureMode _tempMode; // Temperature display mode
  uint8_t _lastLepResult;         // Last lep result
  int BUFFER_LENGTH;
  uint8_t (*waitCommandBegin)(int timeout, struct lepton_communication * this);
  uint8_t (*waitCommandFinish)(int timeout, struct lepton_communication * this);
  uint16_t (*cmdCode)(uint16_t cmdID, uint16_t cmdType);
  void (*sendCommand_raw)(uint16_t cmdCode, struct lepton_communication * this);
  void (*sendCommand_u16)(uint16_t cmdCode, uint16_t value, struct lepton_communication * this);
  void (*sendCommand_u32)(uint16_t cmdCode, uint32_t value, struct lepton_communication * this);
  void (*sendCommand_array)(uint16_t cmdCode, uint16_t * dataWords, int dataLength, struct lepton_communication * this);
  void (*receiveCommand_u16)(uint16_t cmdCode, uint16_t * value, struct lepton_communication * this);
  void (*receiveCommand_u32)(uint16_t cmdCode, uint32_t * value, struct lepton_communication * this);
  void (*receiveCommand_array)(uint16_t cmdCode, uint16_t * readWords, int maxLength, struct lepton_communication * this);
  int (*writeCmdRegister)(uint16_t cmdCode, uint16_t * dataWords, int dataLength, struct lepton_communication * this);
  int (*readDataRegister)(uint16_t * readWords, int maxLength, struct lepton_communication * this);
  int (*writeRegister)(uint16_t regAddress, uint16_t value, struct lepton_communication * this);
  int (*readRegister)(uint16_t regAddress, uint16_t * value, struct lepton_communication * this);
};

    // SYS module commands

struct lepton_sys {
  void (*sys_getCameraStatus_internal)(LEP_SYS_CAM_STATUS * status,void * driver);
  LEP_SYS_CAM_STATUS_STATES (*sys_getCameraStatus)(void * driver);
  void (*sys_getFlirSerialNumber)(char *buffer, int maxLength /* = 16 */,void * driver ); // maxLength must at least be 16, recommended 20
  void (*sys_getCustomerSerialNumber)(char *buffer, int maxLength /* = 64 */ ,void * driver); // maxLength must at least be 64, recommended 80
  uint32_t (*sys_getCameraUptime)(void * driver); // (milliseconds)
  float (*sys_getAuxTemperature)(void * driver);  // min:-273.15C max:382.20C (celsius), min:-459.67F max:719.96F (fahrenheit), min:0.00K max:655.35K (kelvin)
  float (*sys_getFPATemperature)(void * driver);  // min:-273.15C max:382.20C (celsius), min:-459.67F max:719.96F (fahrenheit), min:0.00K max:655.35K (kelvin)
  void (*sys_setTelemetryEnabled)(uint8_t enabled,void * driver); // def:enabled
  uint8_t (*sys_getTelemetryEnabled)(void * driver);
  void (*sys_runFFCNormalization)(void * driver);
#ifndef LEPFLIR_EXCLUDE_EXT_I2C_FUNCS
  void (*sys_runPingCamera)(void * driver);       // return put into lastLepResult
  void (*sys_setTelemetryLocation)(LEP_SYS_TELEMETRY_LOCATION location, void * driver); // def:LEP_TELEMETRY_LOCATION_FOOTER
  LEP_SYS_TELEMETRY_LOCATION (*sys_getTelemetryLocation)(void * driver);
  void (*sys_runFrameAveraging)(void * driver);
  void (*sys_setNumFramesToAverage)(LEP_SYS_FRAME_AVERAGE average, void * driver); // def:LEP_SYS_FA_DIV_8
  LEP_SYS_FRAME_AVERAGE (*sys_getNumFramesToAverage)(void * driver);
  void (*sys_getSceneStatistics)(LEP_SYS_SCENE_STATISTICS * statistics, void * driver);
  void (*sys_setSceneRegion)(LEP_SYS_SCENE_ROI * region, void * driver); // min:0,0/end>beg, max:79,59/beg<end def:{0,0,79,59} (pixels)
  void (*sys_getSceneRegion)(LEP_SYS_SCENE_ROI * region, void * driver);
  uint16_t (*sys_getThermalShutdownCount)(void * driver); // min:0 max:65535 default:270 (pixels)
  void (*sys_setShutterPosition)(LEP_SYS_SHUTTER_POSITION position, void * driver); // def:LEP_SYS_SHUTTER_POSITION_UNKNOWN
  LEP_SYS_SHUTTER_POSITION (*sys_getShutterPosition)(void * driver);
  void (*sys_setFFCShutterMode)(LEP_SYS_FFC_SHUTTER_MODE * mode, void * driver); // see LEP_SYS_FFC_SHUTTER_MODE for defs
  void (*sys_getFFCShutterMode)(LEP_SYS_FFC_SHUTTER_MODE * mode, void * driver);
  LEP_SYS_FFC_STATUS (*sys_getFFCNormalizationStatus)(void * driver); // def:LEP_SYS_FFC_STATUS_READY
#endif
};


    // AGC module commands

struct lepton_agc {
  void (*agc_setAGCEnabled)(uint8_t enabled, void * driver); // def:disabled
  uint8_t (*agc_getAGCEnabled)(void * driver);
  void (*agc_setAGCPolicy)(LEP_AGC_POLICY policy, void * driver); // def:LEP_AGC_HEQ
  LEP_AGC_POLICY (*agc_getAGCPolicy)(void * driver);
  void (*agc_setHEQScaleFactor)(LEP_AGC_HEQ_SCALE_FACTOR factor, void * driver); // def:LEP_AGC_SCALE_TO_8_BITS
  LEP_AGC_HEQ_SCALE_FACTOR (*agc_getHEQScaleFactor)(void * driver);
  void (*agc_setAGCCalcEnabled)(uint8_t enabled, void * driver); // def:disabled
  uint8_t (*agc_getAGCCalcEnabled)(void * driver);
#ifndef LEPFLIR_EXCLUDE_EXT_I2C_FUNCS
  void (*agc_setHistogramRegion)(LEP_AGC_HISTOGRAM_ROI * region, void * driver); // min:0,0/end>beg, max:79,59/beg<end def:{0,0,79,59} (pixels)
  void (*agc_getHistogramRegion)(LEP_AGC_HISTOGRAM_ROI * region, void * driver);
  void (*agc_getHistogramStatistics)(LEP_AGC_HISTOGRAM_STATISTICS * statistics, void * driver); // min:{0,0,0,0} max:{0x3FFF,0x3FFF,0x3FFF,4800} (pixels)
  void (*agc_setHistogramClipPercent)(uint16_t percent, void * driver); // def:0
  uint16_t (*agc_getHistogramClipPercent)(void * driver);
  void (*agc_setHistogramTailSize)(uint16_t size, void * driver); // def:0
  uint16_t (*agc_getHistogramTailSize)(void * driver);
  void (*agc_setLinearMaxGain)(uint16_t gain, void * driver); // def:1
  uint16_t (*agc_getLinearMaxGain)(void * driver);
  void (*agc_setLinearMidpoint)(uint16_t midpoint, void * driver); // min:0 max:256 def:128
  uint16_t (*agc_getLinearMidpoint)(void * driver);
  void (*agc_setLinearDampeningFactor)(uint16_t factor, void * driver); // def:1
  uint16_t (*agc_getLinearDampeningFactor)(void * driver);
  void (*agc_setHEQDampeningFactor)(uint16_t factor, void * driver); // min:0 max:256 def:64
  uint16_t (*agc_getHEQDampeningFactor)(void * driver);
  void (*agc_setHEQMaxGain)(uint16_t gain, void * driver); // def:1
  uint16_t (*agc_getHEQMaxGain)(void * driver);
  void (*agc_setHEQClipLimitHigh)(uint16_t limit, void * driver); // min:0 max:4800 def:4800 (pixels)
  uint16_t (*agc_getHEQClipLimitHigh)(void * driver);
  void (*agc_setHEQClipLimitLow)(uint16_t limit, void * driver); // min:0 max:1024 def:512 (pixels)
  uint16_t (*agc_getHEQClipLimitLow)(void * driver);
  void (*agc_setHEQBinExtension)(uint16_t extension, void * driver); // def:0
  uint16_t (*agc_getHEQBinExtension)(void * driver);
  void (*agc_setHEQMidpoint)(uint16_t midpoint, void * driver); // min:0 max:256 def:128
  uint16_t (*agc_getHEQMidpoint)(void * driver);
  void (*agc_setHEQEmptyCounts)(uint16_t counts, void * driver); // min:0 max:0x3FFF def:2
  uint16_t (*agc_getHEQEmptyCounts)(void * driver);
  void (*agc_setHEQNormalizationFactor)(uint16_t factor, void * driver); // def:1
  uint16_t (*agc_getHEQNormalizationFactor)(void * driver);
#endif
};


    // VID module commands

struct lepton_vid {
  void (*vid_setPolarity)(LEP_VID_POLARITY polarity, void * driver); // def:LEP_VID_WHITE_HOT
  LEP_VID_POLARITY (*vid_getPolarity)(void * driver);
  void (*vid_setPseudoColorLUT)(LEP_VID_PCOLOR_LUT table, void * driver); // def:LEP_VID_FUSION_LUT
  LEP_VID_PCOLOR_LUT (*vid_getPseudoColorLUT)(void * driver);
  void (*vid_setFocusCalcEnabled)(uint8_t enabled, void * driver); // def:disabled
  uint8_t (*vid_getFocusCalcEnabled)(void * driver);
  void (*vid_setFreezeEnabled)(uint8_t enabled, void * driver); // def:disabled
  uint8_t (*vid_getFreezeEnabled)(void * driver);
#ifndef LEPFLIR_EXCLUDE_EXT_I2C_FUNCS
  void (*vid_setUserColorLUT)(LEP_VID_LUT_BUFFER * table, void * driver); // These two methods may not work as intended, possibly leaving the I2C bus on the
  void (*vid_getUserColorLUT)(LEP_VID_LUT_BUFFER * table, void * driver); // FLiR in a non-responding state. A full power cycle may be needed to reset.
  void (*vid_setFocusRegion)(LEP_VID_FOCUS_ROI * region, void * driver); // min:1,1/end>beg+1, max:78,58/beg<end-1 def:{1,1,78,58} (pixels)
  void (*vid_getFocusRegion)(LEP_VID_FOCUS_ROI * region, void * driver);
  void (*vid_setFocusThreshold)(uint32_t threshold, void * driver); // def:30
  uint32_t (*vid_getFocusThreshold)(void * driver);
  uint32_t (*vid_getFocusMetric)(void * driver);
  void (*vid_setSceneBasedNUCEnabled)(uint8_t enabled, void * driver); // def:enabled
  uint8_t (*vid_getSceneBasedNUCEnabled)(void * driver);
  void (*vid_setGamma)(uint32_t gamma, void * driver); // def:58
  uint32_t (*vid_getGamma)(void * driver);
#endif
};

struct lepton_driver {
  struct lepton_callbacks callbacks;
  struct lepton_communication communication;
  struct lepton_sys sys;
  struct lepton_agc agc;
  struct lepton_vid vid;
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


void
lepton_i2cWire_beginTransmission_set_callback(void (*callback) (uint8_t, struct lepton_callbacks *), struct lepton_driver * driver);
void lepton_i2cWire_endTransmission_set_callback(uint8_t(*callback) (struct lepton_callbacks *), struct lepton_driver * driver);
void
lepton_i2cWire_requestFrom_set_callback(uint8_t(*callback)
                                        (uint8_t, uint8_t, struct lepton_callbacks *), struct lepton_driver * driver);
void lepton_i2cWire_write_set_callback(size_t(*callback) (uint8_t, struct lepton_callbacks *), struct lepton_driver * driver);
void lepton_i2cWire_write16_set_callback(size_t(*callback) (uint16_t, struct lepton_callbacks *), struct lepton_driver * driver);
void lepton_i2cWire_read_set_callback(uint8_t(*callback) (struct lepton_callbacks *), struct lepton_driver * driver);
void lepton_i2cWire_read16_set_callback(uint16_t(*callback) (struct lepton_callbacks *), struct lepton_driver * driver);
void lepton_i2cWire_set_buffer_length(int length, struct lepton_driver * driver);
void lepton_millis_set_callback(unsigned long (*callback) (void), struct lepton_driver * driver);
void lepton_delay_set_callback(void (*callback) (unsigned long), struct lepton_driver * driver);

    // uint8_t getChipSelectPin();
LeptonFLiR_ImageStorageMode getImageStorageMode(struct lepton_driver * driver);
LeptonFLiR_TemperatureMode getTemperatureMode(struct lepton_driver * driver);

    // Image descriptors
int getImageWidth(struct lepton_driver * driver);
int getImageHeight(struct lepton_driver * driver);
int getImageBpp(struct lepton_driver * driver);              // Bytes per pixel

    // Module represents temperatures as kelvin x 100 (in integer format). These methods
    // convert to and from the selected temperature mode.
float kelvin100ToTemperature(uint16_t kelvin100, struct lepton_driver * driver);
uint16_t temperatureToKelvin100(float temperature, struct lepton_driver * driver);
const char *getTemperatureSymbol();

uint8_t getLastI2CError();
LEP_RESULT getLastLepResult();

void wordsToHexString(uint16_t * dataWords, int dataLength, char *buffer,
                             int maxLength);
extern float kelvin100ToFahrenheit(uint16_t kelvin100);
extern float kelvin100ToKelvin(uint16_t kelvin100);
extern uint16_t celsiusToKelvin100(float celsius);
extern uint16_t fahrenheitToKelvin100(float fahrenheit);
extern uint16_t kelvinToKelvin100(float kelvin);

void LeptonFLiR_init(LeptonFLiR_ImageStorageMode storageMode,
                     LeptonFLiR_TemperatureMode tempMode,
                     struct lepton_driver * driver);

#endif
