#include "unity.h"
#include "lepton-flir.h"
#include "lepton-comm-internal.h"
#include "mock_lepton-register.h"
#include "example.h"
#include "mock_fake_system.h"
#include "mock_lepton-communication.h"
#include "mock_lepton-sys.h"
#include "mock_lepton-agc.h"
#include "mock_lepton-vid.h"
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

void test_lepton_comm_internal_NeedToImplement(void)
{
  TEST_IGNORE_MESSAGE("Need to Implement lepton-comm-internal");
}
