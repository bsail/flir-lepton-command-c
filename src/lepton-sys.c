#define LEPTON_FLIR_INTERNAL
#include "lepton-sys.h"
#include "lepton-flir.h"
#include "lepton-communication.h"
#ifdef TEST
#include <stdio.h>
#endif


#ifndef LEPFLIR_EXCLUDE_SYS_FUNCS

static void getCameraStatus_internal(LEP_SYS_CAM_STATUS * status, struct lepton_driver * driver)
{
  receiveCommand_array(cmdCode(LEP_CID_SYS_CAM_STATUS, LEP_I2C_COMMAND_TYPE_GET),
                       (uint16_t *) (status), sizeof(LEP_SYS_CAM_STATUS) / 2,&(driver->communication));
}

LEP_SYS_CAM_STATUS_STATES getCameraStatus(struct lepton_driver * driver)
{
  LEP_SYS_CAM_STATUS camStatus;
  getCameraStatus_internal(&camStatus,driver);
  return (LEP_SYS_CAM_STATUS_STATES) camStatus.camStatus;
}

void getFlirSerialNumber(char *buffer, int maxLength, struct lepton_driver * driver)
{
  if (!buffer || maxLength < 16)
    return;
  uint16_t innerBuffer[4];
  receiveCommand_array(cmdCode
                       (LEP_CID_SYS_FLIR_SERIAL_NUMBER,
                        LEP_I2C_COMMAND_TYPE_GET), innerBuffer, 4,&(driver->communication));
#ifndef LEPFLIR_EXCLUDE_MISC_FUNCS
  driver->misc.wordsToHexString(innerBuffer, 4, buffer, maxLength);
#endif
}

void getCustomerSerialNumber(char *buffer, int maxLength, struct lepton_driver * driver)
{
  if (!buffer || maxLength < 64)
    return;
  uint16_t innerBuffer[16];
  receiveCommand_array(cmdCode
                       (LEP_CID_SYS_CUST_SERIAL_NUMBER,
                        LEP_I2C_COMMAND_TYPE_GET), innerBuffer, 16,&(driver->communication));
#ifndef LEPFLIR_EXCLUDE_MISC_FUNCS
  driver->misc.wordsToHexString(innerBuffer, 16, buffer, maxLength);
#endif
}

uint32_t getCameraUptime(struct lepton_driver * driver)
{
  uint32_t uptime = 0;
  receiveCommand_u32(cmdCode(LEP_CID_SYS_CAM_UPTIME, LEP_I2C_COMMAND_TYPE_GET),
                     &uptime,&(driver->communication));
  return uptime;
}

float getAuxTemperature(struct lepton_driver * driver)
{
  uint16_t kelvin100;
  receiveCommand_u16(cmdCode
                     (LEP_CID_SYS_AUX_TEMPERATURE_KELVIN,
                      LEP_I2C_COMMAND_TYPE_GET), &kelvin100,&(driver->communication));
#ifndef LEPFLIR_EXCLUDE_MISC_FUNCS
  return driver->misc.kelvin100ToTemperature(kelvin100,driver);
#else
  return kelvin100*1.0;
#endif
}

float getFPATemperature(struct lepton_driver * driver)
{
  uint16_t kelvin100;
  receiveCommand_u16(cmdCode
                     (LEP_CID_SYS_FPA_TEMPERATURE_KELVIN,
                      LEP_I2C_COMMAND_TYPE_GET), &kelvin100,&(driver->communication));
#ifndef LEPFLIR_EXCLUDE_MISC_FUNCS
  return driver->misc.kelvin100ToTemperature(kelvin100,driver);
#else
  return kelvin100*1.0;
#endif
}

void setTelemetryEnabled(uint8_t enabled, struct lepton_driver * driver)
{
  sendCommand_u32(cmdCode
                  (LEP_CID_SYS_TELEMETRY_ENABLE_STATE,
                   LEP_I2C_COMMAND_TYPE_SET), (uint32_t) enabled,&(driver->communication));
}

uint8_t getTelemetryEnabled(struct lepton_driver * driver)
{
  uint32_t enabled;
  receiveCommand_u32(cmdCode
                     (LEP_CID_SYS_TELEMETRY_ENABLE_STATE,
                      LEP_I2C_COMMAND_TYPE_GET), &enabled,&(driver->communication));
  return enabled;
}

void runFFCNormalization(struct lepton_driver * driver)
{
  sendCommand_raw(cmdCode(LEP_CID_SYS_RUN_FFC, LEP_I2C_COMMAND_TYPE_RUN),&(driver->communication));
}

#ifndef LEPFLIR_EXCLUDE_EXT_I2C_FUNCS

void runPingCamera(struct lepton_driver * driver)
{
  sendCommand_raw(cmdCode(LEP_CID_SYS_PING, LEP_I2C_COMMAND_TYPE_RUN),&(driver->communication));
}

void setTelemetryLocation(LEP_SYS_TELEMETRY_LOCATION location, struct lepton_driver * driver)
{
  sendCommand_u32(cmdCode
                  (LEP_CID_SYS_TELEMETRY_LOCATION, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) location,&(driver->communication));
}

