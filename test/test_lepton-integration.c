#include "unity.h"
#include "lepton-agc.h"
#include "lepton-flir-defs.h"
#include "lepton-flir.h"
#include "lepton-vid.h"
#include "lepton-sys.h"
#include "lepton-comm-internal.h"
#include "lepton-communication.h"
#include <string.h>
#include "example.h"
#include "mock_fake_system.h"
#include "lepton-register.h"

struct lepton_driver driver;

static void telemetry_enabled_common(void)
{
  /* waitCommandBegin */
  critical_i2c_lock_Expect();
  TwoWire_beginTransmission_Expect(LEP_I2C_DEVICE_ADDRESS);
  TwoWire_write_ExpectAndReturn(0,0);
  TwoWire_write_ExpectAndReturn(0x02,0);
  TwoWire_endTransmission_ExpectAndReturn(0);
  critical_i2c_unlock_Expect();

  critical_i2c_lock_Expect();
  TwoWire_requestFrom_ExpectAndReturn(LEP_I2C_DEVICE_ADDRESS,2,2);
  TwoWire_read_ExpectAndReturn(0);
  TwoWire_read_ExpectAndReturn(0);
  critical_i2c_unlock_Expect();

  /* writeCmdRegister */
  critical_i2c_lock_Expect();
  TwoWire_beginTransmission_Expect(LEP_I2C_DEVICE_ADDRESS);
  TwoWire_write_ExpectAndReturn(0,0);
  TwoWire_write_ExpectAndReturn(0x06,0);
  TwoWire_write_ExpectAndReturn(0,0);
  TwoWire_write_ExpectAndReturn(0x02,0);
  TwoWire_endTransmission_ExpectAndReturn(0);
  critical_i2c_unlock_Expect();

  critical_i2c_lock_Expect();
  TwoWire_beginTransmission_Expect(LEP_I2C_DEVICE_ADDRESS);
  TwoWire_write_ExpectAndReturn(0x00,0);
  TwoWire_write_ExpectAndReturn(0x08,0);
  TwoWire_write_ExpectAndReturn(0x00,00);
  TwoWire_write_ExpectAndReturn(0x01,0);
  TwoWire_write_ExpectAndReturn(0x00,0);
  TwoWire_write_ExpectAndReturn(0x00,0);
  TwoWire_endTransmission_ExpectAndReturn(0);
  critical_i2c_unlock_Expect();

  critical_i2c_lock_Expect();
  TwoWire_beginTransmission_Expect(LEP_I2C_DEVICE_ADDRESS);
  TwoWire_write_ExpectAndReturn(0x00,0);
  TwoWire_write_ExpectAndReturn(0x04,0);
  TwoWire_write_ExpectAndReturn(0x02,0);
  TwoWire_write_ExpectAndReturn(0x19,0);
  TwoWire_endTransmission_ExpectAndReturn(0);
  critical_i2c_unlock_Expect();

  critical_i2c_lock_Expect();
  TwoWire_beginTransmission_Expect(LEP_I2C_DEVICE_ADDRESS);
  TwoWire_write_ExpectAndReturn(0,0);
  TwoWire_write_ExpectAndReturn(0x02,0);
  TwoWire_endTransmission_ExpectAndReturn(0);
  critical_i2c_unlock_Expect();

  critical_i2c_lock_Expect();
  TwoWire_requestFrom_ExpectAndReturn(LEP_I2C_DEVICE_ADDRESS,2,2);
  TwoWire_read_ExpectAndReturn(0);
  TwoWire_read_ExpectAndReturn(0);
  critical_i2c_unlock_Expect();
}

