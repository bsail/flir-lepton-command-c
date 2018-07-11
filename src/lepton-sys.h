#ifndef _LEPTON_SYS_H
#define _LEPTON_SYS_H

#include "lepton-flir.h"

#ifndef TEST
#ifndef LEPTON_FLIR_INTERNAL
#error "There is no user accessable data and functions here"
#endif
#endif

#ifndef LEPFLIR_EXCLUDE_SYS_FUNCS

void lepton_sys_init(struct lepton_sys * this);

#endif

#endif // _LEPTON_SYS_H
