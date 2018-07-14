#define LEPTON_FLIR_INTERNAL
#include "lepton-sys.h"
#include "lepton-flir.h"
#include "lepton-communication.h"
#ifdef TEST
#include <stdio.h>
#endif

#ifndef LEPFLIR_EXCLUDE_SYS_FUNCS

static inline void get_common(struct lepton_driver *driver,
                              enum transmission_mode mode, uint16_t cmdID,
                              uint16_t cmdType, uint16_t * data,
                              uint16_t length)
{
  switch (mode) {
  case u16:
    receiveCommand_u16(&(driver->communication), cmdCode(cmdID, cmdType), data);
    break;
  case u32:
    receiveCommand_u32(&(driver->communication), cmdCode(cmdID, cmdType),
                       (uint32_t *) data);
    break;
  case array:
    receiveCommand_array(&(driver->communication), cmdCode(cmdID, cmdType),
                         (uint16_t *) data, length);
    break;
  }

}

LEP_SYS_CAM_STATUS_STATES getCameraStatus(struct lepton_driver *driver)
{
  LEP_SYS_CAM_STATUS_STATES ret = LEP_SYSTEM_COMMUNICATION_ERROR;
  if (driver != 0) {
    LEP_SYS_CAM_STATUS camStatus;
    get_common(driver, array, LEP_CID_SYS_CAM_STATUS, LEP_I2C_COMMAND_TYPE_GET,
               (uint16_t *) & camStatus, sizeof(LEP_SYS_CAM_STATUS) / 2);
    ret = (LEP_SYS_CAM_STATUS_STATES) camStatus.camStatus;
  }
  return ret;
}

void getFlirSerialNumber(struct lepton_driver *driver, char *buffer,
                         int maxLength)
{
  if (driver != 0) {
    if (!buffer || maxLength < 16)
      return;
    uint16_t innerBuffer[4];
    get_common(driver, array, LEP_CID_SYS_FLIR_SERIAL_NUMBER,
               LEP_I2C_COMMAND_TYPE_GET, innerBuffer, 4);
#ifndef LEPFLIR_EXCLUDE_MISC_FUNCS
    driver->misc.wordsToHexString(innerBuffer, 4, buffer, maxLength);
#endif
  }
}

void getCustomerSerialNumber(struct lepton_driver *driver, char *buffer,
                             int maxLength)
{
  if (driver != 0) {
    if (!buffer || maxLength < 64)
      return;
    uint16_t innerBuffer[16];
    get_common(driver, array, LEP_CID_SYS_CUST_SERIAL_NUMBER,
               LEP_I2C_COMMAND_TYPE_GET, innerBuffer, 16);
#ifndef LEPFLIR_EXCLUDE_MISC_FUNCS
    driver->misc.wordsToHexString(innerBuffer, 16, buffer, maxLength);
#endif
  }
}

uint32_t getCameraUptime(struct lepton_driver * driver)
{
  uint32_t uptime = 0;
  if (driver != 0) {
    get_common(driver, u32, LEP_CID_SYS_CAM_UPTIME, LEP_I2C_COMMAND_TYPE_GET,
               (uint16_t *) & uptime, 4);
  }
  return uptime;
}

float getAuxTemperature(struct lepton_driver *driver)
{
  uint16_t kelvin100 = 0;
  if (driver != 0) {
    get_common(driver, u16, LEP_CID_SYS_AUX_TEMPERATURE_KELVIN, LEP_I2C_COMMAND_TYPE_GET,
               (uint16_t *) & kelvin100, 2);
#ifndef LEPFLIR_EXCLUDE_MISC_FUNCS
    return driver->misc.kelvin100ToTemperature(driver, kelvin100);
#else
    return kelvin100 * 1.0;
#endif
  } else
    return 0.0;
}

float getFPATemperature(struct lepton_driver *driver)
{
  uint16_t kelvin100 = 0;
  if (driver != 0) {
    get_common(driver, u16, LEP_CID_SYS_FPA_TEMPERATURE_KELVIN, LEP_I2C_COMMAND_TYPE_GET,
               (uint16_t *) & kelvin100, 2);
#ifndef LEPFLIR_EXCLUDE_MISC_FUNCS
    return driver->misc.kelvin100ToTemperature(driver, kelvin100);
#else
    return kelvin100 * 1.0;
#endif
  } else
    return 0.0;
}

