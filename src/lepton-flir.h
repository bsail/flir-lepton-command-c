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
  struct lepton_callbacks * callbacks;
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
  void (*sys_getCameraStatus_internal)(LEP_SYS_CAM_STATUS * status,struct lepton_communication * communication);
  LEP_SYS_CAM_STATUS_STATES (*sys_getCameraStatus)(struct lepton_communication * communication);
  void (*sys_getFlirSerialNumber)(char *buffer, int maxLength /* = 16 */,struct lepton_communication * communication ); // maxLength must at least be 16, recommended 20
  void (*sys_getCustomerSerialNumber)(char *buffer, int maxLength /* = 64 */ ,struct lepton_communication * communication); // maxLength must at least be 64, recommended 80
  uint32_t (*sys_getCameraUptime)(struct lepton_communication * communication); // (milliseconds)
  float (*sys_getAuxTemperature)(struct lepton_communication * communication);  // min:-273.15C max:382.20C (celsius), min:-459.67F max:719.96F (fahrenheit), min:0.00K max:655.35K (kelvin)
  float (*sys_getFPATemperature)(struct lepton_communication * communication);  // min:-273.15C max:382.20C (celsius), min:-459.67F max:719.96F (fahrenheit), min:0.00K max:655.35K (kelvin)
  void (*sys_setTelemetryEnabled)(uint8_t enabled,struct lepton_communication * communication); // def:enabled
  uint8_t (*sys_getTelemetryEnabled)(struct lepton_communication * communication);
  void (*sys_runFFCNormalization)(struct lepton_communication * communication);
#ifndef LEPFLIR_EXCLUDE_EXT_I2C_FUNCS
  void (*sys_runPingCamera)(struct lepton_communication * communication);       // return put into lastLepResult
  void (*sys_setTelemetryLocation)(LEP_SYS_TELEMETRY_LOCATION location, struct lepton_communication * communication); // def:LEP_TELEMETRY_LOCATION_FOOTER
  LEP_SYS_TELEMETRY_LOCATION (*sys_getTelemetryLocation)(struct lepton_communication * communication);
  void (*sys_runFrameAveraging)(struct lepton_communication * communication);
  void (*sys_setNumFramesToAverage)(LEP_SYS_FRAME_AVERAGE average, struct lepton_communication * communication); // def:LEP_SYS_FA_DIV_8
  LEP_SYS_FRAME_AVERAGE (*sys_getNumFramesToAverage)(struct lepton_communication * communication);
  void (*sys_getSceneStatistics)(LEP_SYS_SCENE_STATISTICS * statistics, struct lepton_communication * communication);
  void (*sys_setSceneRegion)(LEP_SYS_SCENE_ROI * region, struct lepton_communication * communication); // min:0,0/end>beg, max:79,59/beg<end def:{0,0,79,59} (pixels)
  void (*sys_getSceneRegion)(LEP_SYS_SCENE_ROI * region, struct lepton_communication * communication);
  uint16_t (*sys_getThermalShutdownCount)(struct lepton_communication * communication); // min:0 max:65535 default:270 (pixels)
  void (*sys_setShutterPosition)(LEP_SYS_SHUTTER_POSITION position, struct lepton_communication * communication); // def:LEP_SYS_SHUTTER_POSITION_UNKNOWN
  LEP_SYS_SHUTTER_POSITION (*sys_getShutterPosition)(struct lepton_communication * communication);
  void (*sys_setFFCShutterMode)(LEP_SYS_FFC_SHUTTER_MODE * mode, struct lepton_communication * communication); // see LEP_SYS_FFC_SHUTTER_MODE for defs
  void (*sys_getFFCShutterMode)(LEP_SYS_FFC_SHUTTER_MODE * mode, struct lepton_communication * communication);
  LEP_SYS_FFC_STATUS (*sys_getFFCNormalizationStatus)(struct lepton_communication * communication); // def:LEP_SYS_FFC_STATUS_READY
#endif
};

struct lepton_driver {
  struct lepton_callbacks callbacks;
  struct lepton_communication communication;
  struct lepton_sys sys;
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
lepton_i2cWire_beginTransmission_set_callback(void (*callback) (uint8_t addr, struct lepton_callbacks * this));
void lepton_i2cWire_endTransmission_set_callback(uint8_t(*callback) (struct lepton_callbacks * this));
void
lepton_i2cWire_requestFrom_set_callback(uint8_t(*callback)
                                        (uint8_t addr, uint8_t len, struct lepton_callbacks * this));
void lepton_i2cWire_write_set_callback(size_t(*callback) (uint8_t data, struct lepton_callbacks * this));
void lepton_i2cWire_write16_set_callback(size_t(*callback) (uint16_t data, struct lepton_callbacks * this));
void lepton_i2cWire_read_set_callback(uint8_t(*callback) (struct lepton_callbacks * this));
void lepton_i2cWire_read16_set_callback(uint16_t(*callback) (struct lepton_callbacks * this));
void lepton_i2cWire_set_buffer_length(int length);
void lepton_millis_set_callback(unsigned long (*callback) (void));
void lepton_delay_set_callback(void (*callback) (unsigned long));

    // uint8_t getChipSelectPin();
LeptonFLiR_ImageStorageMode getImageStorageMode();
LeptonFLiR_TemperatureMode getTemperatureMode();

    // Image descriptors
int getImageWidth();
int getImageHeight();
int getImageBpp();              // Bytes per pixel

