# Assignment 2 - Virtual Automotive CAN Monitoring System

## Aim

To simulate a simple automotive CAN network with an Engine ECU, Dashboard and CAN Monitor.

## System

```text
Engine ECU ----+
               |
               +---- vcan0 ---- Dashboard
               |
               +-------------- CAN Monitor
```

## CAN messages

- `0x100` EngineData
- `0x200` VehicleSpeed

## Run

First create `vcan0`:

```bash
bash run_demo.sh
```

Then compile:

```bash
make
```

Open three terminals.

Terminal 1:

```bash
./dashboard
```

Terminal 2:

```bash
./can_monitor
```

Terminal 3:

```bash
./engine_ecu
```

## Result

The Engine ECU periodically sends RPM, temperature and speed. The Dashboard displays the decoded values and the Monitor shows the raw CAN frames.
