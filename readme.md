# sensor_module

Plug-in sensor modules on an SFP connector (I²C, SPI and UART), plus the carriers that accept them.
The electrical and mechanical contract is in [interface/INTERFACE.md](interface/INTERFACE.md).

```
interface/            shared symbols, footprints, 3D models + INTERFACE.md
modules/<name>/       one KiCad project per sensor module (xgzp6899, sdp810, ...)
carriers/breakout/    bench carrier: SFP receptacle -> 4-pin header
```

Production carriers (eg [esphome-pressure](https://github.com/gcormier/esphome-pressure))
live in their own repos and pull this repo in as a git submodule pinned to an
`interface-vN` tag.

## Libraries

Every project references the shared library relatively, so nothing needs to be
configured in KiCad:

- modules / carriers here: `${KIPRJMOD}/../../interface/...`
- external carriers: `${KIPRJMOD}/lib/sensor_module/interface/...`

## Releases

CI builds every project under `modules/` and `carriers/`. To release one board, tag it
`<project>-v<N>`, eg `xgzp6899-v1` or `breakout-v2`.

To add a module, copy an existing one into `modules/<new>/`, rename the project
(File > Save As, or rename all four `.kicad_*` files together) and swap the sensor.
