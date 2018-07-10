#include "lepton-communication.h"

#define true 1
#define false 0

#define min(a,b) (((a)<(b))?(a):(b))

uint8_t waitCommandBegin(int timeout, struct lepton_communication * this)
{
  this->_lastLepResult = 0;
  uint16_t status;
  if (this->readRegister(LEP_I2C_STATUS_REG, &status, this))
    return false;

  if (!(status & LEP_I2C_STATUS_BUSY_BIT_MASK))
    return true;

  unsigned long endTime = this->callbacks->millis_callback() + (unsigned long)timeout;

  while ((status & LEP_I2C_STATUS_BUSY_BIT_MASK)
         && (timeout <= 0 || this->callbacks->millis_callback() < endTime)) {
    this->callbacks->delay_callback(1);

    if (this->readRegister(LEP_I2C_STATUS_REG, &status, this))
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
  if (this->readRegister(LEP_I2C_STATUS_REG, &status, this))
    return false;

  if (!(status & LEP_I2C_STATUS_BUSY_BIT_MASK)) {
    this->_lastLepResult =
        (uint8_t) ((status & LEP_I2C_STATUS_ERROR_CODE_BIT_MASK) >>
                   LEP_I2C_STATUS_ERROR_CODE_BIT_SHIFT);
    return true;
  }

  unsigned long endTime = this->callbacks->millis_callback() + (unsigned long)timeout;

  while ((status & LEP_I2C_STATUS_BUSY_BIT_MASK)
         && (timeout <= 0 || this->callbacks->millis_callback() < endTime)) {
    this->callbacks->delay_callback(1);

    if (this->readRegister(LEP_I2C_STATUS_REG, &status, this))
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

uint16_t cmdCode(uint16_t cmdID, uint16_t cmdType)
{
  return (cmdID & LEP_I2C_COMMAND_MODULE_ID_BIT_MASK) | (cmdID &
                                                         LEP_I2C_COMMAND_ID_BIT_MASK)
      | (cmdType & LEP_I2C_COMMAND_TYPE_BIT_MASK);
}

void sendCommand_raw(uint16_t cmdCode, struct lepton_communication * this)
{
  if (this->waitCommandBegin(LEPFLIR_GEN_CMD_TIMEOUT,this)) {

    if (this->writeCmdRegister(cmdCode, NULL, 0,this) == 0) {

      this->waitCommandFinish(LEPFLIR_GEN_CMD_TIMEOUT,this);
    }
  }
}

void sendCommand_u16(uint16_t cmdCode, uint16_t value, struct lepton_communication * this)
{
  if (this->waitCommandBegin(LEPFLIR_GEN_CMD_TIMEOUT,this)) {

    if (this->writeCmdRegister(cmdCode, &value, 1,this) == 0) {

      this->waitCommandFinish(LEPFLIR_GEN_CMD_TIMEOUT,this);
    }
  }
}

void sendCommand_u32(uint16_t cmdCode, uint32_t value, struct lepton_communication * this)
{
  if (this->waitCommandBegin(LEPFLIR_GEN_CMD_TIMEOUT,this)) {

    if (this->writeCmdRegister(cmdCode, (uint16_t *) & value, 2,this) == 0) {

      this->waitCommandFinish(LEPFLIR_GEN_CMD_TIMEOUT,this);
    }
  }
}

void sendCommand_array(uint16_t cmdCode, uint16_t * dataWords, int dataLength, struct lepton_communication * this)
{
  if (this->waitCommandBegin(LEPFLIR_GEN_CMD_TIMEOUT,this)) {

    if (this->writeCmdRegister(cmdCode, dataWords, dataLength,this) == 0) {

      this->waitCommandFinish(LEPFLIR_GEN_CMD_TIMEOUT,this);
    }
  }
}

void receiveCommand_u16(uint16_t cmdCode, uint16_t * value, struct lepton_communication * this)
{
  if (this->waitCommandBegin(LEPFLIR_GEN_CMD_TIMEOUT,this)) {

    if (this->writeRegister(LEP_I2C_COMMAND_REG, cmdCode,this) == 0) {

      if (this->waitCommandFinish(LEPFLIR_GEN_CMD_TIMEOUT,this)) {

        this->readDataRegister(value, 1,this);
      }
    }
  }
}

void receiveCommand_u32(uint16_t cmdCode, uint32_t * value, struct lepton_communication * this)
{
  if (this->waitCommandBegin(LEPFLIR_GEN_CMD_TIMEOUT,this)) {

    if (this->writeRegister(LEP_I2C_COMMAND_REG, cmdCode,this) == 0) {

      if (this->waitCommandFinish(LEPFLIR_GEN_CMD_TIMEOUT,this)) {

        this->readDataRegister((uint16_t *) value, 2,this);
      }
    }
  }
}

void receiveCommand_array(uint16_t cmdCode, uint16_t * readWords, int maxLength, struct lepton_communication * this)
{
  if (this->waitCommandBegin(LEPFLIR_GEN_CMD_TIMEOUT,this)) {

    if (this->writeRegister(LEP_I2C_COMMAND_REG, cmdCode,this) == 0) {

      if (this->waitCommandFinish(LEPFLIR_GEN_CMD_TIMEOUT,this)) {

        this->readDataRegister(readWords, maxLength,this);
      }
    }
  }
}

int writeCmdRegister(uint16_t cmdCode, uint16_t * dataWords, int dataLength, struct lepton_communication * this)
{
  // In avr/libraries/Wire.h and avr/libraries/utility/twi.h, BUFFER_LENGTH controls
  // how many words can be written at once. Therefore, we loop around until all words
  // have been written out into their registers.

  if (dataWords && dataLength) {
    this->callbacks->i2cWire_beginTransmission(LEP_I2C_DEVICE_ADDRESS,this->callbacks);
    this->callbacks->i2cWire_write16(LEP_I2C_DATA_LENGTH_REG,this->callbacks);
    this->callbacks->i2cWire_write16(dataLength,this->callbacks);
    if (this->callbacks->i2cWire_endTransmission(this->callbacks))
      return this->callbacks->_lastI2CError;

    int maxLength = this->BUFFER_LENGTH / 2;
    int writeLength = min(maxLength, dataLength);
    uint16_t regAddress =
        dataLength <= 16 ? LEP_I2C_DATA_0_REG : LEP_I2C_DATA_BUFFER;

    while (dataLength > 0) {
      this->callbacks->i2cWire_beginTransmission(LEP_I2C_DEVICE_ADDRESS,this->callbacks);
      this->callbacks->i2cWire_write16(regAddress,this->callbacks);

      while (writeLength-- > 0)
        this->callbacks->i2cWire_write16(*dataWords++,this->callbacks);

      if (this->callbacks->i2cWire_endTransmission(this->callbacks))
        return this->callbacks->_lastI2CError;

      regAddress += maxLength * 0x02;
      dataLength -= maxLength;
      writeLength = min(maxLength, dataLength);
    }
  }

  this->callbacks->i2cWire_beginTransmission(LEP_I2C_DEVICE_ADDRESS,this->callbacks);
  this->callbacks->i2cWire_write16(LEP_I2C_COMMAND_REG,this->callbacks);
  this->callbacks->i2cWire_write16(cmdCode,this->callbacks);
  return this->callbacks->i2cWire_endTransmission(this->callbacks);
}

int readDataRegister(uint16_t * readWords, int maxLength, struct lepton_communication * this)
{
  this->callbacks->i2cWire_beginTransmission(LEP_I2C_DEVICE_ADDRESS,this->callbacks);
  this->callbacks->i2cWire_write16(LEP_I2C_DATA_LENGTH_REG,this->callbacks);
  if (this->callbacks->i2cWire_endTransmission(this->callbacks))
    return this->callbacks->_lastI2CError;

  int uint8_tsRead = this->callbacks->i2cWire_requestFrom(LEP_I2C_DEVICE_ADDRESS, 2,this->callbacks);
  if (uint8_tsRead != 2) {
    while (uint8_tsRead-- > 0)
      this->callbacks->i2cWire_read(this->callbacks);
    return (this->callbacks->_lastI2CError = 4);
  }

  int readLength = this->callbacks->i2cWire_read16(this->callbacks);

  if (readLength == 0)
    return (this->callbacks->_lastI2CError = 4);

  // In avr/libraries/Wire.h and avr/libraries/utility/twi.h, BUFFER_LENGTH controls
  // how many words can be read at once. Therefore, we loop around until all words
  // have been read out from their registers.

  uint8_tsRead =
      this->callbacks->i2cWire_requestFrom(LEP_I2C_DEVICE_ADDRESS,
                          min(this->BUFFER_LENGTH, readLength),this->callbacks);

  while (uint8_tsRead > 0 && readLength > 0) {

    while (uint8_tsRead > 1 && readLength > 1 && maxLength > 0) {
      *readWords++ = this->callbacks->i2cWire_read16(this->callbacks);
      uint8_tsRead -= 2;
      readLength -= 2;
      --maxLength;
    }

    if (readLength > 0)
      uint8_tsRead +=
          this->callbacks->i2cWire_requestFrom(LEP_I2C_DEVICE_ADDRESS,
                              min(this->BUFFER_LENGTH, readLength),this->callbacks);
  }

  while (uint8_tsRead-- > 0)
    this->callbacks->i2cWire_read(this->callbacks);

  while (maxLength-- > 0)
    *readWords++ = 0;

  return (this->callbacks->_lastI2CError = readLength ? 4 : 0);
}

int writeRegister(uint16_t regAddress, uint16_t value, struct lepton_communication * this)
{
  this->callbacks->i2cWire_beginTransmission(LEP_I2C_DEVICE_ADDRESS,this->callbacks);
  this->callbacks->i2cWire_write16(regAddress,this->callbacks);
  this->callbacks->i2cWire_write16(value,this->callbacks);
  return this->callbacks->i2cWire_endTransmission(this->callbacks);
}

int readRegister(uint16_t regAddress, uint16_t * value, struct lepton_communication * this)
{
  this->callbacks->i2cWire_beginTransmission(LEP_I2C_DEVICE_ADDRESS,this->callbacks);
  this->callbacks->i2cWire_write16(regAddress,this->callbacks);
  if (this->callbacks->i2cWire_endTransmission(this->callbacks))
    return this->callbacks->_lastI2CError;

  int uint8_tsRead = this->callbacks->i2cWire_requestFrom(LEP_I2C_DEVICE_ADDRESS, 2,this->callbacks);
  if (uint8_tsRead != 2) {
    while (uint8_tsRead-- > 0)
      this->callbacks->i2cWire_read(this->callbacks);
    return (this->callbacks->_lastI2CError = 4);
  }

  *value = this->callbacks->i2cWire_read16(this->callbacks);

  return this->callbacks->_lastI2CError;
}

void lepton_communication_init(struct lepton_communication * this, struct lepton_callbacks * callbacks)
{
  this->callbacks = callbacks;
  this->_lastLepResult = 0;

  this->waitCommandBegin = &waitCommandBegin;
  this->waitCommandFinish = &waitCommandFinish;
  this->cmdCode = &cmdCode;
  this->sendCommand_raw = &sendCommand_raw;
  this->sendCommand_u16 = &sendCommand_u16;
  this->sendCommand_u32 = &sendCommand_u32;
  this->sendCommand_array = &sendCommand_array;
  this->receiveCommand_u16 = &receiveCommand_u16;
  this->receiveCommand_u32 = &receiveCommand_u32;
  this->receiveCommand_array = &receiveCommand_array;
  this->writeCmdRegister = &writeCmdRegister;
  this->readDataRegister = &readDataRegister;
  this->writeRegister = &writeRegister;
  this->readRegister = &readRegister;
}
