\c smartmeter

DROP TABLE IF EXISTS "archive" CASCADE;

CREATE TABLE "archive" (
  bucket_1d TIMESTAMPTZ NOT NULL,
  sensor_id INTEGER NOT NULL,
  plan_id INTEGER NOT NULL,
  energy_1d DOUBLE PRECISION,
  total DOUBLE PRECISION,
  CONSTRAINT sensor_id FOREIGN KEY (sensor_id) REFERENCES sensors (id),
  CONSTRAINT plan_id FOREIGN KEY (plan_id) REFERENCES plan (id)
);

SELECT create_hypertable('archive', 'bucket_1d');

-- \copy "archive" FROM 'archive.csv' DELIMITER ',' CSV HEADER

GRANT SELECT ON "archive" TO grafana;
