#include "lepton-agc.h"

void agc_setAGCEnabled(uint8_t enabled, void * driver)
{
  ((struct lepton_driver*)driver)->communication.sendCommand_u32(((struct lepton_driver*)driver)->communication.cmdCode(LEP_CID_AGC_ENABLE_STATE, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) enabled,&(((struct lepton_driver*)driver)->communication));
}

uint8_t agc_getAGCEnabled(void * driver)
{
  uint32_t enabled;
  ((struct lepton_driver*)driver)->communication.receiveCommand_u32(((struct lepton_driver*)driver)->communication.cmdCode
                     (LEP_CID_AGC_ENABLE_STATE, LEP_I2C_COMMAND_TYPE_GET),
                     &enabled,&(((struct lepton_driver*)driver)->communication));
  return enabled;
}

void agc_setAGCPolicy(LEP_AGC_POLICY policy, void * driver)
{
  ((struct lepton_driver*)driver)->communication.sendCommand_u32(((struct lepton_driver*)driver)->communication.cmdCode(LEP_CID_AGC_POLICY, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) policy,&(((struct lepton_driver*)driver)->communication));
}

LEP_AGC_POLICY agc_getAGCPolicy(void * driver)
{
  uint32_t policy;
  ((struct lepton_driver*)driver)->communication.receiveCommand_u32(((struct lepton_driver*)driver)->communication.cmdCode(LEP_CID_AGC_POLICY, LEP_I2C_COMMAND_TYPE_GET),
                     &policy,&(((struct lepton_driver*)driver)->communication));
  return (LEP_AGC_POLICY) policy;
}

void agc_setHEQScaleFactor(LEP_AGC_HEQ_SCALE_FACTOR factor, void * driver)
{
  ((struct lepton_driver*)driver)->communication.sendCommand_u32(((struct lepton_driver*)driver)->communication.cmdCode
                  (LEP_CID_AGC_HEQ_SCALE_FACTOR, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) factor,&(((struct lepton_driver*)driver)->communication));
}

LEP_AGC_HEQ_SCALE_FACTOR agc_getHEQScaleFactor(void * driver)
{
  uint32_t factor;
  ((struct lepton_driver*)driver)->communication.receiveCommand_u32(((struct lepton_driver*)driver)->communication.cmdCode
                     (LEP_CID_AGC_HEQ_SCALE_FACTOR, LEP_I2C_COMMAND_TYPE_GET),
                     &factor,&(((struct lepton_driver*)driver)->communication));
  return (LEP_AGC_HEQ_SCALE_FACTOR) factor;
}

void agc_setAGCCalcEnabled(uint8_t enabled, void * driver)
{
  ((struct lepton_driver*)driver)->communication.sendCommand_u32(((struct lepton_driver*)driver)->communication.cmdCode
                  (LEP_CID_AGC_CALC_ENABLE_STATE, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) enabled,&(((struct lepton_driver*)driver)->communication));
}

uint8_t agc_getAGCCalcEnabled(void * driver)
{
  uint32_t enabled;
  ((struct lepton_driver*)driver)->communication.receiveCommand_u32(((struct lepton_driver*)driver)->communication.cmdCode
                     (LEP_CID_AGC_CALC_ENABLE_STATE, LEP_I2C_COMMAND_TYPE_GET),
                     &enabled,&(((struct lepton_driver*)driver)->communication));
  return enabled;
}

#ifndef LEPFLIR_EXCLUDE_EXT_I2C_FUNCS

void agc_setHistogramRegion(LEP_AGC_HISTOGRAM_ROI * region, void * driver)
{
  if (!region)
    return;
  ((struct lepton_driver*)driver)->communication.sendCommand_array(((struct lepton_driver*)driver)->communication.cmdCode(LEP_CID_AGC_ROI, LEP_I2C_COMMAND_TYPE_SET),
                    (uint16_t *) region, sizeof(LEP_AGC_HISTOGRAM_ROI) / 2,&(((struct lepton_driver*)driver)->communication));
}

