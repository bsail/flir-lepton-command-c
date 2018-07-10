#include "lepton-agc.h"

void agc_setAGCEnabled(uint8_t enabled, struct lepton_communication * communication)
{
  communication->sendCommand_u32(communication->cmdCode(LEP_CID_AGC_ENABLE_STATE, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) enabled,communication);
}

uint8_t agc_getAGCEnabled(struct lepton_communication * communication)
{
  uint32_t enabled;
  communication->receiveCommand_u32(communication->cmdCode
                     (LEP_CID_AGC_ENABLE_STATE, LEP_I2C_COMMAND_TYPE_GET),
                     &enabled,communication);
  return enabled;
}

void agc_setAGCPolicy(LEP_AGC_POLICY policy, struct lepton_communication * communication)
{
  communication->sendCommand_u32(communication->cmdCode(LEP_CID_AGC_POLICY, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) policy,communication);
}

LEP_AGC_POLICY agc_getAGCPolicy(struct lepton_communication * communication)
{
  uint32_t policy;
  communication->receiveCommand_u32(communication->cmdCode(LEP_CID_AGC_POLICY, LEP_I2C_COMMAND_TYPE_GET),
                     &policy,communication);
  return (LEP_AGC_POLICY) policy;
}

void agc_setHEQScaleFactor(LEP_AGC_HEQ_SCALE_FACTOR factor, struct lepton_communication * communication)
{
  communication->sendCommand_u32(communication->cmdCode
                  (LEP_CID_AGC_HEQ_SCALE_FACTOR, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) factor,communication);
}

LEP_AGC_HEQ_SCALE_FACTOR agc_getHEQScaleFactor(struct lepton_communication * communication)
{
  uint32_t factor;
  communication->receiveCommand_u32(communication->cmdCode
                     (LEP_CID_AGC_HEQ_SCALE_FACTOR, LEP_I2C_COMMAND_TYPE_GET),
                     &factor,communication);
  return (LEP_AGC_HEQ_SCALE_FACTOR) factor;
}

void agc_setAGCCalcEnabled(uint8_t enabled, struct lepton_communication * communication)
{
  communication->sendCommand_u32(communication->cmdCode
                  (LEP_CID_AGC_CALC_ENABLE_STATE, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) enabled,communication);
}

uint8_t agc_getAGCCalcEnabled(struct lepton_communication * communication)
{
  uint32_t enabled;
  communication->receiveCommand_u32(communication->cmdCode
                     (LEP_CID_AGC_CALC_ENABLE_STATE, LEP_I2C_COMMAND_TYPE_GET),
                     &enabled,communication);
  return enabled;
}

#ifndef LEPFLIR_EXCLUDE_EXT_I2C_FUNCS

void agc_setHistogramRegion(LEP_AGC_HISTOGRAM_ROI * region, struct lepton_communication * communication)
{
  if (!region)
    return;
  communication->sendCommand_array(communication->cmdCode(LEP_CID_AGC_ROI, LEP_I2C_COMMAND_TYPE_SET),
                    (uint16_t *) region, sizeof(LEP_AGC_HISTOGRAM_ROI) / 2,communication);
}

void agc_getHistogramRegion(LEP_AGC_HISTOGRAM_ROI * region, struct lepton_communication * communication)
{
  if (!region)
    return;
  communication->receiveCommand_array(communication->cmdCode(LEP_CID_AGC_ROI, LEP_I2C_COMMAND_TYPE_GET),
                       (uint16_t *) region, sizeof(LEP_AGC_HISTOGRAM_ROI) / 2,communication);
}

void agc_getHistogramStatistics(LEP_AGC_HISTOGRAM_STATISTICS * statistics, struct lepton_communication * communication)
{
  if (!statistics)
    return;
  communication->receiveCommand_array(communication->cmdCode
                       (LEP_CID_AGC_STATISTICS, LEP_I2C_COMMAND_TYPE_GET),
                       (uint16_t *) statistics,
                       sizeof(LEP_AGC_HISTOGRAM_STATISTICS) / 2,communication);
}

void agc_setHistogramClipPercent(uint16_t percent, struct lepton_communication * communication)
{
  communication->sendCommand_u16(communication->cmdCode
                  (LEP_CID_AGC_HISTOGRAM_CLIP_PERCENT,
                   LEP_I2C_COMMAND_TYPE_SET), percent,communication);
}

