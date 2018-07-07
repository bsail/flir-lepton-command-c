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

#include "lepton-flir.h"
#include <subsystems/os/critical.h>
#if (defined(ARDUINO_ARCH_SAM) || defined(ARDUINO_ARCH_SAMD)) && !defined(LEPFLIR_DISABLE_SCHEDULER)
#include "Scheduler.h"
#define LEPFLIR_USE_SCHEDULER           1
#endif

#define true 1
#define false 0

#define min(a,b) (((a)<(b))?(a):(b))

#define LEPFLIR_GEN_CMD_TIMEOUT         5000        // Timeout for commands to be processed
#define LEPFLIR_SPI_MAX_SPEED           20000000    // Maximum SPI speed for FLiR module
#define LEPFLIR_SPI_MIN_SPEED           2200000     // Minimum SPI speed for FLiR module
#define LEPFLIR_SPI_FRAME_PACKET_SIZE           164 // 2B ID + 2B CRC + 160B for 80x1 14bpp/8bppAGC thermal image data or telemetry data
#define LEPFLIR_SPI_FRAME_PACKET_SIZE16         82

#ifndef LEPFLIR_DISABLE_ALIGNED_MALLOC
static inline int roundUpVal16(int val) { return ((val + 15) & -16); }
static inline byte *roundUpPtr16(byte *ptr) { return ptr ? (byte *)(((uintptr_t)ptr + 15) & -16) : NULL; }
static inline byte *roundUpMalloc16(int size) { return (byte *)malloc((size_t)(size + 15)); }
static inline byte *roundUpSpiFrame16(byte *spiFrame) { return spiFrame ? roundUpPtr16(spiFrame) + 16 - 4 : NULL; }
#else
static inline int roundUpVal16(int val) { return val; }
static inline byte *roundUpPtr16(byte *ptr) { return ptr; }
static inline byte *roundUpMalloc16(int size) { return (byte *)malloc((size_t)size); }
static inline byte *roundUpSpiFrame16(byte *spiFrame) { return spiFrame; }
#endif

static inline uint8_t lowByte(uint16_t p)
{
    return (p & 0x00FF);
}

static inline uint8_t highByte(uint16_t p)
{
    return ((p & 0xFF00) >> 8);
}