void agc_getHistogramRegion(LEP_AGC_HISTOGRAM_ROI * region, void * driver)
{
  if (!region)
    return;
  ((struct lepton_driver*)driver)->communication.receiveCommand_array(((struct lepton_driver*)driver)->communication.cmdCode(LEP_CID_AGC_ROI, LEP_I2C_COMMAND_TYPE_GET),
                       (uint16_t *) region, sizeof(LEP_AGC_HISTOGRAM_ROI) / 2,&(((struct lepton_driver*)driver)->communication));
}

void agc_getHistogramStatistics(LEP_AGC_HISTOGRAM_STATISTICS * statistics, void * driver)
{
  if (!statistics)
    return;
  ((struct lepton_driver*)driver)->communication.receiveCommand_array(((struct lepton_driver*)driver)->communication.cmdCode
                       (LEP_CID_AGC_STATISTICS, LEP_I2C_COMMAND_TYPE_GET),
                       (uint16_t *) statistics,
                       sizeof(LEP_AGC_HISTOGRAM_STATISTICS) / 2,&(((struct lepton_driver*)driver)->communication));
}

void agc_setHistogramClipPercent(uint16_t percent, void * driver)
{
  ((struct lepton_driver*)driver)->communication.sendCommand_u16(((struct lepton_driver*)driver)->communication.cmdCode
                  (LEP_CID_AGC_HISTOGRAM_CLIP_PERCENT,
                   LEP_I2C_COMMAND_TYPE_SET), percent,&(((struct lepton_driver*)driver)->communication));
}

uint16_t agc_getHistogramClipPercent(void * driver)
{
  uint16_t percent;
  ((struct lepton_driver*)driver)->communication.receiveCommand_u16(((struct lepton_driver*)driver)->communication.cmdCode
                     (LEP_CID_AGC_HISTOGRAM_CLIP_PERCENT,
                      LEP_I2C_COMMAND_TYPE_GET), &percent,&(((struct lepton_driver*)driver)->communication));
  return percent;
}

void agc_setHistogramTailSize(uint16_t size, void * driver)
{
  ((struct lepton_driver*)driver)->communication.sendCommand_u16(((struct lepton_driver*)driver)->communication.cmdCode
                  (LEP_CID_AGC_HISTOGRAM_TAIL_SIZE, LEP_I2C_COMMAND_TYPE_SET),
                  size,&(((struct lepton_driver*)driver)->communication));
}

uint16_t agc_getHistogramTailSize(void * driver)
{
  uint16_t size;
  ((struct lepton_driver*)driver)->communication.receiveCommand_u16(((struct lepton_driver*)driver)->communication.cmdCode
                     (LEP_CID_AGC_HISTOGRAM_TAIL_SIZE,
                      LEP_I2C_COMMAND_TYPE_GET), &size,&(((struct lepton_driver*)driver)->communication));
  return size;
}

void agc_setLinearMaxGain(uint16_t gain, void * driver)
{
  ((struct lepton_driver*)driver)->communication.sendCommand_u16(((struct lepton_driver*)driver)->communication.cmdCode
                  (LEP_CID_AGC_LINEAR_MAX_GAIN, LEP_I2C_COMMAND_TYPE_SET),
                  gain,&(((struct lepton_driver*)driver)->communication));
}

uint16_t agc_getLinearMaxGain(void * driver)
{
  uint16_t gain;
  ((struct lepton_driver*)driver)->communication.receiveCommand_u16(((struct lepton_driver*)driver)->communication.cmdCode
                     (LEP_CID_AGC_LINEAR_MAX_GAIN, LEP_I2C_COMMAND_TYPE_GET),
                     &gain,&(((struct lepton_driver*)driver)->communication));
  return gain;
}

