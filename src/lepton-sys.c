#define LEPTON_FLIR_INTERNAL
#include "lepton-sys.h"
#include "lepton-flir.h"
#include "lepton-communication.h"
#include "lepton-get-set.h"
#ifdef TEST
#include <stdio.h>
#endif

#ifndef LEPFLIR_EXCLUDE_SYS_FUNCS

LEP_SYS_CAM_STATUS_STATES getCameraStatus(struct lepton_driver *driver)
{
  LEP_SYS_CAM_STATUS_STATES ret = LEP_SYSTEM_COMMUNICATION_ERROR;
  if (driver) {
    LEP_SYS_CAM_STATUS camStatus;
    get_common(driver, array, LEP_CID_SYS_CAM_STATUS,
               (uint16_t *) & camStatus, sizeof(LEP_SYS_CAM_STATUS) / 2);
    ret = (LEP_SYS_CAM_STATUS_STATES) camStatus.camStatus;
  }
  return ret;
}

void getFlirSerialNumber(struct lepton_driver *driver, char *buffer,
                         int maxLength)
{
  if (driver) {
    if (!buffer || maxLength < 16)
      return;
    uint16_t innerBuffer[4];
    get_common(driver, array, LEP_CID_SYS_FLIR_SERIAL_NUMBER,
               innerBuffer, 4);
#ifndef LEPFLIR_EXCLUDE_MISC_FUNCS
    driver->misc.wordsToHexString(innerBuffer, 4, buffer, maxLength);
#endif
  }
}

void getCustomerSerialNumber(struct lepton_driver *driver, char *buffer,
                             int maxLength)
{
  if (driver) {
    if (!buffer || maxLength < 64)
      return;
    uint16_t innerBuffer[16];
    get_common(driver, array, LEP_CID_SYS_CUST_SERIAL_NUMBER,
                innerBuffer, 16);
#ifndef LEPFLIR_EXCLUDE_MISC_FUNCS
    driver->misc.wordsToHexString(innerBuffer, 16, buffer, maxLength);
#endif
  }
}

uint32_t getCameraUptime(struct lepton_driver * driver)
{
  uint32_t uptime = 0;
  if (driver) {
    get_common(driver, u32, LEP_CID_SYS_CAM_UPTIME, 
               (uint16_t *) & uptime, 4);
  }
  return uptime;
}

