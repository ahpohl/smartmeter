# Smartmeter

Read energy utility meter with IR dongle 

## Introduction

In the beginning of this year my analogue ferraris energy counter was replaced by the electricity network operator with a smart energy meter [ref: easy basiszähler]. This made my previous pulsemeter project [ref: pulemeter] obsolete and I had to come up with something new. Luckily the volkszähler.org project [ref: volkszähler] already supports reading many of the smartmeters available on the market and the website turned out be a great source of information. In the end, however, I didn't stick with vzlogger [ref: vzlogger] software, but created my own smartmeter project from scratch.

## Hardware

It turns out that the Easy Basiszähler is equipped with an optical interface which sends data every second. The communication protocol is described well in the manual [ref: ebz manual]. The data is read with an IR dongle connected to an Arduino One

![Fig. 2: photo of electrical switchbox](resources/smartmeter.jpg)

The components are wired according to the following scheme:

![Fig. 1: schematic diagram](resources/schematic.png)

## Software


### Smartmeter daemon program


### meterN


### InfluxDB with Grafana


## Installation


## Changelog

All notable changes and releases are documented in the [CHANGELOG](CHANGELOG.md).

## Acknowledgements

* *Volkszähler.org* project
* *Tobias Oetiker* for the Round Robin Database library
* *The Mosquitto* project team
* *The PVOutput Team* for providing an excellent website with a simple to use API
* *Grafana and InfluxDB* software stacks
* *Easy Basiszähler* hardware
* *Jean-Marc Louviaux* for meterN and 123Solar projects

## License

This project is licensed under the MIT license - see the [LICENSE](LICENSE) file for details

[2]: https://oss.oetiker.ch/rrdtool/ "Round Robin Database"
[3]: https://pvoutput.org/ "PVOutput is a free service for sharing and comparing PV output data"
[4]: https://pvoutput.org/list.jsp?userid=74913 "Ilvesheim system on PVOutput"
[5]: https://123solar.org/ "123Solar Web Solar logger"
[6]: https://apps.apple.com/au/app/pvoutput-pro/id994297624 "PVOutput Pro mobile app"
[11]: https://aur.archlinux.org/packages/smartmeter "Smartmeter Arch Linux package"
