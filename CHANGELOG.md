# Smartmeter changelog

## v0.3.9 - 2024-11-10
* fix mqtt port config error

## v0.3.8 - 2024-10-27
* add docker compose support
* update readme
* fix compile errors
* make mqtt port optional

## v0.3.7 - 2024-03-27
* add trend based on a model
* improve serial communication

## v0.3.6 - 2023-08-25
* updated retention policies
* create time\_buckets at local time zone
* remove log enum from serial and mqtt classes
* add eclipse project and cross compile option
* dashboard shows last year comparision

## v0.3.5 - 2022-12-29
* fix mqtt status topic not retained
* automatically reconnect to broker

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
