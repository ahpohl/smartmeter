\c smartmeter
ALTER MATERIALIZED VIEW cagg_power SET (timescaledb.materialized_only = false);
ALTER MATERIALIZED VIEW cagg_daily SET (timescaledb.materialized_only = false);

INSERT INTO sensors(id, serial, custom_id, device_id) VALUES
(1, 'EBZ5DD3BZ06ETA_107', '1EBZ0100507409', '1EBZ0100507409');

INSERT INTO plan(id, price, rate) VALUES
(1, 0.2245, 13.56),
(2, 0.2426, 14.66);
