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
#define LEPTON_FLIR_INTERNAL
#include "lepton-flir.h"
#include "lepton-communication.h"
#include "lepton-register.h"
#include "lepton-comm-internal.h"
#include "lepton-sys.h"
#include "lepton-agc.h"
#include "lepton-vid.h"
#include <math.h>

#define true 1
#define false 0

#define min(a,b) (((a)<(b))?(a):(b))

uint16_t getStatusRegister(struct lepton_driver *driver)
{
  if(driver){
    uint16_t status;
    readRegister(&(driver->communication), LEP_I2C_STATUS_REG, &status);
    return status;
  }
  return 0;
}

#ifndef LEPFLIR_EXCLUDE_IMAGE_FUNCS

LeptonFLiR_ImageStorageMode getImageStorageMode(struct lepton_driver * driver)
{
  return driver->_storageMode;
}

LeptonFLiR_TemperatureMode getTemperatureMode(struct lepton_driver * driver)
{
  return driver->_tempMode;
}

int getImageWidth(struct lepton_driver *driver)
{
  switch (driver->_storageMode) {
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

int getImageHeight(struct lepton_driver *driver)
{
  switch (driver->_storageMode) {
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

int getImageBpp(struct lepton_driver *driver)
{
  switch (driver->_storageMode) {
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

#endif

#ifndef LEPFLIR_EXCLUDE_MISC_FUNCS

static inline uint8_t lowByte(uint16_t p)
{
  return (p & 0x00FF);
}

static inline uint8_t highByte(uint16_t p)
{
  return ((p & 0xFF00) >> 8);
}

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

float kelvin100ToTemperature(struct lepton_driver *driver, uint16_t kelvin100)
{
  switch (driver->_tempMode) {
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

uint16_t temperatureToKelvin100(struct lepton_driver * driver,
                                float temperature)
{
  switch (driver->_tempMode) {
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

const char *getTemperatureSymbol(struct lepton_driver *driver)
{
  switch (driver->_tempMode) {
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

#endif

uint8_t getLastI2CError(struct lepton_driver * driver)
{
  return driver->communication.callbacks._lastI2CError;
}

LEP_RESULT getLastLepResult(struct lepton_driver * driver)
{
  return (LEP_RESULT) driver->communication._lastLepResult;
}

#ifdef LEPFLIR_ENABLE_DEBUG_OUTPUT

const char *getLastI2CErrorString(struct lepton_driver * driver)
{
  if(driver==NULL) return 0;
  switch (driver->communication.callbacks._lastI2CError) {
  case 0:
    return LEP_I2C_ERROR_STRING_0;
  case 1:
    return LEP_I2C_ERROR_STRING_1;
  case 2:
    return LEP_I2C_ERROR_STRING_2;
  case 3:
    return LEP_I2C_ERROR_STRING_3;
  default:
    return LEP_I2C_ERROR_STRING_4;
  }
}

const char *getLastLepResultString(struct lepton_driver * driver)
{
  if(driver==NULL) return 0;
  switch (driver->communication._lastLepResult) {
  case LEP_OK:
    return LEP_OK_STRING;
  case LEP_ERROR:
    return LEP_ERROR_STRING;
  case LEP_NOT_READY:
    return LEP_NOT_READY_STRING;
  case LEP_RANGE_ERROR:
    return LEP_RANGE_ERROR_STRING;
  case LEP_CHECKSUM_ERROR:
    return LEP_CHECKSUM_ERROR_STRING;
  case LEP_BAD_ARG_POINTER_ERROR:
    return LEP_BAD_ARG_POINTER_ERROR_STRING;
  case LEP_DATA_SIZE_ERROR:
    return LEP_DATA_SIZE_ERROR_STRING;
  case LEP_UNDEFINED_FUNCTION_ERROR:
    return LEP_UNDEFINED_FUNCTION_ERROR_STRING;
  case LEP_FUNCTION_NOT_SUPPORTED:
    return LEP_FUNCTION_NOT_SUPPORTED_STRING;
  case LEP_OTP_WRITE_ERROR:
    return LEP_OTP_WRITE_ERROR_STRING;
  case LEP_OTP_READ_ERROR:
    return LEP_OTP_READ_ERROR_STRING;
  case LEP_OTP_NOT_PROGRAMMED_ERROR:
    return LEP_OTP_NOT_PROGRAMMED_ERROR_STRING;
  case LEP_ERROR_I2C_BUS_NOT_READY:
    return LEP_ERROR_I2C_BUS_NOT_READY_STRING;
  case LEP_ERROR_I2C_BUFFER_OVERFLOW:
    return LEP_ERROR_I2C_BUFFER_OVERFLOW_STRING;
  case LEP_ERROR_I2C_ARBITRATION_LOST:
    return LEP_ERROR_I2C_ARBITRATION_LOST_STRING;
  case LEP_ERROR_I2C_BUS_ERROR:
    return LEP_ERROR_I2C_BUS_ERROR_STRING;
  case LEP_ERROR_I2C_NACK_RECEIVED:
    return LEP_ERROR_I2C_NACK_RECEIVED_STRING;
  case LEP_ERROR_I2C_FAIL:
    return LEP_ERROR_I2C_FAIL_STRING;
  case LEP_DIV_ZERO_ERROR:
    return LEP_DIV_ZERO_ERROR_STRING;
  case LEP_COMM_PORT_NOT_OPEN:
    return LEP_COMM_PORT_NOT_OPEN_STRING;
  case LEP_COMM_INVALID_PORT_ERROR:
    return LEP_COMM_INVALID_PORT_ERROR_STRING;
  case LEP_COMM_RANGE_ERROR:
    return LEP_COMM_RANGE_ERROR_STRING;
  case LEP_ERROR_CREATING_COMM:
    return LEP_ERROR_CREATING_COMM_STRING;
  case LEP_ERROR_STARTING_COMM:
    return LEP_ERROR_STARTING_COMM_STRING;
  case LEP_ERROR_CLOSING_COMM:
    return LEP_ERROR_CLOSING_COMM_STRING;
  case LEP_COMM_CHECKSUM_ERROR:
    return LEP_COMM_CHECKSUM_ERROR_STRING;
  case LEP_COMM_NO_DEV:
    return LEP_COMM_NO_DEV_STRING;
  case LEP_TIMEOUT_ERROR:
    return LEP_TIMEOUT_ERROR_STRING;
  case LEP_COMM_ERROR_WRITING_COMM:
    return LEP_COMM_ERROR_WRITING_COMM_STRING;
  case LEP_COMM_ERROR_READING_COMM:
    return LEP_COMM_ERROR_READING_COMM_STRING;
  case LEP_COMM_COUNT_ERROR:
    return LEP_COMM_COUNT_ERROR_STRING;
  case LEP_OPERATION_CANCELED:
    return LEP_OPERATION_CANCELED_STRING;
  case LEP_UNDEFINED_ERROR_CODE:
    return LEP_UNDEFINED_ERROR_CODE_STRING;
  default:
    return LEP_OTHER_ERROR_STRING;
  }
}

#endif

void
lepton_i2cWire_beginTransmission_set_callback(struct lepton_driver *driver,
                                              void (*callback) (struct
                                                                lepton_callbacks
                                                                *, uint8_t))
{
  driver->communication.callbacks.i2cWire_beginTransmission = callback;
}

void lepton_i2cWire_endTransmission_set_callback(struct lepton_driver *driver,
                                                 uint8_t(*callback) (struct
                                                                     lepton_callbacks
                                                                     *))
{
  driver->communication.callbacks.i2cWire_endTransmission = callback;
}

void
lepton_i2cWire_requestFrom_set_callback(struct lepton_driver *driver,
                                        uint8_t(*callback)
                                         (struct lepton_callbacks *, uint8_t,
                                          uint8_t))
{
  driver->communication.callbacks.i2cWire_requestFrom = callback;
}

void lepton_i2cWire_write16_set_callback(struct lepton_driver *driver,
                                         size_t(*callback) (struct
                                                            lepton_callbacks *,
                                                            uint16_t))
{
  driver->communication.callbacks.i2cWire_write16 = callback;
}

void lepton_i2cWire_read_set_callback(struct lepton_driver *driver,
                                      uint8_t(*callback) (struct
                                                          lepton_callbacks *))
{
  driver->communication.callbacks.i2cWire_read = callback;
}

void lepton_i2cWire_read16_set_callback(struct lepton_driver *driver,
                                        uint16_t(*callback) (struct
                                                             lepton_callbacks
                                                             *))
{
  driver->communication.callbacks.i2cWire_read16 = callback;
}

void lepton_i2cWire_set_buffer_length(struct lepton_driver *driver, int length)
{
  driver->communication.buffer_length = length;
}

void lepton_millis_set_callback(struct lepton_driver *driver,
                                unsigned long (*callback) (void))
{
  driver->communication.callbacks.millis_callback = callback;
}

void lepton_delay_set_callback(struct lepton_driver *driver,
                               void (*callback) (unsigned long))
{
  driver->communication.callbacks.delay_callback = callback;
}

#ifndef LEPFLIT_EXCLUDE_LOCKING
void mutex_lock_set_callback(struct lepton_driver *driver,
                               void (*callback) (void))
{
  driver->communication.callbacks.mutex_lock = callback;
}

void mutex_unlock_set_callback(struct lepton_driver *driver,
                               void (*callback) (void))
{
  driver->communication.callbacks.mutex_unlock = callback;
}
#endif

void LeptonFLiR_init(
#ifndef LEPFLIR_EXCLUDE_IMAGE_FUNCS
                      LeptonFLiR_ImageStorageMode storageMode,
                      LeptonFLiR_TemperatureMode tempMode,
#endif
                      struct lepton_driver *driver)
{
  driver->getStatusRegister = &getStatusRegister;
  driver->getLastI2CError = &getLastI2CError;
  driver->getLastLepResult = &getLastLepResult;
  driver->communication._lastLepResult = 0;
  driver->communication.callbacks._lastI2CError = 0;
#ifdef LEPFLIR_ENABLE_DEBUG_OUTPUT
  driver->getLastI2CErrorString = &getLastI2CErrorString;
  driver->getLastLepResultString = &getLastLepResultString;
#endif
#ifndef LEPFLIR_EXCLUDE_IMAGE_FUNCS
  driver->_storageMode = storageMode;
  driver->_tempMode = tempMode;
  driver->getImageStorageMode = &getImageStorageMode;
  driver->getTemperatureMode = &getTemperatureMode;
  driver->getImageWidth = &getImageWidth;
  driver->getImageHeight = &getImageHeight;
  driver->getImageBpp = &getImageBpp;
#endif
#ifndef LEPFLIR_EXCLUDE_MISC_FUNCS
  driver->misc.kelvin100ToTemperature = &kelvin100ToTemperature;
  driver->misc.temperatureToKelvin100 = &temperatureToKelvin100;
  driver->misc.getTemperatureSymbol = &getTemperatureSymbol;
  driver->misc.wordsToHexString = &wordsToHexString;
  driver->misc.kelvin100ToCelsius = &kelvin100ToCelsius;
  driver->misc.kelvin100ToFahrenheit = &kelvin100ToFahrenheit;
  driver->misc.kelvin100ToKelvin = &kelvin100ToKelvin;
  driver->misc.celsiusToKelvin100 = &celsiusToKelvin100;
  driver->misc.fahrenheitToKelvin100 = &fahrenheitToKelvin100;
  driver->misc.kelvinToKelvin100 = &kelvinToKelvin100;
#endif
  driver->init.lepton_i2cWire_beginTransmission_set_callback =
      &lepton_i2cWire_beginTransmission_set_callback;
  driver->init.lepton_i2cWire_endTransmission_set_callback =
      &lepton_i2cWire_endTransmission_set_callback;
  driver->init.lepton_i2cWire_requestFrom_set_callback =
      &lepton_i2cWire_requestFrom_set_callback;
  driver->init.lepton_i2cWire_write16_set_callback =
      &lepton_i2cWire_write16_set_callback;
  driver->init.lepton_i2cWire_read_set_callback =
      &lepton_i2cWire_read_set_callback;
  driver->init.lepton_i2cWire_read16_set_callback =
      &lepton_i2cWire_read16_set_callback;
  driver->init.lepton_i2cWire_set_buffer_length =
      &lepton_i2cWire_set_buffer_length;
  driver->init.lepton_millis_set_callback = &lepton_millis_set_callback;
  driver->init.lepton_delay_set_callback = &lepton_delay_set_callback;
#ifndef LEPFLIT_EXCLUDE_LOCKING
  driver->init.mutex_lock_set_callback = &mutex_lock_set_callback;
  driver->init.mutex_unlock_set_callback = &mutex_unlock_set_callback;
#endif
  lepton_communication_init(&(driver->communication));
#ifndef LEPFLIR_EXCLUDE_SYS_FUNCS
  lepton_sys_init(&(driver->sys));
#endif
#ifndef LEPFLIR_EXCLUDE_AGC_FUNCS
  lepton_agc_init(&(driver->agc));
#endif
#ifndef LEPFLIR_EXCLUDE_VID_FUNCS
  lepton_vid_init(&(driver->vid));
#endif
}
