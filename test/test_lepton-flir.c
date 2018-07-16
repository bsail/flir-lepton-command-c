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
  lepton_communication_init_Ignore();
  lepton_sys_init_Ignore();
  lepton_agc_init_Ignore();
  lepton_vid_init_Ignore();
  LeptonFLiR_init(LeptonFLiR_ImageStorageMode_80x60_16bpp,
                  LeptonFLiR_TemperatureMode_Kelvin, &driver);
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

void test_getImageStorageMode_should_work(void)
{
  LeptonFLiR_ImageStorageMode mode = rand();
  driver._storageMode = mode;

  TEST_ASSERT_EQUAL(mode,driver.getImageStorageMode(&driver));
}

void test_getTemperatureMode_should_work(void)
{
  LeptonFLiR_TemperatureMode mode = rand();
  driver._tempMode = mode;

  TEST_ASSERT_EQUAL(mode,driver.getTemperatureMode(&driver));
}

void test_getImageWidth_should_work(void)
{
  driver._storageMode = LeptonFLiR_ImageStorageMode_80x60_16bpp;
  TEST_ASSERT_EQUAL(80,driver.getImageWidth(&driver));
  driver._storageMode = LeptonFLiR_ImageStorageMode_80x60_8bpp;
  TEST_ASSERT_EQUAL(80,driver.getImageWidth(&driver));
  driver._storageMode = LeptonFLiR_ImageStorageMode_40x30_16bpp;
  TEST_ASSERT_EQUAL(40,driver.getImageWidth(&driver));
  driver._storageMode = LeptonFLiR_ImageStorageMode_40x30_8bpp;
  TEST_ASSERT_EQUAL(40,driver.getImageWidth(&driver));
  driver._storageMode = LeptonFLiR_ImageStorageMode_20x15_16bpp;
  TEST_ASSERT_EQUAL(20,driver.getImageWidth(&driver));
  driver._storageMode = LeptonFLiR_ImageStorageMode_20x15_8bpp;
  TEST_ASSERT_EQUAL(20,driver.getImageWidth(&driver));
  driver._storageMode = LeptonFLiR_ImageStorageMode_Count;
  TEST_ASSERT_EQUAL(0,driver.getImageWidth(&driver));
}

void test_getImageHeight_should_work(void)
{
  driver._storageMode = LeptonFLiR_ImageStorageMode_80x60_16bpp;
  TEST_ASSERT_EQUAL(60,driver.getImageHeight(&driver));
  driver._storageMode = LeptonFLiR_ImageStorageMode_80x60_8bpp;
  TEST_ASSERT_EQUAL(60,driver.getImageHeight(&driver));
  driver._storageMode = LeptonFLiR_ImageStorageMode_40x30_16bpp;
  TEST_ASSERT_EQUAL(30,driver.getImageHeight(&driver));
  driver._storageMode = LeptonFLiR_ImageStorageMode_40x30_8bpp;
  TEST_ASSERT_EQUAL(30,driver.getImageHeight(&driver));
  driver._storageMode = LeptonFLiR_ImageStorageMode_20x15_16bpp;
  TEST_ASSERT_EQUAL(15,driver.getImageHeight(&driver));
  driver._storageMode = LeptonFLiR_ImageStorageMode_20x15_8bpp;
  TEST_ASSERT_EQUAL(15,driver.getImageHeight(&driver));
  driver._storageMode = LeptonFLiR_ImageStorageMode_Count;
  TEST_ASSERT_EQUAL(0,driver.getImageHeight(&driver));
}

void test_getImageBpp_should_work(void)
{
  driver._storageMode = LeptonFLiR_ImageStorageMode_80x60_16bpp;
  TEST_ASSERT_EQUAL(2,driver.getImageBpp(&driver));
  driver._storageMode = LeptonFLiR_ImageStorageMode_80x60_8bpp;
  TEST_ASSERT_EQUAL(1,driver.getImageBpp(&driver));
  driver._storageMode = LeptonFLiR_ImageStorageMode_40x30_16bpp;
  TEST_ASSERT_EQUAL(2,driver.getImageBpp(&driver));
  driver._storageMode = LeptonFLiR_ImageStorageMode_40x30_8bpp;
  TEST_ASSERT_EQUAL(1,driver.getImageBpp(&driver));
  driver._storageMode = LeptonFLiR_ImageStorageMode_20x15_16bpp;
  TEST_ASSERT_EQUAL(2,driver.getImageBpp(&driver));
  driver._storageMode = LeptonFLiR_ImageStorageMode_20x15_8bpp;
  TEST_ASSERT_EQUAL(1,driver.getImageBpp(&driver));
  driver._storageMode = LeptonFLiR_ImageStorageMode_Count;
  TEST_ASSERT_EQUAL(0,driver.getImageBpp(&driver));
}






