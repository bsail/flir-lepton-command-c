#ifndef _LEPTON_COMMUNICATION_H
#define _LEPTON_COMMUNICATION_H

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include "lepton-flir.h"

#ifndef TEST
#ifndef LEPTON_FLIR_INTERNAL
#error "There is no user accessable data and functions here"
#endif
#endif

uint16_t cmdCode(uint16_t cmdID, uint16_t cmdType);
void sendCommand_raw (struct lepton_communication * this,uint16_t cmdCode);
void sendCommand_u16 (struct lepton_communication * this,uint16_t cmdCode, uint16_t value);
void sendCommand_u32 (struct lepton_communication * this,uint16_t cmdCode, uint32_t value);
void sendCommand_array (struct lepton_communication * this,uint16_t cmdCode, uint16_t * dataWords, int dataLength);
void receiveCommand_u16 (struct lepton_communication * this,uint16_t cmdCode, uint16_t * value);
void receiveCommand_u32 (struct lepton_communication * this,uint16_t cmdCode, uint32_t * value);
void receiveCommand_array (struct lepton_communication * this,uint16_t cmdCode, uint16_t * readWords, int maxLength);
void lepton_communication_init(struct lepton_communication * this);

#ifdef __cplusplus
}
#endif

#endif // _LEPTON_COMMUNICATION_H
