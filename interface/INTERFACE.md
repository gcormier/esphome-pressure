# Sensor module interface

The contract between a **carrier** (the esphome-pressure main board) and a
**module** (hybrid, …). Anything listed here must match on both sides;
change it only with a new `interface-vN` tag.

Current version: **interface-v3** (v2: 10 mm centreline hole grid, M2 SMT nuts on carriers;
v3: SDP810 screw nuts at 10 and 34 mm replace the 30 mm nut)

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

Every signal pin keeps the **direction the SFP MSA gives it** (host = carrier).
Host-driven SFP pins (TD±, TX_DISABLE, RS0) carry C2P signals; module-driven ones
(RD−, TX_FAULT, RX_LOS) carry P2C signals. The one exception is +5V on pin 13 (RD+ in
the MSA): don't plug a real SFP optic into a carrier, it would see 5 V there and on
RS1.

## Pinout

Pin numbers follow the SFP MSA, so the stock `Interface_Optical:SFP+` symbol still
lines up with the footprints.

| Pin(s)                | Signal      | Dir   | SFP MSA name     | Notes                                          |
|-----------------------|-------------|-------|------------------|------------------------------------------------|
| 1, 10, 11, 14, 17, 20 | GND         | —     | VeeT / VeeR      |                                                |
| 15, 16                | +3V3        | C→P   | VccR / VccT      | 3.3 V supply; all logic is 3.3 V               |
| 9, 13                 | +5V         | C→P   | RS1 / RD+        | 5 V supply only, never a logic level           |
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

### Power

- Each supply has two contacts. SFP contacts are typically rated about 0.5 A each
  (check the HC-SFP-20P datasheet), so budget well under 1 A per rail.
- Mating order (set by the staggered pad lengths on `SFP_Plug`): GND first, then +3V3
  (15, 16), then +5V (9, 13) together with the signals. A module sees ground and
  3.3 V before 5 V.
- A module that needs only one rail leaves the other rail's pins unconnected.
  Never tie +3V3 and +5V together on a module.

### Rules

- **Carrier** fits pull-ups on SDA, SCL, ~SPI_CS, UART_P2C, ~INT and MOD_ABS, so every
  line idles in a defined state whatever module is (or isn't) plugged in.
- **Module** connects only what it uses and leaves the rest unconnected. It never
  drives a C2P pin, and it drives CIPO only while ~SPI_CS is low.
- 5 V parts on a module level-shift to 3.3 V before touching any interface signal.

## Mechanical

- **Module PCB thickness: 1.0 mm.** The SFP receptacle is made for a 1.0 mm paddle
  card; 1.6 mm will not seat.
- Edge fingers: hard gold, bevelled (chamfered) edge.
- Width 20 mm. The SFP tongue and the H2 hole (M2, 2.925 mm from the tongue
  shoulder, on the centreline) are identical on every module.

### Lengths and hole grid

Like M.2, modules come in a few lengths on a 10 mm grid. All positions are measured
from the tongue shoulder, on the centreline.

- Length **L = 10k + 2.5 mm**, shoulder to far edge.
- One M2 end hole (2.2 mm, NPTH) at **L − 2.5 mm** on the centreline, plus H2.
- **Bottom keepout:** at every carrier nut position short of its own end hole (10 and
  34 mm, see Carrier standoffs), a module keeps a Ø6 mm area on B.Cu/B side free of
  parts, pads and vias (tracks and pours under solder mask are fine). The carrier's
  nuts there act as rests.
- **SDP810 screws:** the SDP810 has Ø2.9 mm vertical bores through its body, 24 mm
  apart (datasheet "holes for additional mounting screws"). Its clips don't grip a
  1.0 mm card, so a module carrying one (hybrid) centres it at 22 mm and puts Ø2.2 mm
  NPTH holes at 10 and 34 mm. An **M2×14** screw (pan or socket head, with a washer)
  goes through the sensor body (10.25 mm) and the module (1.0 mm) into the carrier's
  2.5 mm nut. Measure the real body height before buying screws.
- Keep top-side parts clear of the end hole's M2 screw head (MountingHole_2.2mm_M2
  courtyard). With H2, that leaves about 5.4 mm to L − 5 mm on the centreline for parts.
- Start a new module from [modules/skeleton](../modules/skeleton): an L42 board with
  J1, H2, H1, GND pours, and the keepouts drawn as B.Cu rule areas. Dwgs.User marks
  where the L22/L32 edges and end holes go. For a shorter module, move the edge and H1,
  and delete the keepouts at or past H1.

| Size | L       | End hole at | Modules    |
|------|---------|-------------|------------|
| L22  | 22.5 mm | 20 mm       |            |
| L32  | 32.5 mm | 30 mm       | (no carrier has a 30 mm nut since v3) |
| L42  | 42.5 mm | 40 mm       | hybrid     |

### Carrier standoffs

- A carrier fits an **M2 SMT round nut, 2.5 mm high** (SMTSOM225BTR, LCSC C5301773)
  at H2 and at the end-hole position of every size it accepts. Nuts at other grid
  positions are optional rests.
- Carriers today: the main board has nuts at 2.925, 10, 34 and 40 mm (L42 only). 10 and
  34 mm take the SDP810 screws; there is no 20 mm nut (SDP810 leads) and no 30 mm nut
  (it would overlap the 34 mm one).
- 2.5 mm matches the HC-SFP-20P, where the 1.0 mm card's bottom face sits about
  2.6 mm above the host board (from the connector's STEP model). Don't use the
  3.0 mm version: it would bend the card up against the contacts.
- Footprint: `Mounting_Wuerth:Mounting_Wuerth_WA-SMSI-M2_H2.5mm_9774025243` (Ø4.35
  body, 3 mm locating hole, Ø5.8 courtyard) until it has been checked against the
  SMTSOM225BTR drawing. The pad goes to GND.
- Keep carrier parts under the module shadow lower than 2.5 mm, and keep screw heads
  (for example enclosure mounting screws) out of it.

## I²C addresses

| Module    | Sensor        | Address |
|-----------|---------------|---------|
| hybrid    | SDP810 or XGZP6899D (one fitted) | 0x25 or 0x6D |