LEP_SYS_TELEMETRY_LOCATION getTelemetryLocation(struct lepton_driver * driver)
{
  uint32_t location;
  receiveCommand_u32(cmdCode
                     (LEP_CID_SYS_TELEMETRY_LOCATION, LEP_I2C_COMMAND_TYPE_GET),
                     &location,&(driver->communication));
  return (LEP_SYS_TELEMETRY_LOCATION) location;
}

void runFrameAveraging(struct lepton_driver * driver)
{
  sendCommand_raw(cmdCode
                  (LEP_CID_SYS_EXECTUE_FRAME_AVERAGE,
                   LEP_I2C_COMMAND_TYPE_RUN),&(driver->communication));
}

void setNumFramesToAverage(LEP_SYS_FRAME_AVERAGE average, struct lepton_driver * driver)
{
  sendCommand_u32(cmdCode
                  (LEP_CID_SYS_NUM_FRAMES_TO_AVERAGE, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) average,&(driver->communication));
}

LEP_SYS_FRAME_AVERAGE getNumFramesToAverage(struct lepton_driver * driver)
{
  uint32_t average;
  receiveCommand_u32(cmdCode
                     (LEP_CID_SYS_NUM_FRAMES_TO_AVERAGE,
                      LEP_I2C_COMMAND_TYPE_GET), &average,&(driver->communication));
  return (LEP_SYS_FRAME_AVERAGE) average;
}

void getSceneStatistics(LEP_SYS_SCENE_STATISTICS * statistics, struct lepton_driver * driver)
{
  if (!statistics)
    return;
  receiveCommand_array(cmdCode
                       (LEP_CID_SYS_SCENE_STATISTICS, LEP_I2C_COMMAND_TYPE_GET),
                       (uint16_t *) statistics,
                       sizeof(LEP_SYS_SCENE_STATISTICS) / 2,&(driver->communication));
}

void setSceneRegion(LEP_SYS_SCENE_ROI * region, struct lepton_driver * driver)
{
  if (!region)
    return;
  sendCommand_array(cmdCode(LEP_CID_SYS_SCENE_ROI, LEP_I2C_COMMAND_TYPE_SET),
                    (uint16_t *) region, sizeof(LEP_SYS_SCENE_ROI) / 2,&(driver->communication));
}

void getSceneRegion(LEP_SYS_SCENE_ROI * region, struct lepton_driver * driver)
{
  if (!region)
    return;
  receiveCommand_array(cmdCode(LEP_CID_SYS_SCENE_ROI, LEP_I2C_COMMAND_TYPE_GET),
                       (uint16_t *) region, sizeof(LEP_SYS_SCENE_ROI) / 2,&(driver->communication));
}

uint16_t getThermalShutdownCount(struct lepton_driver * driver)
{
  uint16_t count;
  receiveCommand_u16(cmdCode
                     (LEP_CID_SYS_THERMAL_SHUTDOWN_COUNT,
                      LEP_I2C_COMMAND_TYPE_GET), &count,&(driver->communication));
  return count;
}

void setShutterPosition(LEP_SYS_SHUTTER_POSITION position, struct lepton_driver * driver)
{
  sendCommand_u32(cmdCode
                  (LEP_CID_SYS_SHUTTER_POSITION, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) position,&(driver->communication));
}

LEP_SYS_SHUTTER_POSITION getShutterPosition(struct lepton_driver * driver)
{
  uint32_t position;
  receiveCommand_u32(cmdCode
                     (LEP_CID_SYS_SHUTTER_POSITION, LEP_I2C_COMMAND_TYPE_GET),
                     &position,&(driver->communication));
  return (LEP_SYS_SHUTTER_POSITION) position;
}

void setFFCShutterMode(LEP_SYS_FFC_SHUTTER_MODE * mode, struct lepton_driver * driver)
{
  if (!mode)
    return;
  sendCommand_array(cmdCode
                    (LEP_CID_SYS_FFC_SHUTTER_MODE, LEP_I2C_COMMAND_TYPE_SET),
                    (uint16_t *) mode, sizeof(LEP_SYS_FFC_SHUTTER_MODE) / 2,&(driver->communication));
}

void getFFCShutterMode(LEP_SYS_FFC_SHUTTER_MODE * mode, struct lepton_driver * driver)
{
  if (!mode)
    return;
  receiveCommand_array(cmdCode
                       (LEP_CID_SYS_FFC_SHUTTER_MODE, LEP_I2C_COMMAND_TYPE_GET),
                       (uint16_t *) mode, sizeof(LEP_SYS_FFC_SHUTTER_MODE) / 2,&(driver->communication));
}

LEP_SYS_FFC_STATUS getFFCNormalizationStatus(struct lepton_driver * driver)
{
  uint32_t status;
  receiveCommand_u32(cmdCode(LEP_CID_SYS_FFC_STATUS, LEP_I2C_COMMAND_TYPE_GET),
                     &status,&(driver->communication));
  return (LEP_SYS_FFC_STATUS) status;
}

#endif

void lepton_sys_init(struct lepton_sys * this)
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
