#ifndef _LEPTON_AGC_H
#define _LEPTON_AGC_H

#include "lepton-flir.h"

#ifndef TEST
#ifndef LEPTON_FLIR_INTERNAL
#error "There is no user accessable data and functions here"
#endif
#endif

#ifndef LEPFLIR_EXCLUDE_AGC_FUNCS

void lepton_agc_init(struct lepton_agc * this);

#endif

#endif // _LEPTON_AGC_H
