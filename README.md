# Reading a smartmeter with a simple IR dongle 

In the beginning of this year my analogue ferraris energy counter was replaced with a [smart energy meter](https://www.ebzgmbh.de/) by the electricity network operator. This made my previous [Pulsemeter](https://github.com/ahpohl/pulsemeter) project obsolete and I had to come up with something new. Luckily the [volkszähler.org](https://volkszaehler.org/) project already supports reading many of the smartmeters available on the market and the wiki turned out be a great source of information. In the end, however, I didn't stick with the [vzlogger](https://wiki.volkszaehler.org/software/controller/vzlogger) software, but created my own Smartmeter project from scratch.

![Fig. 1: Smartmeter with IR dongle](resources/ebz/smartmeter.png)

## Software

The software stack consists of the following components:
- Smartmeter v0.3.1
- PostgreSQL 13.2 with TimescaleDB 2.2.1 and pg_cron 1.3.1 extenstions
- Mosquitto MQTT broker 2.0.10
- Node-RED 1.3.2
- Grafana 7.5.3

## Documentation

The project documentation is provided in the [Smartmeter Wiki](https://github.com/ahpohl/smartmeter/wiki).

## Installation

Via Git:
```
git clone https://github.com/ahpohl/smartmeter.git
make
sudo make install
```
Or _via_ Arch Linux package manager:
```
sudo pacman -S smartmeter
```

## Changelog

All notable changes and releases are documented in the [CHANGELOG](CHANGELOG.md).

## License

This project is licensed under the MIT license - see the [LICENSE](LICENSE) file for details.
