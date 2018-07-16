#include "unity.h"
#include "lepton-agc.h"
#include "lepton-flir-defs.h"
#include "lepton-flir.h"
#include "mock_lepton-vid.h"
#include "mock_lepton-sys.h"
#include "mock_lepton-comm-internal.h"
#include "mock_lepton-communication.h"
#include "mock_lepton-register.h"
#include <string.h>

struct lepton_driver driver;

void setUp(void)
{
  lepton_communication_init_Ignore();
  lepton_vid_init_Ignore();
  lepton_sys_init_Ignore();
  LeptonFLiR_init(LeptonFLiR_ImageStorageMode_80x60_16bpp,
                  LeptonFLiR_TemperatureMode_Kelvin, &driver);
}

void tearDown(void)
{
}

void test_setAGCEnabled_null_driver_pointer(void)
{
  setAGCEnabled(0,0);
}

void test_setAGCEnabled_should_work(void)
{
  uint8_t enabled = rand();
  uint16_t code = rand()*rand();
  cmdCode_ExpectAndReturn(LEP_CID_AGC_ENABLE_STATE, LEP_I2C_COMMAND_TYPE_SET,code);
  sendCommand_u32_Expect(&(driver.communication),code,enabled);
  setAGCEnabled(&driver,enabled);
}

void test_getAGCEnabled_null_driver_pointer(void)
{
  getAGCEnabled(0);
}

void test_getAGCEnabled_should_work(void)
{
  uint8_t enabled = rand();
  uint16_t code = rand()*rand();

  cmdCode_ExpectAndReturn(LEP_CID_AGC_ENABLE_STATE, LEP_I2C_COMMAND_TYPE_GET,code);
  receiveCommand_u32_Expect(&(driver.communication),code,0);
  receiveCommand_u32_IgnoreArg_value();
  receiveCommand_u32_ReturnThruPtr_value((uint32_t*)&enabled);

  TEST_ASSERT_EQUAL(enabled,getAGCEnabled(&driver));
}

void test_setAGCPolicy_null_driver_pointer(void)
{
  setAGCPolicy(0,0);
}

void test_setAGCPolicy_should_work(void)
{
  LEP_AGC_POLICY policy = rand()*rand()*rand()*rand();
  uint16_t code = rand()*rand();
  cmdCode_ExpectAndReturn(LEP_CID_AGC_POLICY, LEP_I2C_COMMAND_TYPE_SET,code);
  sendCommand_u32_Expect(&(driver.communication),code,policy);

  setAGCPolicy(&driver,policy);
}

void test_getAGCPolicy_null_driver_pointer(void)
{
  getAGCPolicy(0);
}

void test_getAGCPolicy_should_work(void)
{
  uint8_t random = rand();
  uint32_t policy = random;
  uint16_t code = rand()*rand();

  cmdCode_ExpectAndReturn(LEP_CID_AGC_POLICY, LEP_I2C_COMMAND_TYPE_GET,code);
  receiveCommand_u32_Expect(&(driver.communication),code,0);
  receiveCommand_u32_IgnoreArg_value();
  receiveCommand_u32_ReturnThruPtr_value((uint32_t*)&policy);

  TEST_ASSERT_EQUAL(policy,getAGCPolicy(&driver));
}

void test_setHEQScaleFactor_null_driver_pointer(void)
{
  setHEQScaleFactor(0,0);
}

void test_setHEQScaleFactor_should_work(void)
{
  LEP_AGC_HEQ_SCALE_FACTOR factor = rand()*rand()*rand()*rand();
  uint16_t code = rand()*rand();
  cmdCode_ExpectAndReturn(LEP_CID_AGC_HEQ_SCALE_FACTOR, LEP_I2C_COMMAND_TYPE_SET,code);
  sendCommand_u32_Expect(&(driver.communication),code,factor);

  setHEQScaleFactor(&driver,factor);
}

void test_getHEQScaleFactor_null_driver_pointer(void)
{
  getHEQScaleFactor(0);
}

