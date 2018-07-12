#include "unity.h"
#include "lepton-communication.h"
#include "mock_lepton-comm-internal.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_sendCommand_raw_should_work(void)
{
  struct lepton_communication comm;
  uint16_t code = 0xAB;

  waitCommandBegin_ExpectAndReturn(&comm,LEPFLIR_GEN_CMD_TIMEOUT,1);
  writeCmdRegister_ExpectAndReturn(&comm,code, NULL, 0,0);
  waitCommandFinish_ExpectAndReturn(&comm,LEPFLIR_GEN_CMD_TIMEOUT,0);

  sendCommand_raw(&comm,code);
}

void test_sendCommand_raw_should_not_work_on_null_pointer(void)
{
  uint16_t code = 0xAB;

  sendCommand_raw(0,code);
}

void test_sendCommand_u16_should_work(void)
{
  struct lepton_communication comm;
  uint16_t code = 0xAB;
  uint16_t value = rand()*rand();

  waitCommandBegin_ExpectAndReturn(&comm,LEPFLIR_GEN_CMD_TIMEOUT,1);
  writeCmdRegister_ExpectAndReturn(&comm,code, &value, 1,0);
  waitCommandFinish_ExpectAndReturn(&comm,LEPFLIR_GEN_CMD_TIMEOUT,0);

  sendCommand_u16(&comm,code,value);
}

void test_sendCommand_u16_should_not_work_on_null_pointer(void)
{
  uint16_t code = 0xAB;
  uint16_t value = rand()*rand();

  sendCommand_u16(0,code,value);
}

void test_sendCommand_u32_should_work(void)
{
  struct lepton_communication comm;
  uint16_t code = 0xAB;
  uint32_t value = rand()*rand()*rand()*rand();

  waitCommandBegin_ExpectAndReturn(&comm,LEPFLIR_GEN_CMD_TIMEOUT,1);
  writeCmdRegister_ExpectAndReturn(&comm,code, (uint16_t *) &value, 2,0);
  waitCommandFinish_ExpectAndReturn(&comm,LEPFLIR_GEN_CMD_TIMEOUT,0);

  sendCommand_u32(&comm,code,value);
}

void test_sendCommand_u32_should_not_work_on_null_pointer(void)
{
  uint16_t code = 0xAB;
  uint32_t value = rand()*rand()*rand()*rand();

  sendCommand_u32(0,code,value);
}

void test_sendCommand_array_should_work(void)
{
  struct lepton_communication comm;
  uint16_t code = 0xAB;

  uint8_t length = 10;
  uint16_t *array = (uint16_t*) malloc(length*sizeof(uint16_t));
  for(uint8_t i =0;i<length;++i)
    array[i] = rand()*rand();

  waitCommandBegin_ExpectAndReturn(&comm,LEPFLIR_GEN_CMD_TIMEOUT,1);
  writeCmdRegister_ExpectAndReturn(&comm,code, array, length,0);
  waitCommandFinish_ExpectAndReturn(&comm,LEPFLIR_GEN_CMD_TIMEOUT,0);

  sendCommand_array(&comm,code,array, length);
}

void test_sendCommand_array_should_not_work_on_null_pointer_this(void)
{
  uint16_t code = 0xAB;

  uint8_t length = 10;
  uint16_t *array = (uint16_t*) malloc(length*sizeof(uint16_t));
  for(uint8_t i =0;i<length;++i)
    array[i] = rand()*rand();

  sendCommand_array(0,code,array,length);
}

void test_sendCommand_array_should_not_work_on_null_pointer_dataWords(void)
{
  uint16_t code = 0xAB;
  uint8_t length = 10;
  struct lepton_communication comm;
  comm._lastLepResult = 0;

  sendCommand_array(&comm,code,0,length);
  TEST_ASSERT_EQUAL_INT(LEP_UNDEFINED_ERROR_CODE,comm._lastLepResult);
}

void test_receiveCommand_u16_should_not_work_on_null_pointer_this(void)
{
  uint16_t code = 0xAB;

  uint16_t value = 0;

  receiveCommand_u16(0,code,&value);
}

void test_receiveCommand_u16_should_not_work_on_null_pointer_value(void)
{
  uint16_t code = 0xAB;
  struct lepton_communication comm;
  comm._lastLepResult = 0;

  receiveCommand_u16(&comm,code,0);
  TEST_ASSERT_EQUAL_INT(LEP_UNDEFINED_ERROR_CODE,comm._lastLepResult);
}