float getAuxTemperature(struct lepton_driver *driver)
{
  uint16_t kelvin100 = 0;
  if (driver) {
    get_common(driver, u16, LEP_CID_SYS_AUX_TEMPERATURE_KELVIN, 
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
  if (driver) {
    get_common(driver, u16, LEP_CID_SYS_FPA_TEMPERATURE_KELVIN, 
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
  if (driver) {
    #ifdef TEST
    // printf("enabled:%d\n",enabled);
    #endif
    uint32_t en = enabled;
    set_common(driver, u32, LEP_CID_SYS_TELEMETRY_ENABLE_STATE,
               (uint16_t *) & en, 4);
  }
}

uint8_t getTelemetryEnabled(struct lepton_driver *driver)
{
  uint32_t enabled;
  if (driver) {
    get_common(driver, u32, LEP_CID_SYS_TELEMETRY_ENABLE_STATE, 
               (uint16_t *) & enabled, 4);
  }
  return enabled;
}

void runFFCNormalization(struct lepton_driver *driver)
{
  if (driver) {
    sendCommand_raw(&(driver->communication),
                    cmdCode(LEP_CID_SYS_RUN_FFC, LEP_I2C_COMMAND_TYPE_RUN));
  }
}

#ifndef LEPFLIR_EXCLUDE_EXT_I2C_FUNCS

void runPingCamera(struct lepton_driver *driver)
{
  if (driver) {
    sendCommand_raw(&(driver->communication),
                    cmdCode(LEP_CID_SYS_PING, LEP_I2C_COMMAND_TYPE_RUN));
  }
}

void setTelemetryLocation(struct lepton_driver *driver,
                          LEP_SYS_TELEMETRY_LOCATION location)
{
  if (driver) {
    set_common(driver, u32, LEP_CID_SYS_TELEMETRY_LOCATION,
               (uint16_t *) & location, 4);
  }
}

LEP_SYS_TELEMETRY_LOCATION getTelemetryLocation(struct lepton_driver *driver)
{
  uint32_t location;
  if (driver) {
    get_common(driver, u32, LEP_CID_SYS_TELEMETRY_LOCATION, 
               (uint16_t *) & location, 4);
  }
  return (LEP_SYS_TELEMETRY_LOCATION) location;
}

void runFrameAveraging(struct lepton_driver *driver)
{
  if (driver) {
    sendCommand_raw(&(driver->communication),
                    cmdCode(LEP_CID_SYS_EXECTUE_FRAME_AVERAGE,
                            LEP_I2C_COMMAND_TYPE_RUN));
  }
}

void setNumFramesToAverage(struct lepton_driver *driver,
                           LEP_SYS_FRAME_AVERAGE average)
{
  if (driver) {
    set_common(driver, u32, LEP_CID_SYS_NUM_FRAMES_TO_AVERAGE,
               (uint16_t *) & average, 4);
  }
}

LEP_SYS_FRAME_AVERAGE getNumFramesToAverage(struct lepton_driver *driver)
{
  uint32_t average;
  if (driver) {
    get_common(driver, u32, LEP_CID_SYS_NUM_FRAMES_TO_AVERAGE, 
               (uint16_t *) & average, 4);
  }
  return (LEP_SYS_FRAME_AVERAGE) average;
}

void getSceneStatistics(struct lepton_driver *driver,
                        LEP_SYS_SCENE_STATISTICS * statistics)
{
  if (driver) {
    if (!statistics)
      return;
    get_common(driver, array, LEP_CID_SYS_SCENE_STATISTICS,
                (uint16_t *) statistics,
                         sizeof(LEP_SYS_SCENE_STATISTICS) / 2);
  }
}

void setSceneRegion(struct lepton_driver *driver, LEP_SYS_SCENE_ROI * region)
{
  if (driver) {
    if (!region)
      return;
    set_common(driver, array, LEP_CID_SYS_SCENE_ROI,
               (uint16_t *) region, sizeof(LEP_SYS_SCENE_ROI) / 2);
  }
}

void getSceneRegion(struct lepton_driver *driver, LEP_SYS_SCENE_ROI * region)
{
  if (driver) {
    if (!region)
      return;
    get_common(driver, array, LEP_CID_SYS_SCENE_ROI,
                (uint16_t *) region,
                         sizeof(LEP_SYS_SCENE_ROI) / 2);
  }
}

uint16_t getThermalShutdownCount(struct lepton_driver *driver)
{
  uint16_t count;
  if (driver) {
    get_common(driver, u16, LEP_CID_SYS_THERMAL_SHUTDOWN_COUNT, 
               &count, 2);
  }
  return count;
}

void setShutterPosition(struct lepton_driver *driver,
                        LEP_SYS_SHUTTER_POSITION position)
{
  if (driver) {
    set_common(driver, u32, LEP_CID_SYS_SHUTTER_POSITION,
               (uint16_t *) & position, 4);
  }
}

LEP_SYS_SHUTTER_POSITION getShutterPosition(struct lepton_driver *driver)
{
  uint32_t position;
  if (driver) {
    get_common(driver, u32, LEP_CID_SYS_SHUTTER_POSITION, 
               (uint16_t *) & position, 4);
  }
  return (LEP_SYS_SHUTTER_POSITION) position;
}

void setFFCShutterMode(struct lepton_driver *driver,
                       LEP_SYS_FFC_SHUTTER_MODE * mode)
{
  if (driver) {
    if (!mode)
      return;
    set_common(driver, array, LEP_CID_SYS_FFC_SHUTTER_MODE,
               (uint16_t *) mode, sizeof(LEP_SYS_FFC_SHUTTER_MODE) / 2);
  }
}

void getFFCShutterMode(struct lepton_driver *driver,
                       LEP_SYS_FFC_SHUTTER_MODE * mode)
{
  if (driver) {
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
  if (driver) {
    get_common(driver, u32, LEP_CID_SYS_FFC_STATUS, 
               (uint16_t *) & status, 4);
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
