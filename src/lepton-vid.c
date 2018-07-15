#define LEPTON_FLIR_INTERNAL
#include "lepton-vid.h"
#include "lepton-flir.h"
#include "lepton-communication.h"
#include "lepton-get-set.h"

#ifndef LEPFLIR_EXCLUDE_VID_FUNCS

void setPolarity(struct lepton_driver *driver, LEP_VID_POLARITY polarity)
{
  if (driver) {
    set_common(driver, u32, LEP_CID_VID_POLARITY_SELECT,
               (uint16_t *) &polarity, 4);
  }
}

LEP_VID_POLARITY getPolarity(struct lepton_driver *driver)
{
  if (driver) {
    uint32_t polarity;
    get_common(driver, u32, LEP_CID_VID_POLARITY_SELECT,
               (uint16_t *) &polarity, 4);
    return (LEP_VID_POLARITY) polarity;
  }
  return 0;
}

void setPseudoColorLUT(struct lepton_driver *driver, LEP_VID_PCOLOR_LUT table)
{
  if (driver) {
    set_common(driver, u32, LEP_CID_VID_LUT_SELECT,
               (uint16_t *) &table, 4);
  }
}

LEP_VID_PCOLOR_LUT getPseudoColorLUT(struct lepton_driver *driver)
{
  if (driver) {
    uint32_t table;
    get_common(driver, u32, LEP_CID_VID_LUT_SELECT,
               (uint16_t *) &table, 4);
    return (LEP_VID_PCOLOR_LUT) table;
  }
  return 0;
}

void setFocusCalcEnabled(struct lepton_driver *driver, uint8_t enabled)
{
  if (driver) {
    set_common(driver, u32, LEP_CID_VID_FOCUS_CALC_ENABLE,
               (uint16_t *) &enabled, 4);
  }
}

uint8_t getFocusCalcEnabled(struct lepton_driver *driver)
{
  if (driver) {
    uint32_t enabled;
    get_common(driver, u32, LEP_CID_VID_FOCUS_CALC_ENABLE,
               (uint16_t *) &enabled, 4);
    return enabled;
  }
  return 0;
}

void setFreezeEnabled(struct lepton_driver *driver, uint8_t enabled)
{
  if (driver) {
    set_common(driver, u32, LEP_CID_VID_FREEZE_ENABLE,
               (uint16_t *) &enabled, 4);
  }
}

uint8_t getFreezeEnabled(struct lepton_driver *driver)
{
  if (driver) {
    uint32_t enabled;
    get_common(driver, u32, LEP_CID_VID_FREEZE_ENABLE,
               (uint16_t *) &enabled, 4);
    return enabled;
  }
  return 0;
}

#ifndef LEPFLIR_EXCLUDE_EXT_I2C_FUNCS

void setUserColorLUT(struct lepton_driver *driver, LEP_VID_LUT_BUFFER * table)
{
  if (driver) {
    if (!table)
      return;
    set_common(driver, array, LEP_CID_VID_LUT_TRANSFER,
               (uint16_t *) table, sizeof(LEP_VID_LUT_BUFFER) / 2);
  }
}

void getUserColorLUT(struct lepton_driver *driver, LEP_VID_LUT_BUFFER * table)
{
  if (driver) {
    if (!table)
      return;
    get_common(driver, array, LEP_CID_VID_LUT_TRANSFER,
               (uint16_t *) table, sizeof(LEP_VID_LUT_BUFFER) / 2);
  }
}

void setFocusRegion(struct lepton_driver *driver, LEP_VID_FOCUS_ROI * region)
{
  if (driver) {
    if (!region)
      return;
    set_common(driver, array, LEP_CID_VID_FOCUS_ROI,
               (uint16_t *) region, sizeof(LEP_VID_FOCUS_ROI) / 2);
  }
}

void getFocusRegion(struct lepton_driver *driver, LEP_VID_FOCUS_ROI * region)
{
  if (driver) {
    if (!region)
      return;
    get_common(driver, array, LEP_CID_VID_FOCUS_ROI,
               (uint16_t *) region, sizeof(LEP_VID_FOCUS_ROI) / 2);
  }
}

void setFocusThreshold(struct lepton_driver *driver, uint32_t threshold)
{
  if (driver) {
    set_common(driver, u32, LEP_CID_VID_FOCUS_THRESHOLD,
               (uint16_t *) &threshold, 4);
  }
}

uint32_t getFocusThreshold(struct lepton_driver *driver)
{
  if (driver) {
    uint32_t threshold;
    get_common(driver, u32, LEP_CID_VID_FOCUS_THRESHOLD,
               (uint16_t *) &threshold, 4);
    return threshold;
  }
  return 0;
}

uint32_t getFocusMetric(struct lepton_driver * driver)
{
  if (driver) {
    uint32_t metric;
    get_common(driver, u32, LEP_CID_VID_FOCUS_METRIC,
               (uint16_t *) &metric, 4);
    return metric;
  }
  return 0;
}

void setSceneBasedNUCEnabled(struct lepton_driver *driver, uint8_t enabled)
{
  if (driver) {
    set_common(driver, u32, LEP_CID_VID_SBNUC_ENABLE,
               (uint16_t *) &enabled, 4);
  }
}

uint8_t getSceneBasedNUCEnabled(struct lepton_driver *driver)
{
  if (driver) {
    uint32_t enabled;
    get_common(driver, u32, LEP_CID_VID_SBNUC_ENABLE,
               (uint16_t *) &enabled, 4);
    return enabled;
  }
  return 0;
}

void setGamma(struct lepton_driver *driver, uint32_t gamma)
{
  if (driver) {
    set_common(driver, u32, LEP_CID_VID_GAMMA_SELECT,
               (uint16_t *) &gamma, 4);
  }
}

uint32_t getGamma(struct lepton_driver *driver)
{
  if (driver) {
    uint32_t gamma;
    get_common(driver, u32, LEP_CID_VID_GAMMA_SELECT,
               (uint16_t *) &gamma, 4);
    return gamma;
  }
  return 0;
}

#endif

void lepton_vid_init(struct lepton_vid *this)
{
  this->setPolarity = &setPolarity;
  this->getPolarity = &getPolarity;
  this->setPseudoColorLUT = &setPseudoColorLUT;
  this->getPseudoColorLUT = &getPseudoColorLUT;
  this->setFocusCalcEnabled = &setFocusCalcEnabled;
  this->getFocusCalcEnabled = &getFocusCalcEnabled;
  this->setFreezeEnabled = &setFreezeEnabled;
  this->getFreezeEnabled = &getFreezeEnabled;
#ifndef LEPFLIR_EXCLUDE_EXT_I2C_FUNCS
  this->setUserColorLUT = &setUserColorLUT;
  this->getUserColorLUT = &getUserColorLUT;
  this->setFocusRegion = &setFocusRegion;
  this->getFocusRegion = &getFocusRegion;
  this->setFocusThreshold = &setFocusThreshold;
  this->getFocusThreshold = &getFocusThreshold;
  this->getFocusMetric = &getFocusMetric;
  this->setSceneBasedNUCEnabled = &setSceneBasedNUCEnabled;
  this->getSceneBasedNUCEnabled = &getSceneBasedNUCEnabled;
  this->setGamma = &setGamma;
  this->getGamma = &getGamma;
#endif
}

#endif
