#include "lepton-sys.h"
#include "lepton-flir.h"
#include "lepton-communication.h"
#ifdef TEST
#include <stdio.h>
#endif

void sys_getCameraStatus_internal(LEP_SYS_CAM_STATUS * status, void * driver)
{
  ((struct lepton_driver*)driver)->communication.receiveCommand_array(((struct lepton_driver*)driver)->communication.cmdCode
                       (LEP_CID_SYS_CAM_STATUS, LEP_I2C_COMMAND_TYPE_GET),
                       (uint16_t *) (status), sizeof(LEP_SYS_CAM_STATUS) / 2,&(((struct lepton_driver*)driver)->communication));
}

LEP_SYS_CAM_STATUS_STATES sys_getCameraStatus(void * driver)
{
  LEP_SYS_CAM_STATUS camStatus;
  sys_getCameraStatus_internal(&camStatus,&(((struct lepton_driver*)driver)->communication));
  return (LEP_SYS_CAM_STATUS_STATES) camStatus.camStatus;
}

void sys_getFlirSerialNumber(char *buffer, int maxLength, void * driver)
{
  if (!buffer || maxLength < 16)
    return;
  uint16_t innerBuffer[4];
  ((struct lepton_driver*)driver)->communication.receiveCommand_array(((struct lepton_driver*)driver)->communication.cmdCode
                       (LEP_CID_SYS_FLIR_SERIAL_NUMBER,
                        LEP_I2C_COMMAND_TYPE_GET), innerBuffer, 4,&(((struct lepton_driver*)driver)->communication));
  wordsToHexString(innerBuffer, 4, buffer, maxLength);
}

void sys_getCustomerSerialNumber(char *buffer, int maxLength, void * driver)
{
  if (!buffer || maxLength < 64)
    return;
  uint16_t innerBuffer[16];
  ((struct lepton_driver*)driver)->communication.receiveCommand_array(((struct lepton_driver*)driver)->communication.cmdCode
                       (LEP_CID_SYS_CUST_SERIAL_NUMBER,
                        LEP_I2C_COMMAND_TYPE_GET), innerBuffer, 16,&(((struct lepton_driver*)driver)->communication));
  wordsToHexString(innerBuffer, 16, buffer, maxLength);
}

uint32_t sys_getCameraUptime(void * driver)
{
  uint32_t uptime = 0;
  ((struct lepton_driver*)driver)->communication.receiveCommand_u32(((struct lepton_driver*)driver)->communication.cmdCode(LEP_CID_SYS_CAM_UPTIME, LEP_I2C_COMMAND_TYPE_GET),
                     &uptime,&(((struct lepton_driver*)driver)->communication));
  return uptime;
}

float sys_getAuxTemperature(void * driver)
{
  uint16_t kelvin100;
  ((struct lepton_driver*)driver)->communication.receiveCommand_u16(((struct lepton_driver*)driver)->communication.cmdCode
                     (LEP_CID_SYS_AUX_TEMPERATURE_KELVIN,
                      LEP_I2C_COMMAND_TYPE_GET), &kelvin100,&(((struct lepton_driver*)driver)->communication));
  return kelvin100ToTemperature(kelvin100,driver);
}

float sys_getFPATemperature(void * driver)
{
  uint16_t kelvin100;
  ((struct lepton_driver*)driver)->communication.receiveCommand_u16(((struct lepton_driver*)driver)->communication.cmdCode
                     (LEP_CID_SYS_FPA_TEMPERATURE_KELVIN,
                      LEP_I2C_COMMAND_TYPE_GET), &kelvin100,&(((struct lepton_driver*)driver)->communication));
  return kelvin100ToTemperature(kelvin100,driver);
}

void sys_setTelemetryEnabled(uint8_t enabled, void * driver)
{
  ((struct lepton_driver*)driver)->communication.sendCommand_u32(((struct lepton_driver*)driver)->communication.cmdCode
                  (LEP_CID_SYS_TELEMETRY_ENABLE_STATE,
                   LEP_I2C_COMMAND_TYPE_SET), (uint32_t) enabled,&(((struct lepton_driver*)driver)->communication));
}

uint8_t sys_getTelemetryEnabled(void * driver)
{
  uint32_t enabled;
  ((struct lepton_driver*)driver)->communication.receiveCommand_u32(((struct lepton_driver*)driver)->communication.cmdCode
                     (LEP_CID_SYS_TELEMETRY_ENABLE_STATE,
                      LEP_I2C_COMMAND_TYPE_GET), &enabled,&(((struct lepton_driver*)driver)->communication));
  return enabled;
}

void sys_runFFCNormalization(void * driver)
{
  ((struct lepton_driver*)driver)->communication.sendCommand_raw(((struct lepton_driver*)driver)->communication.cmdCode(LEP_CID_SYS_RUN_FFC, LEP_I2C_COMMAND_TYPE_RUN),&(((struct lepton_driver*)driver)->communication));
}

