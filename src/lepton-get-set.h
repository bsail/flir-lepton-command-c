#ifndef _LEPTON_GET_SET_H
#define _LEPTON_GET_SET_H

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include "lepton-flir.h"

#ifndef TEST
#ifndef LEPTON_FLIR_INTERNAL
#error "There is no user accessable data and functions here"
#endif
#endif

static inline void get_common(struct lepton_driver *driver,
                              enum transmission_mode mode, uint16_t cmdID,
                              uint16_t * data,
                              uint16_t length)
{
  switch (mode) {
  case u16:
    receiveCommand_u16(&(driver->communication), cmdCode(cmdID, LEP_I2C_COMMAND_TYPE_GET), data);
    break;
  case u32:
    receiveCommand_u32(&(driver->communication), cmdCode(cmdID, LEP_I2C_COMMAND_TYPE_GET),
                       (uint32_t *) data);
    break;
  case array:
    receiveCommand_array(&(driver->communication), cmdCode(cmdID, LEP_I2C_COMMAND_TYPE_GET),
                         data, length);
    break;
  }
}

static inline void set_common(struct lepton_driver *driver,
                              enum transmission_mode mode, uint16_t cmdID,
                               uint16_t * data,
                              uint16_t length)
{
  switch (mode) {
  case u16:
    sendCommand_u16(&(driver->communication), cmdCode(cmdID, LEP_I2C_COMMAND_TYPE_SET), *data);
    break;
  case u32:
    sendCommand_u32(&(driver->communication), cmdCode(cmdID, LEP_I2C_COMMAND_TYPE_SET),
                       *((uint32_t*)data));
    break;
  case array:
    sendCommand_array(&(driver->communication), cmdCode(cmdID, LEP_I2C_COMMAND_TYPE_SET),
                         data, length);
    break;
  }
}

#ifdef __cplusplus
}
#endif
#endif
