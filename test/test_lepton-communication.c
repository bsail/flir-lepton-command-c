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

  sendCommand_raw(&comm,0xAB);
}
