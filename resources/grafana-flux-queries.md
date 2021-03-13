Grafana Flux queries for InfluxDB 2.0 backend
*********************************************

Total energy:
-------------
```
from(bucket: "smartmeter-live")
    |> range(start: -10s)
    |> filter(fn: (r) => r._measurement == "state" and r._field == "energy")
    |> last()
```

Energy today:
-------------
```
import "date"
today = date.truncate(t: now(), unit: 1d)
data = from(bucket: "smartmeter-live")
    |> range(start: today)
    |> filter(fn: (r) => r._measurement == "state" and r._field == "energy")
    |> drop(columns: ["custom_id","device_id","serial","status"])
day_start = data |> first()
day_end = data |> last() 
union(tables: [day_start, day_end])
    |> group()
    |> difference()
```
