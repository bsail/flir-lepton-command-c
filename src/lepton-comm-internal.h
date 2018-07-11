#ifndef _LEPTON_COMM_INTERNAL_H
#define _LEPTON_COMM_INTERNAL_H

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

uint8_t waitCommandBegin(int timeout, struct lepton_communication * this);
uint8_t waitCommandFinish(int timeout, struct lepton_communication * this);
int writeCmdRegister (uint16_t cmdCode, uint16_t * dataWords, int dataLength, struct lepton_communication * this);
int readDataRegister (uint16_t * readWords, int maxLength, struct lepton_communication * this);
int writeRegister (uint16_t regAddress, uint16_t value, struct lepton_communication * this);
int readRegister (uint16_t regAddress, uint16_t * value, struct lepton_communication * this);

#ifdef __cplusplus
}
#endif

#endif // _LEPTON_COMM_INTERNAL_H
