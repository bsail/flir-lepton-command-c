#include "lepton-sys.h"
#include "lepton-flir.h"
#include "lepton-communication.h"

void sys_getCameraStatus_internal(LEP_SYS_CAM_STATUS * status, struct lepton_communication * communication)
{
  communication->receiveCommand_array(communication->cmdCode
                       (LEP_CID_SYS_CAM_STATUS, LEP_I2C_COMMAND_TYPE_GET),
                       (uint16_t *) (status), sizeof(LEP_SYS_CAM_STATUS) / 2,communication);
}

LEP_SYS_CAM_STATUS_STATES sys_getCameraStatus(struct lepton_communication * communication)
{
  LEP_SYS_CAM_STATUS camStatus;
  sys_getCameraStatus_internal(&camStatus,communication);
  return (LEP_SYS_CAM_STATUS_STATES) camStatus.camStatus;
}

void sys_getFlirSerialNumber(char *buffer, int maxLength, struct lepton_communication * communication)
{
  if (!buffer || maxLength < 16)
    return;
  uint16_t innerBuffer[4];
  communication->receiveCommand_array(communication->cmdCode
                       (LEP_CID_SYS_FLIR_SERIAL_NUMBER,
                        LEP_I2C_COMMAND_TYPE_GET), innerBuffer, 4,communication);
  wordsToHexString(innerBuffer, 4, buffer, maxLength);
}

void sys_getCustomerSerialNumber(char *buffer, int maxLength, struct lepton_communication * communication)
{
  if (!buffer || maxLength < 64)
    return;
  uint16_t innerBuffer[16];
  communication->receiveCommand_array(communication->cmdCode
                       (LEP_CID_SYS_CUST_SERIAL_NUMBER,
                        LEP_I2C_COMMAND_TYPE_GET), innerBuffer, 16,communication);
  wordsToHexString(innerBuffer, 16, buffer, maxLength);
}

uint32_t sys_getCameraUptime(struct lepton_communication * communication)
{
  uint32_t uptime = 0;
  communication->receiveCommand_u32(communication->cmdCode(LEP_CID_SYS_CAM_UPTIME, LEP_I2C_COMMAND_TYPE_GET),
                     &uptime,communication);
  return uptime;
}

float sys_getAuxTemperature(struct lepton_communication * communication)
{
  uint16_t kelvin100;
  communication->receiveCommand_u16(communication->cmdCode
                     (LEP_CID_SYS_AUX_TEMPERATURE_KELVIN,
                      LEP_I2C_COMMAND_TYPE_GET), &kelvin100,communication);
  return kelvin100ToTemperature(kelvin100);
}

float sys_getFPATemperature(struct lepton_communication * communication)
{
  uint16_t kelvin100;
  communication->receiveCommand_u16(communication->cmdCode
                     (LEP_CID_SYS_FPA_TEMPERATURE_KELVIN,
                      LEP_I2C_COMMAND_TYPE_GET), &kelvin100,communication);
  return kelvin100ToTemperature(kelvin100);
}

void sys_setTelemetryEnabled(uint8_t enabled, struct lepton_communication * communication)
{
  communication->sendCommand_u32(communication->cmdCode
                  (LEP_CID_SYS_TELEMETRY_ENABLE_STATE,
                   LEP_I2C_COMMAND_TYPE_SET), (uint32_t) enabled,communication);
}

uint8_t sys_getTelemetryEnabled(struct lepton_communication * communication)
{
  uint32_t enabled;
  communication->receiveCommand_u32(communication->cmdCode
                     (LEP_CID_SYS_TELEMETRY_ENABLE_STATE,
                      LEP_I2C_COMMAND_TYPE_GET), &enabled,communication);
  return enabled;
}

void sys_runFFCNormalization(struct lepton_communication * communication)
{
  communication->sendCommand_raw(communication->cmdCode(LEP_CID_SYS_RUN_FFC, LEP_I2C_COMMAND_TYPE_RUN),communication);
}

#ifndef LEPFLIR_EXCLUDE_EXT_I2C_FUNCS

void sys_runPingCamera(struct lepton_communication * communication)
{
  communication->sendCommand_raw(communication->cmdCode(LEP_CID_SYS_PING, LEP_I2C_COMMAND_TYPE_RUN),communication);
}

void sys_setTelemetryLocation(LEP_SYS_TELEMETRY_LOCATION location, struct lepton_communication * communication)
{
  communication->sendCommand_u32(communication->cmdCode
                  (LEP_CID_SYS_TELEMETRY_LOCATION, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) location,communication);
}

LEP_SYS_TELEMETRY_LOCATION sys_getTelemetryLocation(struct lepton_communication * communication)
{
  uint32_t location;
  communication->receiveCommand_u32(communication->cmdCode
                     (LEP_CID_SYS_TELEMETRY_LOCATION, LEP_I2C_COMMAND_TYPE_GET),
                     &location,communication);
  return (LEP_SYS_TELEMETRY_LOCATION) location;
}