void agc_setLinearMidpoint(uint16_t midpoint, void * driver)
{
  ((struct lepton_driver*)driver)->communication.sendCommand_u16(((struct lepton_driver*)driver)->communication.cmdCode
                  (LEP_CID_AGC_LINEAR_MIDPOINT, LEP_I2C_COMMAND_TYPE_SET),
                  midpoint,&(((struct lepton_driver*)driver)->communication));
}

uint16_t agc_getLinearMidpoint(void * driver)
{
  uint16_t midpoint;
  ((struct lepton_driver*)driver)->communication.receiveCommand_u16(((struct lepton_driver*)driver)->communication.cmdCode
                     (LEP_CID_AGC_LINEAR_MIDPOINT, LEP_I2C_COMMAND_TYPE_GET),
                     &midpoint,&(((struct lepton_driver*)driver)->communication));
  return midpoint;
}

void agc_setLinearDampeningFactor(uint16_t factor, void * driver)
{
  ((struct lepton_driver*)driver)->communication.sendCommand_u16(((struct lepton_driver*)driver)->communication.cmdCode
                  (LEP_CID_AGC_LINEAR_DAMPENING_FACTOR,
                   LEP_I2C_COMMAND_TYPE_SET), factor,&(((struct lepton_driver*)driver)->communication));
}

uint16_t agc_getLinearDampeningFactor(void * driver)
{
  uint16_t factor;
  ((struct lepton_driver*)driver)->communication.receiveCommand_u16(((struct lepton_driver*)driver)->communication.cmdCode
                     (LEP_CID_AGC_LINEAR_DAMPENING_FACTOR,
                      LEP_I2C_COMMAND_TYPE_GET), &factor,&(((struct lepton_driver*)driver)->communication));
  return factor;
}

void agc_setHEQDampeningFactor(uint16_t factor, void * driver)
{
  ((struct lepton_driver*)driver)->communication.sendCommand_u16(((struct lepton_driver*)driver)->communication.cmdCode
                  (LEP_CID_AGC_HEQ_DAMPENING_FACTOR, LEP_I2C_COMMAND_TYPE_SET),
                  factor,&(((struct lepton_driver*)driver)->communication));
}

uint16_t agc_getHEQDampeningFactor(void * driver)
{
  uint16_t factor;
  ((struct lepton_driver*)driver)->communication.receiveCommand_u16(((struct lepton_driver*)driver)->communication.cmdCode
                     (LEP_CID_AGC_HEQ_DAMPENING_FACTOR,
                      LEP_I2C_COMMAND_TYPE_GET), &factor,&(((struct lepton_driver*)driver)->communication));
  return factor;
}

void agc_setHEQMaxGain(uint16_t gain, void * driver)
{
  ((struct lepton_driver*)driver)->communication.sendCommand_u16(((struct lepton_driver*)driver)->communication.cmdCode(LEP_CID_AGC_HEQ_MAX_GAIN, LEP_I2C_COMMAND_TYPE_SET),
                  gain,&(((struct lepton_driver*)driver)->communication));
}

uint16_t agc_getHEQMaxGain(void * driver)
{
  uint16_t gain;
  ((struct lepton_driver*)driver)->communication.receiveCommand_u16(((struct lepton_driver*)driver)->communication.cmdCode
                     (LEP_CID_AGC_HEQ_MAX_GAIN, LEP_I2C_COMMAND_TYPE_GET),
                     &gain,&(((struct lepton_driver*)driver)->communication));
  return gain;
}

void agc_setHEQClipLimitHigh(uint16_t limit, void * driver)
{
  ((struct lepton_driver*)driver)->communication.sendCommand_u16(((struct lepton_driver*)driver)->communication.cmdCode
                  (LEP_CID_AGC_HEQ_CLIP_LIMIT_HIGH, LEP_I2C_COMMAND_TYPE_SET),
                  limit,&(((struct lepton_driver*)driver)->communication));
}

