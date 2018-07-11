#define LEPTON_FLIR_INTERNAL
#include "lepton-agc.h"
#include "lepton-communication.h"

#ifndef LEPFLIR_EXCLUDE_AGC_FUNCS

void setAGCEnabled(uint8_t enabled, struct lepton_driver * driver)
{
  sendCommand_u32(cmdCode(LEP_CID_AGC_ENABLE_STATE, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) enabled,&(driver->communication));
}

uint8_t getAGCEnabled(struct lepton_driver * driver)
{
  uint32_t enabled;
  receiveCommand_u32(cmdCode
                     (LEP_CID_AGC_ENABLE_STATE, LEP_I2C_COMMAND_TYPE_GET),
                     &enabled,&(driver->communication));
  return enabled;
}

void setAGCPolicy(LEP_AGC_POLICY policy, struct lepton_driver * driver)
{
  sendCommand_u32(cmdCode(LEP_CID_AGC_POLICY, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) policy,&(driver->communication));
}

LEP_AGC_POLICY getAGCPolicy(struct lepton_driver * driver)
{
  uint32_t policy;
  receiveCommand_u32(cmdCode(LEP_CID_AGC_POLICY, LEP_I2C_COMMAND_TYPE_GET),
                     &policy,&(driver->communication));
  return (LEP_AGC_POLICY) policy;
}

void setHEQScaleFactor(LEP_AGC_HEQ_SCALE_FACTOR factor, struct lepton_driver * driver)
{
  sendCommand_u32(cmdCode
                  (LEP_CID_AGC_HEQ_SCALE_FACTOR, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) factor,&(driver->communication));
}

LEP_AGC_HEQ_SCALE_FACTOR getHEQScaleFactor(struct lepton_driver * driver)
{
  uint32_t factor;
  receiveCommand_u32(cmdCode
                     (LEP_CID_AGC_HEQ_SCALE_FACTOR, LEP_I2C_COMMAND_TYPE_GET),
                     &factor,&(driver->communication));
  return (LEP_AGC_HEQ_SCALE_FACTOR) factor;
}

void setAGCCalcEnabled(uint8_t enabled, struct lepton_driver * driver)
{
  sendCommand_u32(cmdCode
                  (LEP_CID_AGC_CALC_ENABLE_STATE, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) enabled,&(driver->communication));
}

uint8_t getAGCCalcEnabled(struct lepton_driver * driver)
{
  uint32_t enabled;
  receiveCommand_u32(cmdCode
                     (LEP_CID_AGC_CALC_ENABLE_STATE, LEP_I2C_COMMAND_TYPE_GET),
                     &enabled,&(driver->communication));
  return enabled;
}

#ifndef LEPFLIR_EXCLUDE_EXT_I2C_FUNCS

void setHistogramRegion(LEP_AGC_HISTOGRAM_ROI * region, struct lepton_driver * driver)
{
  if (!region)
    return;
  sendCommand_array(cmdCode(LEP_CID_AGC_ROI, LEP_I2C_COMMAND_TYPE_SET),
                    (uint16_t *) region, sizeof(LEP_AGC_HISTOGRAM_ROI) / 2,&(driver->communication));
}

void getHistogramRegion(LEP_AGC_HISTOGRAM_ROI * region, struct lepton_driver * driver)
{
  if (!region)
    return;
  receiveCommand_array(cmdCode(LEP_CID_AGC_ROI, LEP_I2C_COMMAND_TYPE_GET),
                       (uint16_t *) region, sizeof(LEP_AGC_HISTOGRAM_ROI) / 2,&(driver->communication));
}

void getHistogramStatistics(LEP_AGC_HISTOGRAM_STATISTICS * statistics, struct lepton_driver * driver)
{
  if (!statistics)
    return;
  receiveCommand_array(cmdCode
                       (LEP_CID_AGC_STATISTICS, LEP_I2C_COMMAND_TYPE_GET),
                       (uint16_t *) statistics,
                       sizeof(LEP_AGC_HISTOGRAM_STATISTICS) / 2,&(driver->communication));
}

