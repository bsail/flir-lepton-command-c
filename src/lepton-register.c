#define LEPTON_FLIR_INTERNAL
#include "lepton-register.h"

#define min(a,b) (((a)<(b))?(a):(b))

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

  int bytesRead =
      this->callbacks.i2cWire_requestFrom(&(this->callbacks),
                                          LEP_I2C_DEVICE_ADDRESS, 2);
  if (bytesRead != 2) {
    while (bytesRead-- > 0)
      this->callbacks.i2cWire_read(&(this->callbacks));
    ret = (this->callbacks._lastI2CError = 4);
    #ifndef LEPFLIR_EXCLUDE_LOCKING
    this->callbacks.mutex_unlock();
    #endif
    return ret;
  }

  *value = this->callbacks.i2cWire_read16(&(this->callbacks));

  #ifndef LEPFLIR_EXCLUDE_LOCKING
  this->callbacks.mutex_unlock();
  #endif

  return this->callbacks._lastI2CError;
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

int writeLengthRegister(struct lepton_communication *this, int dataLength)
{
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
  return ret;
}

int selectLengthRegister(struct lepton_communication *this)
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
  return ret;
}

int writeDataCommon(struct lepton_communication *this, uint16_t regAddress, uint16_t * dataWords, int * writeLength)
{
  int writeLengthInternal = *writeLength;
  #ifndef LEPFLIR_EXCLUDE_LOCKING
  this->callbacks.mutex_lock();
  #endif
  this->callbacks.i2cWire_beginTransmission(&(this->callbacks),
                                            LEP_I2C_DEVICE_ADDRESS);
  this->callbacks.i2cWire_write16(&(this->callbacks), regAddress);
  while (writeLengthInternal-- > 0)
    this->callbacks.i2cWire_write16(&(this->callbacks), *dataWords++);
  int ret = this->callbacks.i2cWire_endTransmission(&(this->callbacks));
  #ifndef LEPFLIR_EXCLUDE_LOCKING
  this->callbacks.mutex_unlock();
  #endif
  *writeLength = writeLengthInternal;
  return ret;
}

int writeCommandCode(struct lepton_communication *this, uint16_t cmdCode)
{
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

int readLengthRegister(struct lepton_communication *this, int * readLength)
{
  #ifndef LEPFLIR_EXCLUDE_LOCKING
  this->callbacks.mutex_lock();
  #endif
  int bytesRead =
      this->callbacks.i2cWire_requestFrom(&(this->callbacks),
                                          LEP_I2C_DEVICE_ADDRESS, 2);
  if (bytesRead != 2) {
    while (bytesRead-- > 0)
      this->callbacks.i2cWire_read(&(this->callbacks));
    #ifndef LEPFLIR_EXCLUDE_LOCKING
    this->callbacks.mutex_unlock();
    #endif
    int ret = (this->callbacks._lastI2CError = 4);
    return ret;
  }

  *readLength = this->callbacks.i2cWire_read16(&(this->callbacks));

  #ifndef LEPFLIR_EXCLUDE_LOCKING
  this->callbacks.mutex_unlock();
  #endif

  return 0;
}

void readDataCommon(struct lepton_communication *this, uint16_t * readWords, int maxLength, int readLength)
{
#ifndef LEPFLIR_EXCLUDE_LOCKING
  this->callbacks.mutex_lock();
  #endif

  int bytesRead =
      this->callbacks.i2cWire_requestFrom(&(this->callbacks),
                                          LEP_I2C_DEVICE_ADDRESS,
                                          min(this->buffer_length, readLength));

  while (bytesRead > 0 && readLength > 0) {

    while (bytesRead > 1 && readLength > 1 && maxLength > 0) {
      *readWords++ = this->callbacks.i2cWire_read16(&(this->callbacks));
      bytesRead -= 2;
      readLength -= 2;
      --maxLength;
    }

    if (readLength > 0)
      bytesRead +=
          this->callbacks.i2cWire_requestFrom(&(this->callbacks),
                                              LEP_I2C_DEVICE_ADDRESS,
                                              min(this->buffer_length,
                                                  readLength));
  }

  while (bytesRead-- > 0)
    this->callbacks.i2cWire_read(&(this->callbacks));

  #ifndef LEPFLIR_EXCLUDE_LOCKING
  this->callbacks.mutex_unlock();
  #endif

  while (maxLength-- > 0)
    *readWords++ = 0;
}
