#ifndef _LEPTON_SYS_H
#define _LEPTON_SYS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lepton-flir.h"

#ifndef TEST
#ifndef LEPTON_FLIR_INTERNAL
#error "There is no user accessable data and functions here"
#endif
#endif

#ifndef LEPFLIR_EXCLUDE_SYS_FUNCS

void lepton_sys_init(struct lepton_sys * this);

#endif

#ifdef TEST

LEP_SYS_CAM_STATUS_STATES getCameraStatus(struct lepton_driver * driver);
void getFlirSerialNumber(struct lepton_driver * driver,char *buffer,int maxLength);
void getCustomerSerialNumber(struct lepton_driver * driver,char *buffer,int maxLength);
uint32_t getCameraUptime(struct lepton_driver * driver);
float getAuxTemperature(struct lepton_driver * driver);
float getFPATemperature(struct lepton_driver * driver);
void setTelemetryEnabled(struct lepton_driver * driver,uint8_t enabled);
uint8_t getTelemetryEnabled(struct lepton_driver * driver);
void runFFCNormalization(struct lepton_driver * driver);
void runPingCamera(struct lepton_driver * driver);
void setTelemetryLocation(struct lepton_driver * driver,LEP_SYS_TELEMETRY_LOCATION location);
LEP_SYS_TELEMETRY_LOCATION getTelemetryLocation(struct lepton_driver * driver);
void runFrameAveraging(struct lepton_driver * driver);
void setNumFramesToAverage(struct lepton_driver * driver,LEP_SYS_FRAME_AVERAGE average);
LEP_SYS_FRAME_AVERAGE getNumFramesToAverage(struct lepton_driver * driver);
void getSceneStatistics(struct lepton_driver * driver,LEP_SYS_SCENE_STATISTICS * statistics);
void setSceneRegion(struct lepton_driver * driver,LEP_SYS_SCENE_ROI * region);
void getSceneRegion(struct lepton_driver * driver,LEP_SYS_SCENE_ROI * region);
uint16_t getThermalShutdownCount(struct lepton_driver * driver);
void setShutterPosition(struct lepton_driver * driver,LEP_SYS_SHUTTER_POSITION position);
LEP_SYS_SHUTTER_POSITION getShutterPosition(struct lepton_driver * driver);
void setFFCShutterMode(struct lepton_driver * driver,LEP_SYS_FFC_SHUTTER_MODE * mode);
void getFFCShutterMode(struct lepton_driver * driver,LEP_SYS_FFC_SHUTTER_MODE * mode);
LEP_SYS_FFC_STATUS getFFCNormalizationStatus(struct lepton_driver * driver);

#endif

#ifdef __cplusplus
}
#endif

#endif // _LEPTON_SYS_H
