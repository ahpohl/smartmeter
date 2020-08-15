# Smartmeter

Read energy utility meter with IR dongle 

## Introduction

In the beginning of this year my analogue ferraris energy counter was replaced by the electricity network operator with a smart energy meter [ref: easy basiszähler]. This made my previous pulsemeter project [ref: pulemeter] obsolete and I had to come up with something new. Luckily the volkszähler.org project [ref: volkszähler] already supports reading many of the smartmeters available on the market and the website turned out be a great source of information. In the end, however, I didn't stick with vzlogger [ref: vzlogger] software, but created my own smartmeter project from scratch.

## Hardware

It turns out that the 3-phase Easy Basiszähler energy meter is equipped with an optical interface which sends data every second. The data is read with an IR dongle connected to an Arduino One. The communication protocol is described well in the manual [ref: ebz manual].  This is one of the raw datagrams sent by the smartmeter every second with comments describing the OBIS codes [ref: OBIS code reference]:

```
/EBZ5DD3BZ06ETA_107                # serial number

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
!
EOM
```

![Fig. 2: photo of electrical switchbox](resources/smartmeter.jpg)

The IR dongle is based on the design of the serial TTY model at Volkszähler.org [ref: IR dongle], but slightly modified and directly connected to the Arduino One's UART interface:

![Fig. 1: schematic diagram](resources/schematic.png)

## Software

### Smartmeter daemon program

The smartmeter daemon is responsible for collecting the serial datagrams from the IR dongle and publishing the data to a shared memory ramdisk (so it doesn't wear out SD cards, etc.) and to a MQTT broker on the network such as Mosquitto [ref: mosquitto]. The config paramters are given in a separate config file [ref: to smartmeter.conf] or on the command line. A systemctl service file [ref: smartmeter.service] for continuous operation is provided.

Help output:

```
Energy Smartmeter v0.2.0

Usage: ./build/smartmeter [options]

  -h --help         Show help message
  -V --version      Show build info
  -D --debug        Show debug messages
  -s --serial       Serial device
  -r --ramdisk      Shared memory device
  -H --host         MQTT broker host or ip
  -p --port         MQTT broker port
  -t --topic        MQTT topic to publish
```

For example (test run):
cat resources/arguments.txt

### meterN

The OBIS from the shared memory device are updated every second and this live data can be easily read by meterN. Here is a screenshot how to configure meterN:

[Fig: meterN config]

Once setup, the meter looks like this (in my installation the consumption data from the smartmeter is shown together with production data from 123solar [ref: 123solar]): 

[Fig: meterN screenshot]

### InfluxDB with Grafana



[Fig: Grafana screenshot]

## Installation

Via Git:
git clone https://github.com/ahpohl/smartmeter.git
make
sudo make install

or install directly from AUR:
yaourt -S smartmeter

Permanent installation:
Edit /etc/smartmeter.conf
systemctl enable smartmeter.service
systemctl start smartmeter.service

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