#ifndef LEPFLIR_USE_SOFTWARE_I2C
void LeptonFLiR_LeptonFLiR() {
    // _i2cWire = &i2cWire;
#else
LeptonFLiR_LeptonFLiR(byte spiCSPin) {
#endif
    // _spiCSPin = spiCSPin;
    // _spiSettings = SPISettings(LEPFLIR_SPI_MAX_SPEED, MSBFIRST, SPI_MODE3);
    _storageMode = LeptonFLiR_ImageStorageMode_Count;
    // _csEnableFunc = csEnableFuncDef;
    // _csDisableFunc = csDisableFuncDef;
    // _imageData = _spiFrameData = _telemetryData = NULL;
    // _isReadingNextFrame = false;
    _lastI2CError = _lastLepResult = 0;
}

void LeptonFLiR_init(LeptonFLiR_ImageStorageMode storageMode, LeptonFLiR_TemperatureMode tempMode) {
    _storageMode = storageMode;//(LeptonFLiR_ImageStorageMode)constrain((int)storageMode, 0, (int)LeptonFLiR_ImageStorageMode_Count - 1);
    _tempMode = tempMode;//(LeptonFLiR_TemperatureMode)constrain((int)tempMode, 0, (int)LeptonFLiR_TemperatureMode_Count - 1);
}

LeptonFLiR_ImageStorageMode getImageStorageMode() {
    return _storageMode;
}

LeptonFLiR_TemperatureMode getTemperatureMode() {
    return _tempMode;
}

// void setFastCSFuncs(digitalWriteFunc csEnableFunc, digitalWriteFunc csDisableFunc) {
//     _csEnableFunc = csEnableFunc ? csEnableFunc : csEnableFuncDef;
//     _csDisableFunc = csDisableFunc ? csDisableFunc : csDisableFuncDef;
// }

int getImageWidth() {
    switch (_storageMode) {
        case LeptonFLiR_ImageStorageMode_80x60_16bpp:
        case LeptonFLiR_ImageStorageMode_80x60_8bpp:
            return 80;
        case LeptonFLiR_ImageStorageMode_40x30_16bpp:
        case LeptonFLiR_ImageStorageMode_40x30_8bpp:
            return 40;
        case LeptonFLiR_ImageStorageMode_20x15_16bpp:
        case LeptonFLiR_ImageStorageMode_20x15_8bpp:
            return 20;
        default:
            return 0;
    }
}

int getImageHeight() {
    switch (_storageMode) {
        case LeptonFLiR_ImageStorageMode_80x60_16bpp:
        case LeptonFLiR_ImageStorageMode_80x60_8bpp:
            return 60;
        case LeptonFLiR_ImageStorageMode_40x30_16bpp:
        case LeptonFLiR_ImageStorageMode_40x30_8bpp:
            return 30;
        case LeptonFLiR_ImageStorageMode_20x15_16bpp:
        case LeptonFLiR_ImageStorageMode_20x15_8bpp:
            return 15;
        default:
            return 0;
    }
}

int getImageBpp() {
    switch (_storageMode) {
        case LeptonFLiR_ImageStorageMode_80x60_16bpp:
        case LeptonFLiR_ImageStorageMode_40x30_16bpp:
        case LeptonFLiR_ImageStorageMode_20x15_16bpp:
            return 2;
        case LeptonFLiR_ImageStorageMode_80x60_8bpp:
        case LeptonFLiR_ImageStorageMode_40x30_8bpp:
        case LeptonFLiR_ImageStorageMode_20x15_8bpp:
            return 1;
        default:
            return 0;
    }
}

int getImagePitch() {
    switch (_storageMode) {
        case LeptonFLiR_ImageStorageMode_80x60_16bpp:
            return roundUpVal16(80 * 2);
        case LeptonFLiR_ImageStorageMode_80x60_8bpp:
            return roundUpVal16(80 * 1);
        case LeptonFLiR_ImageStorageMode_40x30_16bpp:
            return roundUpVal16(40 * 2);
        case LeptonFLiR_ImageStorageMode_40x30_8bpp:
            return roundUpVal16(40 * 1);
        case LeptonFLiR_ImageStorageMode_20x15_16bpp:
            return roundUpVal16(20 * 2);
        case LeptonFLiR_ImageStorageMode_20x15_8bpp:
            return roundUpVal16(20 * 1);
        default:
            return 0;
    }
}

int getImageTotalBytes() {
    return ((getImageHeight() - 1) * getImagePitch()) + (getImageWidth() * getImageBpp());
}


int getSPIFrameLines() {
    switch (_storageMode) {
        case LeptonFLiR_ImageStorageMode_80x60_16bpp:
        case LeptonFLiR_ImageStorageMode_80x60_8bpp:
            return 1;
        case LeptonFLiR_ImageStorageMode_40x30_16bpp:
        case LeptonFLiR_ImageStorageMode_40x30_8bpp:
            return 2;
        case LeptonFLiR_ImageStorageMode_20x15_16bpp:
        case LeptonFLiR_ImageStorageMode_20x15_8bpp:
            return 4;
        default:
            return 0;
    }
}

void agc_setAGCEnabled(uint8_t enabled) {
    sendCommand_u32(cmdCode(LEP_CID_AGC_ENABLE_STATE, LEP_I2C_COMMAND_TYPE_SET), (uint32_t)enabled);
}

uint8_t agc_getAGCEnabled() {
    uint32_t enabled;
    receiveCommand_u32(cmdCode(LEP_CID_AGC_ENABLE_STATE, LEP_I2C_COMMAND_TYPE_GET), &enabled);
    return enabled;
}

void agc_setAGCPolicy(LEP_AGC_POLICY policy) {
    sendCommand_u32(cmdCode(LEP_CID_AGC_POLICY, LEP_I2C_COMMAND_TYPE_SET), (uint32_t)policy);
}

LEP_AGC_POLICY agc_getAGCPolicy() {
    uint32_t policy;
    receiveCommand_u32(cmdCode(LEP_CID_AGC_POLICY, LEP_I2C_COMMAND_TYPE_GET), &policy);
    return (LEP_AGC_POLICY)policy;
}

void agc_setHEQScaleFactor(LEP_AGC_HEQ_SCALE_FACTOR factor) {
    sendCommand_u32(cmdCode(LEP_CID_AGC_HEQ_SCALE_FACTOR, LEP_I2C_COMMAND_TYPE_SET), (uint32_t)factor);
}

LEP_AGC_HEQ_SCALE_FACTOR agc_getHEQScaleFactor() {
    uint32_t factor;
    receiveCommand_u32(cmdCode(LEP_CID_AGC_HEQ_SCALE_FACTOR, LEP_I2C_COMMAND_TYPE_GET), &factor);
    return (LEP_AGC_HEQ_SCALE_FACTOR)factor;
}

void agc_setAGCCalcEnabled(uint8_t enabled) {
    sendCommand_u32(cmdCode(LEP_CID_AGC_CALC_ENABLE_STATE, LEP_I2C_COMMAND_TYPE_SET), (uint32_t)enabled);
}

uint8_t agc_getAGCCalcEnabled() {
    uint32_t enabled;
    receiveCommand_u32(cmdCode(LEP_CID_AGC_CALC_ENABLE_STATE, LEP_I2C_COMMAND_TYPE_GET), &enabled);
    return enabled;
}

void sys_getCameraStatus_internal(LEP_SYS_CAM_STATUS *status) {
    receiveCommand_array(cmdCode(LEP_CID_SYS_CAM_STATUS, LEP_I2C_COMMAND_TYPE_GET), (uint16_t *)(status), sizeof(LEP_SYS_CAM_STATUS) / 2);
}

LEP_SYS_CAM_STATUS_STATES sys_getCameraStatus() {
    LEP_SYS_CAM_STATUS camStatus;
    sys_getCameraStatus_internal(&camStatus);
    return (LEP_SYS_CAM_STATUS_STATES)camStatus.camStatus;
}

void sys_getFlirSerialNumber(char *buffer, int maxLength) {
    if (!buffer || maxLength < 16) return;
    uint16_t innerBuffer[4];
    receiveCommand_array(cmdCode(LEP_CID_SYS_FLIR_SERIAL_NUMBER, LEP_I2C_COMMAND_TYPE_GET), innerBuffer, 4);
    wordsToHexString(innerBuffer, 4, buffer, maxLength);
}

void sys_getCustomerSerialNumber(char *buffer, int maxLength) {
    if (!buffer || maxLength < 64) return;
    uint16_t innerBuffer[16];
    receiveCommand_array(cmdCode(LEP_CID_SYS_CUST_SERIAL_NUMBER, LEP_I2C_COMMAND_TYPE_GET), innerBuffer, 16);
    wordsToHexString(innerBuffer, 16, buffer, maxLength);
}

uint32_t sys_getCameraUptime() {
    uint32_t uptime = 0;
    receiveCommand_u32(cmdCode(LEP_CID_SYS_CAM_UPTIME, LEP_I2C_COMMAND_TYPE_GET), &uptime);
    return uptime;
}

float sys_getAuxTemperature() {
    uint16_t kelvin100;
    receiveCommand_u16(cmdCode(LEP_CID_SYS_AUX_TEMPERATURE_KELVIN, LEP_I2C_COMMAND_TYPE_GET), &kelvin100);
    return kelvin100ToTemperature(kelvin100);
}

float sys_getFPATemperature() {
    uint16_t kelvin100;
    receiveCommand_u16(cmdCode(LEP_CID_SYS_FPA_TEMPERATURE_KELVIN, LEP_I2C_COMMAND_TYPE_GET), &kelvin100);
    return kelvin100ToTemperature(kelvin100);
}

void sys_setTelemetryEnabled(uint8_t enabled) {
    sendCommand_u32(cmdCode(LEP_CID_SYS_TELEMETRY_ENABLE_STATE, LEP_I2C_COMMAND_TYPE_SET), (uint32_t)enabled);
}

uint8_t sys_getTelemetryEnabled() {
    uint32_t enabled;
    receiveCommand_u32(cmdCode(LEP_CID_SYS_TELEMETRY_ENABLE_STATE, LEP_I2C_COMMAND_TYPE_GET), &enabled);
    return enabled;
}

void sys_runFFCNormalization() {
    sendCommand_raw(cmdCode(LEP_CID_SYS_RUN_FFC, LEP_I2C_COMMAND_TYPE_RUN));
}

void vid_setPolarity(LEP_VID_POLARITY polarity) {
    sendCommand_u32(cmdCode(LEP_CID_VID_POLARITY_SELECT, LEP_I2C_COMMAND_TYPE_SET), (uint32_t)polarity);
}

LEP_VID_POLARITY vid_getPolarity() {
    uint32_t polarity;
    receiveCommand_u32(cmdCode(LEP_CID_VID_POLARITY_SELECT, LEP_I2C_COMMAND_TYPE_GET), &polarity);
    return (LEP_VID_POLARITY)polarity;
}

void vid_setPseudoColorLUT(LEP_VID_PCOLOR_LUT table) {
    sendCommand_u32(cmdCode(LEP_CID_VID_LUT_SELECT, LEP_I2C_COMMAND_TYPE_SET), (uint32_t)table);
}

LEP_VID_PCOLOR_LUT vid_getPseudoColorLUT() {
    uint32_t table;
    receiveCommand_u32(cmdCode(LEP_CID_VID_LUT_SELECT, LEP_I2C_COMMAND_TYPE_GET), &table);
    return (LEP_VID_PCOLOR_LUT)table;
}

void vid_setFocusCalcEnabled(uint8_t enabled) {
    sendCommand_u32(cmdCode(LEP_CID_VID_FOCUS_CALC_ENABLE, LEP_I2C_COMMAND_TYPE_SET), (uint32_t)enabled);
}

uint8_t vid_getFocusCalcEnabled() {
    uint32_t enabled;
    receiveCommand_u32(cmdCode(LEP_CID_VID_FOCUS_CALC_ENABLE, LEP_I2C_COMMAND_TYPE_GET), &enabled);
    return enabled;
}

void vid_setFreezeEnabled(uint8_t enabled) {
    sendCommand_u32(cmdCode(LEP_CID_VID_FREEZE_ENABLE, LEP_I2C_COMMAND_TYPE_SET), (uint32_t)enabled);
}

uint8_t vid_getFreezeEnabled() {
    uint32_t enabled;
    receiveCommand_u32(cmdCode(LEP_CID_VID_FREEZE_ENABLE, LEP_I2C_COMMAND_TYPE_GET), &enabled);
    return enabled;
}

#ifndef LEPFLIR_EXCLUDE_EXT_I2C_FUNCS

void agc_setHistogramRegion(LEP_AGC_HISTOGRAM_ROI *region) {
    if (!region) return;
    sendCommand_array(cmdCode(LEP_CID_AGC_ROI, LEP_I2C_COMMAND_TYPE_SET), (uint16_t *)region, sizeof(LEP_AGC_HISTOGRAM_ROI) / 2);
}

void agc_getHistogramRegion(LEP_AGC_HISTOGRAM_ROI *region) {
    if (!region) return;
    receiveCommand_array(cmdCode(LEP_CID_AGC_ROI, LEP_I2C_COMMAND_TYPE_GET), (uint16_t *)region, sizeof(LEP_AGC_HISTOGRAM_ROI) / 2);
}

void agc_getHistogramStatistics(LEP_AGC_HISTOGRAM_STATISTICS *statistics) {
    if (!statistics) return;
    receiveCommand_array(cmdCode(LEP_CID_AGC_STATISTICS, LEP_I2C_COMMAND_TYPE_GET), (uint16_t *)statistics, sizeof(LEP_AGC_HISTOGRAM_STATISTICS) / 2);
}

void agc_setHistogramClipPercent(uint16_t percent) {
    sendCommand_u16(cmdCode(LEP_CID_AGC_HISTOGRAM_CLIP_PERCENT, LEP_I2C_COMMAND_TYPE_SET), percent);
}

uint16_t agc_getHistogramClipPercent() {
    uint16_t percent;
    receiveCommand_u16(cmdCode(LEP_CID_AGC_HISTOGRAM_CLIP_PERCENT, LEP_I2C_COMMAND_TYPE_GET), &percent);
    return percent;
}

void agc_setHistogramTailSize(uint16_t size) {
    sendCommand_u16(cmdCode(LEP_CID_AGC_HISTOGRAM_TAIL_SIZE, LEP_I2C_COMMAND_TYPE_SET), size);
}

uint16_t agc_getHistogramTailSize() {
    uint16_t size;
    receiveCommand_u16(cmdCode(LEP_CID_AGC_HISTOGRAM_TAIL_SIZE, LEP_I2C_COMMAND_TYPE_GET), &size);
    return size;
}

void agc_setLinearMaxGain(uint16_t gain) {
    sendCommand_u16(cmdCode(LEP_CID_AGC_LINEAR_MAX_GAIN, LEP_I2C_COMMAND_TYPE_SET), gain);
}

uint16_t agc_getLinearMaxGain() {
    uint16_t gain;
    receiveCommand_u16(cmdCode(LEP_CID_AGC_LINEAR_MAX_GAIN, LEP_I2C_COMMAND_TYPE_GET), &gain);
    return gain;
}

void agc_setLinearMidpoint(uint16_t midpoint) {
    sendCommand_u16(cmdCode(LEP_CID_AGC_LINEAR_MIDPOINT, LEP_I2C_COMMAND_TYPE_SET), midpoint);
}

uint16_t agc_getLinearMidpoint() {
    uint16_t midpoint;
    receiveCommand_u16(cmdCode(LEP_CID_AGC_LINEAR_MIDPOINT, LEP_I2C_COMMAND_TYPE_GET), &midpoint);
    return midpoint;
}

void agc_setLinearDampeningFactor(uint16_t factor) {
    sendCommand_u16(cmdCode(LEP_CID_AGC_LINEAR_DAMPENING_FACTOR, LEP_I2C_COMMAND_TYPE_SET), factor);
}

uint16_t agc_getLinearDampeningFactor() {
    uint16_t factor;
    receiveCommand_u16(cmdCode(LEP_CID_AGC_LINEAR_DAMPENING_FACTOR, LEP_I2C_COMMAND_TYPE_GET), &factor);
    return factor;
}

void agc_setHEQDampeningFactor(uint16_t factor) {
    sendCommand_u16(cmdCode(LEP_CID_AGC_HEQ_DAMPENING_FACTOR, LEP_I2C_COMMAND_TYPE_SET), factor);
}

uint16_t agc_getHEQDampeningFactor() {
    uint16_t factor;
    receiveCommand_u16(cmdCode(LEP_CID_AGC_HEQ_DAMPENING_FACTOR, LEP_I2C_COMMAND_TYPE_GET), &factor);
    return factor;
}

void agc_setHEQMaxGain(uint16_t gain) {
    sendCommand_u16(cmdCode(LEP_CID_AGC_HEQ_MAX_GAIN, LEP_I2C_COMMAND_TYPE_SET), gain);
}

uint16_t agc_getHEQMaxGain() {
    uint16_t gain;
    receiveCommand_u16(cmdCode(LEP_CID_AGC_HEQ_MAX_GAIN, LEP_I2C_COMMAND_TYPE_GET), &gain);
    return gain;
}

void agc_setHEQClipLimitHigh(uint16_t limit) {
    sendCommand_u16(cmdCode(LEP_CID_AGC_HEQ_CLIP_LIMIT_HIGH, LEP_I2C_COMMAND_TYPE_SET), limit);
}

uint16_t agc_getHEQClipLimitHigh() {
    uint16_t limit;
    receiveCommand_u16(cmdCode(LEP_CID_AGC_HEQ_CLIP_LIMIT_HIGH, LEP_I2C_COMMAND_TYPE_GET), &limit);
    return limit;
}

void agc_setHEQClipLimitLow(uint16_t limit) {
    sendCommand_u16(cmdCode(LEP_CID_AGC_HEQ_CLIP_LIMIT_LOW, LEP_I2C_COMMAND_TYPE_SET), limit);
}

uint16_t agc_getHEQClipLimitLow() {
    uint16_t limit;
    receiveCommand_u16(cmdCode(LEP_CID_AGC_HEQ_CLIP_LIMIT_LOW, LEP_I2C_COMMAND_TYPE_GET), &limit);
    return limit;
}

void agc_setHEQBinExtension(uint16_t extension) {
    sendCommand_u16(cmdCode(LEP_CID_AGC_HEQ_BIN_EXTENSION, LEP_I2C_COMMAND_TYPE_SET), extension);
}

uint16_t agc_getHEQBinExtension() {
    uint16_t extension;
    receiveCommand_u16(cmdCode(LEP_CID_AGC_HEQ_BIN_EXTENSION, LEP_I2C_COMMAND_TYPE_GET), &extension);
    return extension;
}

void agc_setHEQMidpoint(uint16_t midpoint) {
    sendCommand_u16(cmdCode(LEP_CID_AGC_HEQ_MIDPOINT, LEP_I2C_COMMAND_TYPE_SET), midpoint);
}

uint16_t agc_getHEQMidpoint() {
    uint16_t midpoint;
    receiveCommand_u16(cmdCode(LEP_CID_AGC_HEQ_MIDPOINT, LEP_I2C_COMMAND_TYPE_GET), &midpoint);
    return midpoint;
}

void agc_setHEQEmptyCounts(uint16_t counts) {
    sendCommand_u16(cmdCode(LEP_CID_AGC_HEQ_EMPTY_COUNTS, LEP_I2C_COMMAND_TYPE_SET), counts);
}

uint16_t agc_getHEQEmptyCounts() {
    uint16_t counts;
    receiveCommand_u16(cmdCode(LEP_CID_AGC_HEQ_EMPTY_COUNTS, LEP_I2C_COMMAND_TYPE_GET), &counts);
    return counts;
}

void agc_setHEQNormalizationFactor(uint16_t factor) {
    sendCommand_u16(cmdCode(LEP_CID_AGC_HEQ_NORMALIZATION_FACTOR, LEP_I2C_COMMAND_TYPE_SET), factor);
}

uint16_t agc_getHEQNormalizationFactor() {
    uint16_t factor;
    receiveCommand_u16(cmdCode(LEP_CID_AGC_HEQ_NORMALIZATION_FACTOR, LEP_I2C_COMMAND_TYPE_GET), &factor);
    return factor;
}

void sys_runPingCamera() {
    sendCommand_raw(cmdCode(LEP_CID_SYS_PING, LEP_I2C_COMMAND_TYPE_RUN));
}

void sys_setTelemetryLocation(LEP_SYS_TELEMETRY_LOCATION location) {
    sendCommand_u32(cmdCode(LEP_CID_SYS_TELEMETRY_LOCATION, LEP_I2C_COMMAND_TYPE_SET), (uint32_t)location);
}

LEP_SYS_TELEMETRY_LOCATION sys_getTelemetryLocation() {
    uint32_t location;
    receiveCommand_u32(cmdCode(LEP_CID_SYS_TELEMETRY_LOCATION, LEP_I2C_COMMAND_TYPE_GET), &location);
    return (LEP_SYS_TELEMETRY_LOCATION)location;
}

void sys_runFrameAveraging() {
    sendCommand_raw(cmdCode(LEP_CID_SYS_EXECTUE_FRAME_AVERAGE, LEP_I2C_COMMAND_TYPE_RUN));
}

void sys_setNumFramesToAverage(LEP_SYS_FRAME_AVERAGE average) {
    sendCommand_u32(cmdCode(LEP_CID_SYS_NUM_FRAMES_TO_AVERAGE, LEP_I2C_COMMAND_TYPE_SET), (uint32_t)average);
}

LEP_SYS_FRAME_AVERAGE sys_getNumFramesToAverage() {
    uint32_t average;
    receiveCommand_u32(cmdCode(LEP_CID_SYS_NUM_FRAMES_TO_AVERAGE, LEP_I2C_COMMAND_TYPE_GET), &average);
    return (LEP_SYS_FRAME_AVERAGE)average;
}

void sys_getSceneStatistics(LEP_SYS_SCENE_STATISTICS *statistics) {
    if (!statistics) return;
    receiveCommand_array(cmdCode(LEP_CID_SYS_SCENE_STATISTICS, LEP_I2C_COMMAND_TYPE_GET), (uint16_t *)statistics, sizeof(LEP_SYS_SCENE_STATISTICS) / 2);
}

void sys_setSceneRegion(LEP_SYS_SCENE_ROI *region) {
    if (!region) return;
    sendCommand_array(cmdCode(LEP_CID_SYS_SCENE_ROI, LEP_I2C_COMMAND_TYPE_SET), (uint16_t *)region, sizeof(LEP_SYS_SCENE_ROI) / 2);
}

void sys_getSceneRegion(LEP_SYS_SCENE_ROI *region) {
    if (!region) return;
    receiveCommand_array(cmdCode(LEP_CID_SYS_SCENE_ROI, LEP_I2C_COMMAND_TYPE_GET), (uint16_t *)region, sizeof(LEP_SYS_SCENE_ROI) / 2);
}

uint16_t sys_getThermalShutdownCount() {
    uint16_t count;
    receiveCommand_u16(cmdCode(LEP_CID_SYS_THERMAL_SHUTDOWN_COUNT, LEP_I2C_COMMAND_TYPE_GET), &count);
    return count;
}

void sys_setShutterPosition(LEP_SYS_SHUTTER_POSITION position) {
    sendCommand_u32(cmdCode(LEP_CID_SYS_SHUTTER_POSITION, LEP_I2C_COMMAND_TYPE_SET), (uint32_t)position);
}

LEP_SYS_SHUTTER_POSITION sys_getShutterPosition() {
    uint32_t position;
    receiveCommand_u32(cmdCode(LEP_CID_SYS_SHUTTER_POSITION, LEP_I2C_COMMAND_TYPE_GET), &position);
    return (LEP_SYS_SHUTTER_POSITION)position;
}

void sys_setFFCShutterMode(LEP_SYS_FFC_SHUTTER_MODE *mode) {
    if (!mode) return;
    sendCommand_array(cmdCode(LEP_CID_SYS_FFC_SHUTTER_MODE, LEP_I2C_COMMAND_TYPE_SET), (uint16_t *)mode, sizeof(LEP_SYS_FFC_SHUTTER_MODE) / 2);
}

void sys_getFFCShutterMode(LEP_SYS_FFC_SHUTTER_MODE *mode) {
    if (!mode) return;
    receiveCommand_array(cmdCode(LEP_CID_SYS_FFC_SHUTTER_MODE, LEP_I2C_COMMAND_TYPE_GET), (uint16_t *)mode, sizeof(LEP_SYS_FFC_SHUTTER_MODE) / 2);
}

LEP_SYS_FFC_STATUS sys_getFFCNormalizationStatus() {
    uint32_t status;
    receiveCommand_u32(cmdCode(LEP_CID_SYS_FFC_STATUS, LEP_I2C_COMMAND_TYPE_GET), &status);
    return (LEP_SYS_FFC_STATUS)status;
}

void vid_setUserColorLUT(LEP_VID_LUT_BUFFER *table) {
    if (!table) return;
    sendCommand_array(cmdCode(LEP_CID_VID_LUT_TRANSFER, LEP_I2C_COMMAND_TYPE_SET), (uint16_t *)table, sizeof(LEP_VID_LUT_BUFFER) / 2);
}

void vid_getUserColorLUT(LEP_VID_LUT_BUFFER *table) {
    if (!table) return;
    receiveCommand_array(cmdCode(LEP_CID_VID_LUT_TRANSFER, LEP_I2C_COMMAND_TYPE_GET), (uint16_t *)table, sizeof(LEP_VID_LUT_BUFFER) / 2);
}

void vid_setFocusRegion(LEP_VID_FOCUS_ROI *region) {
    if (!region) return;
    sendCommand_array(cmdCode(LEP_CID_VID_FOCUS_ROI, LEP_I2C_COMMAND_TYPE_SET), (uint16_t *)region, sizeof(LEP_VID_FOCUS_ROI) / 2);
}

void vid_getFocusRegion(LEP_VID_FOCUS_ROI *region) {
    if (!region) return;
    receiveCommand_array(cmdCode(LEP_CID_VID_FOCUS_ROI, LEP_I2C_COMMAND_TYPE_GET), (uint16_t *)region, sizeof(LEP_VID_FOCUS_ROI) / 2);
}

void vid_setFocusThreshold(uint32_t threshold) {
    sendCommand_u32(cmdCode(LEP_CID_VID_FOCUS_THRESHOLD, LEP_I2C_COMMAND_TYPE_SET), threshold);
}

uint32_t vid_getFocusThreshold() {
    uint32_t threshold;
    receiveCommand_u32(cmdCode(LEP_CID_VID_FOCUS_THRESHOLD, LEP_I2C_COMMAND_TYPE_GET), &threshold);
    return threshold;
}

uint32_t vid_getFocusMetric() {
    uint32_t metric;
    receiveCommand_u32(cmdCode(LEP_CID_VID_FOCUS_METRIC, LEP_I2C_COMMAND_TYPE_GET), &metric);
    return metric;
}

void vid_setSceneBasedNUCEnabled(uint8_t enabled) {
    sendCommand_u32(cmdCode(LEP_CID_VID_SBNUC_ENABLE, LEP_I2C_COMMAND_TYPE_SET), (uint32_t)enabled);
}

uint8_t vid_getSceneBasedNUCEnabled() {
    uint32_t enabled;
    receiveCommand_u32(cmdCode(LEP_CID_VID_SBNUC_ENABLE, LEP_I2C_COMMAND_TYPE_GET), &enabled);
    return enabled;
}

void vid_setGamma(uint32_t gamma) {
    sendCommand_u32(cmdCode(LEP_CID_VID_GAMMA_SELECT, LEP_I2C_COMMAND_TYPE_SET), gamma);
}

uint32_t vid_getGamma() {
    uint32_t gamma;
    receiveCommand_u32(cmdCode(LEP_CID_VID_GAMMA_SELECT, LEP_I2C_COMMAND_TYPE_GET), &gamma);
    return gamma;
}

#endif

static inline void byteToHexString(byte value, char *buffer) {
    byte highNibble = value / 16;
    byte lowNibble = value % 16;
    if (highNibble < 10) buffer[0] = '0' + highNibble;
    else buffer[0] = 'A' + (highNibble - 10);
    if (lowNibble < 10) buffer[1] = '0' + lowNibble;
    else buffer[1] = 'A' + (lowNibble - 10);
}

void wordsToHexString(uint16_t *dataWords, int dataLength, char *buffer, int maxLength) {
    uint8_t insertColons = maxLength >= (dataLength * 4) + (dataLength - 1);

    while (dataLength-- > 0 && maxLength > 3) {
        if (maxLength > 3) {
            byteToHexString(highByte(*dataWords), buffer);
            buffer += 2; maxLength -= 2;
            byteToHexString(lowByte(*dataWords), buffer);
            buffer += 2; maxLength -= 2;
            ++dataWords;
        }

        if (dataLength > 0 && insertColons && maxLength-- > 0)
            *buffer++ = ':';
    }

    if (maxLength-- > 0)
        *buffer++ = '\0';
}

float kelvin100ToCelsius(uint16_t kelvin100) {
    float kelvin = (kelvin100 / 100) + ((kelvin100 % 100) * 0.01f);
    return kelvin - 273.15f;
}

float kelvin100ToFahrenheit(uint16_t kelvin100) {
    float kelvin = (kelvin100 / 100) + ((kelvin100 % 100) * 0.01f);
    return roundf((((kelvin * 9.0f) / 5.0f) - 459.67f) * 100.0f) / 100.0f;
}

float kelvin100ToKelvin(uint16_t kelvin100) {
    return (kelvin100 / 100) + ((kelvin100 % 100) * 0.01f);
}

uint16_t celsiusToKelvin100(float celsius) {
    float kelvin = celsius + 273.15f;
    return (uint16_t)roundf(kelvin * 100.0f);
}

uint16_t fahrenheitToKelvin100(float fahrenheit) {
    float kelvin = ((fahrenheit + 459.67f) * 5.0f) / 9.0f;
    return (uint16_t)roundf(kelvin * 100.0f);
}

uint16_t kelvinToKelvin100(float kelvin) {
    return (uint16_t)roundf(kelvin * 100.0f);
}

float kelvin100ToTemperature(uint16_t kelvin100) {
    switch (_tempMode) {
        case LeptonFLiR_TemperatureMode_Celsius:
            return kelvin100ToCelsius(kelvin100);
        case LeptonFLiR_TemperatureMode_Fahrenheit:
            return kelvin100ToFahrenheit(kelvin100);
        case LeptonFLiR_TemperatureMode_Kelvin:
            return kelvin100ToKelvin(kelvin100);
        default:
            return 0;
    }
}

uint16_t temperatureToKelvin100(float temperature) {
    switch (_tempMode) {
        case LeptonFLiR_TemperatureMode_Celsius:
            return celsiusToKelvin100(temperature);
        case LeptonFLiR_TemperatureMode_Fahrenheit:
            return fahrenheitToKelvin100(temperature);
        case LeptonFLiR_TemperatureMode_Kelvin:
            return kelvinToKelvin100(temperature);
        default:
            return 0;
    }
}

const char *getTemperatureSymbol() {
    switch (_tempMode) {
        case LeptonFLiR_TemperatureMode_Celsius:
            return "°C";
        case LeptonFLiR_TemperatureMode_Fahrenheit:
            return "°F";
        case LeptonFLiR_TemperatureMode_Kelvin:
            return "°K";
        default:
            return "";
    }
}

byte getLastI2CError() {
    return _lastI2CError;
}

LEP_RESULT getLastLepResult() {
    return (LEP_RESULT)_lastLepResult;
}

#ifdef LEPFLIR_ENABLE_DEBUG_OUTPUT

static const char *textForI2CError(byte errorCode) {
    switch (errorCode) {
        case 0:
            return "Success";
        case 1:
            return "Data too long to fit in transmit buffer";
        case 2:
            return "Received NACK on transmit of address";
        case 3:
            return "Received NACK on transmit of data";
        default:
            return "Other error";
    }
}

static const char *textForLepResult(LEP_RESULT errorCode) {
    switch (errorCode) {
        case LEP_OK:
            return "LEP_OK Camera ok";
        case LEP_ERROR:
            return "LEP_ERROR Camera general error";
        case LEP_NOT_READY:
            return "LEP_NOT_READY Camera not ready error";
        case LEP_RANGE_ERROR:
            return "LEP_RANGE_ERROR Camera range error";
        case LEP_CHECKSUM_ERROR:
            return "LEP_CHECKSUM_ERROR Camera checksum error";
        case LEP_BAD_ARG_POINTER_ERROR:
            return "LEP_BAD_ARG_POINTER_ERROR Camera Bad argument  error";
        case LEP_DATA_SIZE_ERROR:
            return "LEP_DATA_SIZE_ERROR Camera byte count error";
        case LEP_UNDEFINED_FUNCTION_ERROR:
            return "LEP_UNDEFINED_FUNCTION_ERROR Camera undefined function error";
        case LEP_FUNCTION_NOT_SUPPORTED:
            return "LEP_FUNCTION_NOT_SUPPORTED Camera function not yet supported error";
        case LEP_OTP_WRITE_ERROR:
            return "LEP_OTP_WRITE_ERROR Camera OTP write error";
        case LEP_OTP_READ_ERROR:
            return "LEP_OTP_READ_ERROR Double bit error detected (uncorrectible)";
        case LEP_OTP_NOT_PROGRAMMED_ERROR:
            return "LEP_OTP_NOT_PROGRAMMED_ERROR Flag read as non-zero";
        case LEP_ERROR_I2C_BUS_NOT_READY:
            return "LEP_ERROR_I2C_BUS_NOT_READY I2C Bus Error - Bus Not Avaialble";
        case LEP_ERROR_I2C_BUFFER_OVERFLOW:
            return "LEP_ERROR_I2C_BUFFER_OVERFLOW I2C Bus Error - Buffer Overflow";
        case LEP_ERROR_I2C_ARBITRATION_LOST:
            return "LEP_ERROR_I2C_ARBITRATION_LOST I2C Bus Error - Bus Arbitration Lost";
        case LEP_ERROR_I2C_BUS_ERROR:
            return "LEP_ERROR_I2C_BUS_ERROR I2C Bus Error - General Bus Error";
        case LEP_ERROR_I2C_NACK_RECEIVED:
            return "LEP_ERROR_I2C_NACK_RECEIVED I2C Bus Error - NACK Received";
        case LEP_ERROR_I2C_FAIL:
            return "LEP_ERROR_I2C_FAIL I2C Bus Error - General Failure";
        case LEP_DIV_ZERO_ERROR:
            return "LEP_DIV_ZERO_ERROR Attempted div by zero";
        case LEP_COMM_PORT_NOT_OPEN:
            return "LEP_COMM_PORT_NOT_OPEN Comm port not open";
        case LEP_COMM_INVALID_PORT_ERROR:
            return "LEP_COMM_INVALID_PORT_ERROR Comm port no such port error";
        case LEP_COMM_RANGE_ERROR:
            return "LEP_COMM_RANGE_ERROR Comm port range error";
        case LEP_ERROR_CREATING_COMM:
            return "LEP_ERROR_CREATING_COMM Error creating comm";
        case LEP_ERROR_STARTING_COMM:
            return "LEP_ERROR_STARTING_COMM Error starting comm";
        case LEP_ERROR_CLOSING_COMM:
            return "LEP_ERROR_CLOSING_COMM Error closing comm";
        case LEP_COMM_CHECKSUM_ERROR:
            return "LEP_COMM_CHECKSUM_ERROR Comm checksum error";
        case LEP_COMM_NO_DEV:
            return "LEP_COMM_NO_DEV No comm device";
        case LEP_TIMEOUT_ERROR:
            return "LEP_TIMEOUT_ERROR Comm timeout error";
        case LEP_COMM_ERROR_WRITING_COMM:
            return "LEP_COMM_ERROR_WRITING_COMM Error writing comm";
        case LEP_COMM_ERROR_READING_COMM:
            return "LEP_COMM_ERROR_READING_COMM Error reading comm";
        case LEP_COMM_COUNT_ERROR:
            return "LEP_COMM_COUNT_ERROR Comm byte count error";
        case LEP_OPERATION_CANCELED:
            return "LEP_OPERATION_CANCELED Camera operation canceled";
        case LEP_UNDEFINED_ERROR_CODE:
            return "LEP_UNDEFINED_ERROR_CODE Undefined error";
        default:
            return "Other error";
    }
}

#endif

uint8_t waitCommandBegin(int timeout) {
    _lastLepResult = 0;
    uint16_t status;
    if (readRegister(LEP_I2C_STATUS_REG, &status))
        return false;

    if (!(status & LEP_I2C_STATUS_BUSY_BIT_MASK))
        return true;

    unsigned long endTime = millis() + (unsigned long)timeout;

    while ((status & LEP_I2C_STATUS_BUSY_BIT_MASK) && (timeout <= 0 || millis() < endTime)) {
#ifdef LEPFLIR_USE_SCHEDULER
        Scheduler.yield();
#else
        delay(1);
#endif

        if (readRegister(LEP_I2C_STATUS_REG, &status))
            return false;
    }

    if (!(status & LEP_I2C_STATUS_BUSY_BIT_MASK))
        return true;
    else {
        _lastLepResult = LEP_TIMEOUT_ERROR;
        return false;
    }
}

uint8_t waitCommandFinish(int timeout) {
    uint16_t status;
    if (readRegister(LEP_I2C_STATUS_REG, &status))
        return false;

    if (!(status & LEP_I2C_STATUS_BUSY_BIT_MASK)) {
        _lastLepResult = (byte)((status & LEP_I2C_STATUS_ERROR_CODE_BIT_MASK) >> LEP_I2C_STATUS_ERROR_CODE_BIT_SHIFT);
        return true;
    }

    unsigned long endTime = millis() + (unsigned long)timeout;

    while ((status & LEP_I2C_STATUS_BUSY_BIT_MASK) && (timeout <= 0 || millis() < endTime)) {
#ifdef LEPFLIR_USE_SCHEDULER
        Scheduler.yield();
#else
        delay(1);
#endif

        if (readRegister(LEP_I2C_STATUS_REG, &status))
            return false;
    }

    if (!(status & LEP_I2C_STATUS_BUSY_BIT_MASK)) {
        _lastLepResult = (byte)((status & LEP_I2C_STATUS_ERROR_CODE_BIT_MASK) >> LEP_I2C_STATUS_ERROR_CODE_BIT_SHIFT);
        return true;
    }
    else {
        _lastLepResult = LEP_TIMEOUT_ERROR;
        return false;
    }
}

uint16_t cmdCode(uint16_t cmdID, uint16_t cmdType) {
    return (cmdID & LEP_I2C_COMMAND_MODULE_ID_BIT_MASK) | (cmdID & LEP_I2C_COMMAND_ID_BIT_MASK) | (cmdType & LEP_I2C_COMMAND_TYPE_BIT_MASK);
}

void sendCommand_raw(uint16_t cmdCode) {
    if (waitCommandBegin(LEPFLIR_GEN_CMD_TIMEOUT)) {

        if (writeCmdRegister(cmdCode, NULL, 0) == 0) {

            waitCommandFinish(LEPFLIR_GEN_CMD_TIMEOUT);
        }
    }
}

void sendCommand_u16(uint16_t cmdCode, uint16_t value) {
    if (waitCommandBegin(LEPFLIR_GEN_CMD_TIMEOUT)) {

        if (writeCmdRegister(cmdCode, &value, 1) == 0) {

            waitCommandFinish(LEPFLIR_GEN_CMD_TIMEOUT);
        }
    }
}

void sendCommand_u32(uint16_t cmdCode, uint32_t value) {
    if (waitCommandBegin(LEPFLIR_GEN_CMD_TIMEOUT)) {

        if (writeCmdRegister(cmdCode, (uint16_t *)&value, 2) == 0) {

            waitCommandFinish(LEPFLIR_GEN_CMD_TIMEOUT);
        }
    }
}

void sendCommand_array(uint16_t cmdCode, uint16_t *dataWords, int dataLength) {
    if (waitCommandBegin(LEPFLIR_GEN_CMD_TIMEOUT)) {

        if (writeCmdRegister(cmdCode, dataWords, dataLength) == 0) {

            waitCommandFinish(LEPFLIR_GEN_CMD_TIMEOUT);
        }
    }
}

void receiveCommand_u16(uint16_t cmdCode, uint16_t *value) {
    if (waitCommandBegin(LEPFLIR_GEN_CMD_TIMEOUT)) {

        if (writeRegister(LEP_I2C_COMMAND_REG, cmdCode) == 0) {

            if (waitCommandFinish(LEPFLIR_GEN_CMD_TIMEOUT)) {

                readDataRegister(value, 1);
            }
        }
    }
}

void receiveCommand_u32(uint16_t cmdCode, uint32_t *value) {
    if (waitCommandBegin(LEPFLIR_GEN_CMD_TIMEOUT)) {

        if (writeRegister(LEP_I2C_COMMAND_REG, cmdCode) == 0) {

            if (waitCommandFinish(LEPFLIR_GEN_CMD_TIMEOUT)) {

                readDataRegister((uint16_t *)value, 2);
            }
        }
    }
}

void receiveCommand_array(uint16_t cmdCode, uint16_t *readWords, int maxLength) {
    if (waitCommandBegin(LEPFLIR_GEN_CMD_TIMEOUT)) {

        if (writeRegister(LEP_I2C_COMMAND_REG, cmdCode) == 0) {

            if (waitCommandFinish(LEPFLIR_GEN_CMD_TIMEOUT)) {

                readDataRegister(readWords, maxLength);
            }
        }
    }
}

int writeCmdRegister(uint16_t cmdCode, uint16_t *dataWords, int dataLength) {
    // In avr/libraries/Wire.h and avr/libraries/utility/twi.h, BUFFER_LENGTH controls
    // how many words can be written at once. Therefore, we loop around until all words
    // have been written out into their registers.

    if (dataWords && dataLength) {
        i2cWire_beginTransmission(LEP_I2C_DEVICE_ADDRESS);
        i2cWire_write16(LEP_I2C_DATA_LENGTH_REG);
        i2cWire_write16(dataLength);
        if (i2cWire_endTransmission())
            return _lastI2CError;

        int maxLength = BUFFER_LENGTH / 2;
        int writeLength = min(maxLength, dataLength);
        uint16_t regAddress = dataLength <= 16 ? LEP_I2C_DATA_0_REG : LEP_I2C_DATA_BUFFER;

        while (dataLength > 0) {
            i2cWire_beginTransmission(LEP_I2C_DEVICE_ADDRESS);
            i2cWire_write16(regAddress);

            while (writeLength-- > 0)
                i2cWire_write16(*dataWords++);

            if (i2cWire_endTransmission())
                return _lastI2CError;

            regAddress += maxLength * 0x02;
            dataLength -= maxLength;
            writeLength = min(maxLength, dataLength);
        }
    }

    i2cWire_beginTransmission(LEP_I2C_DEVICE_ADDRESS);
    i2cWire_write16(LEP_I2C_COMMAND_REG);
    i2cWire_write16(cmdCode);
    return i2cWire_endTransmission();
}

int readDataRegister(uint16_t *readWords, int maxLength) {
    i2cWire_beginTransmission(LEP_I2C_DEVICE_ADDRESS);
    i2cWire_write16(LEP_I2C_DATA_LENGTH_REG);
    if (i2cWire_endTransmission())
        return _lastI2CError;

    int bytesRead = i2cWire_requestFrom(LEP_I2C_DEVICE_ADDRESS, 2);
    if (bytesRead != 2) {
        while (bytesRead-- > 0)
            i2cWire_read();
        return (_lastI2CError = 4);
    }

    int readLength = i2cWire_read16();

    if (readLength == 0)
        return (_lastI2CError = 4);

    // In avr/libraries/Wire.h and avr/libraries/utility/twi.h, BUFFER_LENGTH controls
    // how many words can be read at once. Therefore, we loop around until all words
    // have been read out from their registers.

    bytesRead = i2cWire_requestFrom(LEP_I2C_DEVICE_ADDRESS, min(BUFFER_LENGTH, readLength));

    while (bytesRead > 0 && readLength > 0) {

        while (bytesRead > 1 && readLength > 1 && maxLength > 0) {
            *readWords++ = i2cWire_read16();
            bytesRead -= 2; readLength -= 2; --maxLength;
        }

        if (readLength > 0)
            bytesRead += i2cWire_requestFrom(LEP_I2C_DEVICE_ADDRESS, min(BUFFER_LENGTH, readLength));
    }

    while (bytesRead-- > 0)
        i2cWire_read();

    while (maxLength-- > 0)
        *readWords++ = 0;

    return (_lastI2CError = readLength ? 4 : 0);
}

int writeRegister(uint16_t regAddress, uint16_t value) {
    i2cWire_beginTransmission(LEP_I2C_DEVICE_ADDRESS);
    i2cWire_write16(regAddress);
    i2cWire_write16(value);
    return i2cWire_endTransmission();
}

int readRegister(uint16_t regAddress, uint16_t *value) {
    i2cWire_beginTransmission(LEP_I2C_DEVICE_ADDRESS);
    i2cWire_write16(regAddress);
    if (i2cWire_endTransmission())
        return _lastI2CError;

    int bytesRead = i2cWire_requestFrom(LEP_I2C_DEVICE_ADDRESS, 2);
    if (bytesRead != 2) {
        while (bytesRead-- > 0)
            i2cWire_read();
        return (_lastI2CError = 4);
    }

    *value = i2cWire_read16();

    return _lastI2CError;
}

#ifdef LEPFLIR_USE_SOFTWARE_I2C
uint8_t __attribute__((noinline)) i2c_start(uint8_t addr);
void __attribute__((noinline)) i2c_stop(void) asm("ass_i2c_stop");
uint8_t __attribute__((noinline)) i2c_write(uint8_t value) asm("ass_i2c_write");
uint8_t __attribute__((noinline)) i2c_read(uint8_t last);
#endif

void i2cWire_beginTransmission(uint8_t addr) {
    _lastI2CError = 0;
#ifndef LEPFLIR_USE_SOFTWARE_I2C
    // _i2cWire->beginTransmission(addr);
    critical_i2c_lock();
    TwoWire_beginTransmission(addr);
#else
    i2c_start(addr);
#endif
}

uint8_t i2cWire_endTransmission(void) {
#ifndef LEPFLIR_USE_SOFTWARE_I2C
    // return (_lastI2CError = _i2cWire->endTransmission());
    _lastI2CError = TwoWire_endTransmission();
    critical_i2c_unlock();
    return _lastI2CError;
#else
    i2c_stop();
    return (_lastI2CError = 0);
#endif
}

uint8_t i2cWire_requestFrom(uint8_t addr, uint8_t len) {
#ifndef LEPFLIR_USE_SOFTWARE_I2C
    critical_i2c_lock();
    uint8_t ret = TwoWire_requestFrom(addr, len);
    critical_i2c_unlock();
    // return _i2cWire->requestFrom(addr, len);
    return ret;
#else
    i2c_start(addr | 0x01);
    return (_readBytes = len);
#endif
}

size_t i2cWire_write(uint8_t data) {
#ifndef LEPFLIR_USE_SOFTWARE_I2C
    uint8_t ret = TwoWire_write(data);
    // return _i2cWire->write(data);
    return ret;
#else
    return (size_t)i2c_write(data);
#endif
}

size_t i2cWire_write16(uint16_t data) {
#ifndef LEPFLIR_USE_SOFTWARE_I2C
    // return _i2cWire->write(highByte(data)) + _i2cWire->write(lowByte(data));
    return TwoWire_write(highByte(data)) + TwoWire_write(lowByte(data));
#else
    return (size_t)i2c_write(highByte(data)) + (size_t)i2c_write(lowByte(data));
#endif
}

uint8_t i2cWire_read(void) {
#ifndef LEPFLIR_USE_SOFTWARE_I2C
    return (uint8_t)(TwoWire_read() & 0xFF);
    // return (uint8_t)(_i2cWire->read() & 0xFF);
#else
    if (_readBytes > 1) {
        _readByes -= 1;
        return (uint8_t)(i2c_read(false) & 0xFF);
    }
    else {
        _readBytes = 0;
        return (uint8_t)(i2c_read(true) & 0xFF);
    }
#endif
}

uint16_t i2cWire_read16(void) {
#ifndef LEPFLIR_USE_SOFTWARE_I2C
    return ((uint16_t)(TwoWire_read() & 0xFF) << 8) | (uint16_t)(TwoWire_read() & 0xFF);
    // return ((uint16_t)(_i2cWire->read() & 0xFF) << 8) | (uint16_t)(_i2cWire->read() & 0xFF);
#else
    if (_readBytes > 2) {
        readBytes -= 2;
        return ((uint16_t)(i2c_read(false) & 0xFF) << 8) | (uint16_t)(i2c_read(false) & 0xFF);
    }
    else {
        _readBytes = 0;
        return ((uint16_t)(i2c_read(false) & 0xFF) << 8) | (uint16_t)(i2c_read(true) & 0xFF);
    }
#endif
}