void test_getHEQScaleFactor_should_work(void)
{
  uint8_t random = rand();
  uint32_t factor = random;
  uint16_t code = rand()*rand();

  cmdCode_ExpectAndReturn(LEP_CID_AGC_HEQ_SCALE_FACTOR, LEP_I2C_COMMAND_TYPE_GET,code);
  receiveCommand_u32_Expect(&(driver.communication),code,0);
  receiveCommand_u32_IgnoreArg_value();
  receiveCommand_u32_ReturnThruPtr_value((uint32_t*)&factor);

  TEST_ASSERT_EQUAL(factor,getHEQScaleFactor(&driver));
}

void test_setAGCCalcEnabled_null_driver_pointer(void)
{
  setAGCCalcEnabled(0,0);
}

void test_setAGCCalcEnabled_should_work(void)
{
  uint8_t enabled = rand();
  uint16_t code = rand()*rand();
  cmdCode_ExpectAndReturn(LEP_CID_AGC_CALC_ENABLE_STATE, LEP_I2C_COMMAND_TYPE_SET,code);
  sendCommand_u32_Expect(&(driver.communication),code,enabled);

  setAGCCalcEnabled(&driver,enabled);
}


void test_getAGCCalcEnabled_null_driver_pointer(void)
{
  getAGCCalcEnabled(0);
}

void test_getAGCCalcEnabled_should_work(void)
{
  uint8_t enabled = rand();
  uint16_t code = rand()*rand();

  cmdCode_ExpectAndReturn(LEP_CID_AGC_CALC_ENABLE_STATE, LEP_I2C_COMMAND_TYPE_GET,code);
  receiveCommand_u32_Expect(&(driver.communication),code,0);
  receiveCommand_u32_IgnoreArg_value();
  receiveCommand_u32_ReturnThruPtr_value((uint32_t*)&enabled);

  TEST_ASSERT_EQUAL(enabled,getAGCCalcEnabled(&driver));
}

void test_setHistogramRegion_null_driver_pointer(void)
{
  LEP_AGC_HISTOGRAM_ROI region;
  setHistogramRegion(0, &region);
}

void test_setHistogramRegion_null_pointer_region(void)
{
  setHistogramRegion(&driver,0);
}

void test_setHistogramRegion_should_work(void)
{
  LEP_AGC_HISTOGRAM_ROI region;
  uint16_t code = rand()*rand();

  cmdCode_ExpectAndReturn(LEP_CID_AGC_ROI, LEP_I2C_COMMAND_TYPE_SET,code);
  sendCommand_array_Expect(&(driver.communication),code,(uint16_t*)&region,4);

  setHistogramRegion(&driver,&region);
}

void test_getHistogramRegion_null_driver_pointer(void)
{
  LEP_AGC_HISTOGRAM_ROI region;
  getHistogramRegion(0,&region);
}

void test_getHistogramRegion_should_work(void)
{
  LEP_AGC_HISTOGRAM_ROI region,ideal;
  memset(&region,rand(),sizeof(LEP_AGC_HISTOGRAM_ROI));
  memcpy(&ideal,&region,sizeof(LEP_AGC_HISTOGRAM_ROI));
  uint16_t code = rand()*rand();

  cmdCode_ExpectAndReturn(LEP_CID_AGC_ROI, LEP_I2C_COMMAND_TYPE_GET,code);
  receiveCommand_array_Expect(&(driver.communication),code,0,4);
  receiveCommand_array_IgnoreArg_readWords();
  receiveCommand_array_ReturnThruPtr_readWords((uint16_t*)&region);

  getHistogramRegion(&driver,&region);
  TEST_ASSERT_EQUAL_MEMORY(&ideal,&region,sizeof(LEP_AGC_HISTOGRAM_ROI));
}

void test_getHistogramRegion_null_pointer_region(void)
{
  getHistogramRegion(&driver,0);
}

void test_getHistogramStatistics_null_driver_pointer(void)
{
  LEP_AGC_HISTOGRAM_STATISTICS statistics;
  getHistogramStatistics(0,&statistics);
}

void test_getHistogramStatistics_null_pointer_statistics(void)
{
  getHistogramStatistics(&driver,0);
}