#ifndef LEPFLIR_EXCLUDE_EXT_I2C_FUNCS

void sys_runPingCamera(void * driver)
{
  ((struct lepton_driver*)driver)->communication.sendCommand_raw(((struct lepton_driver*)driver)->communication.cmdCode(LEP_CID_SYS_PING, LEP_I2C_COMMAND_TYPE_RUN),&(((struct lepton_driver*)driver)->communication));
}

void sys_setTelemetryLocation(LEP_SYS_TELEMETRY_LOCATION location, void * driver)
{
  ((struct lepton_driver*)driver)->communication.sendCommand_u32(((struct lepton_driver*)driver)->communication.cmdCode
                  (LEP_CID_SYS_TELEMETRY_LOCATION, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) location,&(((struct lepton_driver*)driver)->communication));
}

LEP_SYS_TELEMETRY_LOCATION sys_getTelemetryLocation(void * driver)
{
  uint32_t location;
  ((struct lepton_driver*)driver)->communication.receiveCommand_u32(((struct lepton_driver*)driver)->communication.cmdCode
                     (LEP_CID_SYS_TELEMETRY_LOCATION, LEP_I2C_COMMAND_TYPE_GET),
                     &location,&(((struct lepton_driver*)driver)->communication));
  return (LEP_SYS_TELEMETRY_LOCATION) location;
}

void sys_runFrameAveraging(void * driver)
{
  ((struct lepton_driver*)driver)->communication.sendCommand_raw(((struct lepton_driver*)driver)->communication.cmdCode
                  (LEP_CID_SYS_EXECTUE_FRAME_AVERAGE,
                   LEP_I2C_COMMAND_TYPE_RUN),&(((struct lepton_driver*)driver)->communication));
}

void sys_setNumFramesToAverage(LEP_SYS_FRAME_AVERAGE average, void * driver)
{
  ((struct lepton_driver*)driver)->communication.sendCommand_u32(((struct lepton_driver*)driver)->communication.cmdCode
                  (LEP_CID_SYS_NUM_FRAMES_TO_AVERAGE, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) average,&(((struct lepton_driver*)driver)->communication));
}

LEP_SYS_FRAME_AVERAGE sys_getNumFramesToAverage(void * driver)
{
  uint32_t average;
  ((struct lepton_driver*)driver)->communication.receiveCommand_u32(((struct lepton_driver*)driver)->communication.cmdCode
                     (LEP_CID_SYS_NUM_FRAMES_TO_AVERAGE,
                      LEP_I2C_COMMAND_TYPE_GET), &average,&(((struct lepton_driver*)driver)->communication));
  return (LEP_SYS_FRAME_AVERAGE) average;
}

void sys_getSceneStatistics(LEP_SYS_SCENE_STATISTICS * statistics, void * driver)
{
  if (!statistics)
    return;
  ((struct lepton_driver*)driver)->communication.receiveCommand_array(((struct lepton_driver*)driver)->communication.cmdCode
                       (LEP_CID_SYS_SCENE_STATISTICS, LEP_I2C_COMMAND_TYPE_GET),
                       (uint16_t *) statistics,
                       sizeof(LEP_SYS_SCENE_STATISTICS) / 2,&(((struct lepton_driver*)driver)->communication));
}

void sys_setSceneRegion(LEP_SYS_SCENE_ROI * region, void * driver)
{
  if (!region)
    return;
  ((struct lepton_driver*)driver)->communication.sendCommand_array(((struct lepton_driver*)driver)->communication.cmdCode(LEP_CID_SYS_SCENE_ROI, LEP_I2C_COMMAND_TYPE_SET),
                    (uint16_t *) region, sizeof(LEP_SYS_SCENE_ROI) / 2,&(((struct lepton_driver*)driver)->communication));
}

void sys_getSceneRegion(LEP_SYS_SCENE_ROI * region, void * driver)
{
  if (!region)
    return;
  ((struct lepton_driver*)driver)->communication.receiveCommand_array(((struct lepton_driver*)driver)->communication.cmdCode(LEP_CID_SYS_SCENE_ROI, LEP_I2C_COMMAND_TYPE_GET),
                       (uint16_t *) region, sizeof(LEP_SYS_SCENE_ROI) / 2,&(((struct lepton_driver*)driver)->communication));
}

uint16_t sys_getThermalShutdownCount(void * driver)
{
  uint16_t count;
  ((struct lepton_driver*)driver)->communication.receiveCommand_u16(((struct lepton_driver*)driver)->communication.cmdCode
                     (LEP_CID_SYS_THERMAL_SHUTDOWN_COUNT,
                      LEP_I2C_COMMAND_TYPE_GET), &count,&(((struct lepton_driver*)driver)->communication));
  return count;
}

