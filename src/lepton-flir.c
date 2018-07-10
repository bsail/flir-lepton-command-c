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
#include "lepton-communication.h"
#include "lepton-sys.h"
#include <math.h>

static struct lepton_callbacks callbacks;
static struct lepton_communication communication;
static struct lepton_sys lepton_sys_storage;

#define true 1
#define false 0

#define min(a,b) (((a)<(b))?(a):(b))

static inline uint8_t lowByte(uint16_t p)
{
  return (p & 0x00FF);
}

static inline uint8_t highByte(uint16_t p)
{
  return ((p & 0xFF00) >> 8);
}


void LeptonFLiR_init(LeptonFLiR_ImageStorageMode storageMode,
                     LeptonFLiR_TemperatureMode tempMode,
                     struct lepton_driver * driver)
{
  communication._storageMode = LeptonFLiR_ImageStorageMode_Count;
  callbacks._lastI2CError = communication._lastLepResult = 0;
  lepton_communication_init(&communication,&callbacks);
  driver->sys = &lepton_sys_storage;
  lepton_sys_init(driver->sys);
  communication._storageMode = storageMode;
  communication._tempMode = tempMode;
}

LeptonFLiR_ImageStorageMode getImageStorageMode()
{
  return communication._storageMode;
}

LeptonFLiR_TemperatureMode getTemperatureMode()
{
  return communication._tempMode;
}

