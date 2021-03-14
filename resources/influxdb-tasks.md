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
day_start = data
	|> first()
day_end = data
	|> last()
energy = union(tables: [day_start, day_end])
  |> filter(fn: (r) =>
    (r._field == "energy"))
  |> group()
	|> difference()
	|> set(key: "_field", value: "energy")
plan = day_end
  |> filter(fn: (r) =>
    (r._field == "rate" or r._field == "price"))
total = day_end
  |> filter(fn: (r) =>
    (r._field == "energy"))
  |> set(key: "_field", value: "total")
res = union(tables: [energy, plan, total])
  |> group(columns: ["_field"], mode: "by")
bill = res
  |> pivot(
       rowKey: ["_time", "_start", "_stop", "_measurement"], 
       columnKey: ["_field"], 
       valueColumn: "_value"
     )
	|> map(fn: (r) =>
		({r with _value: r.energy * r.price + r.rate * 12.0 / 365.0}))
	|> drop(columns: ["energy", "price", "rate", "total"])
	|> set(key: "_field", value: "bill")

union(tables: [res, bill])
	|> group(columns: ["_field"], mode: "by")
  |> set(key: "_measurement", value: "daily")
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
