#include "unity.h"
#include "lepton-flir.h"
#include "lepton-register.h"
#include "mock_lepton-comm-internal.h"
#include "example.h"
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
