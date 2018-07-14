#define LEPTON_FLIR_INTERNAL
#include "lepton-agc.h"
#include "lepton-flir.h"
#include "lepton-communication.h"
#include "lepton-get-set.h"

#ifndef LEPFLIR_EXCLUDE_AGC_FUNCS

void setAGCEnabled(struct lepton_driver *driver, uint8_t enabled)
{
  if (driver) {
    set_common(driver,u32,LEP_CID_AGC_ENABLE_STATE,(uint16_t *) &enabled, 4);
  }
}

uint8_t getAGCEnabled(struct lepton_driver *driver)
{
  if (driver) {
    uint32_t enabled;
    get_common(driver,u32,LEP_CID_AGC_ENABLE_STATE,(uint16_t *) &enabled, 4);
    return enabled;
  }
  return 0;
}

void setAGCPolicy(struct lepton_driver *driver, LEP_AGC_POLICY policy)
{
  if (driver) {
    set_common(driver,u32,LEP_CID_AGC_POLICY,(uint16_t *) &policy, 4);
  }
}

LEP_AGC_POLICY getAGCPolicy(struct lepton_driver *driver)
{
  if (driver) {
    uint32_t policy;
    get_common(driver,u32,LEP_CID_AGC_POLICY,(uint16_t *) &policy, 4);
    return (LEP_AGC_POLICY) policy;
  }
  return 0;
}

void setHEQScaleFactor(struct lepton_driver *driver,
                       LEP_AGC_HEQ_SCALE_FACTOR factor)
{
  if (driver) {
    set_common(driver,u32,LEP_CID_AGC_HEQ_SCALE_FACTOR,(uint16_t *) &factor, 4);
  }
}

LEP_AGC_HEQ_SCALE_FACTOR getHEQScaleFactor(struct lepton_driver *driver)
{
  if (driver) {
    uint32_t factor;
    get_common(driver,u32,LEP_CID_AGC_HEQ_SCALE_FACTOR,(uint16_t *) &factor, 4);
    return (LEP_AGC_HEQ_SCALE_FACTOR) factor;
  }
  return 0;
}

void setAGCCalcEnabled(struct lepton_driver *driver, uint8_t enabled)
{
  if (driver) {
    set_common(driver,u32,LEP_CID_AGC_CALC_ENABLE_STATE,(uint16_t *) &enabled, 4);
  }
}

uint8_t getAGCCalcEnabled(struct lepton_driver *driver)
{
  if (driver) {
    uint32_t enabled;
    get_common(driver,u32,LEP_CID_AGC_CALC_ENABLE_STATE,(uint16_t *) &enabled, 4);
    return enabled;
  }
  return 0;
}

#ifndef LEPFLIR_EXCLUDE_EXT_I2C_FUNCS

void setHistogramRegion(struct lepton_driver *driver,
                        LEP_AGC_HISTOGRAM_ROI * region)
{
  if (driver) {
    if (!region)
      return;
    set_common(driver,array,LEP_CID_AGC_ROI,(uint16_t*)region, sizeof(LEP_AGC_HISTOGRAM_ROI) / 2);
  }
}

void getHistogramRegion(struct lepton_driver *driver,
                        LEP_AGC_HISTOGRAM_ROI * region)
{
  if (driver) {
    if (!region)
      return;
    get_common(driver,array,LEP_CID_AGC_ROI,(uint16_t*)region, sizeof(LEP_AGC_HISTOGRAM_ROI) / 2);
  }
}

void getHistogramStatistics(struct lepton_driver *driver,
                            LEP_AGC_HISTOGRAM_STATISTICS * statistics)
{
  if (driver) {
    if (!statistics)
      return;
    get_common(driver,array,LEP_CID_AGC_STATISTICS,(uint16_t*)statistics, sizeof(LEP_AGC_HISTOGRAM_STATISTICS) / 2);
  }
}

void setHistogramClipPercent(struct lepton_driver *driver, uint16_t percent)
{
  if (driver) {
    set_common(driver,u16,LEP_CID_AGC_HISTOGRAM_CLIP_PERCENT,&percent, 2);
  }
}

uint16_t getHistogramClipPercent(struct lepton_driver *driver)
{
  if (driver) {
    uint16_t percent;
    get_common(driver,u16,LEP_CID_AGC_HISTOGRAM_CLIP_PERCENT,&percent, 2);
    return percent;
  }
  return 0;
}