int getImageWidth()
{
  switch (communication._storageMode) {
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

int getImageHeight()
{
  switch (communication._storageMode) {
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

int getImageBpp()
{
  switch (communication._storageMode) {
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

void agc_setAGCEnabled(uint8_t enabled)
{
  communication.sendCommand_u32(communication.cmdCode(LEP_CID_AGC_ENABLE_STATE, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) enabled,&communication);
}

uint8_t agc_getAGCEnabled()
{
  uint32_t enabled;
  communication.receiveCommand_u32(communication.cmdCode
                     (LEP_CID_AGC_ENABLE_STATE, LEP_I2C_COMMAND_TYPE_GET),
                     &enabled,&communication);
  return enabled;
}

void agc_setAGCPolicy(LEP_AGC_POLICY policy)
{
  communication.sendCommand_u32(communication.cmdCode(LEP_CID_AGC_POLICY, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) policy,&communication);
}

LEP_AGC_POLICY agc_getAGCPolicy()
{
  uint32_t policy;
  communication.receiveCommand_u32(communication.cmdCode(LEP_CID_AGC_POLICY, LEP_I2C_COMMAND_TYPE_GET),
                     &policy,&communication);
  return (LEP_AGC_POLICY) policy;
}

void agc_setHEQScaleFactor(LEP_AGC_HEQ_SCALE_FACTOR factor)
{
  communication.sendCommand_u32(communication.cmdCode
                  (LEP_CID_AGC_HEQ_SCALE_FACTOR, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) factor,&communication);
}

LEP_AGC_HEQ_SCALE_FACTOR agc_getHEQScaleFactor()
{
  uint32_t factor;
  communication.receiveCommand_u32(communication.cmdCode
                     (LEP_CID_AGC_HEQ_SCALE_FACTOR, LEP_I2C_COMMAND_TYPE_GET),
                     &factor,&communication);
  return (LEP_AGC_HEQ_SCALE_FACTOR) factor;
}

void agc_setAGCCalcEnabled(uint8_t enabled)
{
  communication.sendCommand_u32(communication.cmdCode
                  (LEP_CID_AGC_CALC_ENABLE_STATE, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) enabled,&communication);
}

uint8_t agc_getAGCCalcEnabled()
{
  uint32_t enabled;
  communication.receiveCommand_u32(communication.cmdCode
                     (LEP_CID_AGC_CALC_ENABLE_STATE, LEP_I2C_COMMAND_TYPE_GET),
                     &enabled,&communication);
  return enabled;
}

void vid_setPolarity(LEP_VID_POLARITY polarity)
{
  communication.sendCommand_u32(communication.cmdCode
                  (LEP_CID_VID_POLARITY_SELECT, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) polarity,&communication);
}

LEP_VID_POLARITY vid_getPolarity()
{
  uint32_t polarity;
  communication.receiveCommand_u32(communication.cmdCode
                     (LEP_CID_VID_POLARITY_SELECT, LEP_I2C_COMMAND_TYPE_GET),
                     &polarity,&communication);
  return (LEP_VID_POLARITY) polarity;
}

void vid_setPseudoColorLUT(LEP_VID_PCOLOR_LUT table)
{
  communication.sendCommand_u32(communication.cmdCode(LEP_CID_VID_LUT_SELECT, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) table,&communication);
}

LEP_VID_PCOLOR_LUT vid_getPseudoColorLUT()
{
  uint32_t table;
  communication.receiveCommand_u32(communication.cmdCode(LEP_CID_VID_LUT_SELECT, LEP_I2C_COMMAND_TYPE_GET),
                     &table,&communication);
  return (LEP_VID_PCOLOR_LUT) table;
}

void vid_setFocusCalcEnabled(uint8_t enabled)
{
  communication.sendCommand_u32(communication.cmdCode
                  (LEP_CID_VID_FOCUS_CALC_ENABLE, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) enabled,&communication);
}

uint8_t vid_getFocusCalcEnabled()
{
  uint32_t enabled;
  communication.receiveCommand_u32(communication.cmdCode
                     (LEP_CID_VID_FOCUS_CALC_ENABLE, LEP_I2C_COMMAND_TYPE_GET),
                     &enabled,&communication);
  return enabled;
}

void vid_setFreezeEnabled(uint8_t enabled)
{
  communication.sendCommand_u32(communication.cmdCode(LEP_CID_VID_FREEZE_ENABLE, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) enabled,&communication);
}

uint8_t vid_getFreezeEnabled()
{
  uint32_t enabled;
  communication.receiveCommand_u32(communication.cmdCode
                     (LEP_CID_VID_FREEZE_ENABLE, LEP_I2C_COMMAND_TYPE_GET),
                     &enabled,&communication);
  return enabled;
}

#ifndef LEPFLIR_EXCLUDE_EXT_I2C_FUNCS

void agc_setHistogramRegion(LEP_AGC_HISTOGRAM_ROI * region)
{
  if (!region)
    return;
  communication.sendCommand_array(communication.cmdCode(LEP_CID_AGC_ROI, LEP_I2C_COMMAND_TYPE_SET),
                    (uint16_t *) region, sizeof(LEP_AGC_HISTOGRAM_ROI) / 2,&communication);
}

void agc_getHistogramRegion(LEP_AGC_HISTOGRAM_ROI * region)
{
  if (!region)
    return;
  communication.receiveCommand_array(communication.cmdCode(LEP_CID_AGC_ROI, LEP_I2C_COMMAND_TYPE_GET),
                       (uint16_t *) region, sizeof(LEP_AGC_HISTOGRAM_ROI) / 2,&communication);
}

void agc_getHistogramStatistics(LEP_AGC_HISTOGRAM_STATISTICS * statistics)
{
  if (!statistics)
    return;
  communication.receiveCommand_array(communication.cmdCode
                       (LEP_CID_AGC_STATISTICS, LEP_I2C_COMMAND_TYPE_GET),
                       (uint16_t *) statistics,
                       sizeof(LEP_AGC_HISTOGRAM_STATISTICS) / 2,&communication);
}

void agc_setHistogramClipPercent(uint16_t percent)
{
  communication.sendCommand_u16(communication.cmdCode
                  (LEP_CID_AGC_HISTOGRAM_CLIP_PERCENT,
                   LEP_I2C_COMMAND_TYPE_SET), percent,&communication);
}

uint16_t agc_getHistogramClipPercent()
{
  uint16_t percent;
  communication.receiveCommand_u16(communication.cmdCode
                     (LEP_CID_AGC_HISTOGRAM_CLIP_PERCENT,
                      LEP_I2C_COMMAND_TYPE_GET), &percent,&communication);
  return percent;
}

void agc_setHistogramTailSize(uint16_t size)
{
  communication.sendCommand_u16(communication.cmdCode
                  (LEP_CID_AGC_HISTOGRAM_TAIL_SIZE, LEP_I2C_COMMAND_TYPE_SET),
                  size,&communication);
}

uint16_t agc_getHistogramTailSize()
{
  uint16_t size;
  communication.receiveCommand_u16(communication.cmdCode
                     (LEP_CID_AGC_HISTOGRAM_TAIL_SIZE,
                      LEP_I2C_COMMAND_TYPE_GET), &size,&communication);
  return size;
}

void agc_setLinearMaxGain(uint16_t gain)
{
  communication.sendCommand_u16(communication.cmdCode
                  (LEP_CID_AGC_LINEAR_MAX_GAIN, LEP_I2C_COMMAND_TYPE_SET),
                  gain,&communication);
}

uint16_t agc_getLinearMaxGain()
{
  uint16_t gain;
  communication.receiveCommand_u16(communication.cmdCode
                     (LEP_CID_AGC_LINEAR_MAX_GAIN, LEP_I2C_COMMAND_TYPE_GET),
                     &gain,&communication);
  return gain;
}

void agc_setLinearMidpoint(uint16_t midpoint)
{
  communication.sendCommand_u16(communication.cmdCode
                  (LEP_CID_AGC_LINEAR_MIDPOINT, LEP_I2C_COMMAND_TYPE_SET),
                  midpoint,&communication);
}

uint16_t agc_getLinearMidpoint()
{
  uint16_t midpoint;
  communication.receiveCommand_u16(communication.cmdCode
                     (LEP_CID_AGC_LINEAR_MIDPOINT, LEP_I2C_COMMAND_TYPE_GET),
                     &midpoint,&communication);
  return midpoint;
}

void agc_setLinearDampeningFactor(uint16_t factor)
{
  communication.sendCommand_u16(communication.cmdCode
                  (LEP_CID_AGC_LINEAR_DAMPENING_FACTOR,
                   LEP_I2C_COMMAND_TYPE_SET), factor,&communication);
}

uint16_t agc_getLinearDampeningFactor()
{
  uint16_t factor;
  communication.receiveCommand_u16(communication.cmdCode
                     (LEP_CID_AGC_LINEAR_DAMPENING_FACTOR,
                      LEP_I2C_COMMAND_TYPE_GET), &factor,&communication);
  return factor;
}

void agc_setHEQDampeningFactor(uint16_t factor)
{
  communication.sendCommand_u16(communication.cmdCode
                  (LEP_CID_AGC_HEQ_DAMPENING_FACTOR, LEP_I2C_COMMAND_TYPE_SET),
                  factor,&communication);
}

uint16_t agc_getHEQDampeningFactor()
{
  uint16_t factor;
  communication.receiveCommand_u16(communication.cmdCode
                     (LEP_CID_AGC_HEQ_DAMPENING_FACTOR,
                      LEP_I2C_COMMAND_TYPE_GET), &factor,&communication);
  return factor;
}

void agc_setHEQMaxGain(uint16_t gain)
{
  communication.sendCommand_u16(communication.cmdCode(LEP_CID_AGC_HEQ_MAX_GAIN, LEP_I2C_COMMAND_TYPE_SET),
                  gain,&communication);
}

uint16_t agc_getHEQMaxGain()
{
  uint16_t gain;
  communication.receiveCommand_u16(communication.cmdCode
                     (LEP_CID_AGC_HEQ_MAX_GAIN, LEP_I2C_COMMAND_TYPE_GET),
                     &gain,&communication);
  return gain;
}

void agc_setHEQClipLimitHigh(uint16_t limit)
{
  communication.sendCommand_u16(communication.cmdCode
                  (LEP_CID_AGC_HEQ_CLIP_LIMIT_HIGH, LEP_I2C_COMMAND_TYPE_SET),
                  limit,&communication);
}

uint16_t agc_getHEQClipLimitHigh()
{
  uint16_t limit;
  communication.receiveCommand_u16(communication.cmdCode
                     (LEP_CID_AGC_HEQ_CLIP_LIMIT_HIGH,
                      LEP_I2C_COMMAND_TYPE_GET), &limit,&communication);
  return limit;
}

void agc_setHEQClipLimitLow(uint16_t limit)
{
  communication.sendCommand_u16(communication.cmdCode
                  (LEP_CID_AGC_HEQ_CLIP_LIMIT_LOW, LEP_I2C_COMMAND_TYPE_SET),
                  limit,&communication);
}

uint16_t agc_getHEQClipLimitLow()
{
  uint16_t limit;
  communication.receiveCommand_u16(communication.cmdCode
                     (LEP_CID_AGC_HEQ_CLIP_LIMIT_LOW, LEP_I2C_COMMAND_TYPE_GET),
                     &limit,&communication);
  return limit;
}

void agc_setHEQBinExtension(uint16_t extension)
{
  communication.sendCommand_u16(communication.cmdCode
                  (LEP_CID_AGC_HEQ_BIN_EXTENSION, LEP_I2C_COMMAND_TYPE_SET),
                  extension,&communication);
}

uint16_t agc_getHEQBinExtension()
{
  uint16_t extension;
  communication.receiveCommand_u16(communication.cmdCode
                     (LEP_CID_AGC_HEQ_BIN_EXTENSION, LEP_I2C_COMMAND_TYPE_GET),
                     &extension,&communication);
  return extension;
}

void agc_setHEQMidpoint(uint16_t midpoint)
{
  communication.sendCommand_u16(communication.cmdCode(LEP_CID_AGC_HEQ_MIDPOINT, LEP_I2C_COMMAND_TYPE_SET),
                  midpoint,&communication);
}

uint16_t agc_getHEQMidpoint()
{
  uint16_t midpoint;
  communication.receiveCommand_u16(communication.cmdCode
                     (LEP_CID_AGC_HEQ_MIDPOINT, LEP_I2C_COMMAND_TYPE_GET),
                     &midpoint,&communication);
  return midpoint;
}

void agc_setHEQEmptyCounts(uint16_t counts)
{
  communication.sendCommand_u16(communication.cmdCode
                  (LEP_CID_AGC_HEQ_EMPTY_COUNTS, LEP_I2C_COMMAND_TYPE_SET),
                  counts,&communication);
}

uint16_t agc_getHEQEmptyCounts()
{
  uint16_t counts;
  communication.receiveCommand_u16(communication.cmdCode
                     (LEP_CID_AGC_HEQ_EMPTY_COUNTS, LEP_I2C_COMMAND_TYPE_GET),
                     &counts,&communication);
  return counts;
}

void agc_setHEQNormalizationFactor(uint16_t factor)
{
  communication.sendCommand_u16(communication.cmdCode
                  (LEP_CID_AGC_HEQ_NORMALIZATION_FACTOR,
                   LEP_I2C_COMMAND_TYPE_SET), factor,&communication);
}

uint16_t agc_getHEQNormalizationFactor()
{
  uint16_t factor;
  communication.receiveCommand_u16(communication.cmdCode
                     (LEP_CID_AGC_HEQ_NORMALIZATION_FACTOR,
                      LEP_I2C_COMMAND_TYPE_GET), &factor,&communication);
  return factor;
}

void vid_setUserColorLUT(LEP_VID_LUT_BUFFER * table)
{
  if (!table)
    return;
  communication.sendCommand_array(communication.cmdCode(LEP_CID_VID_LUT_TRANSFER, LEP_I2C_COMMAND_TYPE_SET),
                    (uint16_t *) table, sizeof(LEP_VID_LUT_BUFFER) / 2,&communication);
}

void vid_getUserColorLUT(LEP_VID_LUT_BUFFER * table)
{
  if (!table)
    return;
  communication.receiveCommand_array(communication.cmdCode
                       (LEP_CID_VID_LUT_TRANSFER, LEP_I2C_COMMAND_TYPE_GET),
                       (uint16_t *) table, sizeof(LEP_VID_LUT_BUFFER) / 2,&communication);
}

void vid_setFocusRegion(LEP_VID_FOCUS_ROI * region)
{
  if (!region)
    return;
  communication.sendCommand_array(communication.cmdCode(LEP_CID_VID_FOCUS_ROI, LEP_I2C_COMMAND_TYPE_SET),
                    (uint16_t *) region, sizeof(LEP_VID_FOCUS_ROI) / 2,&communication);
}

void vid_getFocusRegion(LEP_VID_FOCUS_ROI * region)
{
  if (!region)
    return;
  communication.receiveCommand_array(communication.cmdCode(LEP_CID_VID_FOCUS_ROI, LEP_I2C_COMMAND_TYPE_GET),
                       (uint16_t *) region, sizeof(LEP_VID_FOCUS_ROI) / 2,&communication);
}

void vid_setFocusThreshold(uint32_t threshold)
{
  communication.sendCommand_u32(communication.cmdCode
                  (LEP_CID_VID_FOCUS_THRESHOLD, LEP_I2C_COMMAND_TYPE_SET),
                  threshold,&communication);
}

uint32_t vid_getFocusThreshold()
{
  uint32_t threshold;
  communication.receiveCommand_u32(communication.cmdCode
                     (LEP_CID_VID_FOCUS_THRESHOLD, LEP_I2C_COMMAND_TYPE_GET),
                     &threshold,&communication);
  return threshold;
}

uint32_t vid_getFocusMetric()
{
  uint32_t metric;
  communication.receiveCommand_u32(communication.cmdCode
                     (LEP_CID_VID_FOCUS_METRIC, LEP_I2C_COMMAND_TYPE_GET),
                     &metric,&communication);
  return metric;
}

void vid_setSceneBasedNUCEnabled(uint8_t enabled)
{
  communication.sendCommand_u32(communication.cmdCode(LEP_CID_VID_SBNUC_ENABLE, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) enabled,&communication);
}

uint8_t vid_getSceneBasedNUCEnabled()
{
  uint32_t enabled;
  communication.receiveCommand_u32(communication.cmdCode
                     (LEP_CID_VID_SBNUC_ENABLE, LEP_I2C_COMMAND_TYPE_GET),
                     &enabled,&communication);
  return enabled;
}

void vid_setGamma(uint32_t gamma)
{
  communication.sendCommand_u32(communication.cmdCode(LEP_CID_VID_GAMMA_SELECT, LEP_I2C_COMMAND_TYPE_SET),
                  gamma,&communication);
}

uint32_t vid_getGamma()
{
  uint32_t gamma;
  communication.receiveCommand_u32(communication.cmdCode
                     (LEP_CID_VID_GAMMA_SELECT, LEP_I2C_COMMAND_TYPE_GET),
                     &gamma,&communication);
  return gamma;
}

#endif

static inline void uint8_tToHexString(uint8_t value, char *buffer)
{
  uint8_t highNibble = value / 16;
  uint8_t lowNibble = value % 16;
  if (highNibble < 10)
    buffer[0] = '0' + highNibble;
  else
    buffer[0] = 'A' + (highNibble - 10);
  if (lowNibble < 10)
    buffer[1] = '0' + lowNibble;
  else
    buffer[1] = 'A' + (lowNibble - 10);
}

void wordsToHexString(uint16_t * dataWords, int dataLength, char *buffer,
                      int maxLength)
{
  uint8_t insertColons = maxLength >= (dataLength * 4) + (dataLength - 1);

  while (dataLength-- > 0 && maxLength > 3) {
    if (maxLength > 3) {
      uint8_tToHexString(highByte(*dataWords), buffer);
      buffer += 2;
      maxLength -= 2;
      uint8_tToHexString(lowByte(*dataWords), buffer);
      buffer += 2;
      maxLength -= 2;
      ++dataWords;
    }

    if (dataLength > 0 && insertColons && maxLength-- > 0)
      *buffer++ = ':';
  }

  if (maxLength-- > 0)
    *buffer++ = '\0';
}

float kelvin100ToCelsius(uint16_t kelvin100)
{
  float kelvin = (kelvin100 / 100) + ((kelvin100 % 100) * 0.01f);
  return kelvin - 273.15f;
}

float kelvin100ToFahrenheit(uint16_t kelvin100)
{
  float kelvin = (kelvin100 / 100) + ((kelvin100 % 100) * 0.01f);
  return roundf((((kelvin * 9.0f) / 5.0f) - 459.67f) * 100.0f) / 100.0f;
}

float kelvin100ToKelvin(uint16_t kelvin100)
{
  return (kelvin100 / 100) + ((kelvin100 % 100) * 0.01f);
}

uint16_t celsiusToKelvin100(float celsius)
{
  float kelvin = celsius + 273.15f;
  return (uint16_t) roundf(kelvin * 100.0f);
}

uint16_t fahrenheitToKelvin100(float fahrenheit)
{
  float kelvin = ((fahrenheit + 459.67f) * 5.0f) / 9.0f;
  return (uint16_t) roundf(kelvin * 100.0f);
}

uint16_t kelvinToKelvin100(float kelvin)
{
  return (uint16_t) roundf(kelvin * 100.0f);
}

float kelvin100ToTemperature(uint16_t kelvin100)
{
  switch (communication._tempMode) {
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

uint16_t temperatureToKelvin100(float temperature)
{
  switch (communication._tempMode) {
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

const char *getTemperatureSymbol()
{
  switch (communication._tempMode) {
  case LeptonFLiR_TemperatureMode_Celsius:
    return "C";
  case LeptonFLiR_TemperatureMode_Fahrenheit:
    return "F";
  case LeptonFLiR_TemperatureMode_Kelvin:
    return "K";
  default:
    return "";
  }
}

uint8_t getLastI2CError()
{
  return callbacks._lastI2CError;
}

LEP_RESULT getLastLepResult()
{
  return (LEP_RESULT) communication._lastLepResult;
}

#ifdef LEPFLIR_ENABLE_DEBUG_OUTPUT

static const char *textForI2CError(uint8_t errorCode)
{
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

static const char *textForLepResult(LEP_RESULT errorCode)
{
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
    return "LEP_DATA_SIZE_ERROR Camera uint8_t count error";
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
    return
        "LEP_ERROR_I2C_ARBITRATION_LOST I2C Bus Error - Bus Arbitration Lost";
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
    return "LEP_COMM_COUNT_ERROR Comm uint8_t count error";
  case LEP_OPERATION_CANCELED:
    return "LEP_OPERATION_CANCELED Camera operation canceled";
  case LEP_UNDEFINED_ERROR_CODE:
    return "LEP_UNDEFINED_ERROR_CODE Undefined error";
  default:
    return "Other error";
  }
}

#endif


void
lepton_i2cWire_beginTransmission_set_callback(void (*callback) (uint8_t addr, struct lepton_callbacks * this))
{
  callbacks.i2cWire_beginTransmission = callback;
}

void lepton_i2cWire_endTransmission_set_callback(uint8_t(*callback) (struct lepton_callbacks * this))
{
  callbacks.i2cWire_endTransmission = callback;
}

void
lepton_i2cWire_requestFrom_set_callback(uint8_t(*callback)
                                        (uint8_t addr, uint8_t len, struct lepton_callbacks * this))
{
  callbacks.i2cWire_requestFrom = callback;
}

void lepton_i2cWire_write_set_callback(size_t(*callback) (uint8_t data, struct lepton_callbacks * this))
{
  callbacks.i2cWire_write = callback;
}

void lepton_i2cWire_write16_set_callback(size_t(*callback) (uint16_t data, struct lepton_callbacks * this))
{
  callbacks.i2cWire_write16 = callback;
}

void lepton_i2cWire_read_set_callback(uint8_t(*callback) (struct lepton_callbacks *))
{
  callbacks.i2cWire_read = callback;
}

void lepton_i2cWire_read16_set_callback(uint16_t(*callback) (struct lepton_callbacks *))
{
  callbacks.i2cWire_read16 = callback;
}

void lepton_i2cWire_set_buffer_length(int length)
{
  communication.BUFFER_LENGTH = length;
}

void lepton_millis_set_callback(unsigned long (*callback) (void))
{
  callbacks.millis_callback = callback;
}

void lepton_delay_set_callback(void (*callback) (unsigned long))
{
  callbacks.delay_callback = callback;
}
