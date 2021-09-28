\c smartmeter

DROP MATERIALIZED VIEW IF EXISTS "monthly_view" CASCADE;
DROP MATERIALIZED VIEW IF EXISTS "daily_view" CASCADE;
DROP MATERIALIZED VIEW IF EXISTS "cagg_power" CASCADE;
DROP MATERIALIZED VIEW IF EXISTS "cagg_daily" CASCADE;
DROP TABLE IF EXISTS "archive" CASCADE;
DROP TABLE IF EXISTS "live" CASCADE;
DROP TABLE IF EXISTS "sensors" CASCADE;
DROP TABLE IF EXISTS "plan" CASCADE;

CREATE EXTENSION IF NOT EXISTS timescaledb;

CREATE TABLE "sensors" (
  id SERIAL PRIMARY KEY,
  serial VARCHAR(50),
  custom_id VARCHAR(50),
  device_id VARCHAR(50)
);

CREATE TABLE "plan" (
  id SERIAL PRIMARY KEY,
  price DOUBLE PRECISION,
  rate DOUBLE PRECISION
);

CREATE TABLE "live" (
  time TIMESTAMPTZ NOT NULL,
  sensor_id INTEGER NOT NULL,
  plan_id INTEGER NOT NULL,
  energy DOUBLE PRECISION,
  power DOUBLE PRECISION,
  power_l1 DOUBLE PRECISION,
  power_l2 DOUBLE PRECISION,
  power_l3 DOUBLE PRECISION,
  voltage_l1 DOUBLE PRECISION,
  voltage_l2 DOUBLE PRECISION,
  voltage_l3 DOUBLE PRECISION,
  lifetime BIGSERIAL,
  status BYTEA,
  CONSTRAINT sensor_id FOREIGN KEY (sensor_id) REFERENCES sensors (id),
  CONSTRAINT plan_id FOREIGN KEY (plan_id) REFERENCES plan (id)
);

SELECT create_hypertable('live', 'time');
SELECT add_retention_policy('live', INTERVAL '7 days');

GRANT INSERT, SELECT ON TABLE live TO nodejs;
GRANT SELECT ON TABLE live TO grafana;

GRANT SELECT ON TABLE sensors TO nodejs;
GRANT SELECT ON TABLE sensors TO grafana;

GRANT SELECT ON TABLE plan TO nodejs;
GRANT SELECT ON TABLE plan TO grafana;
