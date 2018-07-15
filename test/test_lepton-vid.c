#include "unity.h"
#include "lepton-vid.h"
#include "lepton-flir-defs.h"
#include "lepton-flir.h"
#include "mock_lepton-agc.h"
#include "mock_lepton-sys.h"
#include "mock_lepton-comm-internal.h"
#include "mock_lepton-communication.h"
#include <string.h>

struct lepton_driver driver;

void setUp(void)
{
  lepton_communication_init_Ignore();
  lepton_agc_init_Ignore();
  lepton_sys_init_Ignore();
  LeptonFLiR_init(LeptonFLiR_ImageStorageMode_80x60_16bpp,
                  LeptonFLiR_TemperatureMode_Kelvin, &driver);
}

void tearDown(void)
{
}

void test_setPolarity_null_driver(void)
{
  LEP_VID_POLARITY polarity = 0;
  setPolarity(0,polarity);
}

void test_setPolarity_should_work(void)
{
  LEP_VID_POLARITY polarity = rand()*rand()*rand()*rand();
  uint16_t code = rand()*rand();
  cmdCode_ExpectAndReturn(LEP_CID_VID_POLARITY_SELECT, LEP_I2C_COMMAND_TYPE_SET,code);
  sendCommand_u32_Expect(&(driver.communication),code,polarity);

  setPolarity(&driver,polarity);
}

void test_getPolarity_null_driver(void)
{
  getPolarity(0);
}

void test_getPolarity_should_work(void)
{
  uint8_t random = rand();
  uint32_t policy = random;
  uint16_t code = rand()*rand();

  cmdCode_ExpectAndReturn(LEP_CID_VID_POLARITY_SELECT, LEP_I2C_COMMAND_TYPE_GET,code);
  receiveCommand_u32_Expect(&(driver.communication),code,0);
  receiveCommand_u32_IgnoreArg_value();
  receiveCommand_u32_ReturnThruPtr_value((uint32_t*)&policy);

  TEST_ASSERT_EQUAL(policy,getPolarity(&driver));
}


void test_setPseudoColorLUT_null_driver(void)
{
  LEP_VID_PCOLOR_LUT table = 0;
  setPseudoColorLUT(0,table);
}

void test_setPseudoColorLUT_should_work(void)
{
  LEP_VID_POLARITY polarity = rand()*rand()*rand()*rand();
  uint16_t code = rand()*rand();
  cmdCode_ExpectAndReturn(LEP_CID_VID_LUT_SELECT, LEP_I2C_COMMAND_TYPE_SET,code);
  sendCommand_u32_Expect(&(driver.communication),code,polarity);

  setPseudoColorLUT(&driver,polarity);
}

void test_getPseudoColorLUT_null_driver(void)
{
  getPseudoColorLUT(0);
}

void test_getPseudoColorLUT_should_work(void)
{
  uint8_t random = rand();
  uint32_t value = random;
  uint16_t code = rand()*rand();

  cmdCode_ExpectAndReturn(LEP_CID_VID_LUT_SELECT, LEP_I2C_COMMAND_TYPE_GET,code);
  receiveCommand_u32_Expect(&(driver.communication),code,0);
  receiveCommand_u32_IgnoreArg_value();
  receiveCommand_u32_ReturnThruPtr_value((uint32_t*)&value);

  TEST_ASSERT_EQUAL(value,getPseudoColorLUT(&driver));
}

void test_setFocusCalcEnabled_null_driver(void)
{
  setFocusCalcEnabled(0,0);
}

void test_setFocusCalcEnabled_should_work(void)
{
  uint8_t value = rand();
  uint16_t code = rand()*rand();
  cmdCode_ExpectAndReturn(LEP_CID_VID_FOCUS_CALC_ENABLE, LEP_I2C_COMMAND_TYPE_SET,code);
  sendCommand_u32_Expect(&(driver.communication),code,value);

  setFocusCalcEnabled(&driver,value);
}

void test_getFocusCalcEnabled_null_driver(void)
{
  getFocusCalcEnabled(0);
}

void test_getFocusCalcEnabled_should_work(void)
{
  uint8_t value = rand();
  uint16_t code = rand()*rand();

  cmdCode_ExpectAndReturn(LEP_CID_VID_FOCUS_CALC_ENABLE, LEP_I2C_COMMAND_TYPE_GET,code);
  receiveCommand_u32_Expect(&(driver.communication),code,0);
  receiveCommand_u32_IgnoreArg_value();
  receiveCommand_u32_ReturnThruPtr_value((uint32_t*)&value);

  TEST_ASSERT_EQUAL(value,getFocusCalcEnabled(&driver));
}

void test_setFreezeEnabled_null_driver(void)
{
  setFreezeEnabled(0,0);
}

