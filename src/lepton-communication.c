#define LEPTON_FLIR_INTERNAL
#include "lepton-communication.h"
#include "lepton-comm-internal.h"
#ifdef TEST
#include <stdio.h>
#endif

uint16_t cmdCode(uint16_t cmdID, uint16_t cmdType)
{
  return (cmdID & LEP_I2C_COMMAND_MODULE_ID_BIT_MASK) | (cmdID &
                                                         LEP_I2C_COMMAND_ID_BIT_MASK)
      | (cmdType & LEP_I2C_COMMAND_TYPE_BIT_MASK);
}

void sendCommand_raw(uint16_t cmdCode, struct lepton_communication * this)
{
  if (waitCommandBegin(LEPFLIR_GEN_CMD_TIMEOUT,this)) {
    if (writeCmdRegister(cmdCode, NULL, 0,this) == 0) {
      waitCommandFinish(LEPFLIR_GEN_CMD_TIMEOUT,this);
    }
  }
}

void sendCommand_u16(uint16_t cmdCode, uint16_t value, struct lepton_communication * this)
{
  if (waitCommandBegin(LEPFLIR_GEN_CMD_TIMEOUT,this)) {
    if (writeCmdRegister(cmdCode, &value, 1,this) == 0) {
      waitCommandFinish(LEPFLIR_GEN_CMD_TIMEOUT,this);
    }
  }
}

void sendCommand_u32(uint16_t cmdCode, uint32_t value, struct lepton_communication * this)
{
  if (waitCommandBegin(LEPFLIR_GEN_CMD_TIMEOUT,this)) {
    if (writeCmdRegister(cmdCode, (uint16_t *) & value, 2,this) == 0) {
      waitCommandFinish(LEPFLIR_GEN_CMD_TIMEOUT,this);
    }
  }
}

void sendCommand_array(uint16_t cmdCode, uint16_t * dataWords, int dataLength, struct lepton_communication * this)
{
  if (waitCommandBegin(LEPFLIR_GEN_CMD_TIMEOUT,this)) {
    if (writeCmdRegister(cmdCode, dataWords, dataLength,this) == 0) {
      waitCommandFinish(LEPFLIR_GEN_CMD_TIMEOUT,this);
    }
  }
}

void receiveCommand_u16(uint16_t cmdCode, uint16_t * value, struct lepton_communication * this)
{
  if (waitCommandBegin(LEPFLIR_GEN_CMD_TIMEOUT,this)) {
    if (writeRegister(LEP_I2C_COMMAND_REG, cmdCode,this) == 0) {
      if (waitCommandFinish(LEPFLIR_GEN_CMD_TIMEOUT,this)) {
        readDataRegister(value, 1,this);
      }
    }
  }
}

void receiveCommand_u32(uint16_t cmdCode, uint32_t * value, struct lepton_communication * this)
{
  if (waitCommandBegin(LEPFLIR_GEN_CMD_TIMEOUT,this)) {
    if (writeRegister(LEP_I2C_COMMAND_REG, cmdCode,this) == 0) {
      if (waitCommandFinish(LEPFLIR_GEN_CMD_TIMEOUT,this)) {
        readDataRegister((uint16_t *) value, 2,this);
      }
    }
  }
}

void receiveCommand_array(uint16_t cmdCode, uint16_t * readWords, int maxLength, struct lepton_communication * this)
{
  if (waitCommandBegin(LEPFLIR_GEN_CMD_TIMEOUT,this)) {
    if (writeRegister(LEP_I2C_COMMAND_REG, cmdCode,this) == 0) {
      if (waitCommandFinish(LEPFLIR_GEN_CMD_TIMEOUT,this)) {
        readDataRegister(readWords, maxLength,this);
      }
    }
  }
}

void lepton_communication_init(struct lepton_communication * this)
{
  this->_lastLepResult = 0;
}
