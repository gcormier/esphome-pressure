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
- Width 20 mm. The SFP tongue and the H2 hole (M2, 2.925 mm from the tongue
  shoulder, on the centreline) are identical on every module.

### Lengths

Like M.2, modules come in a few lengths. Length **L** is measured from the tongue
shoulder to the far edge. The two far-end M2 holes sit at **L − 2.5 mm**, ±6 mm
from the centreline. A carrier provides standoffs for every length it accepts.

| Size | L       | End holes at | Modules    |
|------|---------|--------------|------------|
| L22  | 22.5 mm | 20 mm        | xgzp6899   |
| L42  | 42.5 mm | 40 mm        | sdp810     |

## I²C addresses

| Module    | Sensor        | Address |
|-----------|---------------|---------|
| xgzp6899  | XGZP6899D     | 0x6D    |
| sdp810    | Sensirion SDP810 | 0x25 |
