\c smartmeter
CREATE EXTENSION IF NOT EXISTS timescaledb;

DROP TABLE IF EXISTS "live" CASCADE;
DROP TABLE IF EXISTS "sensors" CASCADE;
DROP TABLE IF EXISTS "plan" CASCADE;

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

INSERT INTO sensors(id, serial, custom_id, device_id) VALUES
(1, 'EBZ5DD3BZ06ETA_107', '1EBZ0100507409', '1EBZ0100507409');

INSERT INTO plan(id, price, rate) VALUES
(1, 0.2245, 13.56),
(2, 0.2426, 14.66);

CREATE USER nodejs WITH PASSWORD 'nodejs';
CREATE USER grafana WITH PASSWORD 'grafana';

GRANT INSERT, SELECT ON TABLE live TO nodejs;
GRANT SELECT ON TABLE live TO grafana;

GRANT SELECT ON TABLE sensors TO nodejs;
GRANT SELECT ON TABLE sensors TO grafana;

GRANT SELECT ON TABLE plan TO nodejs;
GRANT SELECT ON TABLE plan TO grafana;