void setHistogramClipPercent(uint16_t percent, struct lepton_driver * driver)
{
  sendCommand_u16(cmdCode
                  (LEP_CID_AGC_HISTOGRAM_CLIP_PERCENT,
                   LEP_I2C_COMMAND_TYPE_SET), percent,&(driver->communication));
}

uint16_t getHistogramClipPercent(struct lepton_driver * driver)
{
  uint16_t percent;
  receiveCommand_u16(cmdCode
                     (LEP_CID_AGC_HISTOGRAM_CLIP_PERCENT,
                      LEP_I2C_COMMAND_TYPE_GET), &percent,&(driver->communication));
  return percent;
}

void setHistogramTailSize(uint16_t size, struct lepton_driver * driver)
{
  sendCommand_u16(cmdCode
                  (LEP_CID_AGC_HISTOGRAM_TAIL_SIZE, LEP_I2C_COMMAND_TYPE_SET),
                  size,&(driver->communication));
}

uint16_t getHistogramTailSize(struct lepton_driver * driver)
{
  uint16_t size;
  receiveCommand_u16(cmdCode
                     (LEP_CID_AGC_HISTOGRAM_TAIL_SIZE,
                      LEP_I2C_COMMAND_TYPE_GET), &size,&(driver->communication));
  return size;
}

void setLinearMaxGain(uint16_t gain, struct lepton_driver * driver)
{
  sendCommand_u16(cmdCode
                  (LEP_CID_AGC_LINEAR_MAX_GAIN, LEP_I2C_COMMAND_TYPE_SET),
                  gain,&(driver->communication));
}

uint16_t getLinearMaxGain(struct lepton_driver * driver)
{
  uint16_t gain;
  receiveCommand_u16(cmdCode
                     (LEP_CID_AGC_LINEAR_MAX_GAIN, LEP_I2C_COMMAND_TYPE_GET),
                     &gain,&(driver->communication));
  return gain;
}

void setLinearMidpoint(uint16_t midpoint, struct lepton_driver * driver)
{
  sendCommand_u16(cmdCode
                  (LEP_CID_AGC_LINEAR_MIDPOINT, LEP_I2C_COMMAND_TYPE_SET),
                  midpoint,&(driver->communication));
}

uint16_t getLinearMidpoint(struct lepton_driver * driver)
{
  uint16_t midpoint;
  receiveCommand_u16(cmdCode
                     (LEP_CID_AGC_LINEAR_MIDPOINT, LEP_I2C_COMMAND_TYPE_GET),
                     &midpoint,&(driver->communication));
  return midpoint;
}

void setLinearDampeningFactor(uint16_t factor, struct lepton_driver * driver)
{
  sendCommand_u16(cmdCode
                  (LEP_CID_AGC_LINEAR_DAMPENING_FACTOR,
                   LEP_I2C_COMMAND_TYPE_SET), factor,&(driver->communication));
}

uint16_t getLinearDampeningFactor(struct lepton_driver * driver)
{
  uint16_t factor;
  receiveCommand_u16(cmdCode
                     (LEP_CID_AGC_LINEAR_DAMPENING_FACTOR,
                      LEP_I2C_COMMAND_TYPE_GET), &factor,&(driver->communication));
  return factor;
}

void setHEQDampeningFactor(uint16_t factor, struct lepton_driver * driver)
{
  sendCommand_u16(cmdCode
                  (LEP_CID_AGC_HEQ_DAMPENING_FACTOR, LEP_I2C_COMMAND_TYPE_SET),
                  factor,&(driver->communication));
}

uint16_t getHEQDampeningFactor(struct lepton_driver * driver)
{
  uint16_t factor;
  receiveCommand_u16(cmdCode
                     (LEP_CID_AGC_HEQ_DAMPENING_FACTOR,
                      LEP_I2C_COMMAND_TYPE_GET), &factor,&(driver->communication));
  return factor;
}

void setHEQMaxGain(uint16_t gain, struct lepton_driver * driver)
{
  sendCommand_u16(cmdCode(LEP_CID_AGC_HEQ_MAX_GAIN, LEP_I2C_COMMAND_TYPE_SET),
                  gain,&(driver->communication));
}

