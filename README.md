# Smartmeter

Read energy utility meter with a simple IR dongle

## Introduction

In the beginning of this year my analogue ferraris energy counter was replaced with a [smart energy meter][1] by the electricity network operator. This made my previous [pulsemeter][2] project obsolete and I had to come up with something new. Luckily the [volkszähler.org][3] project already supports reading many of the smartmeters available on the market and the wiki turned out be a great source of information. In the end, however, I didn't stick with the [vzlogger][4] software, but created my own smartmeter project from scratch.

![Fig. 1: Easy Basiszähler](resources/eBZ_DD3_image_small.png)

## Hardware

It turns out that the 3-phase DD3 "Easy Basiszähler" energy meter is equipped with an optical interface which sends data every second. The communication protocol is described well in the [manual](resources/ebz_manual.pdf). This is one of the raw datagrams sent by the smartmeter every second with comments describing the [OBIS codes][5]:

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

The IR dongle used to collect the raw datagrams is based on the design of the [serial TTY model][6] at Volkszähler.org, but with an added USB interface:

![Fig. 2: schematic diagram](resources/IR-dongle_schematic.png)

## Software

### Smartmeter daemon program

The smartmeter daemon is responsible for collecting the serial datagrams from the IR dongle and publishing the data to a shared memory ramdisk (so it doesn't wear out SD cards, etc.) and to a MQTT broker on the network such as Mosquitto [ref: mosquitto]. The config paramters are given in a separate config file [ref: to smartmeter.conf] or on the command line. A systemctl service file [ref: smartmeter.service] for continuous operation is provided.

Help output:

```
Energy Smartmeter v0.2.2

Usage: ./build/smartmeter [options]

  -h --help         Show help message
  -V --version      Show build info
  -D --debug        Show debug messages
  -s --serial       Serial device
  -r --ramdisk      Shared memory device
  -H --host         MQTT broker host or ip
  -p --port         MQTT broker port
  -t --topic        MQTT topic to publish

Electricity tariff:
  -b --rate         Optional basic rate per month
  -k --price        Optional price per kWh
```

For example (test run):
cat resources/arguments.txt

### InfluxDB

For persistant data storage we use InfluxDB. One big advantages of a time series database like InfluxDB over a traditional SQL database is the automatic handling of data retention. With one datagram sent every second, the SQL database gets filled very quicky and querying a month full of data gets very slow.
With InfluxDB we have the possiblilty to configure retention policies and continuous queries, which automatically prune the database after 24 hours and keep only the relevant data such as daily, weekly and monthly energy consumption.

First we create the InfluxDB `smartmeter` and a user `mqtt`:
```
$ influx
CREATE DATABASE "smartmeter"
CREATE USER "mqtt" WITH PASSWORD "mqtt"
GRANT ALL ON "smartmeter" TO "mqtt"
```
Then we create a new retention policy:
```
CREATE RETENTION POLICY "rp28h" ON "smartmeter" DURATION 28h REPLICATION 1 DEFAULT
```
And last the continous queries:
```
cq1h   CREATE CONTINUOUS QUERY cq1h ON smartmeter BEGIN SELECT last(energy) - first(energy) AS energy INTO smartmeter.rp28h.hourly FROM smartmeter.rp28h.state GROUP BY time(1h) TZ('Europe/Berlin') END
cq1d   CREATE CONTINUOUS QUERY cq1d ON smartmeter BEGIN SELECT last(energy) - first(energy) AS energy, (last(energy) - first(energy)) * mean(price) + mean(rate) * 12 / 365 AS bill INTO smartmeter.autogen.daily FROM smartmeter.rp28h.state GROUP BY time(1d) TZ('Europe/Berlin') END
cq30d  CREATE CONTINUOUS QUERY cq30d ON smartmeter BEGIN SELECT sum(energy) AS energy, sum(bill) AS bill INTO smartmeter.autogen.monthly FROM smartmeter.rp370d.daily GROUP BY time(30d) TZ('Europe/Berlin') END
cq365d CREATE CONTINUOUS QUERY cq365d ON smartmeter BEGIN SELECT sum(energy) AS energy, sum(bill) AS bill INTO smartmeter.autogen.yearly FROM smartmeter.rp370d.daily GROUP BY time(365d) TZ('Europe/Berlin') END
```
The first query consolidates the one second data from the smartmeter into one hour timeslots which are kept for 28 hours. The second query calculates the daily `energy` and `bill` values. The cost per day is determined from the tariff information if given in the smartmeter config file. The subsequent queries consolidate the daily `energy` and `bill` further into new `monthly` and `yearly` summaries, respectively.

### Node Red

The Smartmeter daemon outputs json formatted fields and tags to the MQTT broker:
```
[
  {
    "lifetime":19102823,
    "energy":2113.672678,
    "power":225.09,
    "power_l1":60.70,
    "power_l2":75.30,
    "power_l3":89.09,
    "voltage_l1":232.9,
    "voltage_l2":232.2,
    "voltage_l3":232.4,
    "rate":162.72,
    "price":0.2244
  },
  {
    "serial":"EBZ5DD3BZ06ETA_107",
    "custom_id":"1EBZ0100507409",
    "device_id":"1EBZ0100507409",
    "status":"001C0104"
  }
]
```
[Fig: Node Red flow screenshot]

### Grafana

[Fig: Grafana smartmeter dashboard screenshot]

### meterN

The OBIS from the shared memory device are updated every second and this live data can be easily read by meterN. Here is a screenshot how to configure meterN:

[Fig: meterN config]

Once setup, the meter looks like this (in my installation the consumption data from the smartmeter is shown together with production data from 123solar [ref: 123solar]): 

[Fig: meterN screenshot]


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
* *Jean-Marc Louviaux* for meterN and 123Solar projects

## License

This project is licensed under the MIT license - see the [LICENSE](LICENSE) file for details

[1]: https://www.ebzgmbh.de/ "Elektronischer Basiszähler"
[2]: https://github.com/ahpohl/pulsemeter "Pulse energy meter with Arduino and simple LED sensor"
[3]: https://volkszaehler.org/ "volkszähler.org - Das Smartmeter für jeden"
[4]: https://wiki.volkszaehler.org/software/controller/vzlogger "vzlogger - a tool to read and log measurements"
[5]: https://www.promotic.eu/en/pmdoc/Subsystems/Comm/PmDrivers/IEC62056_OBIS.htm "Description of OBIS code for IEC 62056 standard protocol"
[6]: https://wiki.volkszaehler.org/hardware/controllers/ir-schreib-lesekopf-ttl-ausgang "IR-Schreib-Lesekopf, TTL-Interface"

[2]: https://oss.oetiker.ch/rrdtool/ "Round Robin Database"
[3]: https://pvoutput.org/ "PVOutput is a free service for sharing and comparing PV output data"
[4]: https://pvoutput.org/list.jsp?userid=74913 "Ilvesheim system on PVOutput"
[5]: https://123solar.org/ "123Solar Web Solar logger"
[6]: https://apps.apple.com/au/app/pvoutput-pro/id994297624 "PVOutput Pro mobile app"
[11]: https://aur.archlinux.org/packages/smartmeter "Smartmeter Arch Linux package"