void setHistogramTailSize(struct lepton_driver *driver, uint16_t size)
{
  if (driver) {
    set_common(driver,u16,LEP_CID_AGC_HISTOGRAM_TAIL_SIZE,&size, 2);
  }
}

uint16_t getHistogramTailSize(struct lepton_driver *driver)
{
  if (driver) {
    uint16_t size;
    get_common(driver,u16,LEP_CID_AGC_HISTOGRAM_TAIL_SIZE,&size, 2);
    return size;
  }
  return 0;
}

void setLinearMaxGain(struct lepton_driver *driver, uint16_t gain)
{
  if (driver) {
    set_common(driver,u16,LEP_CID_AGC_LINEAR_MAX_GAIN,&gain, 2);
  }
}

uint16_t getLinearMaxGain(struct lepton_driver *driver)
{
  if (driver) {
    uint16_t gain;
    get_common(driver,u16,LEP_CID_AGC_LINEAR_MAX_GAIN,&gain, 2);
    return gain;
  }
  return 0;
}

void setLinearMidpoint(struct lepton_driver *driver, uint16_t midpoint)
{
  if (driver) {
    set_common(driver,u16,LEP_CID_AGC_LINEAR_MIDPOINT,&midpoint, 2);
  }
}

uint16_t getLinearMidpoint(struct lepton_driver *driver)
{
  if (driver) {
    uint16_t midpoint;
    get_common(driver,u16,LEP_CID_AGC_LINEAR_MIDPOINT,&midpoint, 2);
    return midpoint;
  }
  return 0;
}

void setLinearDampeningFactor(struct lepton_driver *driver, uint16_t factor)
{
  if (driver) {
    set_common(driver,u16,LEP_CID_AGC_LINEAR_DAMPENING_FACTOR,&factor, 2);
  }
}

uint16_t getLinearDampeningFactor(struct lepton_driver *driver)
{
  if (driver) {
    uint16_t factor;
    get_common(driver,u16,LEP_CID_AGC_LINEAR_DAMPENING_FACTOR,&factor, 2);
    return factor;
  }
  return 0;
}

void setHEQDampeningFactor(struct lepton_driver *driver, uint16_t factor)
{
  if (driver) {
    set_common(driver,u16,LEP_CID_AGC_HEQ_DAMPENING_FACTOR,&factor, 2);
  }
}

uint16_t getHEQDampeningFactor(struct lepton_driver *driver)
{
  if (driver) {
    uint16_t factor;
    get_common(driver,u16,LEP_CID_AGC_HEQ_DAMPENING_FACTOR,&factor, 2);
    return factor;
  }
  return 0;
}

void setHEQMaxGain(struct lepton_driver *driver, uint16_t gain)
{
  if (driver) {
    set_common(driver,u16,LEP_CID_AGC_HEQ_MAX_GAIN,&gain, 2);
  }
}

uint16_t getHEQMaxGain(struct lepton_driver *driver)
{
  if (driver) {
    uint16_t gain;
    get_common(driver,u16,LEP_CID_AGC_HEQ_MAX_GAIN,&gain, 2);
    return gain;
  }
  return 0;
}

void setHEQClipLimitHigh(struct lepton_driver *driver, uint16_t limit)
{
  if (driver) {
    set_common(driver,u16,LEP_CID_AGC_HEQ_CLIP_LIMIT_HIGH,&limit, 2);
  }
}

uint16_t getHEQClipLimitHigh(struct lepton_driver *driver)
{
  if (driver) {
    uint16_t limit;
    get_common(driver,u16,LEP_CID_AGC_HEQ_CLIP_LIMIT_HIGH,&limit, 2);
    return limit;
  }
  return 0;
}

void setHEQClipLimitLow(struct lepton_driver *driver, uint16_t limit)
{
  if (driver) {
    set_common(driver,u16,LEP_CID_AGC_HEQ_CLIP_LIMIT_LOW,&limit, 2);
  }
}

uint16_t getHEQClipLimitLow(struct lepton_driver *driver)
{
  if (driver) {
    uint16_t limit;
    get_common(driver,u16,LEP_CID_AGC_HEQ_CLIP_LIMIT_LOW,&limit, 2);
    return limit;
  }
  return 0;
}

void setHEQBinExtension(struct lepton_driver *driver, uint16_t extension)
{
  if (driver) {
    set_common(driver,u16,LEP_CID_AGC_HEQ_BIN_EXTENSION,&extension, 2);
  }
}