void setTelemetryEnabled(struct lepton_driver *driver, uint8_t enabled)
{
  if (driver != 0) {
    sendCommand_u32(&(driver->communication),
                    cmdCode(LEP_CID_SYS_TELEMETRY_ENABLE_STATE,
                            LEP_I2C_COMMAND_TYPE_SET), (uint32_t) enabled);
  }
}

uint8_t getTelemetryEnabled(struct lepton_driver *driver)
{
  uint32_t enabled;
  if (driver != 0) {
    receiveCommand_u32(&(driver->communication),
                       cmdCode(LEP_CID_SYS_TELEMETRY_ENABLE_STATE,
                               LEP_I2C_COMMAND_TYPE_GET), &enabled);
  }
  return enabled;
}

void runFFCNormalization(struct lepton_driver *driver)
{
  if (driver != 0) {
    sendCommand_raw(&(driver->communication),
                    cmdCode(LEP_CID_SYS_RUN_FFC, LEP_I2C_COMMAND_TYPE_RUN));
  }
}

#ifndef LEPFLIR_EXCLUDE_EXT_I2C_FUNCS

void runPingCamera(struct lepton_driver *driver)
{
  if (driver != 0) {
    sendCommand_raw(&(driver->communication),
                    cmdCode(LEP_CID_SYS_PING, LEP_I2C_COMMAND_TYPE_RUN));
  }
}

void setTelemetryLocation(struct lepton_driver *driver,
                          LEP_SYS_TELEMETRY_LOCATION location)
{
  if (driver != 0) {
    sendCommand_u32(&(driver->communication),
                    cmdCode(LEP_CID_SYS_TELEMETRY_LOCATION,
                            LEP_I2C_COMMAND_TYPE_SET), (uint32_t) location);
  }
}

LEP_SYS_TELEMETRY_LOCATION getTelemetryLocation(struct lepton_driver *driver)
{
  uint32_t location;
  if (driver != 0) {
    receiveCommand_u32(&(driver->communication),
                       cmdCode(LEP_CID_SYS_TELEMETRY_LOCATION,
                               LEP_I2C_COMMAND_TYPE_GET), &location);
  }
  return (LEP_SYS_TELEMETRY_LOCATION) location;
}

void runFrameAveraging(struct lepton_driver *driver)
{
  if (driver != 0) {
    sendCommand_raw(&(driver->communication),
                    cmdCode(LEP_CID_SYS_EXECTUE_FRAME_AVERAGE,
                            LEP_I2C_COMMAND_TYPE_RUN));
  }
}

void setNumFramesToAverage(struct lepton_driver *driver,
                           LEP_SYS_FRAME_AVERAGE average)
{
  if (driver != 0) {
    sendCommand_u32(&(driver->communication),
                    cmdCode(LEP_CID_SYS_NUM_FRAMES_TO_AVERAGE,
                            LEP_I2C_COMMAND_TYPE_SET), (uint32_t) average);
  }
}

LEP_SYS_FRAME_AVERAGE getNumFramesToAverage(struct lepton_driver *driver)
{
  uint32_t average;
  if (driver != 0) {
    receiveCommand_u32(&(driver->communication),
                       cmdCode(LEP_CID_SYS_NUM_FRAMES_TO_AVERAGE,
                               LEP_I2C_COMMAND_TYPE_GET), &average);
  }
  return (LEP_SYS_FRAME_AVERAGE) average;
}

void getSceneStatistics(struct lepton_driver *driver,
                        LEP_SYS_SCENE_STATISTICS * statistics)
{
  if (driver != 0) {
    if (!statistics)
      return;
    receiveCommand_array(&(driver->communication),
                         cmdCode(LEP_CID_SYS_SCENE_STATISTICS,
                                 LEP_I2C_COMMAND_TYPE_GET),
                         (uint16_t *) statistics,
                         sizeof(LEP_SYS_SCENE_STATISTICS) / 2);
  }
}

void setSceneRegion(struct lepton_driver *driver, LEP_SYS_SCENE_ROI * region)
{
  if (driver != 0) {
    if (!region)
      return;
    sendCommand_array(&(driver->communication),
                      cmdCode(LEP_CID_SYS_SCENE_ROI, LEP_I2C_COMMAND_TYPE_SET),
                      (uint16_t *) region, sizeof(LEP_SYS_SCENE_ROI) / 2);
  }
}

void getSceneRegion(struct lepton_driver *driver, LEP_SYS_SCENE_ROI * region)
{
  if (driver != 0) {
    if (!region)
      return;
    receiveCommand_array(&(driver->communication),
                         cmdCode(LEP_CID_SYS_SCENE_ROI,
                                 LEP_I2C_COMMAND_TYPE_GET), (uint16_t *) region,
                         sizeof(LEP_SYS_SCENE_ROI) / 2);
  }
}

