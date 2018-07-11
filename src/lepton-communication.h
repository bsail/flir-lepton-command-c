#ifndef _LEPTON_COMMUNICATION_H
#define _LEPTON_COMMUNICATION_H

#include <inttypes.h>
#include "lepton-flir.h"

uint16_t cmdCode(uint16_t cmdID, uint16_t cmdType);
void lepton_communication_init(struct lepton_communication * this);

#endif // _LEPTON_COMMUNICATION_H
