#ifndef _LEPTON_REGISTER_H
#define _LEPTON_REGISTER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lepton-flir.h"

#ifndef TEST
#ifndef LEPTON_FLIR_INTERNAL
#error "There is no user accessable data and functions here"
#endif
#endif

  int readRegister(struct lepton_communication *this, uint16_t regAddress,
                   uint16_t * value);
  int writeRegister(struct lepton_communication *this, uint16_t regAddress,
                    uint16_t value);

#ifdef __cplusplus
}
#endif
#endif // _LEPTON_REGISTER_H
