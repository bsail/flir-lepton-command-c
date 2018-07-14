#include "unity.h"
#include "lepton-sys.h"
#include "lepton-flir-defs.h"
#include "lepton-flir.h"
#include "mock_lepton-agc.h"
#include "mock_lepton-vid.h"
#include "mock_lepton-comm-internal.h"
#include "mock_lepton-communication.h"
#include <string.h>

struct lepton_driver driver;

void setUp(void)
{
  lepton_communication_init_Ignore();
  lepton_agc_init_Ignore();
  lepton_vid_init_Ignore();
  LeptonFLiR_init(LeptonFLiR_ImageStorageMode_80x60_16bpp,
                  LeptonFLiR_TemperatureMode_Kelvin, &driver);
}

void tearDown(void)
{
}

void test_getCameraStatus_should_not_segfault_null_pointer(void)
{
  TEST_ASSERT_EQUAL(LEP_SYSTEM_COMMUNICATION_ERROR, getCameraStatus(0));
}

void test_getCameraStatus_should_work(void)
{
  uint32_t status = rand() * rand() * rand() * rand();
  uint16_t code = rand() * rand();
  LEP_SYS_CAM_STATUS camStatus;
  camStatus.camStatus = status;

  cmdCode_ExpectAndReturn(LEP_CID_SYS_CAM_STATUS, LEP_I2C_COMMAND_TYPE_GET,
                          code);
  receiveCommand_array_Expect(&(driver.communication), code, 0,
                              sizeof(LEP_SYS_CAM_STATUS) / 2);
  receiveCommand_array_IgnoreArg_readWords();
  receiveCommand_array_ReturnMemThruPtr_readWords((uint16_t *) & camStatus,
                                                  sizeof(camStatus));

  TEST_ASSERT_EQUAL(status, getCameraStatus(&driver));
}

void test_getFlirSerialNumber_should_not_segfault_null_pointer(void)
{
  char buffer[10];
  getFlirSerialNumber(0, buffer, 10);
}

void test_getCustomerSerialNumber_should_not_segfault_null_pointer(void)
{
  char buffer[10];
  getCustomerSerialNumber(0, buffer, 10);
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
  setTelemetryEnabled(0, ENABLED);
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
  setTelemetryLocation(0, LEP_TELEMETRY_LOCATION_HEADER);
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
  setNumFramesToAverage(0, LEP_SYS_FA_DIV_1);
}

void test_getNumFramesToAverage_should_not_segfault_null_pointer(void)
{
  getNumFramesToAverage(0);
}

void test_getSceneStatistics_should_not_segfault_null_pointer(void)
{
  LEP_SYS_SCENE_STATISTICS statistics;
  getSceneStatistics(0, &statistics);
}

void test_setSceneRegion_should_not_segfault_null_pointer(void)
{
  LEP_SYS_SCENE_ROI region;
  setSceneRegion(0, &region);
}

void test_getSceneRegion_should_not_segfault_null_pointer(void)
{
  LEP_SYS_SCENE_ROI region;
  getSceneRegion(0, &region);
}

void test_getThermalShutdownCount_should_not_segfault_null_pointer(void)
{
  getThermalShutdownCount(0);
}

void test_setShutterPosition_should_not_segfault_null_pointer(void)
{
  setShutterPosition(0, LEP_SYS_SHUTTER_POSITION_IDLE);
}

void test_getShutterPosition_should_not_segfault_null_pointer(void)
{
  getShutterPosition(0);
}

void test_setFFCShutterMode_should_not_segfault_null_pointer(void)
{
  LEP_SYS_FFC_SHUTTER_MODE mode;
  setFFCShutterMode(0, &mode);
}

void test_getFFCShutterMode_should_not_segfault_null_pointer(void)
{
  LEP_SYS_FFC_SHUTTER_MODE mode;
  getFFCShutterMode(0, &mode);
}

void test_getFFCNormalizationStatus_should_not_segfault_null_pointer(void)
{
  getFFCNormalizationStatus(0);
}

void test_getFlirSerialNumber_null_buffer(void)
{
  getFlirSerialNumber(&driver, 0, 16);
}

void test_getFlirSerialNumber_small_buffer(void)
{
  char buffer[10];
  getFlirSerialNumber(&driver, buffer, 10);
}

void test_getFlirSerialNumber_should_work(void)
{
  char buffer[16];
  char *result = "0123456789ABCDEF";
  uint16_t buf[4] = { 0x0123, 0x4567, 0x89AB, 0xCDEF };
  uint16_t code = 0xAB;
  cmdCode_ExpectAndReturn(LEP_CID_SYS_FLIR_SERIAL_NUMBER,
                          LEP_I2C_COMMAND_TYPE_GET, code);
  receiveCommand_array_Expect(&(driver.communication), code, 0, 4);
  receiveCommand_array_IgnoreArg_readWords();
  receiveCommand_array_ReturnMemThruPtr_readWords(buf, 4 * sizeof(buf[0]));

  getFlirSerialNumber(&driver, buffer, 16);

  TEST_ASSERT_EQUAL_MEMORY(result, buffer, 16);
}

void test_getCustomerSerialNumber_null_buffer(void)
{
  getCustomerSerialNumber(&driver, 0, 16);
}

void test_getCustomerSerialNumber_small_buffer(void)
{
  char buffer[10];
  getCustomerSerialNumber(&driver, buffer, 10);
}

