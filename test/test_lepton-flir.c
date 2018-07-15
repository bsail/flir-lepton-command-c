#include "unity.h"
#include "lepton-flir.h"
#include "mock_lepton-communication.h"
#include "mock_lepton-sys.h"
#include "mock_lepton-agc.h"
#include "mock_lepton-vid.h"
#include "mock_lepton-comm-internal.h"

static struct lepton_driver driver;

void setUp(void)
{
}

void tearDown(void)
{
}

void test_getStatusRegister_null_driver_pointer(void)
{
  getStatusRegister(0);
}

void test_getStatusRegister_should_work(void)
{
  uint16_t value = rand()*rand();
  readRegister_ExpectAndReturn(&(driver.communication),LEP_I2C_STATUS_REG,0,0);
  readRegister_IgnoreArg_value();
  readRegister_ReturnThruPtr_value(&value);
  TEST_ASSERT_EQUAL(value,getStatusRegister(&driver));
}
