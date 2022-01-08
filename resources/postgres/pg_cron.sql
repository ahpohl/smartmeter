\c pg_cron

CREATE EXTENSION IF NOT EXISTS pg_cron;

-- regularly purge the log
SELECT cron.schedule('purge_log', '13 0 * * *', $$DELETE
    FROM cron.job_run_details
    WHERE end_time < now() - INTERVAL '3 days'$$);

-- schedule updates
SELECT cron.schedule('smartmeter_daily', '0,15,30,45 * * * *', $$REFRESH
  MATERIALIZED VIEW CONCURRENTLY daily_view$$);

SELECT cron.schedule('smartmeter_monthly', '1,16,31,46 * * * *', $$REFRESH
  MATERIALIZED VIEW CONCURRENTLY monthly_view$$);

SELECT cron.schedule('smartmeter_yearly', '2,17,32,47 * * * *', $$REFRESH
  MATERIALIZED VIEW CONCURRENTLY yearly_view$$);

UPDATE cron.job SET database = 'smartmeter' WHERE 
  jobname = 'smartmeter_daily' OR 
  jobname = 'smartmeter_monthly' OR
  jobname = 'smartmeter_yearly';