void test_receiveCommand_u16_should_work(void)
{
  uint16_t code = 0xAB;
  struct lepton_communication comm;
  comm._lastLepResult = 0;
  const uint16_t value_orig = rand()*rand();
  uint16_t value = value_orig;
  uint16_t received_value = 0;

  waitCommandBegin_ExpectAndReturn(&comm,LEPFLIR_GEN_CMD_TIMEOUT,1);
  writeRegister_ExpectAndReturn(&comm,LEP_I2C_COMMAND_REG,code,0);
  waitCommandFinish_ExpectAndReturn(&comm,LEPFLIR_GEN_CMD_TIMEOUT,1);

  readDataRegister_ExpectAndReturn(&comm,&received_value,1,0);
  readDataRegister_ReturnThruPtr_readWords(&value);

  receiveCommand_u16(&comm,code,&received_value);

  TEST_ASSERT_EQUAL(value_orig,received_value);
}

void test_receiveCommand_u32_should_not_work_on_null_pointer_this(void)
{
  uint16_t code = 0xAB;

  uint32_t value = 0;

  receiveCommand_u32(0,code,&value);
}

void test_receiveCommand_u32_should_not_work_on_null_pointer_value(void)
{
  uint16_t code = 0xAB;
  struct lepton_communication comm;
  comm._lastLepResult = 0;

  receiveCommand_u32(&comm,code,0);
  TEST_ASSERT_EQUAL_INT(LEP_UNDEFINED_ERROR_CODE,comm._lastLepResult);
}

void test_receiveCommand_u32_should_work(void)
{
  uint16_t code = 0xAB;
  struct lepton_communication comm;
  comm._lastLepResult = 0;
  const uint32_t value_orig = rand()*rand()*rand()*rand();
  uint32_t value = value_orig;
  uint32_t received_value = 0;

  waitCommandBegin_ExpectAndReturn(&comm,LEPFLIR_GEN_CMD_TIMEOUT,1);
  writeRegister_ExpectAndReturn(&comm,LEP_I2C_COMMAND_REG,code,0);
  waitCommandFinish_ExpectAndReturn(&comm,LEPFLIR_GEN_CMD_TIMEOUT,1);

  readDataRegister_ExpectAndReturn(&comm,(uint16_t*)&received_value,2,0);
  readDataRegister_ReturnMemThruPtr_readWords((uint16_t*)&value,sizeof(value));

  receiveCommand_u32(&comm,code,&received_value);

  TEST_ASSERT_EQUAL(value_orig,received_value);
}

void test_receiveCommand_array_should_not_work_on_null_pointer_this(void)
{
  uint16_t code = 0xAB;

  uint16_t value[2] = {rand()*rand(),rand()*rand()};
  uint16_t length = 2;

  receiveCommand_array(0,code,value,length);
}

void test_receiveCommand_array_should_not_work_on_null_pointer_value(void)
{
  uint16_t code = 0xAB;
  struct lepton_communication comm;
  comm._lastLepResult = 0;
  uint16_t length = 2;

  receiveCommand_array(&comm,code,0,length);
  TEST_ASSERT_EQUAL_INT(LEP_UNDEFINED_ERROR_CODE,comm._lastLepResult);
}

void test_receiveCommand_array_should_not_work_on_zero_length(void)
{
  uint16_t code = 0xAB;
  struct lepton_communication comm;
  comm._lastLepResult = 0;
  uint16_t value[2] = {rand()*rand(),rand()*rand()};
  uint16_t length = 0;

  receiveCommand_array(&comm,code,value,length);
  TEST_ASSERT_EQUAL_INT(LEP_UNDEFINED_ERROR_CODE,comm._lastLepResult);
}

void test_receiveCommand_array_should_work(void)
{
  uint16_t code = 0xAB;
  struct lepton_communication comm;
  comm._lastLepResult = 0;
  uint16_t length = 64;
  uint16_t *value_orig = (uint16_t *)malloc(length*sizeof(uint16_t));
  uint16_t *value = (uint16_t *)malloc(length*sizeof(uint16_t));
  uint16_t *received_value = (uint16_t *)malloc(length*sizeof(uint16_t));

  for(uint16_t i=0;i<length;++i)
  {
    value_orig[i] = rand()*rand();
    value[i] = value_orig[i];
    received_value[i] = 0;
  }

  waitCommandBegin_ExpectAndReturn(&comm,LEPFLIR_GEN_CMD_TIMEOUT,1);
  writeRegister_ExpectAndReturn(&comm,LEP_I2C_COMMAND_REG,code,0);
  waitCommandFinish_ExpectAndReturn(&comm,LEPFLIR_GEN_CMD_TIMEOUT,1);

  readDataRegister_ExpectAndReturn(&comm,received_value,length,0);
  readDataRegister_ReturnArrayThruPtr_readWords(value,length);

  receiveCommand_array(&comm,code,received_value,length);

  TEST_ASSERT_EQUAL_UINT16_ARRAY(value_orig,received_value,length);
}

