#ifndef _LEPTON_VID_H
#define _LEPTON_VID_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lepton-flir.h"

#ifndef TEST
#ifndef LEPTON_FLIR_INTERNAL
#error "There is no user accessable data and functions here"
#endif
#endif

#ifndef LEPFLIR_EXCLUDE_VID_FUNCS

void lepton_vid_init(struct lepton_vid * this);

#endif

#ifdef __cplusplus
}
#endif

#endif // _LEPTON_VID_H
