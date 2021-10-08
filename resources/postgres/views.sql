\c smartmeter

-- create daily view with data from archive and cagg_daily
DROP MATERIALIZED VIEW IF EXISTS daily_view CASCADE;

CREATE MATERIALIZED VIEW daily_view
AS
SELECT
  bucket_1d AS time,
  energy_1d AS energy,
  energy_1d * price + rate * 12.0 / 365.0 AS bill,
  total,
  price,
  rate
FROM archive JOIN plan ON archive.plan_id = plan.id
GROUP BY bucket_1d, energy_1d, total, price, rate
UNION
SELECT
  bucket_1d AS time,
  energy_1d AS energy,
  energy_1d * price + rate * 12.0 / 365.0 AS bill,
  total,
  price,
  rate
FROM cagg_daily JOIN plan ON cagg_daily.plan_id = plan.id
---WHERE bucket_1d > TIMESTAMP WITH TIME ZONE '2021-10-06 02:00:00+02'
GROUP BY bucket_1d, energy_1d, total, price, rate
ORDER BY time;

-- index
CREATE UNIQUE INDEX daily_idx ON daily_view (time);

-- grant
GRANT SELECT ON TABLE daily_view TO grafana;

--
-- time_bucket month function
-- 

CREATE OR REPLACE FUNCTION _time_bucket (
  bucket_width interval,
  ts timestamptz,
  "offset" interval = '00:00:00'::interval,
  origin timestamptz = '0001-01-01 00:00:00+00'::timestamptz
)
RETURNS TIMESTAMPTZ AS
$body$
/*
millenium = 1000 years
century = 100 years
decade = 10 years
1.5 years aka 1 year 6 months or 18 months
year aka 12 months
half year aka 6 months
quarter aka 3 months
months = months
*/
DECLARE
  months integer;
  bucket_months integer;
  bucket_month integer;
BEGIN
	IF EXTRACT(MONTH FROM bucket_width) >= 1 OR EXTRACT(YEAR FROM bucket_width) >= 1 THEN
    origin := origin + ((0-date_part('timezone_hour', now()))::text || ' hours')::interval;
      bucket_months :=
          (EXTRACT(MONTH FROM bucket_width) + -- months	
          (EXTRACT(YEAR FROM bucket_width) * 12)); -- years
      months := (((EXTRACT(YEAR FROM ts)-EXTRACT(YEAR FROM origin))*12)+EXTRACT(MONTH FROM ts)-1);
      bucket_month := floor(months/bucket_months)*bucket_months;
      
      RETURN make_timestamptz(
          (EXTRACT(YEAR FROM origin)+floor(bucket_month/12))::integer, -- year
          (bucket_month%12)+1, -- month
          1, --day
          0, -- hour
          0, -- minute
          0, --second
          'Z');
	ELSE
      CASE 
      WHEN "offset" > '0s'::interval THEN
	      RETURN public.time_bucket(bucket_width, ts-"offset") + "offset";
      WHEN origin <> '0001-01-01T00:00:00Z'::timestamptz THEN
	      RETURN public.time_bucket(bucket_width, ts, origin);
      ELSE
	      RETURN public.time_bucket(bucket_width, ts);
      END CASE;
    END IF;
END;
$body$
LANGUAGE 'plpgsql'
IMMUTABLE
RETURNS NULL ON NULL INPUT
SECURITY INVOKER;

--
-- create monthly view
--
CREATE MATERIALIZED VIEW monthly_view
AS
SELECT
  _time_bucket('1 month', time) AS time,
  sum(energy) AS energy,
  sum(bill) AS bill,
  first(total, time) AS total
FROM daily_view
GROUP BY _time_bucket('1 month', time)
ORDER BY time;

-- index
CREATE UNIQUE INDEX monthly_idx ON monthly_view (time);

-- grant
GRANT SELECT ON TABLE monthly_view TO grafana;
