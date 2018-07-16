#ifndef EXAMPLE_H_
#define EXAMPLE_H_

#include <stdlib.h>
#include <inttypes.h>
#include "fake_system.h"
#include "lepton-flir.h"

void lepton_beginTransmission(struct lepton_callbacks * this, uint8_t addr);
uint8_t lepton_endTransmission(struct lepton_callbacks * this);
uint8_t lepton_requestFrom(struct lepton_callbacks * this, uint8_t addr, uint8_t len);
size_t lepton_write(struct lepton_callbacks * this, uint8_t data);
size_t lepton_write16(struct lepton_callbacks * this, uint16_t data);
uint8_t lepton_read(struct lepton_callbacks * this);
uint16_t lepton_read16(struct lepton_callbacks * this);

void lepton_setup_callbacks(struct lepton_driver * driver);

#endif