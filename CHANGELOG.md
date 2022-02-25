# Smartmeter changelog

## v0.3.3 - 2022-02-25
* TimescaleDB: replaced \_time\_bucket() with timescaledb\_experimental.time\_bucket\_ng()
* created yearly\_view
* daemon does not exit on serial errors

## v0.3.2 - 2022-01-02
* set debug level in config file
* node-red use smartmeter/live topic

## v0.3.1 - 2021-05-28
* MQTT TLS support (without client certificates)
* added config file
* removed most command line arguments

## v0.3.0 - 2021-05-23
* MQTT username and password support

## v0.2.9 - 2021-05-23
* send last will testament
* remove obex codes and ramdisk
* publish online/offline status
* send email alert

## v0.2.8 - 2021-03-12
* make daemon single threaded
* handle shutdown with signals

## v0.2.7 - 2021-03-10
* add epoch ms timestamp to datagram

## v0.2.6 - 2020-01-05
* new hardware schematic with USB interface
* added MQTT support

## v0.2.0 - 2020-08-14
* initial release
