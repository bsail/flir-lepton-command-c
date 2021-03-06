#include "unity.h"
#include "lepton-config.h"
#include "lepton-flir.h"
#include "lepton-flir-defs.h"
#include "mock_lepton-communication.h"
#include "mock_lepton-sys.h"
#include "mock_lepton-agc.h"
#include "mock_lepton-vid.h"
#include "mock_lepton-comm-internal.h"
#include "mock_lepton-register.h"
#include <alloca.h>
#include <string.h>

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

void test_wordsToHexString_normal(void)
{
  uint16_t buffer_length = 16;
  uint16_t input[4] = {0x0123,0x4567,0x89AB,0xCDEF};
  char *buffer = alloca(buffer_length);
  memset(buffer,0,buffer_length);
  driver.misc.wordsToHexString(input, 4,buffer,buffer_length);
  TEST_ASSERT_EQUAL_MEMORY("0123456789ABCDEF",buffer,buffer_length);
}

void test_wordsToHexString_small_buffer(void)
{
  uint16_t buffer_length = 4;
  uint16_t input[4] = {0x0123,0x4567,0x89AB,0xCDEF};
  char *buffer = alloca(buffer_length);
  memset(buffer,0,buffer_length);
  driver.misc.wordsToHexString(input, 4,buffer,buffer_length);
  TEST_ASSERT_EQUAL_MEMORY("01234",buffer,buffer_length);
}

void test_wordsToHexString_large_buffer(void)
{
  uint16_t buffer_length = 32;
  uint16_t input[4] = {0x0123,0x4567,0x89AB,0xCDEF};
  char *buffer = alloca(buffer_length);
  memset(buffer,0,buffer_length);
  char *ideal = alloca(buffer_length);
  memset(ideal,0,buffer_length);
  strcpy(ideal,"0123:4567:89AB:CDEF");
  driver.misc.wordsToHexString(input, 4,buffer,buffer_length);
  TEST_ASSERT_EQUAL_MEMORY(ideal,buffer,buffer_length);
}

void test_kelvin100ToCelsius(void)
{
  TEST_ASSERT_EQUAL_FLOAT(-273.15f,driver.misc.kelvin100ToCelsius(0));
  TEST_ASSERT_EQUAL_FLOAT(0.0f,driver.misc.kelvin100ToCelsius(27315UL));
  TEST_ASSERT_EQUAL_FLOAT(100.0f,driver.misc.kelvin100ToCelsius(37315UL));
}

void test_kelvin100ToFahrenheit(void)
{
  TEST_ASSERT_EQUAL_FLOAT(-459.67f,driver.misc.kelvin100ToFahrenheit(0));
  TEST_ASSERT_EQUAL_FLOAT(32.0f,driver.misc.kelvin100ToFahrenheit(27315UL));
  TEST_ASSERT_EQUAL_FLOAT(212.0f,driver.misc.kelvin100ToFahrenheit(37315UL));
}

void test_celsiusToKelvin100(void)
{
  TEST_ASSERT_EQUAL_UINT16(27315UL,driver.misc.celsiusToKelvin100(0.0f));
  TEST_ASSERT_EQUAL_UINT16(17315UL,driver.misc.celsiusToKelvin100(-100.0f));
  TEST_ASSERT_EQUAL_UINT16(37315UL,driver.misc.celsiusToKelvin100(100.0f));
}

void test_fahrenheitToKelvin100(void)
{
  TEST_ASSERT_EQUAL_UINT16(25537UL,driver.misc.fahrenheitToKelvin100(0.0f));
  TEST_ASSERT_EQUAL_UINT16(19982UL,driver.misc.fahrenheitToKelvin100(-100.0f));
  TEST_ASSERT_EQUAL_UINT16(31093UL,driver.misc.fahrenheitToKelvin100(100.0f));
}

void test_kelvinToKelvin100(void)
{
  TEST_ASSERT_EQUAL_UINT16(0UL,driver.misc.kelvinToKelvin100(0.0f));
  TEST_ASSERT_EQUAL_UINT16(10000UL,driver.misc.kelvinToKelvin100(100.0f));
  TEST_ASSERT_EQUAL_UINT16(21110UL,driver.misc.kelvinToKelvin100(211.1f));
}

