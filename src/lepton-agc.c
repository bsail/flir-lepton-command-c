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
}

void setLinearMaxGain(struct lepton_driver *driver, uint16_t gain)
{
  if (driver) {
    sendCommand_u16(&(driver->communication),
                    cmdCode(LEP_CID_AGC_LINEAR_MAX_GAIN,
                            LEP_I2C_COMMAND_TYPE_SET), gain);
  }
}

uint16_t getLinearMaxGain(struct lepton_driver *driver)
{
  if (driver) {
    uint16_t gain;
    receiveCommand_u16(&(driver->communication),
                       cmdCode(LEP_CID_AGC_LINEAR_MAX_GAIN,
                               LEP_I2C_COMMAND_TYPE_GET), &gain);
    return gain;
  }
}

void setLinearMidpoint(struct lepton_driver *driver, uint16_t midpoint)
{
  if (driver) {
    sendCommand_u16(&(driver->communication),
                    cmdCode(LEP_CID_AGC_LINEAR_MIDPOINT,
                            LEP_I2C_COMMAND_TYPE_SET), midpoint);
  }
}

uint16_t getLinearMidpoint(struct lepton_driver *driver)
{
  if (driver) {
    uint16_t midpoint;
    receiveCommand_u16(&(driver->communication),
                       cmdCode(LEP_CID_AGC_LINEAR_MIDPOINT,
                               LEP_I2C_COMMAND_TYPE_GET), &midpoint);
    return midpoint;
  }
}

void setLinearDampeningFactor(struct lepton_driver *driver, uint16_t factor)
{
  if (driver) {
    sendCommand_u16(&(driver->communication),
                    cmdCode(LEP_CID_AGC_LINEAR_DAMPENING_FACTOR,
                            LEP_I2C_COMMAND_TYPE_SET), factor);
  }
}

uint16_t getLinearDampeningFactor(struct lepton_driver *driver)
{
  if (driver) {
    uint16_t factor;
    receiveCommand_u16(&(driver->communication),
                       cmdCode(LEP_CID_AGC_LINEAR_DAMPENING_FACTOR,
                               LEP_I2C_COMMAND_TYPE_GET), &factor);
    return factor;
  }
}

void setHEQDampeningFactor(struct lepton_driver *driver, uint16_t factor)
{
  if (driver) {
    sendCommand_u16(&(driver->communication),
                    cmdCode(LEP_CID_AGC_HEQ_DAMPENING_FACTOR,
                            LEP_I2C_COMMAND_TYPE_SET), factor);
  }
}

uint16_t getHEQDampeningFactor(struct lepton_driver *driver)
{
  if (driver) {
    uint16_t factor;
    receiveCommand_u16(&(driver->communication),
                       cmdCode(LEP_CID_AGC_HEQ_DAMPENING_FACTOR,
                               LEP_I2C_COMMAND_TYPE_GET), &factor);
    return factor;
  }
}

void setHEQMaxGain(struct lepton_driver *driver, uint16_t gain)
{
  if (driver) {
    sendCommand_u16(&(driver->communication),
                    cmdCode(LEP_CID_AGC_HEQ_MAX_GAIN, LEP_I2C_COMMAND_TYPE_SET),
                    gain);
  }
}

uint16_t getHEQMaxGain(struct lepton_driver *driver)
{
  if (driver) {
    uint16_t gain;
    receiveCommand_u16(&(driver->communication),
                       cmdCode(LEP_CID_AGC_HEQ_MAX_GAIN,
                               LEP_I2C_COMMAND_TYPE_GET), &gain);
    return gain;
  }
}

void setHEQClipLimitHigh(struct lepton_driver *driver, uint16_t limit)
{
  if (driver) {
    sendCommand_u16(&(driver->communication),
                    cmdCode(LEP_CID_AGC_HEQ_CLIP_LIMIT_HIGH,
                            LEP_I2C_COMMAND_TYPE_SET), limit);
  }
}