void test_setFreezeEnabled_should_work(void)
{
  uint8_t value = rand();
  uint16_t code = rand()*rand();
  cmdCode_ExpectAndReturn(LEP_CID_VID_FREEZE_ENABLE, LEP_I2C_COMMAND_TYPE_SET,code);
  sendCommand_u32_Expect(&(driver.communication),code,value);

  setFreezeEnabled(&driver,value);
}

void test_getFreezeEnabled_null_driver(void)
{
  getFreezeEnabled(0);
}

void test_getFreezeEnabled_should_work(void)
{
  uint8_t value = rand();
  uint16_t code = rand()*rand();

  cmdCode_ExpectAndReturn(LEP_CID_VID_FREEZE_ENABLE, LEP_I2C_COMMAND_TYPE_GET,code);
  receiveCommand_u32_Expect(&(driver.communication),code,0);
  receiveCommand_u32_IgnoreArg_value();
  receiveCommand_u32_ReturnThruPtr_value((uint32_t*)&value);

  TEST_ASSERT_EQUAL(value,getFreezeEnabled(&driver));
}

void test_setUserColorLUT_null_driver(void)
{
  LEP_VID_LUT_BUFFER table;
  setUserColorLUT(0,&table);
}

void test_setUserColorLUT_null_pointer_table(void)
{
  setUserColorLUT(&driver,0);
}

void test_setUserColorLUT_should_work(void)
{
  LEP_VID_LUT_BUFFER value;
  memset(&value,rand(),sizeof(LEP_VID_LUT_BUFFER));
  uint16_t code = rand()*rand();
  cmdCode_ExpectAndReturn(LEP_CID_VID_LUT_TRANSFER, LEP_I2C_COMMAND_TYPE_SET,code);
  sendCommand_array_Expect(&(driver.communication),code,(uint16_t *)&value,512);

  setUserColorLUT(&driver,&value);
}

void test_getUserColorLUT_null_driver(void)
{
  LEP_VID_LUT_BUFFER table;
  getUserColorLUT(0,&table);
}

void test_getUserColorLUT_null_pointer_table(void)
{
  getUserColorLUT(&driver,0);
}

void test_getUserColorLUT_should_work(void)
{
  LEP_VID_LUT_BUFFER value,ideal;
  memset(&value,rand(),sizeof(LEP_VID_LUT_BUFFER));
  memcpy(&ideal,&value,sizeof(LEP_VID_LUT_BUFFER));
  uint16_t code = rand()*rand();

  cmdCode_ExpectAndReturn(LEP_CID_VID_LUT_TRANSFER, LEP_I2C_COMMAND_TYPE_GET,code);
  receiveCommand_array_Expect(&(driver.communication),code,0,512);
  receiveCommand_array_IgnoreArg_readWords();
  receiveCommand_array_ReturnThruPtr_readWords((uint16_t*)&value);

  getUserColorLUT(&driver,&value);
  TEST_ASSERT_EQUAL_MEMORY(&ideal,&value,sizeof(LEP_VID_LUT_BUFFER));
}


void test_setFocusRegion_null_driver(void)
{
  LEP_VID_FOCUS_ROI region;
  setFocusRegion(0,&region);
}

void test_setFocusRegion_null_pointer_region(void)
{
  setFocusRegion(&driver,0);
}

void test_setFocusRegion_should_work(void)
{
  LEP_VID_FOCUS_ROI value;
  memset(&value,rand(),sizeof(LEP_VID_FOCUS_ROI));
  uint16_t code = rand()*rand();
  cmdCode_ExpectAndReturn(LEP_CID_VID_FOCUS_ROI, LEP_I2C_COMMAND_TYPE_SET,code);
  sendCommand_array_Expect(&(driver.communication),code,(uint16_t *)&value,4);

  setFocusRegion(&driver,&value);
}

void test_getFocusRegion_null_driver(void)
{
  LEP_VID_FOCUS_ROI region;
  getFocusRegion(0,&region);
}

void test_getFocusRegion_null_pointer_region(void)
{
  getFocusRegion(&driver,0);
}

void test_getFocusRegion_should_work(void)
{
  LEP_VID_FOCUS_ROI value,ideal;
  memset(&value,rand(),sizeof(LEP_VID_FOCUS_ROI));
  memcpy(&ideal,&value,sizeof(LEP_VID_FOCUS_ROI));
  uint16_t code = rand()*rand();

  cmdCode_ExpectAndReturn(LEP_CID_VID_FOCUS_ROI, LEP_I2C_COMMAND_TYPE_GET,code);
  receiveCommand_array_Expect(&(driver.communication),code,0,4);
  receiveCommand_array_IgnoreArg_readWords();
  receiveCommand_array_ReturnThruPtr_readWords((uint16_t*)&value);

  getFocusRegion(&driver,&value);
  TEST_ASSERT_EQUAL_MEMORY(&ideal,&value,sizeof(LEP_VID_FOCUS_ROI));
}