void test_getTemperatureSymbol(void)
{
  driver._tempMode = LeptonFLiR_TemperatureMode_Celsius;
  TEST_ASSERT_EQUAL_STRING("C",driver.misc.getTemperatureSymbol(&driver));
  driver._tempMode = LeptonFLiR_TemperatureMode_Fahrenheit;
  TEST_ASSERT_EQUAL_STRING("F",driver.misc.getTemperatureSymbol(&driver));
  driver._tempMode = LeptonFLiR_TemperatureMode_Kelvin;
  TEST_ASSERT_EQUAL_STRING("K",driver.misc.getTemperatureSymbol(&driver));
  driver._tempMode = LeptonFLiR_TemperatureMode_Count;
  TEST_ASSERT_EQUAL_STRING("",driver.misc.getTemperatureSymbol(&driver));
}

void test_getLastI2CError(void)
{
  int8_t error = rand();
  driver.communication.callbacks._lastI2CError = error;
  TEST_ASSERT_EQUAL(error,driver.getLastI2CError(&driver));
}

void test_getLastLepResult(void)
{
  int8_t error = rand();
  driver.communication._lastLepResult = error;
  TEST_ASSERT_EQUAL(error,driver.getLastLepResult(&driver));
}

void test_kelvin100ToTemperature(void)
{
  driver._tempMode = LeptonFLiR_TemperatureMode_Celsius;
  TEST_ASSERT_EQUAL_FLOAT(-149.7f,driver.misc.kelvin100ToTemperature(&driver,12345UL));
  driver._tempMode = LeptonFLiR_TemperatureMode_Fahrenheit;
  TEST_ASSERT_EQUAL_FLOAT(-237.46f,driver.misc.kelvin100ToTemperature(&driver,12345UL));
  driver._tempMode = LeptonFLiR_TemperatureMode_Kelvin;
  TEST_ASSERT_EQUAL_FLOAT(123.45f,driver.misc.kelvin100ToTemperature(&driver,12345UL));
  driver._tempMode = LeptonFLiR_TemperatureMode_Count;
  TEST_ASSERT_EQUAL_FLOAT(0.0f,driver.misc.kelvin100ToTemperature(&driver,12345UL));
}

void test_temperatureToKelvin100(void)
{
  driver._tempMode = LeptonFLiR_TemperatureMode_Celsius;
  TEST_ASSERT_EQUAL_UINT16(39660UL,driver.misc.temperatureToKelvin100(&driver,123.45f));
  driver._tempMode = LeptonFLiR_TemperatureMode_Fahrenheit;
  TEST_ASSERT_EQUAL_UINT16(32396UL,driver.misc.temperatureToKelvin100(&driver,123.45f));
  driver._tempMode = LeptonFLiR_TemperatureMode_Kelvin;
  TEST_ASSERT_EQUAL_UINT16(12345UL,driver.misc.temperatureToKelvin100(&driver,123.45f));
  driver._tempMode = LeptonFLiR_TemperatureMode_Count;
  TEST_ASSERT_EQUAL_UINT16(0,driver.misc.temperatureToKelvin100(&driver,123.45f));
}

void test_getLastI2CErrorString_null_driver(void)
{
  TEST_ASSERT_EQUAL_PTR(0,driver.getLastI2CErrorString(0));
}

void test_getLastI2CErrorString_should_work(void)
{
  driver.communication.callbacks._lastI2CError = 0;
  TEST_ASSERT_EQUAL_STRING(LEP_I2C_ERROR_STRING_0,driver.getLastI2CErrorString(&driver));
  driver.communication.callbacks._lastI2CError = 1;
  TEST_ASSERT_EQUAL_STRING(LEP_I2C_ERROR_STRING_1,driver.getLastI2CErrorString(&driver));
  driver.communication.callbacks._lastI2CError = 2;
  TEST_ASSERT_EQUAL_STRING(LEP_I2C_ERROR_STRING_2,driver.getLastI2CErrorString(&driver));
  driver.communication.callbacks._lastI2CError = 3;
  TEST_ASSERT_EQUAL_STRING(LEP_I2C_ERROR_STRING_3,driver.getLastI2CErrorString(&driver));
  driver.communication.callbacks._lastI2CError = 4;
  TEST_ASSERT_EQUAL_STRING(LEP_I2C_ERROR_STRING_4,driver.getLastI2CErrorString(&driver));
}

