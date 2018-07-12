#define LEPTON_FLIR_INTERNAL
#include "lepton-communication.h"
#include "lepton-comm-internal.h"
#ifdef TEST
#include <stdio.h>
#endif

uint16_t cmdCode(uint16_t cmdID,uint16_t cmdType)
{
  return (cmdID & LEP_I2C_COMMAND_MODULE_ID_BIT_MASK) | (cmdID &
                                                         LEP_I2C_COMMAND_ID_BIT_MASK)
      | (cmdType & LEP_I2C_COMMAND_TYPE_BIT_MASK);
}

void sendCommand_raw(struct lepton_communication * this,uint16_t cmdCode)
{
  if(this!=0) {
    if (waitCommandBegin(this,LEPFLIR_GEN_CMD_TIMEOUT)) {
      if (writeCmdRegister(this,cmdCode,NULL,0) == 0) {
        waitCommandFinish(this,LEPFLIR_GEN_CMD_TIMEOUT);
      }
    }
  }
}

void sendCommand_u16(struct lepton_communication * this,uint16_t cmdCode,uint16_t value)
{
  if(this!=0) {
    if (waitCommandBegin(this,LEPFLIR_GEN_CMD_TIMEOUT)) {
      if (writeCmdRegister(this,cmdCode,&value,1) == 0) {
        waitCommandFinish(this,LEPFLIR_GEN_CMD_TIMEOUT);
      }
    }
  }
}

void sendCommand_u32(struct lepton_communication * this,uint16_t cmdCode,uint32_t value)
{
  if(this!=0) {
    if (waitCommandBegin(this,LEPFLIR_GEN_CMD_TIMEOUT)) {
      if (writeCmdRegister(this,cmdCode,(uint16_t *) & value,2) == 0) {
        waitCommandFinish(this,LEPFLIR_GEN_CMD_TIMEOUT);
      }
    }
  }
}

void sendCommand_array(struct lepton_communication * this,uint16_t cmdCode,uint16_t * dataWords,int dataLength)
{
  if(this!=0) {
    if(dataWords!=0) {
      if (waitCommandBegin(this,LEPFLIR_GEN_CMD_TIMEOUT)) {
        if (writeCmdRegister(this,cmdCode,dataWords,dataLength) == 0) {
          waitCommandFinish(this,LEPFLIR_GEN_CMD_TIMEOUT);
        }
      }
    } else {
      this->_lastLepResult = LEP_UNDEFINED_ERROR_CODE;
    }
  }
}

void receiveCommand_u16(struct lepton_communication * this,uint16_t cmdCode,uint16_t * value)
{
  if(this!=0) {
    if(value!=0) {
      if (waitCommandBegin(this,LEPFLIR_GEN_CMD_TIMEOUT)) {
        if (writeRegister(this,LEP_I2C_COMMAND_REG,cmdCode) == 0) {
          if (waitCommandFinish(this,LEPFLIR_GEN_CMD_TIMEOUT)) {
            readDataRegister(this,value,1);
          }
        }
      }
    } else {
      this->_lastLepResult = LEP_UNDEFINED_ERROR_CODE;
    }
  }
}

void receiveCommand_u32(struct lepton_communication * this,uint16_t cmdCode,uint32_t * value)
{
  if (waitCommandBegin(this,LEPFLIR_GEN_CMD_TIMEOUT)) {
    if (writeRegister(this,LEP_I2C_COMMAND_REG,cmdCode) == 0) {
      if (waitCommandFinish(this,LEPFLIR_GEN_CMD_TIMEOUT)) {
        readDataRegister(this,(uint16_t *) value,2);
      }
    }
  }
}

void receiveCommand_array(struct lepton_communication * this,uint16_t cmdCode,uint16_t * readWords,int maxLength)
{
  if (waitCommandBegin(this,LEPFLIR_GEN_CMD_TIMEOUT)) {
    if (writeRegister(this,LEP_I2C_COMMAND_REG,cmdCode) == 0) {
      if (waitCommandFinish(this,LEPFLIR_GEN_CMD_TIMEOUT)) {
        readDataRegister(this,readWords,maxLength);
      }
    }
  }
}

void lepton_communication_init(struct lepton_communication * this)
{
  this->_lastLepResult = 0;
}
