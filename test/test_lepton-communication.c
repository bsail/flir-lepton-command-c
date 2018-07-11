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

  waitCommandBegin_ExpectAndReturn(LEPFLIR_GEN_CMD_TIMEOUT,&comm,1);
  writeCmdRegister_ExpectAndReturn(code, NULL, 0,&comm,0);
  waitCommandFinish_ExpectAndReturn(LEPFLIR_GEN_CMD_TIMEOUT,&comm,0);

  sendCommand_raw(0xAB,&comm);
}
