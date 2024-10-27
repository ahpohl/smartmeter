# Overview

The project started when my mechanical [Ferraris energy counter](https://en.wikipedia.org/wiki/Electricity_meter) was replaced with a digital [smart meter](resources/ebz/datenblatt_dd3.pdf) from eBZ. The DD3 model provides an IR signal which can be read once a second with a simple IR receiver. Initially I used just an Arduino with a [photo transistor circuit](https://github.com/ahpohl/smartmeter/wiki/Arduino-breadboard) on a breadboard to read the signal. Later I have built an [IR dongle](https://github.com/ahpohl/smartmeter/wiki/IR-dongle-pcb) on a real PCB in a nice case for permanent mounting on top of the smart meter.

![Infrared sensor mounted on top of smart meter](resources/ir-dongle/ir_dongle.png)

For the software side, the Smartmeter program reads the raw stream of data from the energy meter and forwards it as a JSON formatted string to a MQTT broker on the network. The data is stored in a TimescaleDB database and visualised on a Grafana dashboard. The installation of the software stack including the setup and maintainance of the database is described on separate [wiki pages](https://github.com/ahpohl/smartmeter/wiki).

## Build instructions

If you need to compile Smartmeter directly from source, a Makefile for (cross-)compilation is provided:

```
make CROSS_COMPILE=aarch64-unknown-linux-gnu
make install
```

Alternatively, there is also an Arch Linux package of [Smartmeter](https://aur.archlinux.org/packages/smartmeter) published in the AUR. 

## Installation

The Arch Linux package provides a systemd service file and installs the configuration file in `/etc/smartmeter/smartmeter.conf`. The service can be started with:

```
systemctl start smartmeter
```

When building directly from source, the daemon can be directly started with:

```
smartmeter --config resources/config/smartmeter_example.conf

```

## Run in Docker

The Arch Linux package installs a docker compose in `/etc/smartmeter/docker-compose.yaml` and a `Dockerfile`. Alternatively, these files are also in the root folder or the source. Before building and starting the container, the serial device used by the IR dongle needs to be set in `docker-compose.yaml` in addtion to `smartmeter.conf` to give Docker access to the IR dongle device.

```
docker-compose up --build
```

## Configuration

The Smartmeter program is configured with a configuration file located in `/etc/smartmeter/smartmeter.conf`. At least the serial device of the IR dongle, the MQTT hostname and topic to publish to needs to be set. Optionally, Smartmeter supports MQTT password authentication and TLS connection encryption. Finally, the electricity tariff, which usually consistsa price per kWh and a monthly (yearly) charge, needs to be set here for the current bill to be shown on the dashboard.

A minimal `smartmeter.conf` config file looks like this:

```
serial_device /dev/ttyUSB0
mqtt_topic smartmeter
mqtt_broker localhost
basic_rate 175.91
price_kwh 0.1983
```

## Smartmeter output

There are two versions of the EBz smart meter, the OD-types and the SM-types. According to the [manual](resources/ebz/ebz_manual.pdf), the OD-type uses the following communication parameters: 9600 Baud, 7 data bits, even parity and 1 stop bit. The serial data can be read directly with a terminal program such as picocom:

```
picocom -b 9600 -d 7 -y e /dev/ttyUSB0

/EBZ5DD3BZ06ETA_107                # '/' start marker, serial number

1-0:0.0.0*255(1EBZ0100507409)      # custom ID
1-0:96.1.0*255(1EBZ0100507409)     # device ID
1-0:1.8.0*255(000125.25688570*kWh) # energy meter
1-0:16.7.0*255(000259.20*W)        # total power
1-0:36.7.0*255(000075.18*W)        # L1 phase power
1-0:56.7.0*255(000092.34*W)        # L2 phase power
1-0:76.7.0*255(000091.68*W)        # L3 phase power
1-0:32.7.0*255(232.4*V)            # L1 phase voltage
1-0:52.7.0*255(231.7*V)            # L2 phase voltage
1-0:72.7.0*255(233.7*V)            # L3 phase voltage
1-0:96.5.0*255(001C0104)           # status
0-0:96.8.0*255(00104443)           # sensor lifetime in secs, 0x
!                                  # '!' end marker
```

Example JSON published by Smartmeter in the `smartmeter/live` topic:

```
[{
  "lifetime":150705410,     # sensor lifetime in secs, 0x
  "energy":17894.01494491,  # AC energy, kWh
  "power":228.46,           # AC power, W
  "power_l1":47.5,          # AC L1 phase power, W 
  "power_l2":66.68,         # AC L2 phase power, W
  "power_l3":114.28,        # AC L3 phase power, W
  "voltage_l1":233.1,       # Voltage L1 phase, V
  "voltage_l2":231.4,       # Voltage L2 phase, V
  "voltage_l3":233,         # Voltage L3 phase, V
  "status":"001C0104",      # EBz status
  "rate":246.13,            # price plan serice charge, € (or whatever)
  "price":0.2445,           # cost per kWh
  "time":1729973193908      # timestamp, secs since epoch
},
{
  "serial":"EBZ5DD3BZ06ETA_107",
  "custom_id":"1EBZ0100507409",
  "device_id":"1EBZ0100507409"
}]
```