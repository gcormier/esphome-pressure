# Sensor module interface

The contract between a **carrier** (esphome-pressure, the breakout board, …) and a
**module** (xgzp6899, sdp810, …). Anything listed here must match on both sides;
change it only with a new `interface-vN` tag.

Current version: **interface-v1**

## Connector

| Side    | Part                               | Footprint                     |
|---------|------------------------------------|-------------------------------|
| Carrier | HC-SFP-20P (SFP host receptacle)   | `sensor_module:SFP+`          |
| Module  | SFP edge fingers (paddle card)     | `sensor_module:SFP_Plug`      |

## Pinout

Pin numbers follow the SFP MSA so the stock `Interface_Optical:SFP+` symbol and
the footprints line up. Only the pins below are used; the rest are left unconnected
on both sides.

| Pin(s)                   | Signal | SFP MSA name     | Notes                     |
|--------------------------|--------|------------------|---------------------------|
| 1, 10, 11, 14, 17, 20    | GND    | VeeT / VeeR      |                           |
| 4                        | SDA    | MOD-DEF2 / SDA   | Pull-ups live on carrier  |
| 5                        | SCL    | MOD-DEF1 / SCL   | Pull-ups live on carrier  |
| 15, 16                   | VDD    | VccR / VccT      | 3.3 V from carrier        |

Reserved for later: pin 6 (MOD_ABS) for module-present detect.

## Mechanical

- **Module PCB thickness: 1.0 mm.** The SFP receptacle is made for a 1.0 mm paddle
  card; 1.6 mm will not seat.
- Edge fingers: hard gold, bevelled (chamfered) edge.

## I²C addresses

| Module    | Sensor        | Address |
|-----------|---------------|---------|
| xgzp6899  | XGZP6899D     | 0x6D    |
| sdp810    | Sensirion SDP810 | 0x25 |
