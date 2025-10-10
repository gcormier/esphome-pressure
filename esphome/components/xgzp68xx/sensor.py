import esphome.codegen as cg
from esphome.components import i2c, sensor
import esphome.config_validation as cv
from esphome.const import (
    CONF_ID,
    CONF_PRESSURE,
    CONF_TEMPERATURE,
    DEVICE_CLASS_PRESSURE,
    DEVICE_CLASS_TEMPERATURE,
    STATE_CLASS_MEASUREMENT,
    UNIT_CELSIUS,
    UNIT_PASCAL,
)

DEPENDENCIES = ["i2c"]
CODEOWNERS = ["@gcormier"]

CONF_PMIN = "pmin"
CONF_PMAX = "pmax"
CONF_PRESSURE_OVERSAMPLING = "pressure_oversampling"
CONF_TEMPERATURE_OVERSAMPLING = "temperature_oversampling"

# Oversampling options: 000b=256x, 001b=512x, 010b=1024x, 011b=2048x, 
#                       100b=4096x, 101b=8192x, 110b=16384x, 111b=32768x
OVERSAMPLING_OPTIONS = {
    256: 0,
    512: 1,
    1024: 2,
    2048: 3,
    4096: 4,
    8192: 5,
    16384: 6,
    32768: 7,
}

xgzp68xx_ns = cg.esphome_ns.namespace("xgzp68xx")
XGZP68XXComponent = xgzp68xx_ns.class_(
    "XGZP68XXComponent", cg.PollingComponent, i2c.I2CDevice
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(XGZP68XXComponent),
            cv.Optional(CONF_PRESSURE): sensor.sensor_schema(
                unit_of_measurement=UNIT_PASCAL,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_PRESSURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_PMIN, default=-500): cv.float_,
            cv.Optional(CONF_PMAX, default=500): cv.float_,
            cv.Optional(CONF_PRESSURE_OVERSAMPLING, default=4096): cv.enum(
                OVERSAMPLING_OPTIONS, int=True
            ),
            cv.Optional(CONF_TEMPERATURE_OVERSAMPLING, default=4096): cv.enum(
                OVERSAMPLING_OPTIONS, int=True
            ),
        }
    )
    .extend(cv.polling_component_schema("60s"))
    .extend(i2c.i2c_device_schema(0x6D))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    if temperature_config := config.get(CONF_TEMPERATURE):
        sens = await sensor.new_sensor(temperature_config)
        cg.add(var.set_temperature_sensor(sens))

    if pressure_config := config.get(CONF_PRESSURE):
        sens = await sensor.new_sensor(pressure_config)
        cg.add(var.set_pressure_sensor(sens))

    cg.add(var.set_pmin(config[CONF_PMIN]))
    cg.add(var.set_pmax(config[CONF_PMAX]))
    cg.add(var.set_pressure_oversampling(config[CONF_PRESSURE_OVERSAMPLING]))
    cg.add(var.set_temperature_oversampling(config[CONF_TEMPERATURE_OVERSAMPLING]))