void test_getHistogramStatistics_should_work(void)
{
  LEP_AGC_HISTOGRAM_STATISTICS statistics,ideal;
  memset(&statistics,rand(),sizeof(LEP_AGC_HISTOGRAM_STATISTICS));
  memcpy(&ideal,&statistics,sizeof(LEP_AGC_HISTOGRAM_STATISTICS));
  uint16_t code = rand()*rand();

  cmdCode_ExpectAndReturn(LEP_CID_AGC_STATISTICS, LEP_I2C_COMMAND_TYPE_GET,code);
  receiveCommand_array_Expect(&(driver.communication),code,0,4);
  receiveCommand_array_IgnoreArg_readWords();
  receiveCommand_array_ReturnThruPtr_readWords((uint16_t*)&statistics);

  getHistogramStatistics(&driver,&statistics);
  TEST_ASSERT_EQUAL_MEMORY(&ideal,&statistics,sizeof(LEP_AGC_HISTOGRAM_STATISTICS));
}


void test_setHistogramClipPercent_null_driver_pointer(void)
{
  setHistogramClipPercent(0,0);
}

void test_setHistogramClipPercent_should_work(void)
{
  uint16_t percent = rand()*rand();
  uint16_t code = rand()*rand();
  cmdCode_ExpectAndReturn(LEP_CID_AGC_HISTOGRAM_CLIP_PERCENT, LEP_I2C_COMMAND_TYPE_SET,code);
  sendCommand_u16_Expect(&(driver.communication),code,percent);

  setHistogramClipPercent(&driver,percent);
}

void test_getHistogramClipPercent_null_driver_pointer(void)
{
  getHistogramClipPercent(0);
}

void test_getHistogramClipPercent_should_work(void)
{
  uint16_t percent = rand()*rand();
  uint16_t code = rand()*rand();

  cmdCode_ExpectAndReturn(LEP_CID_AGC_HISTOGRAM_CLIP_PERCENT, LEP_I2C_COMMAND_TYPE_GET,code);
  receiveCommand_u16_Expect(&(driver.communication),code,0);
  receiveCommand_u16_IgnoreArg_value();
  receiveCommand_u16_ReturnThruPtr_value(&percent);

  TEST_ASSERT_EQUAL(percent,getHistogramClipPercent(&driver));
}

void test_setHistogramTailSize_null_driver_pointer(void)
{
  setHistogramTailSize(0,0);
}

void test_setHistogramTailSize_should_work(void)
{
  uint16_t size = rand()*rand();
  uint16_t code = rand()*rand();
  cmdCode_ExpectAndReturn(LEP_CID_AGC_HISTOGRAM_TAIL_SIZE, LEP_I2C_COMMAND_TYPE_SET,code);
  sendCommand_u16_Expect(&(driver.communication),code,size);

  setHistogramTailSize(&driver,size);
}

void test_getHistogramTailSize_null_driver_pointer(void)
{
  getHistogramTailSize(0);
}

void test_getHistogramTailSize_should_work(void)
{
  uint16_t size = rand()*rand();
  uint16_t code = rand()*rand();

  cmdCode_ExpectAndReturn(LEP_CID_AGC_HISTOGRAM_TAIL_SIZE, LEP_I2C_COMMAND_TYPE_GET,code);
  receiveCommand_u16_Expect(&(driver.communication),code,0);
  receiveCommand_u16_IgnoreArg_value();
  receiveCommand_u16_ReturnThruPtr_value(&size);

  TEST_ASSERT_EQUAL(size,getHistogramTailSize(&driver));
}


void test_setLinearMaxGain_null_driver_pointer(void)
{
  setLinearMaxGain(0,0);
}

void test_setLinearMaxGain_should_work(void)
{
  uint16_t gain = rand()*rand();
  uint16_t code = rand()*rand();
  cmdCode_ExpectAndReturn(LEP_CID_AGC_LINEAR_MAX_GAIN, LEP_I2C_COMMAND_TYPE_SET,code);
  sendCommand_u16_Expect(&(driver.communication),code,gain);

  setLinearMaxGain(&driver,gain);
}

void test_getLinearMaxGain_null_driver_pointer(void)
{
  getLinearMaxGain(0);
}

void test_getLinearMaxGain_should_work(void)
{
  uint16_t gain = rand()*rand();
  uint16_t code = rand()*rand();

  cmdCode_ExpectAndReturn(LEP_CID_AGC_LINEAR_MAX_GAIN, LEP_I2C_COMMAND_TYPE_GET,code);
  receiveCommand_u16_Expect(&(driver.communication),code,0);
  receiveCommand_u16_IgnoreArg_value();
  receiveCommand_u16_ReturnThruPtr_value(&gain);

  TEST_ASSERT_EQUAL(gain,getLinearMaxGain(&driver));
}

