#define LEPTON_FLIR_INTERNAL
#include "lepton-comm-internal.h"
#include "lepton-register.h"

#ifdef TEST
#include <stdio.h>
#endif

#define true 1
#define false 0

#define min(a,b) (((a)<(b))?(a):(b))

static inline uint8_t waitCommandCommon(struct lepton_communication *this, int timeout)
{
  this->_lastLepResult = 0;
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

uint8_t waitCommandBegin(struct lepton_communication *this, int timeout)
{
  return waitCommandCommon(this, timeout);
}

uint8_t waitCommandFinish(struct lepton_communication * this, int timeout)
{
  return waitCommandCommon(this, timeout);
}

int writeCmdRegister(struct lepton_communication *this, uint16_t cmdCode,
                     uint16_t * dataWords, int dataLength)
{
  // In avr/libraries/Wire.h and avr/libraries/utility/twi.h, buffer_length controls
  // how many words can be written at once. Therefore, we loop around until all words
  // have been written out into their registers.

  if (dataWords && dataLength) {
    int ret = writeLengthRegister(this, dataLength);
    if (ret)
      return this->callbacks._lastI2CError;

    int maxLength = this->buffer_length / 2;
    int writeLength = min(maxLength, dataLength);
    uint16_t regAddress =
        dataLength <= 16 ? LEP_I2C_DATA_0_REG : LEP_I2C_DATA_BUFFER;

    while (dataLength > 0) {
      ret = writeDataCommon(this,regAddress,dataWords,&writeLength);
      if (ret)
        return this->callbacks._lastI2CError;

      regAddress += maxLength * 0x02;
      dataLength -= maxLength;
      writeLength = min(maxLength, dataLength);
    }
  }

  int ret = writeCommandCode(this,cmdCode);
  return ret;
}

int readDataRegister(struct lepton_communication *this, uint16_t * readWords,
                     int maxLength)
{
  int ret = selectLengthRegister(this);
  if (ret)
    return this->callbacks._lastI2CError;

  int readLength = 0;
  if(readLengthRegister(this,&readLength)!=0){
    ret = (this->callbacks._lastI2CError = 4);
    return ret;
  }

  if (readLength == 0)
    return (this->callbacks._lastI2CError = 4);

  // In avr/libraries/Wire.h and avr/libraries/utility/twi.h, buffer_length controls
  // how many words can be read at once. Therefore, we loop around until all words
  // have been read out from their registers.

  readDataCommon(this,readWords,maxLength,readLength);

  return (this->callbacks._lastI2CError = readLength ? 4 : 0);
}