static void agc_enabled_common(void)
{
  critical_i2c_lock_Expect();
  TwoWire_beginTransmission_Expect(LEP_I2C_DEVICE_ADDRESS);
  TwoWire_write_ExpectAndReturn(0,0);
  TwoWire_write_ExpectAndReturn(0x02,0);
  TwoWire_endTransmission_ExpectAndReturn(0);
  critical_i2c_unlock_Expect();

  critical_i2c_lock_Expect();
  TwoWire_requestFrom_ExpectAndReturn(LEP_I2C_DEVICE_ADDRESS,2,2);
  TwoWire_read_ExpectAndReturn(0);
  TwoWire_read_ExpectAndReturn(0);
  critical_i2c_unlock_Expect();

  critical_i2c_lock_Expect();
  TwoWire_beginTransmission_Expect(LEP_I2C_DEVICE_ADDRESS);
  TwoWire_write_ExpectAndReturn(0,0);
  TwoWire_write_ExpectAndReturn(0x06,0);
  TwoWire_write_ExpectAndReturn(0,0);
  TwoWire_write_ExpectAndReturn(0x02,0);
  TwoWire_endTransmission_ExpectAndReturn(0);
  critical_i2c_unlock_Expect();

  critical_i2c_lock_Expect();
  TwoWire_beginTransmission_Expect(LEP_I2C_DEVICE_ADDRESS);
  TwoWire_write_ExpectAndReturn(0x00,0);
  TwoWire_write_ExpectAndReturn(0x08,0);
  TwoWire_write_ExpectAndReturn(0x00,00);
  TwoWire_write_ExpectAndReturn(0x01,0);
  TwoWire_write_ExpectAndReturn(0x00,0);
  TwoWire_write_ExpectAndReturn(0x00,0);
  TwoWire_endTransmission_ExpectAndReturn(0);
  critical_i2c_unlock_Expect();

  critical_i2c_lock_Expect();
  TwoWire_beginTransmission_Expect(LEP_I2C_DEVICE_ADDRESS);
  TwoWire_write_ExpectAndReturn(0x00,0);
  TwoWire_write_ExpectAndReturn(0x04,0);
  TwoWire_write_ExpectAndReturn(0x01,0);
  TwoWire_write_ExpectAndReturn(0x01,0);
  TwoWire_endTransmission_ExpectAndReturn(0);
  critical_i2c_unlock_Expect();

  critical_i2c_lock_Expect();
  TwoWire_beginTransmission_Expect(LEP_I2C_DEVICE_ADDRESS);
  TwoWire_write_ExpectAndReturn(0,0);
  TwoWire_write_ExpectAndReturn(0x02,0);
  TwoWire_endTransmission_ExpectAndReturn(0);
  critical_i2c_unlock_Expect();

  critical_i2c_lock_Expect();
  TwoWire_requestFrom_ExpectAndReturn(LEP_I2C_DEVICE_ADDRESS,2,2);
  TwoWire_read_ExpectAndReturn(0);
  TwoWire_read_ExpectAndReturn(0);
  critical_i2c_unlock_Expect();
}



void setUp(void)
{
  delay_Ignore();
  millis_IgnoreAndReturn(0);
  LeptonFLiR_init(LeptonFLiR_ImageStorageMode_80x60_16bpp,
                  LeptonFLiR_TemperatureMode_Kelvin, &driver);
  lepton_setup_callbacks(&driver);
}

void tearDown(void)
{
}

void test_integration_enable_telemetry(void)
{
  telemetry_enabled_common();

  driver.sys.setTelemetryEnabled(&driver,ENABLED);
}

void test_integration_enable_agc(void)
{
  agc_enabled_common();

  driver.agc.setAGCEnabled(&driver,ENABLED);
}

static void get_status_register_common(uint16_t val)
{
  /* getStatusRegister() */
  /* Read register LEP_I2C_STATUS_REG */
  /* Set register address */
  critical_i2c_lock_Expect();
  TwoWire_beginTransmission_Expect(LEP_I2C_DEVICE_ADDRESS);
  TwoWire_write_ExpectAndReturn(0,0);
  TwoWire_write_ExpectAndReturn(0x02,0);
  TwoWire_endTransmission_ExpectAndReturn(0);
  critical_i2c_unlock_Expect();

  /* Request bytes from selected address */
  critical_i2c_lock_Expect();
  TwoWire_requestFrom_ExpectAndReturn(LEP_I2C_DEVICE_ADDRESS,2,2);
  TwoWire_read_ExpectAndReturn((val & 0xFF00) >> 8);
  TwoWire_read_ExpectAndReturn(val & 0x00FF);
  critical_i2c_unlock_Expect();
}