void sys_runFrameAveraging(struct lepton_communication * communication)
{
  communication->sendCommand_raw(communication->cmdCode
                  (LEP_CID_SYS_EXECTUE_FRAME_AVERAGE,
                   LEP_I2C_COMMAND_TYPE_RUN),communication);
}

void sys_setNumFramesToAverage(LEP_SYS_FRAME_AVERAGE average, struct lepton_communication * communication)
{
  communication->sendCommand_u32(communication->cmdCode
                  (LEP_CID_SYS_NUM_FRAMES_TO_AVERAGE, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) average,communication);
}

LEP_SYS_FRAME_AVERAGE sys_getNumFramesToAverage(struct lepton_communication * communication)
{
  uint32_t average;
  communication->receiveCommand_u32(communication->cmdCode
                     (LEP_CID_SYS_NUM_FRAMES_TO_AVERAGE,
                      LEP_I2C_COMMAND_TYPE_GET), &average,communication);
  return (LEP_SYS_FRAME_AVERAGE) average;
}

void sys_getSceneStatistics(LEP_SYS_SCENE_STATISTICS * statistics, struct lepton_communication * communication)
{
  if (!statistics)
    return;
  communication->receiveCommand_array(communication->cmdCode
                       (LEP_CID_SYS_SCENE_STATISTICS, LEP_I2C_COMMAND_TYPE_GET),
                       (uint16_t *) statistics,
                       sizeof(LEP_SYS_SCENE_STATISTICS) / 2,communication);
}

void sys_setSceneRegion(LEP_SYS_SCENE_ROI * region, struct lepton_communication * communication)
{
  if (!region)
    return;
  communication->sendCommand_array(communication->cmdCode(LEP_CID_SYS_SCENE_ROI, LEP_I2C_COMMAND_TYPE_SET),
                    (uint16_t *) region, sizeof(LEP_SYS_SCENE_ROI) / 2,communication);
}

void sys_getSceneRegion(LEP_SYS_SCENE_ROI * region, struct lepton_communication * communication)
{
  if (!region)
    return;
  communication->receiveCommand_array(communication->cmdCode(LEP_CID_SYS_SCENE_ROI, LEP_I2C_COMMAND_TYPE_GET),
                       (uint16_t *) region, sizeof(LEP_SYS_SCENE_ROI) / 2,communication);
}

uint16_t sys_getThermalShutdownCount(struct lepton_communication * communication)
{
  uint16_t count;
  communication->receiveCommand_u16(communication->cmdCode
                     (LEP_CID_SYS_THERMAL_SHUTDOWN_COUNT,
                      LEP_I2C_COMMAND_TYPE_GET), &count,communication);
  return count;
}

void sys_setShutterPosition(LEP_SYS_SHUTTER_POSITION position, struct lepton_communication * communication)
{
  communication->sendCommand_u32(communication->cmdCode
                  (LEP_CID_SYS_SHUTTER_POSITION, LEP_I2C_COMMAND_TYPE_SET),
                  (uint32_t) position,communication);
}

LEP_SYS_SHUTTER_POSITION sys_getShutterPosition(struct lepton_communication * communication)
{
  uint32_t position;
  communication->receiveCommand_u32(communication->cmdCode
                     (LEP_CID_SYS_SHUTTER_POSITION, LEP_I2C_COMMAND_TYPE_GET),
                     &position,communication);
  return (LEP_SYS_SHUTTER_POSITION) position;
}

void sys_setFFCShutterMode(LEP_SYS_FFC_SHUTTER_MODE * mode, struct lepton_communication * communication)
{
  if (!mode)
    return;
  communication->sendCommand_array(communication->cmdCode
                    (LEP_CID_SYS_FFC_SHUTTER_MODE, LEP_I2C_COMMAND_TYPE_SET),
                    (uint16_t *) mode, sizeof(LEP_SYS_FFC_SHUTTER_MODE) / 2,communication);
}

void sys_getFFCShutterMode(LEP_SYS_FFC_SHUTTER_MODE * mode, struct lepton_communication * communication)
{
  if (!mode)
    return;
  communication->receiveCommand_array(communication->cmdCode
                       (LEP_CID_SYS_FFC_SHUTTER_MODE, LEP_I2C_COMMAND_TYPE_GET),
                       (uint16_t *) mode, sizeof(LEP_SYS_FFC_SHUTTER_MODE) / 2,communication);
}

LEP_SYS_FFC_STATUS sys_getFFCNormalizationStatus(struct lepton_communication * communication)
{
  uint32_t status;
  communication->receiveCommand_u32(communication->cmdCode(LEP_CID_SYS_FFC_STATUS, LEP_I2C_COMMAND_TYPE_GET),
                     &status,communication);
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