void test_setFocusThreshold_null_driver(void)
{
  setFocusThreshold(0,0);
}

void test_setFocusThreshold_should_work(void)
{
  uint32_t value = rand()*rand()*rand()*rand();
  uint16_t code = rand()*rand();
  cmdCode_ExpectAndReturn(LEP_CID_VID_FOCUS_THRESHOLD, LEP_I2C_COMMAND_TYPE_SET,code);
  sendCommand_u32_Expect(&(driver.communication),code,value);

  setFocusThreshold(&driver,value);
}

void test_getFocusThreshold_null_driver(void)
{
  getFocusThreshold(0);
}

void test_getFocusThreshold_should_work(void)
{
  uint32_t value = rand()*rand()*rand()*rand();
  uint16_t code = rand()*rand();

  cmdCode_ExpectAndReturn(LEP_CID_VID_FOCUS_THRESHOLD, LEP_I2C_COMMAND_TYPE_GET,code);
  receiveCommand_u32_Expect(&(driver.communication),code,0);
  receiveCommand_u32_IgnoreArg_value();
  receiveCommand_u32_ReturnThruPtr_value((uint32_t*)&value);

  TEST_ASSERT_EQUAL(value,getFocusThreshold(&driver));
}

void test_getFocusMetric_null_driver(void)
{
  getFocusMetric(0);
}

void test_getFocusMetric_should_work(void)
{
  uint32_t value = rand()*rand()*rand()*rand();
  uint16_t code = rand()*rand();

  cmdCode_ExpectAndReturn(LEP_CID_VID_FOCUS_METRIC, LEP_I2C_COMMAND_TYPE_GET,code);
  receiveCommand_u32_Expect(&(driver.communication),code,0);
  receiveCommand_u32_IgnoreArg_value();
  receiveCommand_u32_ReturnThruPtr_value((uint32_t*)&value);

  TEST_ASSERT_EQUAL(value,getFocusMetric(&driver));
}

void test_setSceneBasedNUCEnabled_null_driver(void)
{
  setSceneBasedNUCEnabled(0,0);
}

void test_setSceneBasedNUCEnabled_should_work(void)
{
  uint8_t value = rand();
  uint16_t code = rand()*rand();
  cmdCode_ExpectAndReturn(LEP_CID_VID_SBNUC_ENABLE, LEP_I2C_COMMAND_TYPE_SET,code);
  sendCommand_u32_Expect(&(driver.communication),code,value);

  setSceneBasedNUCEnabled(&driver,value);
}

void test_getSceneBasedNUCEnabled_null_driver(void)
{
  getSceneBasedNUCEnabled(0);
}

void test_getSceneBasedNUCEnabled_should_work(void)
{
  uint8_t value = rand();
  uint16_t code = rand()*rand();

  cmdCode_ExpectAndReturn(LEP_CID_VID_SBNUC_ENABLE, LEP_I2C_COMMAND_TYPE_GET,code);
  receiveCommand_u32_Expect(&(driver.communication),code,0);
  receiveCommand_u32_IgnoreArg_value();
  receiveCommand_u32_ReturnThruPtr_value((uint32_t*)&value);

  TEST_ASSERT_EQUAL(value,getSceneBasedNUCEnabled(&driver));
}

void test_setGamma_null_driver(void)
{
  setGamma(0,0);
}

void test_setGamma_should_work(void)
{
  uint32_t value = rand()*rand()*rand()*rand();
  uint16_t code = rand()*rand();
  cmdCode_ExpectAndReturn(LEP_CID_VID_GAMMA_SELECT, LEP_I2C_COMMAND_TYPE_SET,code);
  sendCommand_u32_Expect(&(driver.communication),code,value);

  setGamma(&driver,value);
}

void test_getGamma_null_driver(void)
{
  getGamma(0);
}

void test_getGamma_should_work(void)
{
  uint32_t value = rand()*rand()*rand()*rand();
  uint16_t code = rand()*rand();

  cmdCode_ExpectAndReturn(LEP_CID_VID_GAMMA_SELECT, LEP_I2C_COMMAND_TYPE_GET,code);
  receiveCommand_u32_Expect(&(driver.communication),code,0);
  receiveCommand_u32_IgnoreArg_value();
  receiveCommand_u32_ReturnThruPtr_value((uint32_t*)&value);

  TEST_ASSERT_EQUAL(value,getGamma(&driver));
}
