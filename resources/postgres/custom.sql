\c smartmeter
ALTER MATERIALIZED VIEW cagg_power SET (timescaledb.materialized_only = false);
ALTER MATERIALIZED VIEW cagg_daily SET (timescaledb.materialized_only = false);