void test_setLinearMidpoint_null_driver_pointer(void)
{
  setLinearMidpoint(0,0);
}

void test_setLinearMidpoint_should_work(void)
{
  uint16_t value = rand()*rand();
  uint16_t code = rand()*rand();
  cmdCode_ExpectAndReturn(LEP_CID_AGC_LINEAR_MIDPOINT, LEP_I2C_COMMAND_TYPE_SET,code);
  sendCommand_u16_Expect(&(driver.communication),code,value);

  setLinearMidpoint(&driver,value);
}

void test_getLinearMidpoint_null_driver_pointer(void)
{
  getLinearMidpoint(0);
}

void test_getLinearMidpoint_should_work(void)
{
  uint16_t value = rand()*rand();
  uint16_t code = rand()*rand();

  cmdCode_ExpectAndReturn(LEP_CID_AGC_LINEAR_MIDPOINT, LEP_I2C_COMMAND_TYPE_GET,code);
  receiveCommand_u16_Expect(&(driver.communication),code,0);
  receiveCommand_u16_IgnoreArg_value();
  receiveCommand_u16_ReturnThruPtr_value(&value);

  TEST_ASSERT_EQUAL(value,getLinearMidpoint(&driver));
}

void test_setLinearDampeningFactor_null_driver_pointer(void)
{
  setLinearDampeningFactor(0,0);
}

void test_setLinearDampeningFactor_should_work(void)
{
  uint16_t value = rand()*rand();
  uint16_t code = rand()*rand();
  cmdCode_ExpectAndReturn(LEP_CID_AGC_LINEAR_DAMPENING_FACTOR, LEP_I2C_COMMAND_TYPE_SET,code);
  sendCommand_u16_Expect(&(driver.communication),code,value);

  setLinearDampeningFactor(&driver,value);
}

void test_getLinearDampeningFactor_null_driver_pointer(void)
{
  getLinearDampeningFactor(0);
}

void test_getLinearDampeningFactor_should_work(void)
{
  uint16_t value = rand()*rand();
  uint16_t code = rand()*rand();

  cmdCode_ExpectAndReturn(LEP_CID_AGC_LINEAR_DAMPENING_FACTOR, LEP_I2C_COMMAND_TYPE_GET,code);
  receiveCommand_u16_Expect(&(driver.communication),code,0);
  receiveCommand_u16_IgnoreArg_value();
  receiveCommand_u16_ReturnThruPtr_value(&value);

  TEST_ASSERT_EQUAL(value,getLinearDampeningFactor(&driver));
}

void test_setHEQDampeningFactor_null_driver_pointer(void)
{
  setHEQDampeningFactor(0,0);
}

void test_setHEQDampeningFactor_should_work(void)
{
  uint16_t value = rand()*rand();
  uint16_t code = rand()*rand();
  cmdCode_ExpectAndReturn(LEP_CID_AGC_HEQ_DAMPENING_FACTOR, LEP_I2C_COMMAND_TYPE_SET,code);
  sendCommand_u16_Expect(&(driver.communication),code,value);

  setHEQDampeningFactor(&driver,value);
}

void test_getHEQDampeningFactor_null_driver_pointer(void)
{
  getHEQDampeningFactor(0);
}

void test_getHEQDampeningFactor_should_work(void)
{
  uint16_t value = rand()*rand();
  uint16_t code = rand()*rand();

  cmdCode_ExpectAndReturn(LEP_CID_AGC_HEQ_DAMPENING_FACTOR, LEP_I2C_COMMAND_TYPE_GET,code);
  receiveCommand_u16_Expect(&(driver.communication),code,0);
  receiveCommand_u16_IgnoreArg_value();
  receiveCommand_u16_ReturnThruPtr_value(&value);

  TEST_ASSERT_EQUAL(value,getHEQDampeningFactor(&driver));
}

void test_setHEQMaxGain_null_driver_pointer(void)
{
  setHEQMaxGain(0,0);
}

void test_setHEQMaxGain_should_work(void)
{
  uint16_t value = rand()*rand();
  uint16_t code = rand()*rand();
  cmdCode_ExpectAndReturn(LEP_CID_AGC_HEQ_MAX_GAIN, LEP_I2C_COMMAND_TYPE_SET,code);
  sendCommand_u16_Expect(&(driver.communication),code,value);

  setHEQMaxGain(&driver,value);
}

