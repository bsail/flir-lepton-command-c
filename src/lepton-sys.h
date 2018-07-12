#ifndef _LEPTON_SYS_H
#define _LEPTON_SYS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lepton-flir.h"

#ifndef TEST
#ifndef LEPTON_FLIR_INTERNAL
#error "There is no user accessable data and functions here"
#endif
#endif

#ifndef LEPFLIR_EXCLUDE_SYS_FUNCS

void lepton_sys_init(struct lepton_sys * this);

#endif

#ifdef TEST

LEP_SYS_CAM_STATUS_STATES getCameraStatus(struct lepton_driver * driver);

#endif

#ifdef __cplusplus
}
#endif

#endif // _LEPTON_SYS_H