uint16_t agc_getHEQClipLimitHigh(void * driver)
{
  uint16_t limit;
  ((struct lepton_driver*)driver)->communication.receiveCommand_u16(((struct lepton_driver*)driver)->communication.cmdCode
                     (LEP_CID_AGC_HEQ_CLIP_LIMIT_HIGH,
                      LEP_I2C_COMMAND_TYPE_GET), &limit,&(((struct lepton_driver*)driver)->communication));
  return limit;
}

void agc_setHEQClipLimitLow(uint16_t limit, void * driver)
{
  ((struct lepton_driver*)driver)->communication.sendCommand_u16(((struct lepton_driver*)driver)->communication.cmdCode
                  (LEP_CID_AGC_HEQ_CLIP_LIMIT_LOW, LEP_I2C_COMMAND_TYPE_SET),
                  limit,&(((struct lepton_driver*)driver)->communication));
}

uint16_t agc_getHEQClipLimitLow(void * driver)
{
  uint16_t limit;
  ((struct lepton_driver*)driver)->communication.receiveCommand_u16(((struct lepton_driver*)driver)->communication.cmdCode
                     (LEP_CID_AGC_HEQ_CLIP_LIMIT_LOW, LEP_I2C_COMMAND_TYPE_GET),
                     &limit,&(((struct lepton_driver*)driver)->communication));
  return limit;
}

void agc_setHEQBinExtension(uint16_t extension, void * driver)
{
  ((struct lepton_driver*)driver)->communication.sendCommand_u16(((struct lepton_driver*)driver)->communication.cmdCode
                  (LEP_CID_AGC_HEQ_BIN_EXTENSION, LEP_I2C_COMMAND_TYPE_SET),
                  extension,&(((struct lepton_driver*)driver)->communication));
}

uint16_t agc_getHEQBinExtension(void * driver)
{
  uint16_t extension;
  ((struct lepton_driver*)driver)->communication.receiveCommand_u16(((struct lepton_driver*)driver)->communication.cmdCode
                     (LEP_CID_AGC_HEQ_BIN_EXTENSION, LEP_I2C_COMMAND_TYPE_GET),
                     &extension,&(((struct lepton_driver*)driver)->communication));
  return extension;
}

void agc_setHEQMidpoint(uint16_t midpoint, void * driver)
{
  ((struct lepton_driver*)driver)->communication.sendCommand_u16(((struct lepton_driver*)driver)->communication.cmdCode(LEP_CID_AGC_HEQ_MIDPOINT, LEP_I2C_COMMAND_TYPE_SET),
                  midpoint,&(((struct lepton_driver*)driver)->communication));
}

uint16_t agc_getHEQMidpoint(void * driver)
{
  uint16_t midpoint;
  ((struct lepton_driver*)driver)->communication.receiveCommand_u16(((struct lepton_driver*)driver)->communication.cmdCode
                     (LEP_CID_AGC_HEQ_MIDPOINT, LEP_I2C_COMMAND_TYPE_GET),
                     &midpoint,&(((struct lepton_driver*)driver)->communication));
  return midpoint;
}

void agc_setHEQEmptyCounts(uint16_t counts, void * driver)
{
  ((struct lepton_driver*)driver)->communication.sendCommand_u16(((struct lepton_driver*)driver)->communication.cmdCode
                  (LEP_CID_AGC_HEQ_EMPTY_COUNTS, LEP_I2C_COMMAND_TYPE_SET),
                  counts,&(((struct lepton_driver*)driver)->communication));
}

uint16_t agc_getHEQEmptyCounts(void * driver)
{
  uint16_t counts;
  ((struct lepton_driver*)driver)->communication.receiveCommand_u16(((struct lepton_driver*)driver)->communication.cmdCode
                     (LEP_CID_AGC_HEQ_EMPTY_COUNTS, LEP_I2C_COMMAND_TYPE_GET),
                     &counts,&(((struct lepton_driver*)driver)->communication));
  return counts;
}

