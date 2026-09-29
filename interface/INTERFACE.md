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

Schematic symbol for both sides: `sensor_module:SensorModule` (in
`sensor_module.kicad_sym`). Set the footprint per side as above.

## Naming and direction

One vocabulary for every protocol, borrowed from SPI's COPI/CIPO:

- **C = controller = the carrier. P = peripheral = the module.**
- **C2P** = controller → peripheral, **P2C** = peripheral → controller.
- SPI: `SPI_COPI` (formerly MOSI) and `SPI_CIPO` (formerly MISO).
- UART: `UART_C2P` and `UART_P2C`, never TX/RX. Whoever drives the wire connects its
  TX to it: the carrier's TX goes to `UART_C2P`, the module's TX goes to `UART_P2C`.
  Both sides use the same net names, so there is nothing to cross over.

Every pin keeps the **direction the SFP MSA gives it** (host = carrier). Host-driven
SFP pins (TD±, TX_DISABLE, RS0/1) carry C2P signals; module-driven ones (RD±,
TX_FAULT, RX_LOS) carry P2C signals. Outputs never face outputs, even if a real SFP
optic ends up in a carrier.

## Pinout

Pin numbers follow the SFP MSA, so the stock `Interface_Optical:SFP+` symbol still
lines up with the footprints.

| Pin(s)                | Signal      | Dir   | SFP MSA name     | Notes                                          |
|-----------------------|-------------|-------|------------------|------------------------------------------------|
| 1, 10, 11, 14, 17, 20 | GND         | —     | VeeT / VeeR      |                                                |
| 15, 16                | VDD         | C→P   | VccR / VccT      | 3.3 V from carrier; all logic is 3.3 V         |
| 4                     | SDA         | ↔     | MOD-DEF2 / SDA   | Pull-up on carrier                             |
| 5                     | SCL         | C→P   | MOD-DEF1 / SCL   | Pull-up on carrier                             |
| 18                    | SPI_SCK     | C→P   | TD+              |                                                |
| 19                    | SPI_COPI    | C→P   | TD−              | a.k.a. MOSI                                    |
| 12                    | SPI_CIPO    | P→C   | RD−              | a.k.a. MISO; module tri-states when CS is high |
| 7                     | ~SPI_CS     | C→P   | RS0              | Active low; pull-up on carrier                 |
| 3                     | UART_C2P    | C→P   | TX_DISABLE       | Carrier TX → module RX                         |
| 8                     | UART_P2C    | P→C   | RX_LOS           | Module TX → carrier RX; pull-up on carrier     |
| 2                     | ~INT        | P→C   | TX_FAULT         | Open-drain, active low; pull-up on carrier     |
| 6                     | MOD_ABS     | P→C   | MOD_ABS          | Module ties to GND; pull-up on carrier = absent |
| 9                     | SPARE_C2P   | C→P   | RS1              | Reserved                                       |
| 13                    | SPARE_P2C   | P→C   | RD+              | Reserved                                       |

### Rules

- **Carrier** fits pull-ups on SDA, SCL, ~SPI_CS, UART_P2C, ~INT and MOD_ABS, so every
  line idles in a defined state whatever module is (or isn't) plugged in.
- **Module** connects only what it uses and leaves the rest unconnected. It never
  drives a C2P pin, and it drives CIPO only while ~SPI_CS is low.
- The spares keep their direction when they get assigned (a C2P spare stays C2P).

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
