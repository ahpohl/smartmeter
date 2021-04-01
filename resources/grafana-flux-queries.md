Grafana Flux queries for InfluxDB 2.0 backend
---------------------------------------------

Total energy:
*************
```
from(bucket: "smartmeter-live")
    |> range(start: -10s)
    |> filter(fn: (r) => r._measurement == "state" and r._field == "energy")
    |> drop(columns: ["custom_id","device_id","serial","status"])
    |> last()
```

Energy today:
*************
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

Current power gauge:
********************
```
from(bucket: "smartmeter-live")
    |> range(start: -10s)
    |> filter(fn: (r) => r._measurement == "state" and r._field == "power")
    |> drop(columns: ["custom_id","device_id","serial","status"])
    |> last()
```

Power graph:
************
```
from(bucket: "smartmeter-live")
    |> range(start: v.timeRangeStart, stop: v.timeRangeStop)
    |> filter(fn: (r) => r._measurement == "hourly" and r._field == "energy")
    |> drop(columns: ["custom_id","device_id","serial","status"])
```

Last bill:
**********
```
from(bucket: "smartmeter")
    |> range(start: -60d)
    |> filter(fn: (r) => r._measurement == "monthly" and r._field == "bill")
    |> drop(columns: ["custom_id","device_id","serial","status"])
    |> last()
```

Current bill:
*************
```
import "date"
month = date.truncate(t: now(), unit: 1mo)
from(bucket: "smartmeter")
    |> range(start: month)
    |> filter(fn: (r) => r._measurement == "daily" and r._field == "bill")
    |> drop(columns: ["custom_id","device_id","serial","status"])
    |> sum()
```

Energy hour bar graph:
**********************
```
from(bucket: "smartmeter-live")
    |> range(start: v.timeRangeStart, stop: v.timeRangeStop)
    |> filter(fn: (r) => r._measurement == "hourly" and r._field == "energy")
    |> drop(columns: ["custom_id","device_id","serial","status"])
```

Daily energy/cost bar graph:
***********************
```
from(bucket: "smartmeter")
    |> range(start: v.timeRangeStart, stop: v.timeRangeStop)
    |> filter(fn: (r) => r._measurement == "daily" and r._field == "energy")
    |> drop(columns: ["custom_id","device_id","serial","status"])
    |> yield(name: "energy")

from(bucket: "smartmeter")
    |> range(start: v.timeRangeStart, stop: v.timeRangeStop)
    |> filter(fn: (r) => r._measurement == "daily" and r._field == "cost")
    |> drop(columns: ["custom_id","device_id","serial","status"])
    |> yield(name: "cost")
```

Monthly energy/cost bar graph:
******************************
```
from(bucket: "smartmeter")
    |> range(start: v.timeRangeStart, stop: v.timeRangeStop)
    |> filter(fn: (r) => r._measurement == "monthly" and r._field == "energy")
    |> drop(columns: ["custom_id","device_id","serial","status"])
    |> yield(name: "energy")

from(bucket: "smartmeter")
    |> range(start: v.timeRangeStart, stop: v.timeRangeStop)
    |> filter(fn: (r) => r._measurement == "monthly" and r._field == "cost")
    |> drop(columns: ["custom_id","device_id","serial","status"])
    |> yield(name: "cost")
```
