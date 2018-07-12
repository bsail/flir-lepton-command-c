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

void test_sendCommand_array_should_not_work_on_null_pointer(void)
{
  uint16_t code = 0xAB;

  uint8_t length = 10;
  uint16_t *array = (uint16_t*) malloc(length*sizeof(uint16_t));
  for(uint8_t i =0;i<length;++i)
    array[i] = rand()*rand();

  sendCommand_array(0,code,array,length);
}












