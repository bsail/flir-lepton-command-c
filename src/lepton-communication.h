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
void sendCommand_raw (uint16_t cmdCode, struct lepton_communication * this);
void sendCommand_u16 (uint16_t cmdCode, uint16_t value, struct lepton_communication * this);
void sendCommand_u32 (uint16_t cmdCode, uint32_t value, struct lepton_communication * this);
void sendCommand_array (uint16_t cmdCode, uint16_t * dataWords, int dataLength, struct lepton_communication * this);
void receiveCommand_u16 (uint16_t cmdCode, uint16_t * value, struct lepton_communication * this);
void receiveCommand_u32 (uint16_t cmdCode, uint32_t * value, struct lepton_communication * this);
void receiveCommand_array (uint16_t cmdCode, uint16_t * readWords, int maxLength, struct lepton_communication * this);
void lepton_communication_init(struct lepton_communication * this);

#ifdef __cplusplus
}
#endif

#endif // _LEPTON_COMMUNICATION_H