void sys_setShutterPosition(LEP_SYS_SHUTTER_POSITION position, void * driver)
{
  ((struct lepton_driver*)driver)->communication.sendCommand_u32(((struct lepton_driver*)driver)->communication.cmdCode
                  (LEP_CID_SYS_SHUTTER_POSITION, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) position,&(((struct lepton_driver*)driver)->communication));
}

LEP_SYS_SHUTTER_POSITION sys_getShutterPosition(void * driver)
{
  uint32_t position;
  ((struct lepton_driver*)driver)->communication.receiveCommand_u32(((struct lepton_driver*)driver)->communication.cmdCode
                     (LEP_CID_SYS_SHUTTER_POSITION, LEP_I2C_COMMAND_TYPE_GET),
                     &position,&(((struct lepton_driver*)driver)->communication));
  return (LEP_SYS_SHUTTER_POSITION) position;
}

void sys_setFFCShutterMode(LEP_SYS_FFC_SHUTTER_MODE * mode, void * driver)
{
  if (!mode)
    return;
  ((struct lepton_driver*)driver)->communication.sendCommand_array(((struct lepton_driver*)driver)->communication.cmdCode
                    (LEP_CID_SYS_FFC_SHUTTER_MODE, LEP_I2C_COMMAND_TYPE_SET),
                    (uint16_t *) mode, sizeof(LEP_SYS_FFC_SHUTTER_MODE) / 2,&(((struct lepton_driver*)driver)->communication));
}

void sys_getFFCShutterMode(LEP_SYS_FFC_SHUTTER_MODE * mode, void * driver)
{
  if (!mode)
    return;
  ((struct lepton_driver*)driver)->communication.receiveCommand_array(((struct lepton_driver*)driver)->communication.cmdCode
                       (LEP_CID_SYS_FFC_SHUTTER_MODE, LEP_I2C_COMMAND_TYPE_GET),
                       (uint16_t *) mode, sizeof(LEP_SYS_FFC_SHUTTER_MODE) / 2,&(((struct lepton_driver*)driver)->communication));
}

LEP_SYS_FFC_STATUS sys_getFFCNormalizationStatus(void * driver)
{
  uint32_t status;
  ((struct lepton_driver*)driver)->communication.receiveCommand_u32(((struct lepton_driver*)driver)->communication.cmdCode(LEP_CID_SYS_FFC_STATUS, LEP_I2C_COMMAND_TYPE_GET),
                     &status,&(((struct lepton_driver*)driver)->communication));
  return (LEP_SYS_FFC_STATUS) status;
}

#endif

void lepton_sys_init(struct lepton_sys * this)
{
  this->sys_getCameraStatus_internal = &sys_getCameraStatus_internal;
  this->sys_getCameraStatus = &sys_getCameraStatus;
  this->sys_getFlirSerialNumber = &sys_getFlirSerialNumber;
  this->sys_getCustomerSerialNumber = &sys_getCustomerSerialNumber;
  this->sys_getCameraUptime = &sys_getCameraUptime;
  this->sys_getAuxTemperature = &sys_getAuxTemperature;
  this->sys_getFPATemperature = &sys_getFPATemperature;
  this->sys_setTelemetryEnabled = &sys_setTelemetryEnabled;
  this->sys_getTelemetryEnabled = &sys_getTelemetryEnabled;
  this->sys_runFFCNormalization = &sys_runFFCNormalization;

#ifndef LEPFLIR_EXCLUDE_EXT_I2C_FUNCS
  this->sys_runPingCamera = &sys_runPingCamera;
  this->sys_setTelemetryLocation = &sys_setTelemetryLocation;
  this->sys_getTelemetryLocation = &sys_getTelemetryLocation;
  this->sys_runFrameAveraging = &sys_runFrameAveraging;
  this->sys_setNumFramesToAverage = &sys_setNumFramesToAverage;
  this->sys_getNumFramesToAverage = &sys_getNumFramesToAverage;
  this->sys_getSceneStatistics = &sys_getSceneStatistics;
  this->sys_setSceneRegion = &sys_setSceneRegion;
  this->sys_getSceneRegion = &sys_getSceneRegion;
  this->sys_getThermalShutdownCount = &sys_getThermalShutdownCount;
  this->sys_setShutterPosition = &sys_setShutterPosition;
  this->sys_getShutterPosition = &sys_getShutterPosition;
  this->sys_setFFCShutterMode = &sys_setFFCShutterMode;
  this->sys_getFFCShutterMode = &sys_getFFCShutterMode;
  this->sys_getFFCNormalizationStatus = &sys_getFFCNormalizationStatus;
#endif
}