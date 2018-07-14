#include "unity.h"
#include "lepton-vid.h"
#include "lepton-flir-defs.h"
#include "lepton-flir.h"
#include "mock_lepton-agc.h"
#include "mock_lepton-sys.h"
#include "mock_lepton-comm-internal.h"
#include "mock_lepton-communication.h"

struct lepton_driver driver;

void setUp(void)
{
  lepton_communication_init_Ignore();
  lepton_agc_init_Ignore();
  lepton_sys_init_Ignore();
  LeptonFLiR_init(LeptonFLiR_ImageStorageMode_80x60_16bpp,
                  LeptonFLiR_TemperatureMode_Kelvin, &driver);
}

void tearDown(void)
{
}

void test_setPolarity_null_driver(void)
{
  LEP_VID_POLARITY polarity;
  setPolarity(0,polarity);
}

void test_getPolarity_null_driver(void)
{
  getPolarity(0);
}

void test_setPseudoColorLUT_null_driver(void)
{
  LEP_VID_PCOLOR_LUT table;
  setPseudoColorLUT(0,table);
}

void test_getPseudoColorLUT_null_driver(void)
{
  getPseudoColorLUT(0);
}

void test_setFocusCalcEnabled_null_driver(void)
{
  setFocusCalcEnabled(0,0);
}

void test_getFocusCalcEnabled_null_driver(void)
{
  getFocusCalcEnabled(0);
}

void test_setFreezeEnabled_null_driver(void)
{
  setFreezeEnabled(0,0);
}

void test_getFreezeEnabled_null_driver(void)
{
  getFreezeEnabled(0);
}

void test_setUserColorLUT_null_driver(void)
{
  LEP_VID_LUT_BUFFER table;
  setUserColorLUT(0,&table);
}

void test_getUserColorLUT_null_driver(void)
{
  LEP_VID_LUT_BUFFER table;
  getUserColorLUT(0,&table);
}

void test_setFocusRegion_null_driver(void)
{
  LEP_VID_FOCUS_ROI region;
  setFocusRegion(0,&region);
}

void test_getFocusRegion_null_driver(void)
{
  LEP_VID_FOCUS_ROI region;
  getFocusRegion(0,&region);
}

void test_setFocusThreshold_null_driver(void)
{
  setFocusThreshold(0,0);
}

void test_getFocusThreshold_null_driver(void)
{
  getFocusThreshold(0);
}

void test_getFocusMetric_null_driver(void)
{
  getFocusMetric(0);
}

void test_setSceneBasedNUCEnabled_null_driver(void)
{
  setSceneBasedNUCEnabled(0,0);
}

void test_getSceneBasedNUCEnabled_null_driver(void)
{
  getSceneBasedNUCEnabled(0);
}

void test_setGamma_null_driver(void)
{
  setGamma(0,0);
}

void test_getGamma_null_driver(void)
{
  getGamma(0);
}