uint16_t getHEQBinExtension(struct lepton_driver *driver)
{
  if (driver) {
    uint16_t extension;
    get_common(driver,u16,LEP_CID_AGC_HEQ_BIN_EXTENSION,&extension, 2);
    return extension;
  }
  return 0;
}

void setHEQMidpoint(struct lepton_driver *driver, uint16_t midpoint)
{
  if (driver) {
    set_common(driver,u16,LEP_CID_AGC_HEQ_MIDPOINT,&midpoint, 2);
  }
}

uint16_t getHEQMidpoint(struct lepton_driver *driver)
{
  if (driver) {
    uint16_t midpoint;
    get_common(driver,u16,LEP_CID_AGC_HEQ_MIDPOINT,&midpoint, 2);
    return midpoint;
  }
  return 0;
}

void setHEQEmptyCounts(struct lepton_driver *driver, uint16_t counts)
{
  if (driver) {
    set_common(driver,u16,LEP_CID_AGC_HEQ_EMPTY_COUNTS,&counts, 2);
  }
}

uint16_t getHEQEmptyCounts(struct lepton_driver *driver)
{
  if (driver) {
    uint16_t counts;
    get_common(driver,u16,LEP_CID_AGC_HEQ_EMPTY_COUNTS,&counts, 2);
    return counts;
  }
  return 0;
}

void setHEQNormalizationFactor(struct lepton_driver *driver, uint16_t factor)
{
  if (driver) {
    set_common(driver,u16,LEP_CID_AGC_HEQ_NORMALIZATION_FACTOR,&factor, 2);
  }
}

uint16_t getHEQNormalizationFactor(struct lepton_driver *driver)
{
  if (driver) {
    uint16_t factor;
    get_common(driver,u16,LEP_CID_AGC_HEQ_NORMALIZATION_FACTOR,&factor, 2);
    return factor;
  }
  return 0;
}

#endif

void lepton_agc_init(struct lepton_agc *this)
{
  this->setAGCEnabled = &setAGCEnabled;
  this->getAGCEnabled = &getAGCEnabled;
  this->setAGCPolicy = &setAGCPolicy;
  this->getAGCPolicy = &getAGCPolicy;
  this->setHEQScaleFactor = &setHEQScaleFactor;
  this->getHEQScaleFactor = &getHEQScaleFactor;
  this->setAGCCalcEnabled = &setAGCCalcEnabled;
  this->getAGCCalcEnabled = &getAGCCalcEnabled;
#ifndef LEPFLIR_EXCLUDE_EXT_I2C_FUNCS
  this->setHistogramRegion = &setHistogramRegion;
  this->getHistogramRegion = &getHistogramRegion;
  this->getHistogramStatistics = &getHistogramStatistics;
  this->setHistogramClipPercent = &setHistogramClipPercent;
  this->getHistogramClipPercent = &getHistogramClipPercent;
  this->setHistogramTailSize = &setHistogramTailSize;
  this->getHistogramTailSize = &getHistogramTailSize;
  this->setLinearMaxGain = &setLinearMaxGain;
  this->getLinearMaxGain = &getLinearMaxGain;
  this->setLinearMidpoint = &setLinearMidpoint;
  this->getLinearMidpoint = &getLinearMidpoint;
  this->setLinearDampeningFactor = &setLinearDampeningFactor;
  this->getLinearDampeningFactor = &getLinearDampeningFactor;
  this->setHEQDampeningFactor = &setHEQDampeningFactor;
  this->getHEQDampeningFactor = &getHEQDampeningFactor;
  this->setHEQMaxGain = &setHEQMaxGain;
  this->getHEQMaxGain = &getHEQMaxGain;
  this->setHEQClipLimitHigh = &setHEQClipLimitHigh;
  this->getHEQClipLimitHigh = &getHEQClipLimitHigh;
  this->setHEQClipLimitLow = &setHEQClipLimitLow;
  this->getHEQClipLimitLow = &getHEQClipLimitLow;
  this->setHEQBinExtension = &setHEQBinExtension;
  this->getHEQBinExtension = &getHEQBinExtension;
  this->setHEQMidpoint = &setHEQMidpoint;
  this->getHEQMidpoint = &getHEQMidpoint;
  this->setHEQEmptyCounts = &setHEQEmptyCounts;
  this->getHEQEmptyCounts = &getHEQEmptyCounts;
  this->setHEQNormalizationFactor = &setHEQNormalizationFactor;
  this->getHEQNormalizationFactor = &getHEQNormalizationFactor;
#endif
}

#endif