void test_getCustomerSerialNumber_should_work(void)
{
  char buffer[64];
  char *result =
      "0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF";
  uint16_t buf[16] =
      { 0x0123, 0x4567, 0x89AB, 0xCDEF, 0x0123, 0x4567, 0x89AB, 0xCDEF, 0x0123,
0x4567, 0x89AB, 0xCDEF, 0x0123, 0x4567, 0x89AB, 0xCDEF };
  uint16_t code = 0xAB;
  cmdCode_ExpectAndReturn(LEP_CID_SYS_CUST_SERIAL_NUMBER,
                          LEP_I2C_COMMAND_TYPE_GET, code);
  receiveCommand_array_Expect(&(driver.communication), code, 0, 16);
  receiveCommand_array_IgnoreArg_readWords();
  receiveCommand_array_ReturnMemThruPtr_readWords(buf, 16 * sizeof(buf[0]));

  getCustomerSerialNumber(&driver, buffer, 64);

  TEST_ASSERT_EQUAL_MEMORY(result, buffer, 64);
}

void test_getCameraUptime_should_work(void)
{
  uint8_t code = 0xAB;
  uint32_t value = 0x12345678;
  cmdCode_ExpectAndReturn(LEP_CID_SYS_CAM_UPTIME, LEP_I2C_COMMAND_TYPE_GET,
                          code);
  receiveCommand_u32_Expect(&(driver.communication), code, 0);
  receiveCommand_u32_IgnoreArg_value();
  receiveCommand_u32_ReturnThruPtr_value(&value);
  TEST_ASSERT_EQUAL(0x12345678, getCameraUptime(&driver));
}

void test_getAuxTemperature_should_work(void)
{
  uint8_t code = 0xAB;
  uint16_t value = 12345;
  cmdCode_ExpectAndReturn(LEP_CID_SYS_AUX_TEMPERATURE_KELVIN,
                          LEP_I2C_COMMAND_TYPE_GET,
                          code);
  receiveCommand_u16_Expect(&(driver.communication), code, 0);
  receiveCommand_u16_IgnoreArg_value();
  receiveCommand_u16_ReturnThruPtr_value(&value);
  TEST_ASSERT_EQUAL_FLOAT(123.45, getAuxTemperature(&driver));
}

void test_getFPATemperature_should_work(void)
{
  uint8_t code = 0xAB;
  uint16_t value = 12345;
  cmdCode_ExpectAndReturn(LEP_CID_SYS_FPA_TEMPERATURE_KELVIN,
                          LEP_I2C_COMMAND_TYPE_GET,
                          code);
  receiveCommand_u16_Expect(&(driver.communication), code, 0);
  receiveCommand_u16_IgnoreArg_value();
  receiveCommand_u16_ReturnThruPtr_value(&value);
  TEST_ASSERT_EQUAL_FLOAT(123.45, getFPATemperature(&driver));
}

void test_getTelemetryEnabled_should_work(void)
{
  uint8_t code = 0xAB;
  uint32_t value = 0x12;
  cmdCode_ExpectAndReturn(LEP_CID_SYS_TELEMETRY_ENABLE_STATE, LEP_I2C_COMMAND_TYPE_GET,
                          code);
  receiveCommand_u32_Expect(&(driver.communication), code, 0);
  receiveCommand_u32_IgnoreArg_value();
  receiveCommand_u32_ReturnThruPtr_value(&value);
  TEST_ASSERT_EQUAL(0x12, getTelemetryEnabled(&driver));
}

void test_getTelemetryLocation_should_work(void)
{
  uint8_t code = 0xAB;
  LEP_SYS_TELEMETRY_LOCATION value = LEP_TELEMETRY_LOCATION_FOOTER;
  cmdCode_ExpectAndReturn(LEP_CID_SYS_TELEMETRY_LOCATION, LEP_I2C_COMMAND_TYPE_GET,
                          code);
  receiveCommand_u32_Expect(&(driver.communication), code, 0);
  receiveCommand_u32_IgnoreArg_value();
  receiveCommand_u32_ReturnThruPtr_value(&value);
  TEST_ASSERT_EQUAL(LEP_TELEMETRY_LOCATION_FOOTER, getTelemetryLocation(&driver));
}

void test_getNumFramesToAverage_should_work(void)
{
  uint8_t code = 0xAB;
  LEP_SYS_FRAME_AVERAGE value = LEP_SYS_FA_DIV_128;
  cmdCode_ExpectAndReturn(LEP_CID_SYS_NUM_FRAMES_TO_AVERAGE, LEP_I2C_COMMAND_TYPE_GET,
                          code);
  receiveCommand_u32_Expect(&(driver.communication), code, 0);
  receiveCommand_u32_IgnoreArg_value();
  receiveCommand_u32_ReturnThruPtr_value(&value);
  TEST_ASSERT_EQUAL(LEP_SYS_FA_DIV_128, getNumFramesToAverage(&driver));
}

void test_getShutterPosition_should_work(void)
{
  uint8_t code = 0xAB;
  uint32_t value = LEP_SYS_SHUTTER_POSITION_CLOSED;
  cmdCode_ExpectAndReturn(LEP_CID_SYS_SHUTTER_POSITION, LEP_I2C_COMMAND_TYPE_GET,
                          code);
  receiveCommand_u32_Expect(&(driver.communication), code, 0);
  receiveCommand_u32_IgnoreArg_value();
  receiveCommand_u32_ReturnThruPtr_value(&value);
  TEST_ASSERT_EQUAL(LEP_SYS_SHUTTER_POSITION_CLOSED, getShutterPosition(&driver));
}

