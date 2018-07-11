#define LEPTON_FLIR_INTERNAL
#include "lepton-vid.h"
#include "lepton-communication.h"

#ifndef LEPFLIR_EXCLUDE_VID_FUNCS

void setPolarity(LEP_VID_POLARITY polarity, struct lepton_driver * driver)
{
  sendCommand_u32(cmdCode
                  (LEP_CID_VID_POLARITY_SELECT, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) polarity,&(driver->communication));
}

LEP_VID_POLARITY getPolarity(struct lepton_driver * driver)
{
  uint32_t polarity;
  receiveCommand_u32(cmdCode
                     (LEP_CID_VID_POLARITY_SELECT, LEP_I2C_COMMAND_TYPE_GET),
                     &polarity,&(driver->communication));
  return (LEP_VID_POLARITY) polarity;
}

void setPseudoColorLUT(LEP_VID_PCOLOR_LUT table, struct lepton_driver * driver)
{
  sendCommand_u32(cmdCode(LEP_CID_VID_LUT_SELECT, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) table,&(driver->communication));
}

LEP_VID_PCOLOR_LUT getPseudoColorLUT(struct lepton_driver * driver)
{
  uint32_t table;
  receiveCommand_u32(cmdCode(LEP_CID_VID_LUT_SELECT, LEP_I2C_COMMAND_TYPE_GET),
                     &table,&(driver->communication));
  return (LEP_VID_PCOLOR_LUT) table;
}

void setFocusCalcEnabled(uint8_t enabled, struct lepton_driver * driver)
{
  sendCommand_u32(cmdCode
                  (LEP_CID_VID_FOCUS_CALC_ENABLE, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) enabled,&(driver->communication));
}

uint8_t getFocusCalcEnabled(struct lepton_driver * driver)
{
  uint32_t enabled;
  receiveCommand_u32(cmdCode
                     (LEP_CID_VID_FOCUS_CALC_ENABLE, LEP_I2C_COMMAND_TYPE_GET),
                     &enabled,&(driver->communication));
  return enabled;
}

void setFreezeEnabled(uint8_t enabled, struct lepton_driver * driver)
{
  sendCommand_u32(cmdCode(LEP_CID_VID_FREEZE_ENABLE, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) enabled,&(driver->communication));
}

uint8_t getFreezeEnabled(struct lepton_driver * driver)
{
  uint32_t enabled;
  receiveCommand_u32(cmdCode
                     (LEP_CID_VID_FREEZE_ENABLE, LEP_I2C_COMMAND_TYPE_GET),
                     &enabled,&(driver->communication));
  return enabled;
}

#ifndef LEPFLIR_EXCLUDE_EXT_I2C_FUNCS

void setUserColorLUT(LEP_VID_LUT_BUFFER * table, struct lepton_driver * driver)
{
  if (!table)
    return;
  sendCommand_array(cmdCode(LEP_CID_VID_LUT_TRANSFER, LEP_I2C_COMMAND_TYPE_SET),
                    (uint16_t *) table, sizeof(LEP_VID_LUT_BUFFER) / 2,&(driver->communication));
}

void getUserColorLUT(LEP_VID_LUT_BUFFER * table, struct lepton_driver * driver)
{
  if (!table)
    return;
  receiveCommand_array(cmdCode
                       (LEP_CID_VID_LUT_TRANSFER, LEP_I2C_COMMAND_TYPE_GET),
                       (uint16_t *) table, sizeof(LEP_VID_LUT_BUFFER) / 2,&(driver->communication));
}

void setFocusRegion(LEP_VID_FOCUS_ROI * region, struct lepton_driver * driver)
{
  if (!region)
    return;
  sendCommand_array(cmdCode(LEP_CID_VID_FOCUS_ROI, LEP_I2C_COMMAND_TYPE_SET),
                    (uint16_t *) region, sizeof(LEP_VID_FOCUS_ROI) / 2,&(driver->communication));
}

void getFocusRegion(LEP_VID_FOCUS_ROI * region, struct lepton_driver * driver)
{
  if (!region)
    return;
  receiveCommand_array(cmdCode(LEP_CID_VID_FOCUS_ROI, LEP_I2C_COMMAND_TYPE_GET),
                       (uint16_t *) region, sizeof(LEP_VID_FOCUS_ROI) / 2,&(driver->communication));
}

void setFocusThreshold(uint32_t threshold, struct lepton_driver * driver)
{
  sendCommand_u32(cmdCode
                  (LEP_CID_VID_FOCUS_THRESHOLD, LEP_I2C_COMMAND_TYPE_SET),
                  threshold,&(driver->communication));
}

uint32_t getFocusThreshold(struct lepton_driver * driver)
{
  uint32_t threshold;
  receiveCommand_u32(cmdCode
                     (LEP_CID_VID_FOCUS_THRESHOLD, LEP_I2C_COMMAND_TYPE_GET),
                     &threshold,&(driver->communication));
  return threshold;
}

uint32_t getFocusMetric(struct lepton_driver * driver)
{
  uint32_t metric;
  receiveCommand_u32(cmdCode
                     (LEP_CID_VID_FOCUS_METRIC, LEP_I2C_COMMAND_TYPE_GET),
                     &metric,&(driver->communication));
  return metric;
}

void setSceneBasedNUCEnabled(uint8_t enabled, struct lepton_driver * driver)
{
  sendCommand_u32(cmdCode(LEP_CID_VID_SBNUC_ENABLE, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) enabled,&(driver->communication));
}

uint8_t getSceneBasedNUCEnabled(struct lepton_driver * driver)
{
  uint32_t enabled;
  receiveCommand_u32(cmdCode
                     (LEP_CID_VID_SBNUC_ENABLE, LEP_I2C_COMMAND_TYPE_GET),
                     &enabled,&(driver->communication));
  return enabled;
}

void setGamma(uint32_t gamma, struct lepton_driver * driver)
{
  sendCommand_u32(cmdCode(LEP_CID_VID_GAMMA_SELECT, LEP_I2C_COMMAND_TYPE_SET),
                  gamma,&(driver->communication));
}

uint32_t getGamma(struct lepton_driver * driver)
{
  uint32_t gamma;
  receiveCommand_u32(cmdCode
                     (LEP_CID_VID_GAMMA_SELECT, LEP_I2C_COMMAND_TYPE_GET),
                     &gamma,&(driver->communication));
  return gamma;
}

#endif

void lepton_vid_init(struct lepton_vid * this)
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