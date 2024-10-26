# Read electricity meter with a simple IR dongle

The Smartmeter project enables the readout of the IR data signal from my [smart energy meter](resources/ebz/datenblatt_dd3.pdf). To get started, an Arduino with an IR phototransistor can be used. Later I have built my own IR dongle with an USB interface for permanent installation on top of the energy meter.

The Smartmeter program outputs a JSON formatted string and sends it to a MQTT broker on the network. From there, the data is forwarded into a time series database for permanent data storage and for visualization. The complete software stack consists of the following components, which need to be installed and configured separately:

- Smartmeter program with IR dongle for data readout
- Mosquitto MQTT broker
- Node-RED
- PostgreSQL with TimescaleDB
- Grafana for visualization

## Run in Docker

The software stack is light weight in terms of necessary resources and runs on any SBC such as an Odroid C2/C4 or Raspberry Pi. Docker compose (currently only for Smartmeter) is also supported:

```
docker-compose up --build
```

## Documentation

The project documentation covers the IR dongle hardware, software configuration and database setup and is located on separate [Wiki pages](https://github.com/ahpohl/smartmeter/wiki).

## Changelog

All notable changes and releases are documented in the [CHANGELOG](CHANGELOG.md).

## License

This project is licensed under the MIT license - see the [LICENSE](LICENSE) file for details
