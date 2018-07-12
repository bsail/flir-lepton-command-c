#include "unity.h"
#include "lepton-sys.h"
#include "mock_lepton-flir.h"
#include "mock_lepton-communication.h"
#include <string.h>

struct lepton_driver driver;

void setUp(void)
{
  // memset(&driver,0,sizeof(driver));
}

void tearDown(void)
{
}

void test_getCameraStatus_should_work(void)
{
  uint32_t status = rand()*rand()*rand()*rand();
  uint16_t code = rand()*rand();
  LEP_SYS_CAM_STATUS camStatus;
  camStatus.camStatus = status;

  cmdCode_ExpectAndReturn(LEP_CID_SYS_CAM_STATUS,LEP_I2C_COMMAND_TYPE_GET,code);
  receiveCommand_array_Expect(&(driver.communication),code,0,sizeof(LEP_SYS_CAM_STATUS) / 2);
  receiveCommand_array_IgnoreArg_readWords();
  receiveCommand_array_ReturnMemThruPtr_readWords((uint16_t*)&camStatus,sizeof(camStatus));

  TEST_ASSERT_EQUAL(status,getCameraStatus(&driver));
}

void test_getCameraStatus_should_not_segfault_null_pointer(void)
{
  TEST_ASSERT_EQUAL(LEP_SYSTEM_COMMUNICATION_ERROR,getCameraStatus(0));
}
