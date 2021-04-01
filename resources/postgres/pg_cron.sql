\c pg_cron
CREATE EXTENSION IF NOT EXISTS pg_cron;

-- regularly purge the log
SELECT cron.schedule('purge_log', '13 0 * * *', $$DELETE
    FROM cron.job_run_details
    WHERE end_time < now() - INTERVAL '3 days'$$);

-- schedule updates
SELECT cron.schedule('daily_view', '0,15,30,45 * * * *', $$REFRESH
  MATERIALIZED VIEW CONCURRENTLY daily_view$$);

SELECT cron.schedule('monthly_view', '1,16,31,46 * * * *', $$REFRESH
  MATERIALIZED VIEW CONCURRENTLY monthly_view$$);

UPDATE cron.job SET database = 'smartmeter' WHERE 
  jobname = 'daily_view' OR jobname = 'monthly_view';
