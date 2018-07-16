#include "example.h"
#include "fake_system.h"

static inline uint8_t lowByte(uint16_t p)
{
    return (p & 0x00FF);
}

static inline uint8_t highByte(uint16_t p)
{
    return ((p & 0xFF00) >> 8);
}

void lepton_beginTransmission(struct lepton_callbacks * this, uint8_t addr) {
    this->_lastI2CError = 0;
    // critical_i2c_lock();
    TwoWire_beginTransmission(addr);
}

uint8_t lepton_endTransmission(struct lepton_callbacks * this) {
    this->_lastI2CError = TwoWire_endTransmission();
    // critical_i2c_unlock();
    return this->_lastI2CError;
}

uint8_t lepton_requestFrom(__attribute__((unused)) struct lepton_callbacks * this, uint8_t addr, uint8_t len) {
    // critical_i2c_lock();
    uint8_t ret = TwoWire_requestFrom(addr, len);
    // critical_i2c_unlock();
    return ret;
}

size_t lepton_write16(__attribute__((unused)) struct lepton_callbacks * this, uint16_t data) {
    return TwoWire_write(highByte(data)) + TwoWire_write(lowByte(data));
}

uint8_t lepton_read(__attribute__((unused)) struct lepton_callbacks * this) {
    return (uint8_t)(TwoWire_read() & 0xFF);
}

uint16_t lepton_read16(__attribute__((unused)) struct lepton_callbacks * this) {
    return ((uint16_t)(TwoWire_read() & 0xFF) << 8) | (uint16_t)(TwoWire_read() & 0xFF);
}


void lepton_setup_callbacks(struct lepton_driver * driver)
{
  driver->init.lepton_i2cWire_beginTransmission_set_callback(driver,&lepton_beginTransmission);
  driver->init.lepton_i2cWire_endTransmission_set_callback(driver,&lepton_endTransmission);
  driver->init.lepton_i2cWire_requestFrom_set_callback(driver,&lepton_requestFrom);
  driver->init.lepton_i2cWire_write16_set_callback(driver,&lepton_write16);
  driver->init.lepton_i2cWire_read_set_callback(driver,&lepton_read);
  driver->init.lepton_i2cWire_read16_set_callback(driver,&lepton_read16);
  driver->init.lepton_i2cWire_set_buffer_length(driver,200);
  driver->init.lepton_millis_set_callback(driver,&millis);
  driver->init.lepton_delay_set_callback(driver,&delay);
  driver->init.mutex_lock_set_callback(driver,&critical_i2c_lock);
  driver->init.mutex_unlock_set_callback(driver,&critical_i2c_unlock);
}