uint16_t agc_getHistogramClipPercent(struct lepton_communication * communication)
{
  uint16_t percent;
  communication->receiveCommand_u16(communication->cmdCode
                     (LEP_CID_AGC_HISTOGRAM_CLIP_PERCENT,
                      LEP_I2C_COMMAND_TYPE_GET), &percent,communication);
  return percent;
}

void agc_setHistogramTailSize(uint16_t size, struct lepton_communication * communication)
{
  communication->sendCommand_u16(communication->cmdCode
                  (LEP_CID_AGC_HISTOGRAM_TAIL_SIZE, LEP_I2C_COMMAND_TYPE_SET),
                  size,communication);
}

uint16_t agc_getHistogramTailSize(struct lepton_communication * communication)
{
  uint16_t size;
  communication->receiveCommand_u16(communication->cmdCode
                     (LEP_CID_AGC_HISTOGRAM_TAIL_SIZE,
                      LEP_I2C_COMMAND_TYPE_GET), &size,communication);
  return size;
}

void agc_setLinearMaxGain(uint16_t gain, struct lepton_communication * communication)
{
  communication->sendCommand_u16(communication->cmdCode
                  (LEP_CID_AGC_LINEAR_MAX_GAIN, LEP_I2C_COMMAND_TYPE_SET),
                  gain,communication);
}

uint16_t agc_getLinearMaxGain(struct lepton_communication * communication)
{
  uint16_t gain;
  communication->receiveCommand_u16(communication->cmdCode
                     (LEP_CID_AGC_LINEAR_MAX_GAIN, LEP_I2C_COMMAND_TYPE_GET),
                     &gain,communication);
  return gain;
}

void agc_setLinearMidpoint(uint16_t midpoint, struct lepton_communication * communication)
{
  communication->sendCommand_u16(communication->cmdCode
                  (LEP_CID_AGC_LINEAR_MIDPOINT, LEP_I2C_COMMAND_TYPE_SET),
                  midpoint,communication);
}

uint16_t agc_getLinearMidpoint(struct lepton_communication * communication)
{
  uint16_t midpoint;
  communication->receiveCommand_u16(communication->cmdCode
                     (LEP_CID_AGC_LINEAR_MIDPOINT, LEP_I2C_COMMAND_TYPE_GET),
                     &midpoint,communication);
  return midpoint;
}

void agc_setLinearDampeningFactor(uint16_t factor, struct lepton_communication * communication)
{
  communication->sendCommand_u16(communication->cmdCode
                  (LEP_CID_AGC_LINEAR_DAMPENING_FACTOR,
                   LEP_I2C_COMMAND_TYPE_SET), factor,communication);
}

uint16_t agc_getLinearDampeningFactor(struct lepton_communication * communication)
{
  uint16_t factor;
  communication->receiveCommand_u16(communication->cmdCode
                     (LEP_CID_AGC_LINEAR_DAMPENING_FACTOR,
                      LEP_I2C_COMMAND_TYPE_GET), &factor,communication);
  return factor;
}

void agc_setHEQDampeningFactor(uint16_t factor, struct lepton_communication * communication)
{
  communication->sendCommand_u16(communication->cmdCode
                  (LEP_CID_AGC_HEQ_DAMPENING_FACTOR, LEP_I2C_COMMAND_TYPE_SET),
                  factor,communication);
}

uint16_t agc_getHEQDampeningFactor(struct lepton_communication * communication)
{
  uint16_t factor;
  communication->receiveCommand_u16(communication->cmdCode
                     (LEP_CID_AGC_HEQ_DAMPENING_FACTOR,
                      LEP_I2C_COMMAND_TYPE_GET), &factor,communication);
  return factor;
}

void agc_setHEQMaxGain(uint16_t gain, struct lepton_communication * communication)
{
  communication->sendCommand_u16(communication->cmdCode(LEP_CID_AGC_HEQ_MAX_GAIN, LEP_I2C_COMMAND_TYPE_SET),
                  gain,communication);
}