void test_getHEQMaxGain_null_driver_pointer(void)
{
  getHEQMaxGain(0);
}

void test_getHEQMaxGain_should_work(void)
{
  uint16_t value = rand()*rand();
  uint16_t code = rand()*rand();

  cmdCode_ExpectAndReturn(LEP_CID_AGC_HEQ_MAX_GAIN, LEP_I2C_COMMAND_TYPE_GET,code);
  receiveCommand_u16_Expect(&(driver.communication),code,0);
  receiveCommand_u16_IgnoreArg_value();
  receiveCommand_u16_ReturnThruPtr_value(&value);

  TEST_ASSERT_EQUAL(value,getHEQMaxGain(&driver));
}


void test_setHEQClipLimitHigh_null_driver_pointer(void)
{
  setHEQClipLimitHigh(0,0);
}

void test_setHEQClipLimitHigh_should_work(void)
{
  uint16_t value = rand()*rand();
  uint16_t code = rand()*rand();
  cmdCode_ExpectAndReturn(LEP_CID_AGC_HEQ_CLIP_LIMIT_HIGH, LEP_I2C_COMMAND_TYPE_SET,code);
  sendCommand_u16_Expect(&(driver.communication),code,value);

  setHEQClipLimitHigh(&driver,value);
}

void test_getHEQClipLimitHigh_null_driver_pointer(void)
{
  getHEQClipLimitHigh(0);
}

void test_getHEQClipLimitHigh_should_work(void)
{
  uint16_t value = rand()*rand();
  uint16_t code = rand()*rand();

  cmdCode_ExpectAndReturn(LEP_CID_AGC_HEQ_CLIP_LIMIT_HIGH, LEP_I2C_COMMAND_TYPE_GET,code);
  receiveCommand_u16_Expect(&(driver.communication),code,0);
  receiveCommand_u16_IgnoreArg_value();
  receiveCommand_u16_ReturnThruPtr_value(&value);

  TEST_ASSERT_EQUAL(value,getHEQClipLimitHigh(&driver));
}

void test_setHEQClipLimitLow_null_driver_pointer(void)
{
  setHEQClipLimitLow(0,0);
}

void test_setHEQClipLimitLow_should_work(void)
{
  uint16_t value = rand()*rand();
  uint16_t code = rand()*rand();
  cmdCode_ExpectAndReturn(LEP_CID_AGC_HEQ_CLIP_LIMIT_LOW, LEP_I2C_COMMAND_TYPE_SET,code);
  sendCommand_u16_Expect(&(driver.communication),code,value);

  setHEQClipLimitLow(&driver,value);
}

void test_getHEQClipLimitLow_null_driver_pointer(void)
{
  getHEQClipLimitLow(0);
}

void test_getHEQClipLimitLow_should_work(void)
{
  uint16_t value = rand()*rand();
  uint16_t code = rand()*rand();

  cmdCode_ExpectAndReturn(LEP_CID_AGC_HEQ_CLIP_LIMIT_LOW, LEP_I2C_COMMAND_TYPE_GET,code);
  receiveCommand_u16_Expect(&(driver.communication),code,0);
  receiveCommand_u16_IgnoreArg_value();
  receiveCommand_u16_ReturnThruPtr_value(&value);

  TEST_ASSERT_EQUAL(value,getHEQClipLimitLow(&driver));
}

void test_setHEQBinExtension_null_driver_pointer(void)
{
  setHEQBinExtension(0,0);
}

void test_setHEQBinExtension_should_work(void)
{
  uint16_t value = rand()*rand();
  uint16_t code = rand()*rand();
  cmdCode_ExpectAndReturn(LEP_CID_AGC_HEQ_BIN_EXTENSION, LEP_I2C_COMMAND_TYPE_SET,code);
  sendCommand_u16_Expect(&(driver.communication),code,value);

  setHEQBinExtension(&driver,value);
}

void test_getHEQBinExtension_null_driver_pointer(void)
{
  getHEQBinExtension(0);
}