void test_getLastLepResultString_null_driver(void)
{
  TEST_ASSERT_EQUAL_PTR(0,driver.getLastLepResultString(0));
}

void test_getLastLepResultString_should_work(void)
{
  driver.communication._lastLepResult = LEP_OK;
  TEST_ASSERT_EQUAL_STRING(LEP_OK_STRING,driver.getLastLepResultString(&driver));
  driver.communication._lastLepResult = LEP_ERROR;
  TEST_ASSERT_EQUAL_STRING(LEP_ERROR_STRING,driver.getLastLepResultString(&driver));
  driver.communication._lastLepResult = LEP_NOT_READY;
  TEST_ASSERT_EQUAL_STRING(LEP_NOT_READY_STRING,driver.getLastLepResultString(&driver));
  driver.communication._lastLepResult = LEP_RANGE_ERROR;
  TEST_ASSERT_EQUAL_STRING(LEP_RANGE_ERROR_STRING,driver.getLastLepResultString(&driver));
  driver.communication._lastLepResult = LEP_CHECKSUM_ERROR;
  TEST_ASSERT_EQUAL_STRING(LEP_CHECKSUM_ERROR_STRING,driver.getLastLepResultString(&driver));
  driver.communication._lastLepResult = LEP_BAD_ARG_POINTER_ERROR;
  TEST_ASSERT_EQUAL_STRING(LEP_BAD_ARG_POINTER_ERROR_STRING,driver.getLastLepResultString(&driver));
  driver.communication._lastLepResult = LEP_DATA_SIZE_ERROR;
  TEST_ASSERT_EQUAL_STRING(LEP_DATA_SIZE_ERROR_STRING,driver.getLastLepResultString(&driver));
  driver.communication._lastLepResult = LEP_UNDEFINED_FUNCTION_ERROR;
  TEST_ASSERT_EQUAL_STRING(LEP_UNDEFINED_FUNCTION_ERROR_STRING,driver.getLastLepResultString(&driver));
  driver.communication._lastLepResult = LEP_FUNCTION_NOT_SUPPORTED;
  TEST_ASSERT_EQUAL_STRING(LEP_FUNCTION_NOT_SUPPORTED_STRING,driver.getLastLepResultString(&driver));
  driver.communication._lastLepResult = LEP_OTP_WRITE_ERROR;
  TEST_ASSERT_EQUAL_STRING(LEP_OTP_WRITE_ERROR_STRING,driver.getLastLepResultString(&driver));
  driver.communication._lastLepResult = LEP_OTP_READ_ERROR;
  TEST_ASSERT_EQUAL_STRING(LEP_OTP_READ_ERROR_STRING,driver.getLastLepResultString(&driver));
  driver.communication._lastLepResult = LEP_OTP_NOT_PROGRAMMED_ERROR;
  TEST_ASSERT_EQUAL_STRING(LEP_OTP_NOT_PROGRAMMED_ERROR_STRING,driver.getLastLepResultString(&driver));
  driver.communication._lastLepResult = LEP_ERROR_I2C_BUS_NOT_READY;
  TEST_ASSERT_EQUAL_STRING(LEP_ERROR_I2C_BUS_NOT_READY_STRING,driver.getLastLepResultString(&driver));
  driver.communication._lastLepResult = LEP_ERROR_I2C_BUFFER_OVERFLOW;
  TEST_ASSERT_EQUAL_STRING(LEP_ERROR_I2C_BUFFER_OVERFLOW_STRING,driver.getLastLepResultString(&driver));
  driver.communication._lastLepResult = LEP_ERROR_I2C_ARBITRATION_LOST;
  TEST_ASSERT_EQUAL_STRING(LEP_ERROR_I2C_ARBITRATION_LOST_STRING,driver.getLastLepResultString(&driver));
  driver.communication._lastLepResult = LEP_ERROR_I2C_BUS_ERROR;
  TEST_ASSERT_EQUAL_STRING(LEP_ERROR_I2C_BUS_ERROR_STRING,driver.getLastLepResultString(&driver));
  driver.communication._lastLepResult = LEP_ERROR_I2C_NACK_RECEIVED;
  TEST_ASSERT_EQUAL_STRING(LEP_ERROR_I2C_NACK_RECEIVED_STRING,driver.getLastLepResultString(&driver));
  driver.communication._lastLepResult = LEP_ERROR_I2C_FAIL;
  TEST_ASSERT_EQUAL_STRING(LEP_ERROR_I2C_FAIL_STRING,driver.getLastLepResultString(&driver));
  driver.communication._lastLepResult = LEP_DIV_ZERO_ERROR;
  TEST_ASSERT_EQUAL_STRING(LEP_DIV_ZERO_ERROR_STRING,driver.getLastLepResultString(&driver));
  driver.communication._lastLepResult = LEP_COMM_PORT_NOT_OPEN;
  TEST_ASSERT_EQUAL_STRING(LEP_COMM_PORT_NOT_OPEN_STRING,driver.getLastLepResultString(&driver));
  driver.communication._lastLepResult = LEP_COMM_INVALID_PORT_ERROR;
  TEST_ASSERT_EQUAL_STRING(LEP_COMM_INVALID_PORT_ERROR_STRING,driver.getLastLepResultString(&driver));
  driver.communication._lastLepResult = LEP_COMM_RANGE_ERROR;
  TEST_ASSERT_EQUAL_STRING(LEP_COMM_RANGE_ERROR_STRING,driver.getLastLepResultString(&driver));
  driver.communication._lastLepResult = LEP_ERROR_CREATING_COMM;
  TEST_ASSERT_EQUAL_STRING(LEP_ERROR_CREATING_COMM_STRING,driver.getLastLepResultString(&driver));
  driver.communication._lastLepResult = LEP_ERROR_STARTING_COMM;
  TEST_ASSERT_EQUAL_STRING(LEP_ERROR_STARTING_COMM_STRING,driver.getLastLepResultString(&driver));
  driver.communication._lastLepResult = LEP_ERROR_CLOSING_COMM;
  TEST_ASSERT_EQUAL_STRING(LEP_ERROR_CLOSING_COMM_STRING,driver.getLastLepResultString(&driver));
  driver.communication._lastLepResult = LEP_COMM_CHECKSUM_ERROR;
  TEST_ASSERT_EQUAL_STRING(LEP_COMM_CHECKSUM_ERROR_STRING,driver.getLastLepResultString(&driver));
  driver.communication._lastLepResult = LEP_COMM_NO_DEV;
  TEST_ASSERT_EQUAL_STRING(LEP_COMM_NO_DEV_STRING,driver.getLastLepResultString(&driver));
  driver.communication._lastLepResult = LEP_TIMEOUT_ERROR;
  TEST_ASSERT_EQUAL_STRING(LEP_TIMEOUT_ERROR_STRING,driver.getLastLepResultString(&driver));
  driver.communication._lastLepResult = LEP_COMM_ERROR_WRITING_COMM;
  TEST_ASSERT_EQUAL_STRING(LEP_COMM_ERROR_WRITING_COMM_STRING,driver.getLastLepResultString(&driver));
  driver.communication._lastLepResult = LEP_COMM_ERROR_READING_COMM;
  TEST_ASSERT_EQUAL_STRING(LEP_COMM_ERROR_READING_COMM_STRING,driver.getLastLepResultString(&driver));
  driver.communication._lastLepResult = LEP_COMM_COUNT_ERROR;
  TEST_ASSERT_EQUAL_STRING(LEP_COMM_COUNT_ERROR_STRING,driver.getLastLepResultString(&driver));
  driver.communication._lastLepResult = LEP_OPERATION_CANCELED;
  TEST_ASSERT_EQUAL_STRING(LEP_OPERATION_CANCELED_STRING,driver.getLastLepResultString(&driver));
  driver.communication._lastLepResult = LEP_UNDEFINED_ERROR_CODE;
  TEST_ASSERT_EQUAL_STRING(LEP_UNDEFINED_ERROR_CODE_STRING,driver.getLastLepResultString(&driver));
  driver.communication._lastLepResult = -125;
  TEST_ASSERT_EQUAL_STRING(LEP_OTHER_ERROR_STRING,driver.getLastLepResultString(&driver));
}
