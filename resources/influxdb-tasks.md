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
option task = {name: "cq1d", every: 1d}

data = from(bucket: "smartmeter-live")
	|> range(start: -1d)
	|> filter(fn: (r) =>
		(r._measurement == "state" and (r._field == "energy" or r._field == "rate" or r._field == "price")))
	|> drop(columns: ["custom_id", "device_id", "serial", "status"])
	|> pivot(rowKey: ["_time"], columnKey: ["_field"], valueColumn: "_value")
day_start = data
	|> first(column: "energy")
day_end = data
	|> last(column: "energy")
res = union(tables: [day_start, day_end])
	|> group()
energy = res
	|> difference(columns: ["energy"])
	|> rename(columns: {energy: "_value"})
	|> set(key: "_measurement", value: "daily")
	|> set(key: "_field", value: "energy")
bill = energy
	|> map(fn: (r) =>
		({
			_start: r._start,
			_stop: r._stop,
			_time: r._time,
			_value: r._value * r.price + r.rate * 12.0 / 365.0,
			price: r.price,
			rate: r.rate,
		}))
	|> set(key: "_measurement", value: "daily")
	|> set(key: "_field", value: "bill")
total = day_end
	|> rename(columns: {energy: "_value"})
	|> set(key: "_measurement", value: "daily")
	|> set(key: "_field", value: "total")

union(tables: [energy, bill, total])
	|> group(columns: ["_field"], mode: "by")
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
