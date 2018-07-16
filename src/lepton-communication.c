#define LEPTON_FLIR_INTERNAL
#include "lepton-communication.h"
#include "lepton-comm-internal.h"
#include "lepton-register.h"
#ifdef TEST
#include <stdio.h>
#endif

uint16_t cmdCode(uint16_t cmdID, uint16_t cmdType)
{
  return (cmdID & LEP_I2C_COMMAND_MODULE_ID_BIT_MASK) | (cmdID &
                                                         LEP_I2C_COMMAND_ID_BIT_MASK)
      | (cmdType & LEP_I2C_COMMAND_TYPE_BIT_MASK);
}

static inline void sendCommand_common(struct lepton_communication *this,
                                      uint16_t cmdCode, uint16_t * value,
                                      int dataLength, uint8_t isRaw)
{
  if (this != 0) {
    if ((value != 0) && (dataLength != 0)) {
      if (waitCommandBegin(this, LEPFLIR_GEN_CMD_TIMEOUT)) {
        #ifdef TEST
        // printf("Value:%d, Length:%d",*value,dataLength);
        #endif
        if (writeCmdRegister(this, cmdCode, value, dataLength) == 0) {
          waitCommandFinish(this, LEPFLIR_GEN_CMD_TIMEOUT);
        }
      }
    } else if (isRaw == 1) {
      if (waitCommandBegin(this, LEPFLIR_GEN_CMD_TIMEOUT)) {
        if (writeCmdRegister(this, cmdCode, NULL, 0) == 0) {
          waitCommandFinish(this, LEPFLIR_GEN_CMD_TIMEOUT);
        }
      }
    } else {
      this->_lastLepResult = LEP_UNDEFINED_ERROR_CODE;
    }
  }
}

void sendCommand_raw(struct lepton_communication *this, uint16_t cmdCode)
{
  sendCommand_common(this, cmdCode, 0, 0, 1);
}

void sendCommand_u16(struct lepton_communication *this, uint16_t cmdCode,
                     uint16_t value)
{
  sendCommand_common(this, cmdCode, &value, 1, 0);
}

void sendCommand_u32(struct lepton_communication *this, uint16_t cmdCode,
                     uint32_t value)
{
  sendCommand_common(this, cmdCode, (uint16_t *) & value, 2, 0);
}

void sendCommand_array(struct lepton_communication *this, uint16_t cmdCode,
                       uint16_t * dataWords, int dataLength)
{
  sendCommand_common(this, cmdCode, dataWords, dataLength, 0);
}

static inline void receiveCommand_common(struct lepton_communication *this,
                                         uint16_t cmdCode, uint16_t * value,
                                         int dataLength)
{
  if (this != 0) {
    if ((value != 0) && (dataLength != 0)) {
      if (waitCommandBegin(this, LEPFLIR_GEN_CMD_TIMEOUT)) {
        if (writeRegister(this, LEP_I2C_COMMAND_REG, cmdCode) == 0) {
          if (waitCommandFinish(this, LEPFLIR_GEN_CMD_TIMEOUT)) {
            readDataRegister(this, value, dataLength);
          }
        }
      }
    } else {
      this->_lastLepResult = LEP_UNDEFINED_ERROR_CODE;
    }
  }
}

void receiveCommand_u16(struct lepton_communication *this, uint16_t cmdCode,
                        uint16_t * value)
{
  receiveCommand_common(this, cmdCode, value, 1);
}

void receiveCommand_u32(struct lepton_communication *this, uint16_t cmdCode,
                        uint32_t * value)
{
  receiveCommand_common(this, cmdCode, (uint16_t *) value, 2);
}

void receiveCommand_array(struct lepton_communication *this, uint16_t cmdCode,
                          uint16_t * readWords, int maxLength)
{
  receiveCommand_common(this, cmdCode, readWords, maxLength);
}

void lepton_communication_init(struct lepton_communication *this)
{
  if (this != 0)
    this->_lastLepResult = 0;
}
