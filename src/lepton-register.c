#define LEPTON_FLIR_INTERNAL
#include "lepton-register.h"

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
