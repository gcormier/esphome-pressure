#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace xgzp68xx {

class XGZP68XXComponent : public PollingComponent, public sensor::Sensor, public i2c::I2CDevice {
 public:
  SUB_SENSOR(temperature)
  SUB_SENSOR(pressure)
  void set_pmin(float pmin) { this->pmin_ = pmin; }
  void set_pmax(float pmax) { this->pmax_ = pmax; }
  void set_pressure_oversampling(uint8_t oversampling) { this->pressure_oversampling_ = oversampling; }
  void set_temperature_oversampling(uint8_t oversampling) { this->temperature_oversampling_ = oversampling; }

  void update() override;
  void setup() override;
  void dump_config() override;

 protected:
  /// Internal method to read the pressure from the component after it has been scheduled.
  void read_pressure_();
  float pmin_;
  float pmax_;
  uint8_t pressure_oversampling_;
  uint8_t temperature_oversampling_;
  int32_t temp_byte1_;
  float temp_shift_n_;
};

}  // namespace xgzp68xx
}  // namespace esphome
