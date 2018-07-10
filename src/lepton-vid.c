#include "lepton-vid.h"

void vid_setPolarity(LEP_VID_POLARITY polarity, void * driver)
{
  ((struct lepton_driver*)driver)->communication.sendCommand_u32(((struct lepton_driver*)driver)->communication.cmdCode
                  (LEP_CID_VID_POLARITY_SELECT, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) polarity,&(((struct lepton_driver*)driver)->communication));
}

LEP_VID_POLARITY vid_getPolarity(void * driver)
{
  uint32_t polarity;
  ((struct lepton_driver*)driver)->communication.receiveCommand_u32(((struct lepton_driver*)driver)->communication.cmdCode
                     (LEP_CID_VID_POLARITY_SELECT, LEP_I2C_COMMAND_TYPE_GET),
                     &polarity,&(((struct lepton_driver*)driver)->communication));
  return (LEP_VID_POLARITY) polarity;
}

void vid_setPseudoColorLUT(LEP_VID_PCOLOR_LUT table, void * driver)
{
  ((struct lepton_driver*)driver)->communication.sendCommand_u32(((struct lepton_driver*)driver)->communication.cmdCode(LEP_CID_VID_LUT_SELECT, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) table,&(((struct lepton_driver*)driver)->communication));
}

LEP_VID_PCOLOR_LUT vid_getPseudoColorLUT(void * driver)
{
  uint32_t table;
  ((struct lepton_driver*)driver)->communication.receiveCommand_u32(((struct lepton_driver*)driver)->communication.cmdCode(LEP_CID_VID_LUT_SELECT, LEP_I2C_COMMAND_TYPE_GET),
                     &table,&(((struct lepton_driver*)driver)->communication));
  return (LEP_VID_PCOLOR_LUT) table;
}

void vid_setFocusCalcEnabled(uint8_t enabled, void * driver)
{
  ((struct lepton_driver*)driver)->communication.sendCommand_u32(((struct lepton_driver*)driver)->communication.cmdCode
                  (LEP_CID_VID_FOCUS_CALC_ENABLE, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) enabled,&(((struct lepton_driver*)driver)->communication));
}

uint8_t vid_getFocusCalcEnabled(void * driver)
{
  uint32_t enabled;
  ((struct lepton_driver*)driver)->communication.receiveCommand_u32(((struct lepton_driver*)driver)->communication.cmdCode
                     (LEP_CID_VID_FOCUS_CALC_ENABLE, LEP_I2C_COMMAND_TYPE_GET),
                     &enabled,&(((struct lepton_driver*)driver)->communication));
  return enabled;
}

void vid_setFreezeEnabled(uint8_t enabled, void * driver)
{
  ((struct lepton_driver*)driver)->communication.sendCommand_u32(((struct lepton_driver*)driver)->communication.cmdCode(LEP_CID_VID_FREEZE_ENABLE, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) enabled,&(((struct lepton_driver*)driver)->communication));
}

uint8_t vid_getFreezeEnabled(void * driver)
{
  uint32_t enabled;
  ((struct lepton_driver*)driver)->communication.receiveCommand_u32(((struct lepton_driver*)driver)->communication.cmdCode
                     (LEP_CID_VID_FREEZE_ENABLE, LEP_I2C_COMMAND_TYPE_GET),
                     &enabled,&(((struct lepton_driver*)driver)->communication));
  return enabled;
}

#ifndef LEPFLIR_EXCLUDE_EXT_I2C_FUNCS

void vid_setUserColorLUT(LEP_VID_LUT_BUFFER * table, void * driver)
{
  if (!table)
    return;
  ((struct lepton_driver*)driver)->communication.sendCommand_array(((struct lepton_driver*)driver)->communication.cmdCode(LEP_CID_VID_LUT_TRANSFER, LEP_I2C_COMMAND_TYPE_SET),
                    (uint16_t *) table, sizeof(LEP_VID_LUT_BUFFER) / 2,&(((struct lepton_driver*)driver)->communication));
}

void vid_getUserColorLUT(LEP_VID_LUT_BUFFER * table, void * driver)
{
  if (!table)
    return;
  ((struct lepton_driver*)driver)->communication.receiveCommand_array(((struct lepton_driver*)driver)->communication.cmdCode
                       (LEP_CID_VID_LUT_TRANSFER, LEP_I2C_COMMAND_TYPE_GET),
                       (uint16_t *) table, sizeof(LEP_VID_LUT_BUFFER) / 2,&(((struct lepton_driver*)driver)->communication));
}

void vid_setFocusRegion(LEP_VID_FOCUS_ROI * region, void * driver)
{
  if (!region)
    return;
  ((struct lepton_driver*)driver)->communication.sendCommand_array(((struct lepton_driver*)driver)->communication.cmdCode(LEP_CID_VID_FOCUS_ROI, LEP_I2C_COMMAND_TYPE_SET),
                    (uint16_t *) region, sizeof(LEP_VID_FOCUS_ROI) / 2,&(((struct lepton_driver*)driver)->communication));
}

