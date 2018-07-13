#include "unity.h"
#include "lepton-sys.h"
#include "lepton-flir-defs.h"
#include "mock_lepton-flir.h"
#include "mock_lepton-communication.h"
#include <string.h>

struct lepton_driver driver;

void setUp(void)
{
  // memset(&driver,0,sizeof(driver));
}

void tearDown(void)
{
}

void test_getCameraStatus_should_not_segfault_null_pointer(void)
{
  TEST_ASSERT_EQUAL(LEP_SYSTEM_COMMUNICATION_ERROR,getCameraStatus(0));
}

void test_getCameraStatus_should_work(void)
{
  uint32_t status = rand()*rand()*rand()*rand();
  uint16_t code = rand()*rand();
  LEP_SYS_CAM_STATUS camStatus;
  camStatus.camStatus = status;

  cmdCode_ExpectAndReturn(LEP_CID_SYS_CAM_STATUS,LEP_I2C_COMMAND_TYPE_GET,code);
  receiveCommand_array_Expect(&(driver.communication),code,0,sizeof(LEP_SYS_CAM_STATUS) / 2);
  receiveCommand_array_IgnoreArg_readWords();
  receiveCommand_array_ReturnMemThruPtr_readWords((uint16_t*)&camStatus,sizeof(camStatus));

  TEST_ASSERT_EQUAL(status,getCameraStatus(&driver));
}

void test_getFlirSerialNumber_should_not_segfault_null_pointer(void)
{
  char buffer[10];
  getFlirSerialNumber(0,buffer,10);
}
void test_getCustomerSerialNumber_should_not_segfault_null_pointer(void)
{
  char buffer[10];
  getCustomerSerialNumber(0,buffer,10);
}
void test_getCameraUptime_should_not_segfault_null_pointer(void)
{
  getCameraUptime(0);
}
void test_getAuxTemperature_should_not_segfault_null_pointer(void)
{
  getAuxTemperature(0);
}
void test_getFPATemperature_should_not_segfault_null_pointer(void)
{
  getFPATemperature(0);
}
void test_setTelemetryEnabled_should_not_segfault_null_pointer(void)
{
  setTelemetryEnabled(0,ENABLED);
}
void test_getTelemetryEnabled_should_not_segfault_null_pointer(void)
{
  getTelemetryEnabled(0);
}
void test_runFFCNormalization_should_not_segfault_null_pointer(void)
{
  runFFCNormalization(0);
}
void test_runPingCamera_should_not_segfault_null_pointer(void)
{
  runPingCamera(0);
}
void test_setTelemetryLocation_should_not_segfault_null_pointer(void)
{
  setTelemetryLocation(0,LEP_TELEMETRY_LOCATION_HEADER);
}
void test_getTelemetryLocation_should_not_segfault_null_pointer(void)
{
  getTelemetryLocation(0);
}
void test_runFrameAveraging_should_not_segfault_null_pointer(void)
{
  runFrameAveraging(0);
}
void test_setNumFramesToAverage_should_not_segfault_null_pointer(void)
{
  setNumFramesToAverage(0,LEP_SYS_FA_DIV_1);
}
void test_getNumFramesToAverage_should_not_segfault_null_pointer(void)
{
  getNumFramesToAverage(0);
}
void test_getSceneStatistics_should_not_segfault_null_pointer(void)
{
  LEP_SYS_SCENE_STATISTICS statistics;
  getSceneStatistics(0,&statistics);
}
void test_setSceneRegion_should_not_segfault_null_pointer(void)
{
  LEP_SYS_SCENE_ROI region;
  setSceneRegion(0,&region);
}
void test_getSceneRegion_should_not_segfault_null_pointer(void)
{
  LEP_SYS_SCENE_ROI region;
  getSceneRegion(0,&region);
}
void test_getThermalShutdownCount_should_not_segfault_null_pointer(void)
{
  getThermalShutdownCount(0);
}
void test_setShutterPosition_should_not_segfault_null_pointer(void)
{
  setShutterPosition(0,LEP_SYS_SHUTTER_POSITION_IDLE);
}
void test_getShutterPosition_should_not_segfault_null_pointer(void)
{
  getShutterPosition(0);
}
void test_setFFCShutterMode_should_not_segfault_null_pointer(void)
{
  LEP_SYS_FFC_SHUTTER_MODE mode;
  setFFCShutterMode(0,&mode);
}
void test_getFFCShutterMode_should_not_segfault_null_pointer(void)
{
  LEP_SYS_FFC_SHUTTER_MODE mode;
  getFFCShutterMode(0,&mode);
}
void test_getFFCNormalizationStatus_should_not_segfault_null_pointer(void)
{
  getFFCNormalizationStatus(0);
}


