    // AGC module commands

void agc_setAGCEnabled(uint8_t enabled); // def:disabled
uint8_t agc_getAGCEnabled();

void agc_setAGCPolicy(LEP_AGC_POLICY policy); // def:LEP_AGC_HEQ
LEP_AGC_POLICY agc_getAGCPolicy();

void agc_setHEQScaleFactor(LEP_AGC_HEQ_SCALE_FACTOR factor); // def:LEP_AGC_SCALE_TO_8_BITS
LEP_AGC_HEQ_SCALE_FACTOR agc_getHEQScaleFactor();

void agc_setAGCCalcEnabled(uint8_t enabled); // def:disabled
uint8_t agc_getAGCCalcEnabled();

    // VID module commands

void vid_setPolarity(LEP_VID_POLARITY polarity); // def:LEP_VID_WHITE_HOT
LEP_VID_POLARITY vid_getPolarity();

void vid_setPseudoColorLUT(LEP_VID_PCOLOR_LUT table); // def:LEP_VID_FUSION_LUT
LEP_VID_PCOLOR_LUT vid_getPseudoColorLUT();

void vid_setFocusCalcEnabled(uint8_t enabled); // def:disabled
uint8_t vid_getFocusCalcEnabled();

void vid_setFreezeEnabled(uint8_t enabled); // def:disabled
uint8_t vid_getFreezeEnabled();

#ifndef LEPFLIR_EXCLUDE_EXT_I2C_FUNCS

    // AGC extended module commands

void agc_setHistogramRegion(LEP_AGC_HISTOGRAM_ROI * region); // min:0,0/end>beg, max:79,59/beg<end def:{0,0,79,59} (pixels)
void agc_getHistogramRegion(LEP_AGC_HISTOGRAM_ROI * region);

void agc_getHistogramStatistics(LEP_AGC_HISTOGRAM_STATISTICS * statistics); // min:{0,0,0,0} max:{0x3FFF,0x3FFF,0x3FFF,4800} (pixels)

void agc_setHistogramClipPercent(uint16_t percent); // def:0
uint16_t agc_getHistogramClipPercent();

void agc_setHistogramTailSize(uint16_t size); // def:0
uint16_t agc_getHistogramTailSize();

void agc_setLinearMaxGain(uint16_t gain); // def:1
uint16_t agc_getLinearMaxGain();

void agc_setLinearMidpoint(uint16_t midpoint); // min:0 max:256 def:128
uint16_t agc_getLinearMidpoint();

void agc_setLinearDampeningFactor(uint16_t factor); // def:1
uint16_t agc_getLinearDampeningFactor();

void agc_setHEQDampeningFactor(uint16_t factor); // min:0 max:256 def:64
uint16_t agc_getHEQDampeningFactor();

void agc_setHEQMaxGain(uint16_t gain); // def:1
uint16_t agc_getHEQMaxGain();

void agc_setHEQClipLimitHigh(uint16_t limit); // min:0 max:4800 def:4800 (pixels)
uint16_t agc_getHEQClipLimitHigh();

void agc_setHEQClipLimitLow(uint16_t limit); // min:0 max:1024 def:512 (pixels)
uint16_t agc_getHEQClipLimitLow();

void agc_setHEQBinExtension(uint16_t extension); // def:0
uint16_t agc_getHEQBinExtension();

void agc_setHEQMidpoint(uint16_t midpoint); // min:0 max:256 def:128
uint16_t agc_getHEQMidpoint();

void agc_setHEQEmptyCounts(uint16_t counts); // min:0 max:0x3FFF def:2
uint16_t agc_getHEQEmptyCounts();

void agc_setHEQNormalizationFactor(uint16_t factor); // def:1
uint16_t agc_getHEQNormalizationFactor();

    // SYS extended module commands

    // VID extended module commands

void vid_setUserColorLUT(LEP_VID_LUT_BUFFER * table); // These two methods may not work as intended, possibly leaving the I2C bus on the
void vid_getUserColorLUT(LEP_VID_LUT_BUFFER * table); // FLiR in a non-responding state. A full power cycle may be needed to reset.

void vid_setFocusRegion(LEP_VID_FOCUS_ROI * region); // min:1,1/end>beg+1, max:78,58/beg<end-1 def:{1,1,78,58} (pixels)
void vid_getFocusRegion(LEP_VID_FOCUS_ROI * region);

void vid_setFocusThreshold(uint32_t threshold); // def:30
uint32_t vid_getFocusThreshold();

uint32_t vid_getFocusMetric();

void vid_setSceneBasedNUCEnabled(uint8_t enabled); // def:enabled
uint8_t vid_getSceneBasedNUCEnabled();

void vid_setGamma(uint32_t gamma); // def:58
uint32_t vid_getGamma();

#endif

    // Module represents temperatures as kelvin x 100 (in integer format). These methods
    // convert to and from the selected temperature mode.
float kelvin100ToTemperature(uint16_t kelvin100);
uint16_t temperatureToKelvin100(float temperature);
const char *getTemperatureSymbol();

uint8_t getLastI2CError();
LEP_RESULT getLastLepResult();

void wordsToHexString(uint16_t * dataWords, int dataLength, char *buffer,
                             int maxLength);
extern float kelvin100ToCelsius(uint16_t kelvin100);
extern float kelvin100ToFahrenheit(uint16_t kelvin100);
extern float kelvin100ToKelvin(uint16_t kelvin100);
extern uint16_t celsiusToKelvin100(float celsius);
extern uint16_t fahrenheitToKelvin100(float fahrenheit);
extern uint16_t kelvinToKelvin100(float kelvin);

void LeptonFLiR_init(LeptonFLiR_ImageStorageMode storageMode,
                     LeptonFLiR_TemperatureMode tempMode,
                     struct lepton_driver * driver);

#endif