uint16_t getHEQMaxGain(struct lepton_driver * driver)
{
  uint16_t gain;
  receiveCommand_u16(cmdCode
                     (LEP_CID_AGC_HEQ_MAX_GAIN, LEP_I2C_COMMAND_TYPE_GET),
                     &gain,&(driver->communication));
  return gain;
}

void setHEQClipLimitHigh(uint16_t limit, struct lepton_driver * driver)
{
  sendCommand_u16(cmdCode
                  (LEP_CID_AGC_HEQ_CLIP_LIMIT_HIGH, LEP_I2C_COMMAND_TYPE_SET),
                  limit,&(driver->communication));
}

uint16_t getHEQClipLimitHigh(struct lepton_driver * driver)
{
  uint16_t limit;
  receiveCommand_u16(cmdCode
                     (LEP_CID_AGC_HEQ_CLIP_LIMIT_HIGH,
                      LEP_I2C_COMMAND_TYPE_GET), &limit,&(driver->communication));
  return limit;
}

void setHEQClipLimitLow(uint16_t limit, struct lepton_driver * driver)
{
  sendCommand_u16(cmdCode
                  (LEP_CID_AGC_HEQ_CLIP_LIMIT_LOW, LEP_I2C_COMMAND_TYPE_SET),
                  limit,&(driver->communication));
}

uint16_t getHEQClipLimitLow(struct lepton_driver * driver)
{
  uint16_t limit;
  receiveCommand_u16(cmdCode
                     (LEP_CID_AGC_HEQ_CLIP_LIMIT_LOW, LEP_I2C_COMMAND_TYPE_GET),
                     &limit,&(driver->communication));
  return limit;
}

void setHEQBinExtension(uint16_t extension, struct lepton_driver * driver)
{
  sendCommand_u16(cmdCode
                  (LEP_CID_AGC_HEQ_BIN_EXTENSION, LEP_I2C_COMMAND_TYPE_SET),
                  extension,&(driver->communication));
}

uint16_t getHEQBinExtension(struct lepton_driver * driver)
{
  uint16_t extension;
  receiveCommand_u16(cmdCode
                     (LEP_CID_AGC_HEQ_BIN_EXTENSION, LEP_I2C_COMMAND_TYPE_GET),
                     &extension,&(driver->communication));
  return extension;
}

void setHEQMidpoint(uint16_t midpoint, struct lepton_driver * driver)
{
  sendCommand_u16(cmdCode(LEP_CID_AGC_HEQ_MIDPOINT, LEP_I2C_COMMAND_TYPE_SET),
                  midpoint,&(driver->communication));
}

uint16_t getHEQMidpoint(struct lepton_driver * driver)
{
  uint16_t midpoint;
  receiveCommand_u16(cmdCode
                     (LEP_CID_AGC_HEQ_MIDPOINT, LEP_I2C_COMMAND_TYPE_GET),
                     &midpoint,&(driver->communication));
  return midpoint;
}

void setHEQEmptyCounts(uint16_t counts, struct lepton_driver * driver)
{
  sendCommand_u16(cmdCode
                  (LEP_CID_AGC_HEQ_EMPTY_COUNTS, LEP_I2C_COMMAND_TYPE_SET),
                  counts,&(driver->communication));
}

uint16_t getHEQEmptyCounts(struct lepton_driver * driver)
{
  uint16_t counts;
  receiveCommand_u16(cmdCode
                     (LEP_CID_AGC_HEQ_EMPTY_COUNTS, LEP_I2C_COMMAND_TYPE_GET),
                     &counts,&(driver->communication));
  return counts;
}

void setHEQNormalizationFactor(uint16_t factor, struct lepton_driver * driver)
{
  sendCommand_u16(cmdCode
                  (LEP_CID_AGC_HEQ_NORMALIZATION_FACTOR,
                   LEP_I2C_COMMAND_TYPE_SET), factor,&(driver->communication));
}

uint16_t getHEQNormalizationFactor(struct lepton_driver * driver)
{
  uint16_t factor;
  receiveCommand_u16(cmdCode
                     (LEP_CID_AGC_HEQ_NORMALIZATION_FACTOR,
                      LEP_I2C_COMMAND_TYPE_GET), &factor,&(driver->communication));
  return factor;
}

#endif

void lepton_agc_init(struct lepton_agc * this)
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