uint16_t getHEQClipLimitHigh(struct lepton_driver *driver)
{
  if (driver) {
    uint16_t limit;
    receiveCommand_u16(&(driver->communication),
                       cmdCode(LEP_CID_AGC_HEQ_CLIP_LIMIT_HIGH,
                               LEP_I2C_COMMAND_TYPE_GET), &limit);
    return limit;
  }
}

void setHEQClipLimitLow(struct lepton_driver *driver, uint16_t limit)
{
  if (driver) {
    sendCommand_u16(&(driver->communication),
                    cmdCode(LEP_CID_AGC_HEQ_CLIP_LIMIT_LOW,
                            LEP_I2C_COMMAND_TYPE_SET), limit);
  }
}

uint16_t getHEQClipLimitLow(struct lepton_driver *driver)
{
  if (driver) {
    uint16_t limit;
    receiveCommand_u16(&(driver->communication),
                       cmdCode(LEP_CID_AGC_HEQ_CLIP_LIMIT_LOW,
                               LEP_I2C_COMMAND_TYPE_GET), &limit);
    return limit;
  }
}

void setHEQBinExtension(struct lepton_driver *driver, uint16_t extension)
{
  if (driver) {
    sendCommand_u16(&(driver->communication),
                    cmdCode(LEP_CID_AGC_HEQ_BIN_EXTENSION,
                            LEP_I2C_COMMAND_TYPE_SET), extension);
  }
}

uint16_t getHEQBinExtension(struct lepton_driver *driver)
{
  if (driver) {
    uint16_t extension;
    receiveCommand_u16(&(driver->communication),
                       cmdCode(LEP_CID_AGC_HEQ_BIN_EXTENSION,
                               LEP_I2C_COMMAND_TYPE_GET), &extension);
    return extension;
  }
}

void setHEQMidpoint(struct lepton_driver *driver, uint16_t midpoint)
{
  if (driver) {
    sendCommand_u16(&(driver->communication),
                    cmdCode(LEP_CID_AGC_HEQ_MIDPOINT, LEP_I2C_COMMAND_TYPE_SET),
                    midpoint);
  }
}

uint16_t getHEQMidpoint(struct lepton_driver *driver)
{
  if (driver) {
    uint16_t midpoint;
    receiveCommand_u16(&(driver->communication),
                       cmdCode(LEP_CID_AGC_HEQ_MIDPOINT,
                               LEP_I2C_COMMAND_TYPE_GET), &midpoint);
    return midpoint;
  }
}

void setHEQEmptyCounts(struct lepton_driver *driver, uint16_t counts)
{
  if (driver) {
    sendCommand_u16(&(driver->communication),
                    cmdCode(LEP_CID_AGC_HEQ_EMPTY_COUNTS,
                            LEP_I2C_COMMAND_TYPE_SET), counts);
  }
}

uint16_t getHEQEmptyCounts(struct lepton_driver *driver)
{
  if (driver) {
    uint16_t counts;
    receiveCommand_u16(&(driver->communication),
                       cmdCode(LEP_CID_AGC_HEQ_EMPTY_COUNTS,
                               LEP_I2C_COMMAND_TYPE_GET), &counts);
    return counts;
  }
}

void setHEQNormalizationFactor(struct lepton_driver *driver, uint16_t factor)
{
  if (driver) {
    sendCommand_u16(&(driver->communication),
                    cmdCode(LEP_CID_AGC_HEQ_NORMALIZATION_FACTOR,
                            LEP_I2C_COMMAND_TYPE_SET), factor);
  }
}

uint16_t getHEQNormalizationFactor(struct lepton_driver *driver)
{
  if (driver) {
    uint16_t factor;
    receiveCommand_u16(&(driver->communication),
                       cmdCode(LEP_CID_AGC_HEQ_NORMALIZATION_FACTOR,
                               LEP_I2C_COMMAND_TYPE_GET), &factor);
    return factor;
  }
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
