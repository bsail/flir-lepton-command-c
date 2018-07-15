#define LEPTON_FLIR_INTERNAL
#include "lepton-comm-internal.h"

#ifdef TEST
#include <stdio.h>
#endif

#define true 1
#define false 0

#define min(a,b) (((a)<(b))?(a):(b))

uint8_t waitCommandBegin(struct lepton_communication *this, int timeout)
{
  this->_lastLepResult = 0;
  uint16_t status;
  if (readRegister(this, LEP_I2C_STATUS_REG, &status))
    return false;

  if (!(status & LEP_I2C_STATUS_BUSY_BIT_MASK))
    return true;

  unsigned long endTime =
      this->callbacks.millis_callback() + (unsigned long)timeout;

  while ((status & LEP_I2C_STATUS_BUSY_BIT_MASK)
         && (timeout <= 0 || this->callbacks.millis_callback() < endTime)) {
    this->callbacks.delay_callback(1);

    if (readRegister(this, LEP_I2C_STATUS_REG, &status))
      return false;
  }

  if (!(status & LEP_I2C_STATUS_BUSY_BIT_MASK))
    return true;
  else {
    this->_lastLepResult = LEP_TIMEOUT_ERROR;
    return false;
  }
}

uint8_t waitCommandFinish(struct lepton_communication * this, int timeout)
{
  uint16_t status;
  if (readRegister(this, LEP_I2C_STATUS_REG, &status))
    return false;

  if (!(status & LEP_I2C_STATUS_BUSY_BIT_MASK)) {
    this->_lastLepResult =
        (uint8_t) ((status & LEP_I2C_STATUS_ERROR_CODE_BIT_MASK) >>
                   LEP_I2C_STATUS_ERROR_CODE_BIT_SHIFT);
    return true;
  }

  unsigned long endTime =
      this->callbacks.millis_callback() + (unsigned long)timeout;

  while ((status & LEP_I2C_STATUS_BUSY_BIT_MASK)
         && (timeout <= 0 || this->callbacks.millis_callback() < endTime)) {
    this->callbacks.delay_callback(1);

    if (readRegister(this, LEP_I2C_STATUS_REG, &status))
      return false;
  }

  if (!(status & LEP_I2C_STATUS_BUSY_BIT_MASK)) {
    this->_lastLepResult =
        (uint8_t) ((status & LEP_I2C_STATUS_ERROR_CODE_BIT_MASK) >>
                   LEP_I2C_STATUS_ERROR_CODE_BIT_SHIFT);
    return true;
  } else {
    this->_lastLepResult = LEP_TIMEOUT_ERROR;
    return false;
  }
}

int writeCmdRegister(struct lepton_communication *this, uint16_t cmdCode,
                     uint16_t * dataWords, int dataLength)
{
  // In avr/libraries/Wire.h and avr/libraries/utility/twi.h, buffer_length controls
  // how many words can be written at once. Therefore, we loop around until all words
  // have been written out into their registers.

  if (dataWords && dataLength) {
    #ifndef LEPFLIR_EXCLUDE_LOCKING
    this->callbacks.mutex_lock();
    #endif
    this->callbacks.i2cWire_beginTransmission(&(this->callbacks),
                                              LEP_I2C_DEVICE_ADDRESS);
    this->callbacks.i2cWire_write16(&(this->callbacks),
                                    LEP_I2C_DATA_LENGTH_REG);
    this->callbacks.i2cWire_write16(&(this->callbacks), dataLength);
    int ret = this->callbacks.i2cWire_endTransmission(&(this->callbacks));
    #ifndef LEPFLIR_EXCLUDE_LOCKING
    this->callbacks.mutex_unlock();
    #endif
    if (ret)
      return this->callbacks._lastI2CError;

    int maxLength = this->buffer_length / 2;
    int writeLength = min(maxLength, dataLength);
    uint16_t regAddress =
        dataLength <= 16 ? LEP_I2C_DATA_0_REG : LEP_I2C_DATA_BUFFER;

    while (dataLength > 0) {
      #ifndef LEPFLIR_EXCLUDE_LOCKING
      this->callbacks.mutex_lock();
      #endif
      this->callbacks.i2cWire_beginTransmission(&(this->callbacks),
                                                LEP_I2C_DEVICE_ADDRESS);
      this->callbacks.i2cWire_write16(&(this->callbacks), regAddress);

      while (writeLength-- > 0)
        this->callbacks.i2cWire_write16(&(this->callbacks), *dataWords++);
      ret = this->callbacks.i2cWire_endTransmission(&(this->callbacks));
      #ifndef LEPFLIR_EXCLUDE_LOCKING
      this->callbacks.mutex_unlock();
      #endif
      if (ret)
        return this->callbacks._lastI2CError;

      regAddress += maxLength * 0x02;
      dataLength -= maxLength;
      writeLength = min(maxLength, dataLength);
    }
  }

  #ifndef LEPFLIR_EXCLUDE_LOCKING
  this->callbacks.mutex_lock();
  #endif
  this->callbacks.i2cWire_beginTransmission(&(this->callbacks),
                                            LEP_I2C_DEVICE_ADDRESS);
  this->callbacks.i2cWire_write16(&(this->callbacks), LEP_I2C_COMMAND_REG);
  this->callbacks.i2cWire_write16(&(this->callbacks), cmdCode);
  int ret = this->callbacks.i2cWire_endTransmission(&(this->callbacks));
  
  #ifndef LEPFLIR_EXCLUDE_LOCKING
  this->callbacks.mutex_unlock();
  #endif
  return ret;
}

