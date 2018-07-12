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

uint8_t waitCommandBegin(struct lepton_communication * this,int timeout);
uint8_t waitCommandFinish(struct lepton_communication * this,int timeout);
int writeCmdRegister (struct lepton_communication * this,uint16_t cmdCode, uint16_t * dataWords, int dataLength);
int readDataRegister (struct lepton_communication * this,uint16_t * readWords, int maxLength);
int writeRegister (struct lepton_communication * this,uint16_t regAddress, uint16_t value);
int readRegister (struct lepton_communication * this,uint16_t regAddress, uint16_t * value);

#ifdef __cplusplus
}
#endif

#endif // _LEPTON_COMM_INTERNAL_H
