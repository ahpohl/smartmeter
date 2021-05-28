# Read energy utility meter with a simple IR dongle

The Smartmeter project enables the readout of the IR data signal from my smart energy meter. The necessary IR dongle is adapted from [volkszaehler.org](https://wiki.volkszaehler.org/hardware/controllers/ir-schreib-lesekopf-ttl-ausgang) with an added USB interface. Instead of building your own PCB, an Arduino with an IR phototransistor can also be used to get started. The daemon outputs a JSON formatted string to a MQTT broker on the network. From there the data is forwarded into a time series database for permanent data storage and for visualisation. 

The complete software stack consists of the following components, which need to be installed and configured separately:
- Smartmeter daemon for IR dongle control
- Mosquitto MQTT broker
- Node-RED (MQTT client, PostgreSQL and optional email alerts)
- PostgreSQL with TimescaleDB and pg_cron extenstions
- Grafana for visualisation

## Documentation

The project documentation covers the IR dongle hardware, software and database configuration and is located on separate [Wiki pages](https://github.com/ahpohl/smartmeter/wiki).

## Installation

Via Git:
```
git clone https://github.com/ahpohl/smartmeter.git
make
sudo make install
```
Or via Arch Linux package ([smartmeter](https://aur.archlinux.org/packages/smartmeter)):
```
yaourt -S smartmeter
```
Configuration and startup:
```
nano /etc/smartmeter.conf
systemctl enable smartmeter.service
systemctl start smartmeter.service
```

## Changelog

All notable changes and releases are documented in the [CHANGELOG](CHANGELOG.md).

## License

This project is licensed under the MIT license - see the [LICENSE](LICENSE) file for details
