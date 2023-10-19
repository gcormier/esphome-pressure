### esphome status
I've contributed the code to esphome ( https://github.com/esphome/esphome/pull/5562 ) for this sensor and it should hopefully be merged sometime in October/November.

<img  src="images/render.png" width=60% />

# esphome-pressure
This is a custom device for reading differential pressure. The most useful example I can think of is monitoring your pressure
drop across your air filter. This is a great way to know when to change your filter.

It is based on an ESP32-C3-MINI, which has native USB-C for flashing.

The pressure sensor part number will control the differential range that can be expected, as well as the accuracy (+/- 2%). I've chosen +/- 0.5kPa, which is about 2" of pressure drop. Most HVAC systems are designed to have a pressure drop of 0.1" (0.025kPa).

Be careful when setting this up - I'm sure you can damage the sensor if you expose it to too much differential pressure.

<a href="https://www.tindie.com/products/gcormier/esphome-pressure/"><img src="https://d2ss6ovg47m0r5.cloudfront.net/badges/tindie-mediums.png" alt="I sell on Tindie" width="150" height="78"></a>

## Power Supply
It can be powered by USC-C, or, by a wider range of input voltages on the screw terminals. It was designed for 24VAC, which is a common voltage used in HVAC systems. It can also work with a broader range of both AC and DC input voltages. The absolute maximum voltage for the switching regulator is 40VDC, which means up to around 28VAC. The minimum DC is approximately 9VDC.

It has a 500mA polyfuse directly off the screw terminal inputs to protect your HVAC system. The USB-C port is unfused and should be protected by the upstream device.

## Flashing / Programming
The USB-C port can be used to program. The first time, the reset button must be held. After this, all subsequent programming does not require access to the button.

There is also a header for the typical serial flashing with GPIO0(9) and EN exposed. It follows my [eflashy32](https://github.com/gcormier/eflashy32) pinout.

## Enclosure
The PCB will fit inside a [Hammond 1593KBK](https://www.hammfg.com/part/1593KBK) case. You can either cut (CNC) or 3D-print the faceplates. You can superglue some magnets in the case to have it stick to your HVAC system.

