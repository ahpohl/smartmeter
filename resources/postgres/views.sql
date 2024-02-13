\c smartmeter

-- create daily view with data from archive and cagg_daily
DROP MATERIALIZED VIEW IF EXISTS daily_view CASCADE;

CREATE MATERIALIZED VIEW daily_view
AS
SELECT
  bucket_1d AS time,
  energy_1d AS energy,
  energy_1d * price + rate / 365.0 AS bill,
  total,
  price,
  rate
FROM archive JOIN plan ON archive.plan_id = plan.id
GROUP BY bucket_1d, energy_1d, total, price, rate
UNION
SELECT
  bucket_1d AS time,
  energy_1d AS energy,
  energy_1d * price + rate / 365.0 AS bill,
  total,
  price,
  rate
FROM cagg_daily JOIN plan ON cagg_daily.plan_id = plan.id
-- insert end time of archive
WHERE bucket_1d > TIMESTAMP WITH TIME ZONE '2023-12-27 00:00:00+01'
GROUP BY bucket_1d, energy_1d, total, price, rate
ORDER BY time;

-- index
CREATE UNIQUE INDEX daily_idx ON daily_view (time);

-- grant
GRANT SELECT ON TABLE daily_view TO grafana;
GRANT SELECT ON TABLE daily_view TO nodejs;

--
-- create monthly view
--
CREATE MATERIALIZED VIEW monthly_view
AS
SELECT
  time_bucket('1 month', time, 'Europe/Berlin') AS bucket_1m,
  sum(energy) AS energy,
  sum(bill) AS bill,
  first(total, time) AS total,
  avg(energy) AS avg,
  min(energy) AS min,
  max(energy) AS max
FROM daily_view
GROUP BY bucket_1m
ORDER BY bucket_1m;

-- index
CREATE UNIQUE INDEX monthly_idx ON monthly_view (bucket_1m);

-- grant
GRANT SELECT ON TABLE monthly_view TO grafana;
GRANT SELECT ON TABLE monthly_view TO nodejs;

--
-- create yearly view
--
CREATE MATERIALIZED VIEW yearly_view
AS
SELECT
  time_bucket('1 year', time, 'Europe/Berlin') AS bucket_1y,
  count(*) as days,
  sum(energy) AS energy,
  sum(bill) AS bill,
  first(total, time) AS total
FROM daily_view
GROUP BY bucket_1y
ORDER BY bucket_1y;

-- index
CREATE UNIQUE INDEX yearly_idx ON yearly_view (bucket_1y);

-- grant
GRANT SELECT ON TABLE yearly_view TO grafana;
GRANT SELECT ON TABLE yearly_view TO nodejs;