uint16_t agc_getHEQMaxGain(struct lepton_communication * communication)
{
  uint16_t gain;
  communication->receiveCommand_u16(communication->cmdCode
                     (LEP_CID_AGC_HEQ_MAX_GAIN, LEP_I2C_COMMAND_TYPE_GET),
                     &gain,communication);
  return gain;
}

void agc_setHEQClipLimitHigh(uint16_t limit, struct lepton_communication * communication)
{
  communication->sendCommand_u16(communication->cmdCode
                  (LEP_CID_AGC_HEQ_CLIP_LIMIT_HIGH, LEP_I2C_COMMAND_TYPE_SET),
                  limit,communication);
}

uint16_t agc_getHEQClipLimitHigh(struct lepton_communication * communication)
{
  uint16_t limit;
  communication->receiveCommand_u16(communication->cmdCode
                     (LEP_CID_AGC_HEQ_CLIP_LIMIT_HIGH,
                      LEP_I2C_COMMAND_TYPE_GET), &limit,communication);
  return limit;
}

void agc_setHEQClipLimitLow(uint16_t limit, struct lepton_communication * communication)
{
  communication->sendCommand_u16(communication->cmdCode
                  (LEP_CID_AGC_HEQ_CLIP_LIMIT_LOW, LEP_I2C_COMMAND_TYPE_SET),
                  limit,communication);
}

uint16_t agc_getHEQClipLimitLow(struct lepton_communication * communication)
{
  uint16_t limit;
  communication->receiveCommand_u16(communication->cmdCode
                     (LEP_CID_AGC_HEQ_CLIP_LIMIT_LOW, LEP_I2C_COMMAND_TYPE_GET),
                     &limit,communication);
  return limit;
}

void agc_setHEQBinExtension(uint16_t extension, struct lepton_communication * communication)
{
  communication->sendCommand_u16(communication->cmdCode
                  (LEP_CID_AGC_HEQ_BIN_EXTENSION, LEP_I2C_COMMAND_TYPE_SET),
                  extension,communication);
}

uint16_t agc_getHEQBinExtension(struct lepton_communication * communication)
{
  uint16_t extension;
  communication->receiveCommand_u16(communication->cmdCode
                     (LEP_CID_AGC_HEQ_BIN_EXTENSION, LEP_I2C_COMMAND_TYPE_GET),
                     &extension,communication);
  return extension;
}

void agc_setHEQMidpoint(uint16_t midpoint, struct lepton_communication * communication)
{
  communication->sendCommand_u16(communication->cmdCode(LEP_CID_AGC_HEQ_MIDPOINT, LEP_I2C_COMMAND_TYPE_SET),
                  midpoint,communication);
}

uint16_t agc_getHEQMidpoint(struct lepton_communication * communication)
{
  uint16_t midpoint;
  communication->receiveCommand_u16(communication->cmdCode
                     (LEP_CID_AGC_HEQ_MIDPOINT, LEP_I2C_COMMAND_TYPE_GET),
                     &midpoint,communication);
  return midpoint;
}

void agc_setHEQEmptyCounts(uint16_t counts, struct lepton_communication * communication)
{
  communication->sendCommand_u16(communication->cmdCode
                  (LEP_CID_AGC_HEQ_EMPTY_COUNTS, LEP_I2C_COMMAND_TYPE_SET),
                  counts,communication);
}

uint16_t agc_getHEQEmptyCounts(struct lepton_communication * communication)
{
  uint16_t counts;
  communication->receiveCommand_u16(communication->cmdCode
                     (LEP_CID_AGC_HEQ_EMPTY_COUNTS, LEP_I2C_COMMAND_TYPE_GET),
                     &counts,communication);
  return counts;
}

void agc_setHEQNormalizationFactor(uint16_t factor, struct lepton_communication * communication)
{
  communication->sendCommand_u16(communication->cmdCode
                  (LEP_CID_AGC_HEQ_NORMALIZATION_FACTOR,
                   LEP_I2C_COMMAND_TYPE_SET), factor,communication);
}

uint16_t agc_getHEQNormalizationFactor(struct lepton_communication * communication)
{
  uint16_t factor;
  communication->receiveCommand_u16(communication->cmdCode
                     (LEP_CID_AGC_HEQ_NORMALIZATION_FACTOR,
                      LEP_I2C_COMMAND_TYPE_GET), &factor,communication);
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
