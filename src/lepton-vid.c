#include "lepton-vid.h"

void vid_setPolarity(LEP_VID_POLARITY polarity, struct lepton_communication * communication)
{
  communication->sendCommand_u32(communication->cmdCode
                  (LEP_CID_VID_POLARITY_SELECT, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) polarity,communication);
}

LEP_VID_POLARITY vid_getPolarity(struct lepton_communication * communication)
{
  uint32_t polarity;
  communication->receiveCommand_u32(communication->cmdCode
                     (LEP_CID_VID_POLARITY_SELECT, LEP_I2C_COMMAND_TYPE_GET),
                     &polarity,communication);
  return (LEP_VID_POLARITY) polarity;
}

void vid_setPseudoColorLUT(LEP_VID_PCOLOR_LUT table, struct lepton_communication * communication)
{
  communication->sendCommand_u32(communication->cmdCode(LEP_CID_VID_LUT_SELECT, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) table,communication);
}

LEP_VID_PCOLOR_LUT vid_getPseudoColorLUT(struct lepton_communication * communication)
{
  uint32_t table;
  communication->receiveCommand_u32(communication->cmdCode(LEP_CID_VID_LUT_SELECT, LEP_I2C_COMMAND_TYPE_GET),
                     &table,communication);
  return (LEP_VID_PCOLOR_LUT) table;
}

void vid_setFocusCalcEnabled(uint8_t enabled, struct lepton_communication * communication)
{
  communication->sendCommand_u32(communication->cmdCode
                  (LEP_CID_VID_FOCUS_CALC_ENABLE, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) enabled,communication);
}

uint8_t vid_getFocusCalcEnabled(struct lepton_communication * communication)
{
  uint32_t enabled;
  communication->receiveCommand_u32(communication->cmdCode
                     (LEP_CID_VID_FOCUS_CALC_ENABLE, LEP_I2C_COMMAND_TYPE_GET),
                     &enabled,communication);
  return enabled;
}

void vid_setFreezeEnabled(uint8_t enabled, struct lepton_communication * communication)
{
  communication->sendCommand_u32(communication->cmdCode(LEP_CID_VID_FREEZE_ENABLE, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) enabled,communication);
}

uint8_t vid_getFreezeEnabled(struct lepton_communication * communication)
{
  uint32_t enabled;
  communication->receiveCommand_u32(communication->cmdCode
                     (LEP_CID_VID_FREEZE_ENABLE, LEP_I2C_COMMAND_TYPE_GET),
                     &enabled,communication);
  return enabled;
}

#ifndef LEPFLIR_EXCLUDE_EXT_I2C_FUNCS

void vid_setUserColorLUT(LEP_VID_LUT_BUFFER * table, struct lepton_communication * communication)
{
  if (!table)
    return;
  communication->sendCommand_array(communication->cmdCode(LEP_CID_VID_LUT_TRANSFER, LEP_I2C_COMMAND_TYPE_SET),
                    (uint16_t *) table, sizeof(LEP_VID_LUT_BUFFER) / 2,communication);
}

void vid_getUserColorLUT(LEP_VID_LUT_BUFFER * table, struct lepton_communication * communication)
{
  if (!table)
    return;
  communication->receiveCommand_array(communication->cmdCode
                       (LEP_CID_VID_LUT_TRANSFER, LEP_I2C_COMMAND_TYPE_GET),
                       (uint16_t *) table, sizeof(LEP_VID_LUT_BUFFER) / 2,communication);
}

void vid_setFocusRegion(LEP_VID_FOCUS_ROI * region, struct lepton_communication * communication)
{
  if (!region)
    return;
  communication->sendCommand_array(communication->cmdCode(LEP_CID_VID_FOCUS_ROI, LEP_I2C_COMMAND_TYPE_SET),
                    (uint16_t *) region, sizeof(LEP_VID_FOCUS_ROI) / 2,communication);
}

void vid_getFocusRegion(LEP_VID_FOCUS_ROI * region, struct lepton_communication * communication)
{
  if (!region)
    return;
  communication->receiveCommand_array(communication->cmdCode(LEP_CID_VID_FOCUS_ROI, LEP_I2C_COMMAND_TYPE_GET),
                       (uint16_t *) region, sizeof(LEP_VID_FOCUS_ROI) / 2,communication);
}

void vid_setFocusThreshold(uint32_t threshold, struct lepton_communication * communication)
{
  communication->sendCommand_u32(communication->cmdCode
                  (LEP_CID_VID_FOCUS_THRESHOLD, LEP_I2C_COMMAND_TYPE_SET),
                  threshold,communication);
}

uint32_t vid_getFocusThreshold(struct lepton_communication * communication)
{
  uint32_t threshold;
  communication->receiveCommand_u32(communication->cmdCode
                     (LEP_CID_VID_FOCUS_THRESHOLD, LEP_I2C_COMMAND_TYPE_GET),
                     &threshold,communication);
  return threshold;
}

uint32_t vid_getFocusMetric(struct lepton_communication * communication)
{
  uint32_t metric;
  communication->receiveCommand_u32(communication->cmdCode
                     (LEP_CID_VID_FOCUS_METRIC, LEP_I2C_COMMAND_TYPE_GET),
                     &metric,communication);
  return metric;
}

void vid_setSceneBasedNUCEnabled(uint8_t enabled, struct lepton_communication * communication)
{
  communication->sendCommand_u32(communication->cmdCode(LEP_CID_VID_SBNUC_ENABLE, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) enabled,communication);
}

uint8_t vid_getSceneBasedNUCEnabled(struct lepton_communication * communication)
{
  uint32_t enabled;
  communication->receiveCommand_u32(communication->cmdCode
                     (LEP_CID_VID_SBNUC_ENABLE, LEP_I2C_COMMAND_TYPE_GET),
                     &enabled,communication);
  return enabled;
}

void vid_setGamma(uint32_t gamma, struct lepton_communication * communication)
{
  communication->sendCommand_u32(communication->cmdCode(LEP_CID_VID_GAMMA_SELECT, LEP_I2C_COMMAND_TYPE_SET),
                  gamma,communication);
}

uint32_t vid_getGamma(struct lepton_communication * communication)
{
  uint32_t gamma;
  communication->receiveCommand_u32(communication->cmdCode
                     (LEP_CID_VID_GAMMA_SELECT, LEP_I2C_COMMAND_TYPE_GET),
                     &gamma,communication);
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