int readDataRegister(struct lepton_communication *this, uint16_t * readWords,
                     int maxLength)
{
  #ifndef LEPFLIR_EXCLUDE_LOCKING
  this->callbacks.mutex_lock();
  #endif
  this->callbacks.i2cWire_beginTransmission(&(this->callbacks),
                                            LEP_I2C_DEVICE_ADDRESS);
  this->callbacks.i2cWire_write16(&(this->callbacks), LEP_I2C_DATA_LENGTH_REG);
  int ret = this->callbacks.i2cWire_endTransmission(&(this->callbacks));
  #ifndef LEPFLIR_EXCLUDE_LOCKING
  this->callbacks.mutex_unlock();
  #endif
  if (ret)
    return this->callbacks._lastI2CError;

  #ifndef LEPFLIR_EXCLUDE_LOCKING
  this->callbacks.mutex_lock();
  #endif

  int uint8_tsRead =
      this->callbacks.i2cWire_requestFrom(&(this->callbacks),
                                          LEP_I2C_DEVICE_ADDRESS, 2);

#ifdef TEST
  //printf("000:%d\n",uint8_tsRead);
#endif

  if (uint8_tsRead != 2) {
    while (uint8_tsRead-- > 0)
      this->callbacks.i2cWire_read(&(this->callbacks));
    #ifndef LEPFLIR_EXCLUDE_LOCKING
    this->callbacks.mutex_unlock();
    #endif
    ret = (this->callbacks._lastI2CError = 4);
    return ret;
  }

  int readLength = this->callbacks.i2cWire_read16(&(this->callbacks));

  #ifndef LEPFLIR_EXCLUDE_LOCKING
  this->callbacks.mutex_unlock();
  #endif

#ifdef TEST
  //printf("100:%d\n",readLength);
#endif

  if (readLength == 0)
    return (this->callbacks._lastI2CError = 4);

  // In avr/libraries/Wire.h and avr/libraries/utility/twi.h, buffer_length controls
  // how many words can be read at once. Therefore, we loop around until all words
  // have been read out from their registers.

  #ifndef LEPFLIR_EXCLUDE_LOCKING
  this->callbacks.mutex_lock();
  #endif

  uint8_tsRead =
      this->callbacks.i2cWire_requestFrom(&(this->callbacks),
                                          LEP_I2C_DEVICE_ADDRESS,
                                          min(this->buffer_length, readLength));

  while (uint8_tsRead > 0 && readLength > 0) {

    while (uint8_tsRead > 1 && readLength > 1 && maxLength > 0) {
      *readWords++ = this->callbacks.i2cWire_read16(&(this->callbacks));
      uint8_tsRead -= 2;
      readLength -= 2;
      --maxLength;
    }

    if (readLength > 0)
      uint8_tsRead +=
          this->callbacks.i2cWire_requestFrom(&(this->callbacks),
                                              LEP_I2C_DEVICE_ADDRESS,
                                              min(this->buffer_length,
                                                  readLength));
  }

  while (uint8_tsRead-- > 0)
    this->callbacks.i2cWire_read(&(this->callbacks));

  #ifndef LEPFLIR_EXCLUDE_LOCKING
  this->callbacks.mutex_unlock();
  #endif

  while (maxLength-- > 0)
    *readWords++ = 0;
#ifdef TEST
  // printf("200:%d\n",readLength);
#endif
  return (this->callbacks._lastI2CError = readLength ? 4 : 0);
}

int writeRegister(struct lepton_communication *this, uint16_t regAddress,
                  uint16_t value)
{
  #ifdef TEST
  // printf("Reg:%d\n",regAddress);
  #endif
  #ifndef LEPFLIR_EXCLUDE_LOCKING
  this->callbacks.mutex_lock();
  #endif
  this->callbacks.i2cWire_beginTransmission(&(this->callbacks),
                                            LEP_I2C_DEVICE_ADDRESS);
  this->callbacks.i2cWire_write16(&(this->callbacks), regAddress);
  this->callbacks.i2cWire_write16(&(this->callbacks), value);
  int ret = this->callbacks.i2cWire_endTransmission(&(this->callbacks));
  #ifndef LEPFLIR_EXCLUDE_LOCKING
  this->callbacks.mutex_unlock();
  #endif
  return ret;
}

int readRegister(struct lepton_communication *this, uint16_t regAddress,
                 uint16_t * value)
{
  #ifndef LEPFLIR_EXCLUDE_LOCKING
  this->callbacks.mutex_lock();
  #endif
  this->callbacks.i2cWire_beginTransmission(&(this->callbacks),
                                            LEP_I2C_DEVICE_ADDRESS);
  this->callbacks.i2cWire_write16(&(this->callbacks), regAddress);
  int ret = this->callbacks.i2cWire_endTransmission(&(this->callbacks));
  #ifndef LEPFLIR_EXCLUDE_LOCKING
  this->callbacks.mutex_unlock();
  #endif
  if (ret)
    return this->callbacks._lastI2CError;

  #ifndef LEPFLIR_EXCLUDE_LOCKING
  this->callbacks.mutex_lock();
  #endif

  int uint8_tsRead =
      this->callbacks.i2cWire_requestFrom(&(this->callbacks),
                                          LEP_I2C_DEVICE_ADDRESS, 2);
  if (uint8_tsRead != 2) {
    while (uint8_tsRead-- > 0)
      this->callbacks.i2cWire_read(&(this->callbacks));
    return (this->callbacks._lastI2CError = 4);
  }

  *value = this->callbacks.i2cWire_read16(&(this->callbacks));

  #ifndef LEPFLIR_EXCLUDE_LOCKING
  this->callbacks.mutex_unlock();
  #endif

  return this->callbacks._lastI2CError;
}