uint16_t getThermalShutdownCount(struct lepton_driver *driver)
{
  uint16_t count;
  if (driver != 0) {
    receiveCommand_u16(&(driver->communication),
                       cmdCode(LEP_CID_SYS_THERMAL_SHUTDOWN_COUNT,
                               LEP_I2C_COMMAND_TYPE_GET), &count);
  }
  return count;
}

void setShutterPosition(struct lepton_driver *driver,
                        LEP_SYS_SHUTTER_POSITION position)
{
  if (driver != 0) {
    sendCommand_u32(&(driver->communication),
                    cmdCode(LEP_CID_SYS_SHUTTER_POSITION,
                            LEP_I2C_COMMAND_TYPE_SET), (uint32_t) position);
  }
}

LEP_SYS_SHUTTER_POSITION getShutterPosition(struct lepton_driver *driver)
{
  uint32_t position;
  if (driver != 0) {
    receiveCommand_u32(&(driver->communication),
                       cmdCode(LEP_CID_SYS_SHUTTER_POSITION,
                               LEP_I2C_COMMAND_TYPE_GET), &position);
  }
  return (LEP_SYS_SHUTTER_POSITION) position;
}

void setFFCShutterMode(struct lepton_driver *driver,
                       LEP_SYS_FFC_SHUTTER_MODE * mode)
{
  if (driver != 0) {
    if (!mode)
      return;
    sendCommand_array(&(driver->communication),
                      cmdCode(LEP_CID_SYS_FFC_SHUTTER_MODE,
                              LEP_I2C_COMMAND_TYPE_SET), (uint16_t *) mode,
                      sizeof(LEP_SYS_FFC_SHUTTER_MODE) / 2);
  }
}

void getFFCShutterMode(struct lepton_driver *driver,
                       LEP_SYS_FFC_SHUTTER_MODE * mode)
{
  if (driver != 0) {
    if (!mode)
      return;
    receiveCommand_array(&(driver->communication),
                         cmdCode(LEP_CID_SYS_FFC_SHUTTER_MODE,
                                 LEP_I2C_COMMAND_TYPE_GET), (uint16_t *) mode,
                         sizeof(LEP_SYS_FFC_SHUTTER_MODE) / 2);
  }
}

LEP_SYS_FFC_STATUS getFFCNormalizationStatus(struct lepton_driver *driver)
{
  uint32_t status;
  if (driver != 0) {
    receiveCommand_u32(&(driver->communication),
                       cmdCode(LEP_CID_SYS_FFC_STATUS,
                               LEP_I2C_COMMAND_TYPE_GET), &status);
  }
  return (LEP_SYS_FFC_STATUS) status;
}

#endif

void lepton_sys_init(struct lepton_sys *this)
{
  this->getCameraStatus = &getCameraStatus;
  this->getFlirSerialNumber = &getFlirSerialNumber;
  this->getCustomerSerialNumber = &getCustomerSerialNumber;
  this->getCameraUptime = &getCameraUptime;
  this->getAuxTemperature = &getAuxTemperature;
  this->getFPATemperature = &getFPATemperature;
  this->setTelemetryEnabled = &setTelemetryEnabled;
  this->getTelemetryEnabled = &getTelemetryEnabled;
  this->runFFCNormalization = &runFFCNormalization;

#ifndef LEPFLIR_EXCLUDE_EXT_I2C_FUNCS
  this->runPingCamera = &runPingCamera;
  this->setTelemetryLocation = &setTelemetryLocation;
  this->getTelemetryLocation = &getTelemetryLocation;
  this->runFrameAveraging = &runFrameAveraging;
  this->setNumFramesToAverage = &setNumFramesToAverage;
  this->getNumFramesToAverage = &getNumFramesToAverage;
  this->getSceneStatistics = &getSceneStatistics;
  this->setSceneRegion = &setSceneRegion;
  this->getSceneRegion = &getSceneRegion;
  this->getThermalShutdownCount = &getThermalShutdownCount;
  this->setShutterPosition = &setShutterPosition;
  this->getShutterPosition = &getShutterPosition;
  this->setFFCShutterMode = &setFFCShutterMode;
  this->getFFCShutterMode = &getFFCShutterMode;
  this->getFFCNormalizationStatus = &getFFCNormalizationStatus;
#endif
}

#endif
