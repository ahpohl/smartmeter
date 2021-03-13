Tasks for InfluxDB 2.0 replacing former continuous queries
----------------------------------------------------------

Hourly:
*******
```
option task = {
  name: "cq1h",
  every: 1h
}
data = from(bucket: "smartmeter-live")
  |> range(start: -28h)
  |> filter(fn: (r) => r._measurement == "state" and r._field == "energy")
  |> drop(columns: ["custom_id", "device_id", "serial", "status"])
day_start = data |> aggregateWindow(every: 1h, fn: first)
day_end = data |> aggregateWindow(every: 1h, fn: last)
join(tables: {t1: day_start, t2: day_end}, on: ["_time", "_field", "_measurement", "_start", "_stop"])
  |> map(fn: (r) => ({r with _value: r._value_t2 - r._value_t1}))
  |> drop(columns: ["_value_t1", "_value_t2"])
  |> set(key: "_measurement", value: "hourly")
  |> to(bucket: "smartmeter-live")
```

Daily:
******
```
option task = {
  name: "cq1d",
  every: 1d
}
data = from(bucket: "smartmeter-live")
  |> range(start: -28h)
  |> filter(fn: (r) =>
    (r._measurement == "state" and (r._field == "energy" or r._field == "rate" or r._field == "price")))
  |> drop(columns: ["custom_id", "device_id", "serial", "status"])
  |> pivot(rowKey: ["_time"], columnKey: ["_field"], valueColumn: "_value")
day_start = data
  |> aggregateWindow(every: 1d, fn: first, column: "energy")
day_end = data
  |> aggregateWindow(every: 1d, fn: last, column: "energy")
res = join(tables: {t1: day_start, t2: day_end}, on: ["_start","_stop","_time","_measurement","price","rate"])
energy = res
  |> map(fn: (r) =>
    ({
      _measurement: r._measurement,
      _start: r._start,
      _stop: r._stop,
      _time: r._time,
      _value: r.energy_t2 - r.energy_t1,
    }))
  |> drop(columns: ["energy_t1", "r.energy_t2", "price", "rate"])
  |> set(key: "_measurement", value: "daily")
  |> set(key: "_field", value: "energy")
bill = res
  |> map(fn: (r) =>
    ({
      _measurement: r._measurement,
      _start: r._start,
      _stop: r._stop,
      _time: r._time,
      _value: (r.energy_t2 - r.energy_t1) * r.price + r.rate * 12.0 / 365.0,
    }))
  |> drop(columns: ["energy_t1", "energy_t2", "price", "rate"])
  |> set(key: "_measurement", value: "daily")
  |> set(key: "_field", value: "bill")
total = res
  |> drop(columns: ["energy_t1", "price", "rate"])
  |> rename(columns: {energy_t2: "_value"})
  |> set(key: "_measurement", value: "daily")
  |> set(key: "_field", value: "total")

union(tables: [bill, energy, total])
  |> group(columns: ["_field"], mode: "by")
  |> to(bucket: "smartmeter")
```

Monthly:
********
```
option task = {
  name: "cq1mo",
  every: 1mo
}
from(bucket: "smartmeter")
  |> range(start: -370d)
  |> filter(fn: (r) => r._measurement == "daily" and (r._field == "energy" or r._field == "bill"))
  |> aggregateWindow(every: 1mo, fn: sum)
  |> set(key: "_measurement", value: "monthly")
  |> to(bucket: "smartmeter")
```
