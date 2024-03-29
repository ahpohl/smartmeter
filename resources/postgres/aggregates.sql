\c smartmeter

--
-- daily continuous aggregate
--
DROP MATERIALIZED VIEW IF EXISTS cagg_daily CASCADE;

CREATE MATERIALIZED VIEW cagg_daily
WITH (timescaledb.continuous, timescaledb.materialized_only=true) 
AS
SELECT
  time_bucket('1 day', time, 'Europe/Berlin') AS bucket_1d,
  sensor_id,
  plan_id,
  last(energy, time) - first(energy, time) AS energy_1d,
  first(energy, time) AS total
FROM live
GROUP BY bucket_1d, plan_id, sensor_id
WITH NO DATA;

-- refresh policy
SELECT add_continuous_aggregate_policy('cagg_daily',
  start_offset => INTERVAL '7 days',
  end_offset => INTERVAL '1 day',
  schedule_interval => '1 day');

--
-- average power
--
DROP MATERIALIZED VIEW IF EXISTS cagg_power CASCADE;

CREATE MATERIALIZED VIEW cagg_power
WITH (timescaledb.continuous, timescaledb.materialized_only=true)
AS
SELECT
  time_bucket('5 minutes', time, 'Europe/Berlin') as bucket_5m,
  avg(power) as power_5m,
  avg(power_l1) as power_l1_5m,
  avg(power_l2) as power_l2_5m,
  avg(power_l3) as power_l3_5m
FROM live
GROUP BY bucket_5m           
WITH NO DATA;

-- refresh policy
SELECT add_continuous_aggregate_policy('cagg_power',
  start_offset => INTERVAL '3 hours',
  end_offset => INTERVAL '5 minutes',
  schedule_interval => '5 minutes');

-- retention policy
SELECT add_retention_policy('cagg_power', INTERVAL '1 year');

-- grant
GRANT SELECT ON TABLE cagg_power TO grafana;

--
-- load profile for Fronius Solar.creator
--
DROP MATERIALIZED VIEW IF EXISTS cagg_minutes CASCADE;

CREATE MATERIALIZED VIEW cagg_minutes
WITH (timescaledb.continuous, timescaledb.materialized_only=true) 
AS
SELECT
  time_bucket('15 minutes', time, 'Europe/Berlin') AS bucket_15m,
  sensor_id,
  plan_id,
  last(energy, time) - first(energy, time) AS energy_15m,
  first(energy, time) AS total
FROM live
GROUP BY bucket_15m, plan_id, sensor_id
WITH NO DATA;

-- refresh policy
SELECT add_continuous_aggregate_policy('cagg_minutes',
  start_offset => INTERVAL '3 hours',
  end_offset => INTERVAL '15 minutes',
  schedule_interval => '15 minutes');

-- data retention
SELECT add_retention_policy('cagg_minutes', INTERVAL '10 years');
