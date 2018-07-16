#ifndef _FAKE_SYSTEM_H
#define _FAKE_SYSTEM_H

#include <inttypes.h>

void delay(long unsigned int delattime);
void critical_i2c_lock(void);
void critical_i2c_unlock(void);
long unsigned int millis(void);

void TwoWire_beginTransmission(uint8_t address);
uint8_t TwoWire_endTransmission(void);
uint8_t TwoWire_requestFrom(uint8_t address, uint8_t quantity);
uint8_t TwoWire_write(uint8_t data);
int TwoWire_read(void);

#endif /* _FAKE_SYSTEM_H */