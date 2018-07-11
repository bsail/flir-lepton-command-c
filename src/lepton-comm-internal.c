#define LEPTON_FLIR_INTERNAL
#include "lepton-comm-internal.h"

#define true 1
#define false 0

#define min(a,b) (((a)<(b))?(a):(b))

uint8_t waitCommandBegin(int timeout, struct lepton_communication * this)
{
  this->_lastLepResult = 0;
  uint16_t status;
  if (readRegister(LEP_I2C_STATUS_REG, &status, this))
    return false;

  if (!(status & LEP_I2C_STATUS_BUSY_BIT_MASK))
    return true;

  unsigned long endTime = this->callbacks.millis_callback() + (unsigned long)timeout;

  while ((status & LEP_I2C_STATUS_BUSY_BIT_MASK)
         && (timeout <= 0 || this->callbacks.millis_callback() < endTime)) {
    this->callbacks.delay_callback(1);

    if (readRegister(LEP_I2C_STATUS_REG, &status, this))
      return false;
  }

  if (!(status & LEP_I2C_STATUS_BUSY_BIT_MASK))
    return true;
  else {
    this->_lastLepResult = LEP_TIMEOUT_ERROR;
    return false;
  }
}

uint8_t waitCommandFinish(int timeout, struct lepton_communication * this)
{
  uint16_t status;
  if (readRegister(LEP_I2C_STATUS_REG, &status, this))
    return false;

  if (!(status & LEP_I2C_STATUS_BUSY_BIT_MASK)) {
    this->_lastLepResult =
        (uint8_t) ((status & LEP_I2C_STATUS_ERROR_CODE_BIT_MASK) >>
                   LEP_I2C_STATUS_ERROR_CODE_BIT_SHIFT);
    return true;
  }

  unsigned long endTime = this->callbacks.millis_callback() + (unsigned long)timeout;

  while ((status & LEP_I2C_STATUS_BUSY_BIT_MASK)
         && (timeout <= 0 || this->callbacks.millis_callback() < endTime)) {
    this->callbacks.delay_callback(1);

    if (readRegister(LEP_I2C_STATUS_REG, &status, this))
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

int writeCmdRegister(uint16_t cmdCode, uint16_t * dataWords, int dataLength, struct lepton_communication * this)
{
  // In avr/libraries/Wire.h and avr/libraries/utility/twi.h, buffer_length controls
  // how many words can be written at once. Therefore, we loop around until all words
  // have been written out into their registers.

  if (dataWords && dataLength) {
    this->callbacks.i2cWire_beginTransmission(LEP_I2C_DEVICE_ADDRESS,&(this->callbacks));
    this->callbacks.i2cWire_write16(LEP_I2C_DATA_LENGTH_REG,&(this->callbacks));
    this->callbacks.i2cWire_write16(dataLength,&(this->callbacks));
    if (this->callbacks.i2cWire_endTransmission(&(this->callbacks)))
      return this->callbacks._lastI2CError;

    int maxLength = this->buffer_length / 2;
    int writeLength = min(maxLength, dataLength);
    uint16_t regAddress =
        dataLength <= 16 ? LEP_I2C_DATA_0_REG : LEP_I2C_DATA_BUFFER;

    while (dataLength > 0) {
      this->callbacks.i2cWire_beginTransmission(LEP_I2C_DEVICE_ADDRESS,&(this->callbacks));
      this->callbacks.i2cWire_write16(regAddress,&(this->callbacks));

      while (writeLength-- > 0)
        this->callbacks.i2cWire_write16(*dataWords++,&(this->callbacks));

      if (this->callbacks.i2cWire_endTransmission(&(this->callbacks)))
        return this->callbacks._lastI2CError;

      regAddress += maxLength * 0x02;
      dataLength -= maxLength;
      writeLength = min(maxLength, dataLength);
    }
  }

  this->callbacks.i2cWire_beginTransmission(LEP_I2C_DEVICE_ADDRESS,&(this->callbacks));
  this->callbacks.i2cWire_write16(LEP_I2C_COMMAND_REG,&(this->callbacks));
  this->callbacks.i2cWire_write16(cmdCode,&(this->callbacks));
  return this->callbacks.i2cWire_endTransmission(&(this->callbacks));
}

int readDataRegister(uint16_t * readWords, int maxLength, struct lepton_communication * this)
{
  this->callbacks.i2cWire_beginTransmission(LEP_I2C_DEVICE_ADDRESS,&(this->callbacks));
  this->callbacks.i2cWire_write16(LEP_I2C_DATA_LENGTH_REG,&(this->callbacks));
  if (this->callbacks.i2cWire_endTransmission(&(this->callbacks)))
    return this->callbacks._lastI2CError;

  int uint8_tsRead = this->callbacks.i2cWire_requestFrom(LEP_I2C_DEVICE_ADDRESS, 2,&(this->callbacks));

  #ifdef TEST
  //printf("000:%d\n",uint8_tsRead);
  #endif


  if (uint8_tsRead != 2) {
    while (uint8_tsRead-- > 0)
      this->callbacks.i2cWire_read(&(this->callbacks));
    return (this->callbacks._lastI2CError = 4);
  }

  int readLength = this->callbacks.i2cWire_read16(&(this->callbacks));

  #ifdef TEST
  //printf("100:%d\n",readLength);
  #endif

  if (readLength == 0)
    return (this->callbacks._lastI2CError = 4);

  // In avr/libraries/Wire.h and avr/libraries/utility/twi.h, buffer_length controls
  // how many words can be read at once. Therefore, we loop around until all words
  // have been read out from their registers.

  uint8_tsRead =
      this->callbacks.i2cWire_requestFrom(LEP_I2C_DEVICE_ADDRESS,
                          min(this->buffer_length, readLength),&(this->callbacks));

  while (uint8_tsRead > 0 && readLength > 0) {

    while (uint8_tsRead > 1 && readLength > 1 && maxLength > 0) {
      *readWords++ = this->callbacks.i2cWire_read16(&(this->callbacks));
      uint8_tsRead -= 2;
      readLength -= 2;
      --maxLength;
    }

    if (readLength > 0)
      uint8_tsRead +=
          this->callbacks.i2cWire_requestFrom(LEP_I2C_DEVICE_ADDRESS,
                              min(this->buffer_length, readLength),&(this->callbacks));
  }

  while (uint8_tsRead-- > 0)
    this->callbacks.i2cWire_read(&(this->callbacks));

  while (maxLength-- > 0)
    *readWords++ = 0;
  #ifdef TEST
  //printf("200:%d\n",readLength);
  #endif
  return (this->callbacks._lastI2CError = readLength ? 4 : 0);
}

int writeRegister(uint16_t regAddress, uint16_t value, struct lepton_communication * this)
{
  this->callbacks.i2cWire_beginTransmission(LEP_I2C_DEVICE_ADDRESS,&(this->callbacks));
  this->callbacks.i2cWire_write16(regAddress,&(this->callbacks));
  this->callbacks.i2cWire_write16(value,&(this->callbacks));
  return this->callbacks.i2cWire_endTransmission(&(this->callbacks));
}

int readRegister(uint16_t regAddress, uint16_t * value, struct lepton_communication * this)
{
  this->callbacks.i2cWire_beginTransmission(LEP_I2C_DEVICE_ADDRESS,&(this->callbacks));
  this->callbacks.i2cWire_write16(regAddress,&(this->callbacks));
  if (this->callbacks.i2cWire_endTransmission(&(this->callbacks)))
    return this->callbacks._lastI2CError;

  int uint8_tsRead = this->callbacks.i2cWire_requestFrom(LEP_I2C_DEVICE_ADDRESS, 2,&(this->callbacks));
  if (uint8_tsRead != 2) {
    while (uint8_tsRead-- > 0)
      this->callbacks.i2cWire_read(&(this->callbacks));
    return (this->callbacks._lastI2CError = 4);
  }

  *value = this->callbacks.i2cWire_read16(&(this->callbacks));

  return this->callbacks._lastI2CError;
}