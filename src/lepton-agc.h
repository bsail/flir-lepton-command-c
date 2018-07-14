#ifndef _LEPTON_AGC_H
#define _LEPTON_AGC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lepton-flir.h"

#ifndef TEST
#ifndef LEPTON_FLIR_INTERNAL
#error "There is no user accessable data and functions here"
#endif
#endif

#ifndef LEPFLIR_EXCLUDE_AGC_FUNCS

  void lepton_agc_init(struct lepton_agc *this);

#endif

#ifdef TEST
  void setAGCEnabled(struct lepton_driver *driver, uint8_t enabled);
  uint8_t getAGCEnabled(struct lepton_driver *driver);
  void setAGCPolicy(struct lepton_driver *driver, LEP_AGC_POLICY policy);
  LEP_AGC_POLICY getAGCPolicy(struct lepton_driver *driver);
  void setHEQScaleFactor(struct lepton_driver *driver,
                         LEP_AGC_HEQ_SCALE_FACTOR factor);
  LEP_AGC_HEQ_SCALE_FACTOR getHEQScaleFactor(struct lepton_driver *driver);
  void setAGCCalcEnabled(struct lepton_driver *driver, uint8_t enabled);
  uint8_t getAGCCalcEnabled(struct lepton_driver *driver);
  void setHistogramRegion(struct lepton_driver *driver,
                          LEP_AGC_HISTOGRAM_ROI * region);
  void getHistogramRegion(struct lepton_driver *driver,
                          LEP_AGC_HISTOGRAM_ROI * region);
  void getHistogramStatistics(struct lepton_driver *driver,
                              LEP_AGC_HISTOGRAM_STATISTICS * statistics);
  void setHistogramClipPercent(struct lepton_driver *driver, uint16_t percent);
  uint16_t getHistogramClipPercent(struct lepton_driver *driver);
  void setHistogramTailSize(struct lepton_driver *driver, uint16_t size);
  uint16_t getHistogramTailSize(struct lepton_driver *driver);
  void setLinearMaxGain(struct lepton_driver *driver, uint16_t gain);
  uint16_t getLinearMaxGain(struct lepton_driver *driver);
  void setLinearMidpoint(struct lepton_driver *driver, uint16_t midpoint);
  uint16_t getLinearMidpoint(struct lepton_driver *driver);
  void setLinearDampeningFactor(struct lepton_driver *driver, uint16_t factor);
  uint16_t getLinearDampeningFactor(struct lepton_driver *driver);
  void setHEQDampeningFactor(struct lepton_driver *driver, uint16_t factor);
  uint16_t getHEQDampeningFactor(struct lepton_driver *driver);
  void setHEQMaxGain(struct lepton_driver *driver, uint16_t gain);
  uint16_t getHEQMaxGain(struct lepton_driver *driver);
  void setHEQClipLimitHigh(struct lepton_driver *driver, uint16_t limit);
  uint16_t getHEQClipLimitHigh(struct lepton_driver *driver);
  void setHEQClipLimitLow(struct lepton_driver *driver, uint16_t limit);
  uint16_t getHEQClipLimitLow(struct lepton_driver *driver);
  void setHEQBinExtension(struct lepton_driver *driver, uint16_t extension);
  uint16_t getHEQBinExtension(struct lepton_driver *driver);
  void setHEQMidpoint(struct lepton_driver *driver, uint16_t midpoint);
  uint16_t getHEQMidpoint(struct lepton_driver *driver);
  void setHEQEmptyCounts(struct lepton_driver *driver, uint16_t counts);
  uint16_t getHEQEmptyCounts(struct lepton_driver *driver);
  void setHEQNormalizationFactor(struct lepton_driver *driver, uint16_t factor);
  uint16_t getHEQNormalizationFactor(struct lepton_driver *driver);
#endif

#ifdef __cplusplus
}
#endif
#endif                          // _LEPTON_AGC_H