void agc_setHEQNormalizationFactor(uint16_t factor, void * driver)
{
  ((struct lepton_driver*)driver)->communication.sendCommand_u16(((struct lepton_driver*)driver)->communication.cmdCode
                  (LEP_CID_AGC_HEQ_NORMALIZATION_FACTOR,
                   LEP_I2C_COMMAND_TYPE_SET), factor,&(((struct lepton_driver*)driver)->communication));
}

uint16_t agc_getHEQNormalizationFactor(void * driver)
{
  uint16_t factor;
  ((struct lepton_driver*)driver)->communication.receiveCommand_u16(((struct lepton_driver*)driver)->communication.cmdCode
                     (LEP_CID_AGC_HEQ_NORMALIZATION_FACTOR,
                      LEP_I2C_COMMAND_TYPE_GET), &factor,&(((struct lepton_driver*)driver)->communication));
  return factor;
}

#endif

void lepton_agc_init(struct lepton_agc * this)
{
  this->agc_setAGCEnabled = &agc_setAGCEnabled;
  this->agc_getAGCEnabled = &agc_getAGCEnabled;
  this->agc_setAGCPolicy = &agc_setAGCPolicy;
  this->agc_getAGCPolicy = &agc_getAGCPolicy;
  this->agc_setHEQScaleFactor = &agc_setHEQScaleFactor;
  this->agc_getHEQScaleFactor = &agc_getHEQScaleFactor;
  this->agc_setAGCCalcEnabled = &agc_setAGCCalcEnabled;
  this->agc_getAGCCalcEnabled = &agc_getAGCCalcEnabled;
#ifndef LEPFLIR_EXCLUDE_EXT_I2C_FUNCS
  this->agc_setHistogramRegion = &agc_setHistogramRegion;
  this->agc_getHistogramRegion = &agc_getHistogramRegion;
  this->agc_getHistogramStatistics = &agc_getHistogramStatistics;
  this->agc_setHistogramClipPercent = &agc_setHistogramClipPercent;
  this->agc_getHistogramClipPercent = &agc_getHistogramClipPercent;
  this->agc_setHistogramTailSize = &agc_setHistogramTailSize;
  this->agc_getHistogramTailSize = &agc_getHistogramTailSize;
  this->agc_setLinearMaxGain = &agc_setLinearMaxGain;
  this->agc_getLinearMaxGain = &agc_getLinearMaxGain;
  this->agc_setLinearMidpoint = &agc_setLinearMidpoint;
  this->agc_getLinearMidpoint = &agc_getLinearMidpoint;
  this->agc_setLinearDampeningFactor = &agc_setLinearDampeningFactor;
  this->agc_getLinearDampeningFactor = &agc_getLinearDampeningFactor;
  this->agc_setHEQDampeningFactor = &agc_setHEQDampeningFactor;
  this->agc_getHEQDampeningFactor = &agc_getHEQDampeningFactor;
  this->agc_setHEQMaxGain = &agc_setHEQMaxGain;
  this->agc_getHEQMaxGain = &agc_getHEQMaxGain;
  this->agc_setHEQClipLimitHigh = &agc_setHEQClipLimitHigh;
  this->agc_getHEQClipLimitHigh = &agc_getHEQClipLimitHigh;
  this->agc_setHEQClipLimitLow = &agc_setHEQClipLimitLow;
  this->agc_getHEQClipLimitLow = &agc_getHEQClipLimitLow;
  this->agc_setHEQBinExtension = &agc_setHEQBinExtension;
  this->agc_getHEQBinExtension = &agc_getHEQBinExtension;
  this->agc_setHEQMidpoint = &agc_setHEQMidpoint;
  this->agc_getHEQMidpoint = &agc_getHEQMidpoint;
  this->agc_setHEQEmptyCounts = &agc_setHEQEmptyCounts;
  this->agc_getHEQEmptyCounts = &agc_getHEQEmptyCounts;
  this->agc_setHEQNormalizationFactor = &agc_setHEQNormalizationFactor;
  this->agc_getHEQNormalizationFactor = &agc_getHEQNormalizationFactor;
#endif
}
