#include "unity.h"
#include "lepton-agc.h"
#include "lepton-flir-defs.h"
#include "lepton-flir.h"
#include "mock_lepton-vid.h"
#include "mock_lepton-sys.h"
#include "mock_lepton-comm-internal.h"
#include "mock_lepton-communication.h"

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
  LEP_AGC_POLICY policy = rand()*rand()*rand()*rand();
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
  LEP_AGC_HEQ_SCALE_FACTOR factor = rand()*rand()*rand()*rand();
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

void test_getAGCCalcEnabled_null_driver_pointer(void)
{
  getAGCCalcEnabled(0);
}

void test_setHistogramRegion_null_driver_pointer(void)
{
  LEP_AGC_HISTOGRAM_ROI region;
  setHistogramRegion(0, &region);
}

void test_getHistogramRegion_null_driver_pointer(void)
{
  LEP_AGC_HISTOGRAM_ROI region;
  getHistogramRegion(0,&region);
}

void test_getHistogramStatistics_null_driver_pointer(void)
{
  LEP_AGC_HISTOGRAM_STATISTICS statistics;
  getHistogramStatistics(0,&statistics);
}

void test_setHistogramClipPercent_null_driver_pointer(void)
{
  setHistogramClipPercent(0,0);
}

void test_getHistogramClipPercent_null_driver_pointer(void)
{
  getHistogramClipPercent(0);
}

void test_setHistogramTailSize_null_driver_pointer(void)
{
  setHistogramTailSize(0,0);
}

void test_getHistogramTailSize_null_driver_pointer(void)
{
  getHistogramTailSize(0);
}

void test_setLinearMaxGain_null_driver_pointer(void)
{
  setLinearMaxGain(0,0);
}

void test_getLinearMaxGain_null_driver_pointer(void)
{
  getLinearMaxGain(0);
}

void test_setLinearMidpoint_null_driver_pointer(void)
{
  setLinearMidpoint(0,0);
}

void test_getLinearMidpoint_null_driver_pointer(void)
{
  getLinearMidpoint(0);
}

void test_setLinearDampeningFactor_null_driver_pointer(void)
{
  setLinearDampeningFactor(0,0);
}

void test_getLinearDampeningFactor_null_driver_pointer(void)
{
  getLinearDampeningFactor(0);
}

void test_setHEQDampeningFactor_null_driver_pointer(void)
{
  setHEQDampeningFactor(0,0);
}

void test_getHEQDampeningFactor_null_driver_pointer(void)
{
  getHEQDampeningFactor(0);
}

void test_setHEQMaxGain_null_driver_pointer(void)
{
  setHEQMaxGain(0,0);
}

void test_getHEQMaxGain_null_driver_pointer(void)
{
  getHEQMaxGain(0);
}

void test_setHEQClipLimitHigh_null_driver_pointer(void)
{
  setHEQClipLimitHigh(0,0);
}

void test_getHEQClipLimitHigh_null_driver_pointer(void)
{
  getHEQClipLimitHigh(0);
}

void test_setHEQClipLimitLow_null_driver_pointer(void)
{
  setHEQClipLimitLow(0,0);
}

void test_getHEQClipLimitLow_null_driver_pointer(void)
{
  getHEQClipLimitLow(0);
}

void test_setHEQBinExtension_null_driver_pointer(void)
{
  setHEQBinExtension(0,0);
}

void test_getHEQBinExtension_null_driver_pointer(void)
{
  getHEQBinExtension(0);
}

void test_setHEQMidpoint_null_driver_pointer(void)
{
  setHEQMidpoint(0,0);
}

void test_getHEQMidpoint_null_driver_pointer(void)
{
  getHEQMidpoint(0);
}

void test_setHEQEmptyCounts_null_driver_pointer(void)
{
  setHEQEmptyCounts(0,0);
}

void test_getHEQEmptyCounts_null_driver_pointer(void)
{
  getHEQEmptyCounts(0);
}

void test_setHEQNormalizationFactor_null_driver_pointer(void)
{
  setHEQNormalizationFactor(0,0);
}

void test_getHEQNormalizationFactor_null_driver_pointer(void)
{
  getHEQNormalizationFactor(0);
}

