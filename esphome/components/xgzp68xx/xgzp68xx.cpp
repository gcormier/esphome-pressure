#include "xgzp68xx.h"
#include "esphome/core/hal.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "esphome/components/i2c/i2c.h"

#include <cinttypes>

namespace esphome {
namespace xgzp68xx {

static const char *const TAG = "xgzp68xx.sensor";

static const uint8_t CMD_ADDRESS = 0x30;
static const uint8_t SYSCONFIG_ADDRESS = 0xA5;
static const uint8_t PCONFIG_ADDRESS = 0xA6;
static const uint8_t READ_COMMAND = 0x0A;

void XGZP68XXComponent::update() {
  // Request temp + pressure acquisition
  this->write_register(0x30, &READ_COMMAND, 1);

  // Wait 20mS per datasheet
  this->set_timeout("measurement", 20, [this]() {
    uint8_t data[5];
    uint32_t pressure_raw;
    uint16_t temperature_raw;
    float pressure_in_pa, temperature;
    int success;

    // Read the sensor data (registers 0x04-0x08)
    success = this->read_register(0x04, data, 5);
    if (success != 0) {
      ESP_LOGE(TAG, "Failed to read sensor data! Error code: %i", success);
      return;
    }

    pressure_raw = encode_uint24(data[0], data[1], data[2]);
    temperature_raw = encode_uint16(data[3], data[4]);

    // Convert the pressure data using the formula from the datasheet
    ESP_LOGV(TAG, "Got raw pressure=%" PRIu32 ", raw temperature=%u", pressure_raw, temperature_raw);
    ESP_LOGV(TAG, "PMIN=%.2f, PMAX=%.2f", this->pmin_, this->pmax_);

    // Formula: P = sum / 2^21 * (PMAX - PMIN) + PMIN (Pa)
    // If sum >= 8388608 (2^23), then sum = sum - 2^24 (negative pressure)
    if (pressure_raw >= 8388608) {
      // Negative pressure range
      pressure_in_pa = (pressure_raw - 16777216.0f) / 2097152.0f * (this->pmax_ - this->pmin_) + this->pmin_;
    } else {
      // Positive pressure range
      pressure_in_pa = pressure_raw / 2097152.0f * (this->pmax_ - this->pmin_) + this->pmin_;
    }

    // Calculate temperature using new formula:
    // Final_T = (Inter_T - Byte1) / 2^Shift_N + 25
    int32_t inter_t;
    if (temperature_raw > 32768) {
      inter_t = temperature_raw - 65536;
    } else {
      inter_t = temperature_raw;
    }
    
    temperature = (inter_t - this->temp_byte1_) / powf(2.0f, this->temp_shift_n_) + 25.0f;

    if (this->pressure_sensor_ != nullptr)
      this->pressure_sensor_->publish_state(pressure_in_pa);

    if (this->temperature_sensor_ != nullptr)
      this->temperature_sensor_->publish_state(temperature);
  });  // end of set_timeout
}

void XGZP68XXComponent::setup() {
  uint8_t config;
  uint8_t temp_cal_reg[2];

  // Set oversampling rates in register 0x02
  // Bits [4:2] for pressure, bits [7:5] for temperature
  uint8_t oversampling_config = ((this->temperature_oversampling_ & 0b111) << 5) | 
                                 ((this->pressure_oversampling_ & 0b111) << 2);
  this->write_register(0x02, &oversampling_config, 1);
  
  ESP_LOGCONFIG(TAG, "Set pressure oversampling to %u, temperature oversampling to %u", 
                this->pressure_oversampling_, this->temperature_oversampling_);

  // Read temperature calibration values from registers 0x20 and 0x21
  this->read_register(0x20, &temp_cal_reg[0], 1);
  this->read_register(0x21, &temp_cal_reg[1], 1);
  
  // Calculate Byte1 from register 0x20
  // Bits [6:0] are the exponent of 2, bit [7] is the sign bit
  uint8_t exponent = temp_cal_reg[0] & 0x7F;
  bool is_negative = (temp_cal_reg[0] & 0x80) != 0;
  this->temp_byte1_ = (1 << exponent);
  if (is_negative) {
    this->temp_byte1_ = -this->temp_byte1_;
  }
  
  // Calculate Shift_N from register 0x21
  this->temp_shift_n_ = temp_cal_reg[1] / 10.0f;
  
  ESP_LOGCONFIG(TAG, "Temperature calibration: Byte1=%d, Shift_N=%.1f", 
                this->temp_byte1_, this->temp_shift_n_);

  // Display some sample bits to confirm we are talking to the sensor
  this->read_register(SYSCONFIG_ADDRESS, &config, 1);
  ESP_LOGCONFIG(TAG,
                "Gain value is %d\n"
                "XGZP68xx started!",
                (config >> 3) & 0b111);
}

void XGZP68XXComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "XGZP68xx:");
  LOG_SENSOR("  ", "Temperature: ", this->temperature_sensor_);
  LOG_SENSOR("  ", "Pressure: ", this->pressure_sensor_);
  LOG_I2C_DEVICE(this);
  if (this->is_failed()) {
    ESP_LOGE(TAG, "  Connection failed");
  }
  LOG_UPDATE_INTERVAL(this);
}

}  // namespace xgzp68xx
}  // namespace esphome
