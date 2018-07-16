#include "unity.h"
#include "lepton-flir.h"
#include "lepton-comm-internal.h"
#include "example.h"
#include "mock_lepton-register.h"
#include "mock_fake_system.h"
#include "lepton-communication.h"
#include "mock_lepton-sys.h"
#include "mock_lepton-agc.h"
#include "mock_lepton-vid.h"
#include <alloca.h>
#include <string.h>

static struct lepton_driver driver;

void setUp(void)
{
  lepton_sys_init_Ignore();
  lepton_agc_init_Ignore();
  lepton_vid_init_Ignore();
  LeptonFLiR_init(LeptonFLiR_ImageStorageMode_80x60_16bpp,
                  LeptonFLiR_TemperatureMode_Kelvin, &driver);
  lepton_setup_callbacks(&driver);
}

void tearDown(void)
{
}

void test_waitCommandBegin_read_register_fail_1(void)
{
  readRegister_ExpectAndReturn(&(driver.communication), LEP_I2C_STATUS_REG, 0, 1);
  readRegister_IgnoreArg_value();
  TEST_ASSERT_EQUAL(0,waitCommandBegin(&(driver.communication),1000));
}

void test_waitCommandBegin_success_no_wait(void)
{
  uint16_t status;
  readRegister_ExpectAndReturn(&(driver.communication), LEP_I2C_STATUS_REG, 0, 0);
  readRegister_IgnoreArg_value();
  status = 0;
  readRegister_ReturnThruPtr_value(&status);
  TEST_ASSERT_EQUAL(1,waitCommandBegin(&(driver.communication),1000));
}

void test_waitCommandBegin_read_register_fail_2(void)
{
  uint16_t status;
  readRegister_ExpectAndReturn(&(driver.communication), LEP_I2C_STATUS_REG, 0, 0);
  readRegister_IgnoreArg_value();
  status = 1;
  readRegister_ReturnThruPtr_value(&status);
  millis_ExpectAndReturn(100);
  millis_ExpectAndReturn(101);
  delay_Expect(1);
  readRegister_ExpectAndReturn(&(driver.communication), LEP_I2C_STATUS_REG, 0, 1);
  readRegister_IgnoreArg_value();
  TEST_ASSERT_EQUAL(0,waitCommandBegin(&(driver.communication),1000));
}

void test_waitCommandBegin_timeout(void)
{
  uint16_t status;
  readRegister_ExpectAndReturn(&(driver.communication), LEP_I2C_STATUS_REG, 0, 0);
  readRegister_IgnoreArg_value();
  status = 1;
  readRegister_ReturnThruPtr_value(&status);
  millis_ExpectAndReturn(100);
  millis_ExpectAndReturn(101);
  delay_Expect(1);
  readRegister_ExpectAndReturn(&(driver.communication), LEP_I2C_STATUS_REG, 0, 0);
  readRegister_IgnoreArg_value();
  status = 1;
  readRegister_ReturnThruPtr_value(&status);
  millis_ExpectAndReturn(1101);
  TEST_ASSERT_EQUAL(0,waitCommandBegin(&(driver.communication),1000));
  TEST_ASSERT_EQUAL(LEP_TIMEOUT_ERROR,driver.communication._lastLepResult);
}

void test_waitCommandBegin_success_wait(void)
{
  uint16_t status;
  readRegister_ExpectAndReturn(&(driver.communication), LEP_I2C_STATUS_REG, 0, 0);
  readRegister_IgnoreArg_value();
  status = 1;
  readRegister_ReturnThruPtr_value(&status);
  millis_ExpectAndReturn(100);
  millis_ExpectAndReturn(101);
  delay_Expect(1);
  readRegister_ExpectAndReturn(&(driver.communication), LEP_I2C_STATUS_REG, 0, 0);
  readRegister_IgnoreArg_value();
  uint16_t newstatus = 0;
  readRegister_ReturnThruPtr_value(&newstatus);
  // millis_ExpectAndReturn(1101);
  TEST_ASSERT_EQUAL(1,waitCommandBegin(&(driver.communication),1000));
}

void test_waitCommandFinish_read_register_fail_1(void)
{
  readRegister_ExpectAndReturn(&(driver.communication), LEP_I2C_STATUS_REG, 0, 1);
  readRegister_IgnoreArg_value();
  TEST_ASSERT_EQUAL(0,waitCommandFinish(&(driver.communication),1000));
}

void test_waitCommandFinish_success_no_wait(void)
{
  uint16_t status;
  readRegister_ExpectAndReturn(&(driver.communication), LEP_I2C_STATUS_REG, 0, 0);
  readRegister_IgnoreArg_value();
  status = 0;
  readRegister_ReturnThruPtr_value(&status);
  TEST_ASSERT_EQUAL(1,waitCommandFinish(&(driver.communication),1000));
}