void test_getHEQBinExtension_should_work(void)
{
  uint16_t value = rand()*rand();
  uint16_t code = rand()*rand();

  cmdCode_ExpectAndReturn(LEP_CID_AGC_HEQ_BIN_EXTENSION, LEP_I2C_COMMAND_TYPE_GET,code);
  receiveCommand_u16_Expect(&(driver.communication),code,0);
  receiveCommand_u16_IgnoreArg_value();
  receiveCommand_u16_ReturnThruPtr_value(&value);

  TEST_ASSERT_EQUAL(value,getHEQBinExtension(&driver));
}

void test_setHEQMidpoint_null_driver_pointer(void)
{
  setHEQMidpoint(0,0);
}

void test_setHEQMidpoint_should_work(void)
{
  uint16_t value = rand()*rand();
  uint16_t code = rand()*rand();
  cmdCode_ExpectAndReturn(LEP_CID_AGC_HEQ_MIDPOINT, LEP_I2C_COMMAND_TYPE_SET,code);
  sendCommand_u16_Expect(&(driver.communication),code,value);

  setHEQMidpoint(&driver,value);
}

void test_getHEQMidpoint_null_driver_pointer(void)
{
  getHEQMidpoint(0);
}

void test_getHEQMidpoint_should_work(void)
{
  uint16_t value = rand()*rand();
  uint16_t code = rand()*rand();

  cmdCode_ExpectAndReturn(LEP_CID_AGC_HEQ_MIDPOINT, LEP_I2C_COMMAND_TYPE_GET,code);
  receiveCommand_u16_Expect(&(driver.communication),code,0);
  receiveCommand_u16_IgnoreArg_value();
  receiveCommand_u16_ReturnThruPtr_value(&value);

  TEST_ASSERT_EQUAL(value,getHEQMidpoint(&driver));
}

void test_setHEQEmptyCounts_null_driver_pointer(void)
{
  setHEQEmptyCounts(0,0);
}

void test_setHEQEmptyCounts_should_work(void)
{
  uint16_t value = rand()*rand();
  uint16_t code = rand()*rand();
  cmdCode_ExpectAndReturn(LEP_CID_AGC_HEQ_EMPTY_COUNTS, LEP_I2C_COMMAND_TYPE_SET,code);
  sendCommand_u16_Expect(&(driver.communication),code,value);

  setHEQEmptyCounts(&driver,value);
}

void test_getHEQEmptyCounts_null_driver_pointer(void)
{
  getHEQEmptyCounts(0);
}

void test_getHEQEmptyCounts_should_work(void)
{
  uint16_t value = rand()*rand();
  uint16_t code = rand()*rand();

  cmdCode_ExpectAndReturn(LEP_CID_AGC_HEQ_EMPTY_COUNTS, LEP_I2C_COMMAND_TYPE_GET,code);
  receiveCommand_u16_Expect(&(driver.communication),code,0);
  receiveCommand_u16_IgnoreArg_value();
  receiveCommand_u16_ReturnThruPtr_value(&value);

  TEST_ASSERT_EQUAL(value,getHEQEmptyCounts(&driver));
}

void test_setHEQNormalizationFactor_null_driver_pointer(void)
{
  setHEQNormalizationFactor(0,0);
}

void test_setHEQNormalizationFactor_should_work(void)
{
  uint16_t value = rand()*rand();
  uint16_t code = rand()*rand();
  cmdCode_ExpectAndReturn(LEP_CID_AGC_HEQ_NORMALIZATION_FACTOR, LEP_I2C_COMMAND_TYPE_SET,code);
  sendCommand_u16_Expect(&(driver.communication),code,value);

  setHEQNormalizationFactor(&driver,value);
}

void test_getHEQNormalizationFactor_null_driver_pointer(void)
{
  getHEQNormalizationFactor(0);
}

void test_getHEQNormalizationFactor_should_work(void)
{
  uint16_t value = rand()*rand();
  uint16_t code = rand()*rand();

  cmdCode_ExpectAndReturn(LEP_CID_AGC_HEQ_NORMALIZATION_FACTOR, LEP_I2C_COMMAND_TYPE_GET,code);
  receiveCommand_u16_Expect(&(driver.communication),code,0);
  receiveCommand_u16_IgnoreArg_value();
  receiveCommand_u16_ReturnThruPtr_value(&value);

  TEST_ASSERT_EQUAL(value,getHEQNormalizationFactor(&driver));
}
