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
  int writeLengthRegister(struct lepton_communication *this, int dataLength);
  int selectLengthRegister(struct lepton_communication *this);
  int writeDataCommon(struct lepton_communication *this, uint16_t regAddress, uint16_t * dataWords, int * writeLength);
  int writeCommandCode(struct lepton_communication *this, uint16_t cmdCode);
  int readLengthRegister(struct lepton_communication *this, int * readLength);
  void readDataCommon(struct lepton_communication *this, uint16_t * readWords, int maxLength, int readLength);

#ifdef __cplusplus
}
#endif
#endif // _LEPTON_REGISTER_H
