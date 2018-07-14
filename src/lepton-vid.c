#define LEPTON_FLIR_INTERNAL
#include "lepton-vid.h"
#include "lepton-communication.h"

#ifndef LEPFLIR_EXCLUDE_VID_FUNCS

void setPolarity(struct lepton_driver *driver, LEP_VID_POLARITY polarity)
{
  sendCommand_u32(&(driver->communication),
                  cmdCode(LEP_CID_VID_POLARITY_SELECT,
                          LEP_I2C_COMMAND_TYPE_SET), (uint32_t) polarity);
}

LEP_VID_POLARITY getPolarity(struct lepton_driver *driver)
{
  uint32_t polarity;
  receiveCommand_u32(&(driver->communication),
                     cmdCode(LEP_CID_VID_POLARITY_SELECT,
                             LEP_I2C_COMMAND_TYPE_GET), &polarity);
  return (LEP_VID_POLARITY) polarity;
}

void setPseudoColorLUT(struct lepton_driver *driver, LEP_VID_PCOLOR_LUT table)
{
  sendCommand_u32(&(driver->communication),
                  cmdCode(LEP_CID_VID_LUT_SELECT, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) table);
}

LEP_VID_PCOLOR_LUT getPseudoColorLUT(struct lepton_driver *driver)
{
  uint32_t table;
  receiveCommand_u32(&(driver->communication),
                     cmdCode(LEP_CID_VID_LUT_SELECT, LEP_I2C_COMMAND_TYPE_GET),
                     &table);
  return (LEP_VID_PCOLOR_LUT) table;
}

void setFocusCalcEnabled(struct lepton_driver *driver, uint8_t enabled)
{
  sendCommand_u32(&(driver->communication),
                  cmdCode(LEP_CID_VID_FOCUS_CALC_ENABLE,
                          LEP_I2C_COMMAND_TYPE_SET), (uint32_t) enabled);
}

uint8_t getFocusCalcEnabled(struct lepton_driver *driver)
{
  uint32_t enabled;
  receiveCommand_u32(&(driver->communication),
                     cmdCode(LEP_CID_VID_FOCUS_CALC_ENABLE,
                             LEP_I2C_COMMAND_TYPE_GET), &enabled);
  return enabled;
}

void setFreezeEnabled(struct lepton_driver *driver, uint8_t enabled)
{
  sendCommand_u32(&(driver->communication),
                  cmdCode(LEP_CID_VID_FREEZE_ENABLE, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) enabled);
}

uint8_t getFreezeEnabled(struct lepton_driver *driver)
{
  uint32_t enabled;
  receiveCommand_u32(&(driver->communication),
                     cmdCode(LEP_CID_VID_FREEZE_ENABLE,
                             LEP_I2C_COMMAND_TYPE_GET), &enabled);
  return enabled;
}

#ifndef LEPFLIR_EXCLUDE_EXT_I2C_FUNCS

void setUserColorLUT(struct lepton_driver *driver, LEP_VID_LUT_BUFFER * table)
{
  if (!table)
    return;
  sendCommand_array(&(driver->communication),
                    cmdCode(LEP_CID_VID_LUT_TRANSFER, LEP_I2C_COMMAND_TYPE_SET),
                    (uint16_t *) table, sizeof(LEP_VID_LUT_BUFFER) / 2);
}

void getUserColorLUT(struct lepton_driver *driver, LEP_VID_LUT_BUFFER * table)
{
  if (!table)
    return;
  receiveCommand_array(&(driver->communication),
                       cmdCode(LEP_CID_VID_LUT_TRANSFER,
                               LEP_I2C_COMMAND_TYPE_GET), (uint16_t *) table,
                       sizeof(LEP_VID_LUT_BUFFER) / 2);
}

void setFocusRegion(struct lepton_driver *driver, LEP_VID_FOCUS_ROI * region)
{
  if (!region)
    return;
  sendCommand_array(&(driver->communication),
                    cmdCode(LEP_CID_VID_FOCUS_ROI, LEP_I2C_COMMAND_TYPE_SET),
                    (uint16_t *) region, sizeof(LEP_VID_FOCUS_ROI) / 2);
}

void getFocusRegion(struct lepton_driver *driver, LEP_VID_FOCUS_ROI * region)
{
  if (!region)
    return;
  receiveCommand_array(&(driver->communication),
                       cmdCode(LEP_CID_VID_FOCUS_ROI, LEP_I2C_COMMAND_TYPE_GET),
                       (uint16_t *) region, sizeof(LEP_VID_FOCUS_ROI) / 2);
}

void setFocusThreshold(struct lepton_driver *driver, uint32_t threshold)
{
  sendCommand_u32(&(driver->communication),
                  cmdCode(LEP_CID_VID_FOCUS_THRESHOLD,
                          LEP_I2C_COMMAND_TYPE_SET), threshold);
}

uint32_t getFocusThreshold(struct lepton_driver *driver)
{
  uint32_t threshold;
  receiveCommand_u32(&(driver->communication),
                     cmdCode(LEP_CID_VID_FOCUS_THRESHOLD,
                             LEP_I2C_COMMAND_TYPE_GET), &threshold);
  return threshold;
}

uint32_t getFocusMetric(struct lepton_driver * driver)
{
  uint32_t metric;
  receiveCommand_u32(&(driver->communication),
                     cmdCode(LEP_CID_VID_FOCUS_METRIC,
                             LEP_I2C_COMMAND_TYPE_GET), &metric);
  return metric;
}

void setSceneBasedNUCEnabled(struct lepton_driver *driver, uint8_t enabled)
{
  sendCommand_u32(&(driver->communication),
                  cmdCode(LEP_CID_VID_SBNUC_ENABLE, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) enabled);
}

uint8_t getSceneBasedNUCEnabled(struct lepton_driver *driver)
{
  uint32_t enabled;
  receiveCommand_u32(&(driver->communication),
                     cmdCode(LEP_CID_VID_SBNUC_ENABLE,
                             LEP_I2C_COMMAND_TYPE_GET), &enabled);
  return enabled;
}

void setGamma(struct lepton_driver *driver, uint32_t gamma)
{
  sendCommand_u32(&(driver->communication),
                  cmdCode(LEP_CID_VID_GAMMA_SELECT, LEP_I2C_COMMAND_TYPE_SET),
                  gamma);
}

uint32_t getGamma(struct lepton_driver *driver)
{
  uint32_t gamma;
  receiveCommand_u32(&(driver->communication),
                     cmdCode(LEP_CID_VID_GAMMA_SELECT,
                             LEP_I2C_COMMAND_TYPE_GET), &gamma);
  return gamma;
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