void test_waitCommandFinish_success_wait(void)
{
  uint16_t status;
  readRegister_ExpectAndReturn(&(driver.communication), LEP_I2C_STATUS_REG, 0, 0);
  readRegister_IgnoreArg_value();
  status = 1;
  readRegister_ReturnThruPtr_value(&status);
  millis_ExpectAndReturn(100);
  millis_ExpectAndReturn(101);
  delay_Expect(1);
  readRegister_ExpectAndReturn(&(driver.communication), LEP_I2C_STATUS_REG, 0, 0);
  readRegister_IgnoreArg_value();
  uint16_t newstatus = 0;
  readRegister_ReturnThruPtr_value(&newstatus);
  // millis_ExpectAndReturn(1101);
  TEST_ASSERT_EQUAL(1,waitCommandFinish(&(driver.communication),1000));
}

void test_waitCommandFinish_read_register_fail_2(void)
{
  uint16_t status;
  readRegister_ExpectAndReturn(&(driver.communication), LEP_I2C_STATUS_REG, 0, 0);
  readRegister_IgnoreArg_value();
  status = 1;
  readRegister_ReturnThruPtr_value(&status);
  millis_ExpectAndReturn(100);
  millis_ExpectAndReturn(101);
  delay_Expect(1);
  readRegister_ExpectAndReturn(&(driver.communication), LEP_I2C_STATUS_REG, 0, 1);
  readRegister_IgnoreArg_value();
  TEST_ASSERT_EQUAL(0,waitCommandFinish(&(driver.communication),1000));
}

void test_waitCommandFinish_timeout(void)
{
  uint16_t status;
  readRegister_ExpectAndReturn(&(driver.communication), LEP_I2C_STATUS_REG, 0, 0);
  readRegister_IgnoreArg_value();
  status = 1;
  readRegister_ReturnThruPtr_value(&status);
  millis_ExpectAndReturn(100);
  millis_ExpectAndReturn(101);
  delay_Expect(1);
  readRegister_ExpectAndReturn(&(driver.communication), LEP_I2C_STATUS_REG, 0, 0);
  readRegister_IgnoreArg_value();
  status = 1;
  readRegister_ReturnThruPtr_value(&status);
  millis_ExpectAndReturn(1101);
  TEST_ASSERT_EQUAL(0,waitCommandFinish(&(driver.communication),1000));
  TEST_ASSERT_EQUAL(LEP_TIMEOUT_ERROR,driver.communication._lastLepResult);
}

void test_writeCmdRegister_writeLengthRegisterFailed(void)
{
  uint16_t code = rand()*rand();
  uint16_t length = 16;
  uint16_t * buffer = alloca(length);
  memset(buffer,0,length);
  driver.communication.callbacks._lastI2CError = 0x04;

  writeLengthRegister_ExpectAndReturn(&(driver.communication), length, 1);

  TEST_ASSERT_EQUAL(0x04,writeCmdRegister(&(driver.communication),code,buffer,length));
}

void test_writeCmdRegister_writeDataCommonFailed(void)
{
  uint16_t code = rand()*rand();
  uint16_t length = 16;
  uint16_t * buffer = alloca(length);
  memset(buffer,0,length);
  driver.communication.callbacks._lastI2CError = 0x04;

  writeLengthRegister_ExpectAndReturn(&(driver.communication), length, 0);
  writeDataCommon_ExpectAndReturn(&(driver.communication),LEP_I2C_DATA_0_REG,buffer,0, 1);
  writeDataCommon_IgnoreArg_writeLength();

  TEST_ASSERT_EQUAL(0x04,writeCmdRegister(&(driver.communication),code,buffer,length));
}

void test_readDataRegister_selectLengthRegisterFailed(void)
{
  uint16_t length = 16;
  uint16_t * buffer = alloca(length);
  memset(buffer,0,length);
  driver.communication.callbacks._lastI2CError = 0x04;

  selectLengthRegister_ExpectAndReturn(&(driver.communication), 1);

  TEST_ASSERT_EQUAL(0x04,readDataRegister(&(driver.communication),buffer,length));
}

void test_readDataRegister_readLengthRegisterFailed(void)
{
  uint16_t length = 16;
  uint16_t * buffer = alloca(length);
  memset(buffer,0,length);
  driver.communication.callbacks._lastI2CError = 0x04;

  selectLengthRegister_ExpectAndReturn(&(driver.communication), 0);
  readLengthRegister_ExpectAndReturn(&(driver.communication),0,1);
  readLengthRegister_IgnoreArg_readLength();

  TEST_ASSERT_EQUAL(0x04,readDataRegister(&(driver.communication),buffer,length));
}

void test_readDataRegister_readLengthRegisterFailed_returned_zero_length(void)
{
  uint16_t length = 16;
  uint16_t * buffer = alloca(length);
  memset(buffer,0,length);
  driver.communication.callbacks._lastI2CError = 0x04;

  selectLengthRegister_ExpectAndReturn(&(driver.communication), 0);
  readLengthRegister_ExpectAndReturn(&(driver.communication),0,0);
  readLengthRegister_IgnoreArg_readLength();
  int readLength = 0;
  readLengthRegister_ReturnThruPtr_readLength(&readLength);

  TEST_ASSERT_EQUAL(0x04,readDataRegister(&(driver.communication),buffer,length));
}