void test_integration_get_status_register(void)
{
  uint16_t status = rand()*rand();
  get_status_register_common(status);

  TEST_ASSERT_EQUAL(status,driver.getStatusRegister(&driver));
}

static void get_uptime_common(uint32_t uptime)
{
  /* getCameraUptime() */
  /* get_common() */
  /* receiveCommand_u32() */
  /* receiveCommand_common() */
  /* waitCommandBegin() */
  /* readRegister() */
  critical_i2c_lock_Expect();
  TwoWire_beginTransmission_Expect(LEP_I2C_DEVICE_ADDRESS);
  TwoWire_write_ExpectAndReturn(0x00,0);
  TwoWire_write_ExpectAndReturn(0x02,0);
  TwoWire_endTransmission_ExpectAndReturn(0);
  critical_i2c_unlock_Expect();

  critical_i2c_lock_Expect();
  TwoWire_requestFrom_ExpectAndReturn(LEP_I2C_DEVICE_ADDRESS,2,2);
  TwoWire_read_ExpectAndReturn(0);
  TwoWire_read_ExpectAndReturn(0);
  critical_i2c_unlock_Expect();


  critical_i2c_lock_Expect();
  TwoWire_beginTransmission_Expect(LEP_I2C_DEVICE_ADDRESS);
  TwoWire_write_ExpectAndReturn(0,0);
  TwoWire_write_ExpectAndReturn(0x04,0);
  TwoWire_write_ExpectAndReturn(0x02,0);
  TwoWire_write_ExpectAndReturn(0x0C,0);
  TwoWire_endTransmission_ExpectAndReturn(0);
  critical_i2c_unlock_Expect();

  critical_i2c_lock_Expect();
  TwoWire_beginTransmission_Expect(LEP_I2C_DEVICE_ADDRESS);
  TwoWire_write_ExpectAndReturn(0x00,0);
  TwoWire_write_ExpectAndReturn(0x02,0);
  TwoWire_endTransmission_ExpectAndReturn(0);
  critical_i2c_unlock_Expect();

  critical_i2c_lock_Expect();
  TwoWire_requestFrom_ExpectAndReturn(LEP_I2C_DEVICE_ADDRESS,2,2);
  TwoWire_read_ExpectAndReturn(0);
  TwoWire_read_ExpectAndReturn(0);
  critical_i2c_unlock_Expect();



  critical_i2c_lock_Expect();
  TwoWire_beginTransmission_Expect(LEP_I2C_DEVICE_ADDRESS);
  TwoWire_write_ExpectAndReturn(0,0);
  TwoWire_write_ExpectAndReturn(0x06,0);
  TwoWire_endTransmission_ExpectAndReturn(0);
  critical_i2c_unlock_Expect();

  critical_i2c_lock_Expect();
  TwoWire_requestFrom_ExpectAndReturn(LEP_I2C_DEVICE_ADDRESS,2,2);
  TwoWire_read_ExpectAndReturn(0);
  TwoWire_read_ExpectAndReturn(4);
  critical_i2c_unlock_Expect();

  critical_i2c_lock_Expect();
  TwoWire_requestFrom_ExpectAndReturn(LEP_I2C_DEVICE_ADDRESS,4,4);
  TwoWire_read_ExpectAndReturn((uptime & 0x0000FF00) >> 8);
  TwoWire_read_ExpectAndReturn(uptime & 0x000000FF);
  TwoWire_read_ExpectAndReturn((uptime & 0xFF000000) >> 24);
  TwoWire_read_ExpectAndReturn((uptime & 0x00FF0000) >> 16);
  critical_i2c_unlock_Expect();

}

void test_integration_get_uptime(void)
{
  uint32_t uptime = rand()*rand()*rand()*rand();

  get_uptime_common(uptime);

  TEST_ASSERT_EQUAL(uptime,driver.sys.getCameraUptime(&driver));
}
