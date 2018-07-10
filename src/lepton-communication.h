#ifndef _LEPTON_COMMUNICATION_H
#define _LEPTON_COMMUNICATION_H

#include <inttypes.h>
#include "lepton-flir.h"

struct lepton_communication {
  struct lepton_callbacks * callbacks;
  LeptonFLiR_ImageStorageMode _storageMode; // Image data storage mode
  LeptonFLiR_TemperatureMode _tempMode; // Temperature display mode
  uint8_t _lastLepResult;         // Last lep result
  int BUFFER_LENGTH;
  uint8_t (*waitCommandBegin)(int timeout, struct lepton_communication * this);
  uint8_t (*waitCommandFinish)(int timeout, struct lepton_communication * this);
  uint16_t (*cmdCode)(uint16_t cmdID, uint16_t cmdType);
  void (*sendCommand_raw)(uint16_t cmdCode, struct lepton_communication * this);
  void (*sendCommand_u16)(uint16_t cmdCode, uint16_t value, struct lepton_communication * this);
  void (*sendCommand_u32)(uint16_t cmdCode, uint32_t value, struct lepton_communication * this);
  void (*sendCommand_array)(uint16_t cmdCode, uint16_t * dataWords, int dataLength, struct lepton_communication * this);
  void (*receiveCommand_u16)(uint16_t cmdCode, uint16_t * value, struct lepton_communication * this);
  void (*receiveCommand_u32)(uint16_t cmdCode, uint32_t * value, struct lepton_communication * this);
  void (*receiveCommand_array)(uint16_t cmdCode, uint16_t * readWords, int maxLength, struct lepton_communication * this);
  int (*writeCmdRegister)(uint16_t cmdCode, uint16_t * dataWords, int dataLength, struct lepton_communication * this);
  int (*readDataRegister)(uint16_t * readWords, int maxLength, struct lepton_communication * this);
  int (*writeRegister)(uint16_t regAddress, uint16_t value, struct lepton_communication * this);
  int (*readRegister)(uint16_t regAddress, uint16_t * value, struct lepton_communication * this);
};

void lepton_communication_init(struct lepton_communication * this, struct lepton_callbacks * callbacks);

#endif // _LEPTON_COMMUNICATION_H
