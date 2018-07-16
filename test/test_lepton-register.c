#include "unity.h"
#include "lepton-flir.h"
#include "lepton-register.h"
#include "mock_lepton-comm-internal.h"
#include "example-lepton.h"
#include "mock_fake_system.h"
#include "lepton-communication.h"
#include "mock_lepton-sys.h"
#include "mock_lepton-agc.h"
#include "mock_lepton-vid.h"
#include <alloca.h>
#include <string.h>
#include <inttypes.h>

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

void test_readRegister_error_on_transmission_1(void)
{
  uint16_t address = rand()*rand();
  uint16_t value = rand()*rand();
  uint8_t ret = 4;//rand();

  critical_i2c_lock_Expect();
  TwoWire_beginTransmission_Expect(LEP_I2C_DEVICE_ADDRESS);
  TwoWire_write_ExpectAndReturn(((address & (0xFF00UL)) >> 8),0);
  TwoWire_write_ExpectAndReturn(((address & (0x00FFUL)) >> 0),0);
  TwoWire_endTransmission_ExpectAndReturn(ret);
  critical_i2c_unlock_Expect();

  TEST_ASSERT_EQUAL(ret,readRegister(&(driver.communication),address,&value));
}

void test_readRegister_error_on_transmission_2(void)
{
  uint16_t address = rand()*rand();
  uint16_t value = rand()*rand();
  uint8_t ret = 4;//rand();

  critical_i2c_lock_Expect();
  TwoWire_beginTransmission_Expect(LEP_I2C_DEVICE_ADDRESS);
  TwoWire_write_ExpectAndReturn(((address & (0xFF00UL)) >> 8),0);
  TwoWire_write_ExpectAndReturn(((address & (0x00FFUL)) >> 0),0);
  TwoWire_endTransmission_ExpectAndReturn(0);
  critical_i2c_unlock_Expect();

  critical_i2c_lock_Expect();
  TwoWire_requestFrom_ExpectAndReturn(LEP_I2C_DEVICE_ADDRESS,2,1);
  TwoWire_read_ExpectAndReturn(0xFF);
  critical_i2c_unlock_Expect();

  TEST_ASSERT_EQUAL(ret,readRegister(&(driver.communication),address,&value));
}

void test_readLengthRegister_bytesRead_not_aligned_by_two(void)
{
  int length;
  // uint8_t error = rand();

  critical_i2c_lock_Expect();
  TwoWire_requestFrom_ExpectAndReturn(LEP_I2C_DEVICE_ADDRESS,2,1);
  TwoWire_read_ExpectAndReturn(0x00);
  // TwoWire_read_ExpectAndReturn(0x01);
  critical_i2c_unlock_Expect();
  
  TEST_ASSERT_EQUAL(4,readLengthRegister(&(driver.communication),&length));
  TEST_ASSERT_EQUAL(4,driver.communication.callbacks._lastI2CError);
}

void test_readDataCommon_bytesRead_small_buffer(void)
{
  uint16_t data[12];
  memset(data,0,12*sizeof(uint16_t));
  uint16_t ideal[12] = {0x0001,0x0203,0x0405,0x0607,0x0809,0,0,0,0,0,0,0};
  int maxLength = 12;
  driver.communication.buffer_length = 4;
  int readLength = 10;

  critical_i2c_lock_Expect();
  TwoWire_requestFrom_ExpectAndReturn(LEP_I2C_DEVICE_ADDRESS,4,4);
  TwoWire_read_ExpectAndReturn(0x00);
  TwoWire_read_ExpectAndReturn(0x01);
  TwoWire_read_ExpectAndReturn(0x02);
  TwoWire_read_ExpectAndReturn(0x03);

  TwoWire_requestFrom_ExpectAndReturn(LEP_I2C_DEVICE_ADDRESS,4,4);
  TwoWire_read_ExpectAndReturn(0x04);
  TwoWire_read_ExpectAndReturn(0x05);
  TwoWire_read_ExpectAndReturn(0x06);
  TwoWire_read_ExpectAndReturn(0x07);

  TwoWire_requestFrom_ExpectAndReturn(LEP_I2C_DEVICE_ADDRESS,2,3);
  TwoWire_read_ExpectAndReturn(0x08);
  TwoWire_read_ExpectAndReturn(0x09);

  TwoWire_read_ExpectAndReturn(0xFF);
  // TwoWire_requestFrom_ExpectAndReturn(LEP_I2C_DEVICE_ADDRESS,1,1);

  critical_i2c_unlock_Expect();
  
  readDataCommon(&(driver.communication),data,maxLength,readLength);
  TEST_ASSERT_EQUAL(0,driver.communication.callbacks._lastI2CError);
  TEST_ASSERT_EQUAL_HEX16_ARRAY(ideal,data,12);
}