void vid_getFocusRegion(LEP_VID_FOCUS_ROI * region, void * driver)
{
  if (!region)
    return;
  ((struct lepton_driver*)driver)->communication.receiveCommand_array(((struct lepton_driver*)driver)->communication.cmdCode(LEP_CID_VID_FOCUS_ROI, LEP_I2C_COMMAND_TYPE_GET),
                       (uint16_t *) region, sizeof(LEP_VID_FOCUS_ROI) / 2,&(((struct lepton_driver*)driver)->communication));
}

void vid_setFocusThreshold(uint32_t threshold, void * driver)
{
  ((struct lepton_driver*)driver)->communication.sendCommand_u32(((struct lepton_driver*)driver)->communication.cmdCode
                  (LEP_CID_VID_FOCUS_THRESHOLD, LEP_I2C_COMMAND_TYPE_SET),
                  threshold,&(((struct lepton_driver*)driver)->communication));
}

uint32_t vid_getFocusThreshold(void * driver)
{
  uint32_t threshold;
  ((struct lepton_driver*)driver)->communication.receiveCommand_u32(((struct lepton_driver*)driver)->communication.cmdCode
                     (LEP_CID_VID_FOCUS_THRESHOLD, LEP_I2C_COMMAND_TYPE_GET),
                     &threshold,&(((struct lepton_driver*)driver)->communication));
  return threshold;
}

uint32_t vid_getFocusMetric(void * driver)
{
  uint32_t metric;
  ((struct lepton_driver*)driver)->communication.receiveCommand_u32(((struct lepton_driver*)driver)->communication.cmdCode
                     (LEP_CID_VID_FOCUS_METRIC, LEP_I2C_COMMAND_TYPE_GET),
                     &metric,&(((struct lepton_driver*)driver)->communication));
  return metric;
}

void vid_setSceneBasedNUCEnabled(uint8_t enabled, void * driver)
{
  ((struct lepton_driver*)driver)->communication.sendCommand_u32(((struct lepton_driver*)driver)->communication.cmdCode(LEP_CID_VID_SBNUC_ENABLE, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) enabled,&(((struct lepton_driver*)driver)->communication));
}

uint8_t vid_getSceneBasedNUCEnabled(void * driver)
{
  uint32_t enabled;
  ((struct lepton_driver*)driver)->communication.receiveCommand_u32(((struct lepton_driver*)driver)->communication.cmdCode
                     (LEP_CID_VID_SBNUC_ENABLE, LEP_I2C_COMMAND_TYPE_GET),
                     &enabled,&(((struct lepton_driver*)driver)->communication));
  return enabled;
}

void vid_setGamma(uint32_t gamma, void * driver)
{
  ((struct lepton_driver*)driver)->communication.sendCommand_u32(((struct lepton_driver*)driver)->communication.cmdCode(LEP_CID_VID_GAMMA_SELECT, LEP_I2C_COMMAND_TYPE_SET),
                  gamma,&(((struct lepton_driver*)driver)->communication));
}

uint32_t vid_getGamma(void * driver)
{
  uint32_t gamma;
  ((struct lepton_driver*)driver)->communication.receiveCommand_u32(((struct lepton_driver*)driver)->communication.cmdCode
                     (LEP_CID_VID_GAMMA_SELECT, LEP_I2C_COMMAND_TYPE_GET),
                     &gamma,&(((struct lepton_driver*)driver)->communication));
  return gamma;
}

#endif

void lepton_vid_init(struct lepton_vid * this)
{
  this->vid_setPolarity = &vid_setPolarity;
  this->vid_getPolarity = &vid_getPolarity;
  this->vid_setPseudoColorLUT = &vid_setPseudoColorLUT;
  this->vid_getPseudoColorLUT = &vid_getPseudoColorLUT;
  this->vid_setFocusCalcEnabled = &vid_setFocusCalcEnabled;
  this->vid_getFocusCalcEnabled = &vid_getFocusCalcEnabled;
  this->vid_setFreezeEnabled = &vid_setFreezeEnabled;
  this->vid_getFreezeEnabled = &vid_getFreezeEnabled;
#ifndef LEPFLIR_EXCLUDE_EXT_I2C_FUNCS
  this->vid_setUserColorLUT = &vid_setUserColorLUT;
  this->vid_getUserColorLUT = &vid_getUserColorLUT;
  this->vid_setFocusRegion = &vid_setFocusRegion;
  this->vid_getFocusRegion = &vid_getFocusRegion;
  this->vid_setFocusThreshold = &vid_setFocusThreshold;
  this->vid_getFocusThreshold = &vid_getFocusThreshold;
  this->vid_getFocusMetric = &vid_getFocusMetric;
  this->vid_setSceneBasedNUCEnabled = &vid_setSceneBasedNUCEnabled;
  this->vid_getSceneBasedNUCEnabled = &vid_getSceneBasedNUCEnabled;
  this->vid_setGamma = &vid_setGamma;
  this->vid_getGamma = &vid_getGamma;
#endif
}
