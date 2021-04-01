\c smartmeter

--
-- hourly continuous aggregate
--
DROP MATERIALIZED VIEW IF EXISTS cagg_hourly;

-- hourly aggregate
CREATE MATERIALIZED VIEW cagg_hourly
WITH (timescaledb.continuous, timescaledb.materialized_only=true)
AS
SELECT
  time_bucket('1 hour', time) as bucket_1h,
  last(energy, time) - first(energy, time) as energy_1h,
  first(energy, time) as total
FROM live
GROUP BY bucket_1h           
WITH NO DATA;

-- hourly refresh policy
SELECT add_continuous_aggregate_policy('cagg_hourly',
  start_offset => INTERVAL '1 day',
  end_offset => INTERVAL '1 minute',
  schedule_interval => '1 hour');

-- retention policy
SELECT add_retention_policy('cagg_hourly', INTERVAL '7 days');

-- grant
GRANT SELECT ON TABLE cagg_hourly TO grafana;

--
-- daily continuous aggregate
--
DROP MATERIALIZED VIEW IF EXISTS cagg_daily;

CREATE MATERIALIZED VIEW cagg_daily
WITH (timescaledb.continuous, timescaledb.materialized_only=true) 
AS
SELECT
  time_bucket('1 day', time) AS bucket_1d,
  sensor_id,
  plan_id,
  last(energy, time) - first(energy, time) AS energy_1d,
  first(energy, time) AS total
FROM live
GROUP BY bucket_1d, plan_id, sensor_id
WITH NO DATA;

-- daily refresh policy
SELECT add_continuous_aggregate_policy('cagg_daily',
  start_offset => INTERVAL '3 days',
  end_offset => INTERVAL '1 hour',
  schedule_interval => '1 hour');
