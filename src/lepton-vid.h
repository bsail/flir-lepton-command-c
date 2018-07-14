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

  void lepton_vid_init(struct lepton_vid *this);

#endif

#ifdef TEST
void setPolarity(struct lepton_driver *driver, LEP_VID_POLARITY polarity);
LEP_VID_POLARITY getPolarity(struct lepton_driver *driver);
void setPseudoColorLUT(struct lepton_driver *driver, LEP_VID_PCOLOR_LUT table);
LEP_VID_PCOLOR_LUT getPseudoColorLUT(struct lepton_driver *driver);
void setFocusCalcEnabled(struct lepton_driver *driver, uint8_t enabled);
uint8_t getFocusCalcEnabled(struct lepton_driver *driver);
void setFreezeEnabled(struct lepton_driver *driver, uint8_t enabled);
uint8_t getFreezeEnabled(struct lepton_driver *driver);
void setUserColorLUT(struct lepton_driver *driver, LEP_VID_LUT_BUFFER * table);
void getUserColorLUT(struct lepton_driver *driver, LEP_VID_LUT_BUFFER * table);
void setFocusRegion(struct lepton_driver *driver, LEP_VID_FOCUS_ROI * region);
void getFocusRegion(struct lepton_driver *driver, LEP_VID_FOCUS_ROI * region);
void setFocusThreshold(struct lepton_driver *driver, uint32_t threshold);
uint32_t getFocusThreshold(struct lepton_driver *driver);
uint32_t getFocusMetric(struct lepton_driver * driver);
void setSceneBasedNUCEnabled(struct lepton_driver *driver, uint8_t enabled);
uint8_t getSceneBasedNUCEnabled(struct lepton_driver *driver);
void setGamma(struct lepton_driver *driver, uint32_t gamma);
uint32_t getGamma(struct lepton_driver *driver);
#endif

#ifdef __cplusplus
}
#endif
#endif                          // _LEPTON_VID_H
