\c smartmeter

\dx
\x

SELECT * FROM timescaledb_information.continuous_aggregates;
SELECT * FROM timescaledb_information.jobs WHERE application_name LIKE 'Refresh Continuous%';
SELECT * FROM pg_matviews;

SELECT view_name, schedule_interval, 
  config ->> 'start_offset' as start_offset,
  config ->> 'end_offset' as end_offset,
  next_start,
  materialization_hypertable_name
FROM timescaledb_information.continuous_aggregates ca
  INNER JOIN timescaledb_information.jobs j 
    ON ca.materialization_hypertable_name = j.hypertable_name;

\c pg_cron

SELECT * FROM cron.job;
