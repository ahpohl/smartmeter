Install PostgreSQL and TimescaleDB
************************************

$ yaourt -S postgresql timescaledb

packages:
* postgresql-13.2-1
* timescaledb-2.1.1-1

init database:
$ su - postgres -c "initdb --locale en_US.UTF-8 -D '/var/lib/postgres/data'

edit config file `/var/lib/postgres/data/postgresql.conf`:
shared_preload_libraries = 'timescaledb' 

enable service:
$ systemctl enable postgresql
$ systemctl start postgresql

Setup Smartmeter:
******************

Database tables
---------------

$ psql -U postgres
CREATE DATABASE smartmeter;

Load the necessary configuration from the file `smartmeter.sql`.
$ psql -U postgres < smartmeter.sql

The script enables the timescaledb extionsion on the smartmeter databases, creates the tables `live`, `plan` and `sensors`, creates a hypertable `live` and prepopulates `sensor` and `plan` information. It also creates new users `grafana` with password `grafana` and `nodejs` with password `nodejs` and grant access to the tables. A retention policy for the live data is created and set to 7 days, after which data is removed from the database automatically.

Verify that the timescaledb extension has been enabled and the tables successfully created:

smartmeter=# \dx
                                      List of installed extensions
    Name     | Version |   Schema   |                            Description                            
-------------+---------+------------+-------------------------------------------------------------------
 plpgsql     | 1.0     | pg_catalog | PL/pgSQL procedural language
 timescaledb | 2.1.1   | public     | Enables scalable inserts and complex queries for time-series data
(2 rows)

smartmeter=# \dt
          List of relations
 Schema |  Name   | Type  |  Owner   
--------+---------+-------+----------
 public | live    | table | postgres
 public | plan    | table | postgres
 public | sensors | table | postgres
(3 rows)

Aggregates
----------

The smartmeter inserts a new datapoint every second into the live metric. Over the years this live data accumulates to a substantial amount of data overwhelming storage and memory requirements. TimescaleDB allows to aggregate data into smaller chunks using continuous aggregate queries. Create hourly and daily aggreagates:

$ psql -U postgres < aggregates.sql

Verify that the cagg's have been successfuly created:

smartmeter=# \d cagg_hourly
                       View "public.cagg_hourly"
  Column   |           Type           | Collation | Nullable | Default 
-----------+--------------------------+-----------+----------+---------
 bucket_1h | timestamp with time zone |           |          | 
 energy_1h | double precision         |           |          | 
 total     | double precision         |           |          | 

smartmeter=# \d cagg_daily
                       View "public.cagg_daily"
  Column   |           Type           | Collation | Nullable | Default 
-----------+--------------------------+-----------+----------+---------
 bucket_1d | timestamp with time zone |           |          | 
 sensor_id | integer                  |           |          | 
 plan_id   | integer                  |           |          | 
 energy_1d | double precision         |           |          | 
 total     | double precision         |           |          | 

Verify the schedule intervals:

smartmeter=# SELECT view_name, schedule_interval, 
  config ->> 'start_offset' as start_offset,
  config ->> 'end_offset' as end_offset,
  next_start,
  materialization_hypertable_name
FROM timescaledb_information.continuous_aggregates ca
  INNER JOIN timescaledb_information.jobs j 
    ON ca.materialization_hypertable_name = j.hypertable_name
  WHERE view_name = 'cagg_daily';
 view_name  | schedule_interval | start_offset | end_offset |          next_start           | materialization_hypertable_name 
------------+-------------------+--------------+------------+-------------------------------+---------------------------------
 cagg_daily | 01:00:00          | 3 days       | 01:00:00   | 2021-03-31 23:02:19.615278+02 | _materialized_hypertable_3
(1 row)

smartmeter=# SELECT view_name, schedule_interval, 
  config ->> 'start_offset' as start_offset,
  config ->> 'end_offset' as end_offset,
  next_start,
  materialization_hypertable_name
FROM timescaledb_information.continuous_aggregates ca
  INNER JOIN timescaledb_information.jobs j 
    ON ca.materialization_hypertable_name = j.hypertable_name
  WHERE view_name = 'cagg_hourly';
  view_name  | schedule_interval | start_offset | end_offset |          next_start           | materialization_hypertable_name 
-------------+-------------------+--------------+------------+-------------------------------+---------------------------------
 cagg_hourly | 1 day             |              |            | 2021-04-01 22:02:19.59859+02  | _materialized_hypertable_2
 cagg_hourly | 01:00:00          | 1 day        | 00:01:00   | 2021-03-31 23:02:19.611876+02 | _materialized_hypertable_2
(2 rows)

 Daily aggregates are created every hour in chunks of 3 days up to the past hour. Hourly aggregates are created every hour up in 1 day chunks up to the last minute. Hourly data is automatically deleted after 7 days and daily data is kept forever.

Aggregates are not created up to the current time. Real-time aggregates allow to query data cached and already processed data from the aggregate table together with the unprocessed live table in one combined query to present up-to-date aggregated data. Real-time updates have been disabled during creation of the continuous aggreagate. Enable real-time aggregates with:

smartmeter=# ALTER MATERIALIZED VIEW cagg_hourly SET (timescaledb.materialized_only = false);
smartmeter=# ALTER MATERIALIZED VIEW cagg_daily SET (timescaledb.materialized_only = false);

Materialized views
------------------

Due to limitations in the contuinuous aggreagte queries, it is not allowed to calculate the bill and to join external tables. Therefore the daily aggregate is not queried directly, but a materialized view is queried instead. Create daily and monthly views:

$ psql -U postgres < views.sql

Install pg_cron:
******************

Materialized views do not get updated automatically like continous aggregates do. Therefore we need an external mechanism to keep the views up to date. For this we install the pg_cron extension.

$ git clone https://aur.archlinux.org/pg_cron

edit PKGBUILD:
pkgver=1.3.1
makedepends=('postgresql' 'clang' 'llvm' 'distcc')
sha512sums=('e9758e1c3dfa12be05a9a24060d46a90299a821e0067690f07ea3356faade5fb3c157259aa553bbdb0fdb5582baedc8e51fb119ea0e78a5c88fb8bd3e6d047a6')

$ makepkg -s
$ pacman -U pg_cron-1.3.1-1-aarch64.pkg.tar.xz

Edit `postgresql.conf`:
shared_preload_libraries = 'timescaledb,pg_cron'        # (change requires restart)
cron.database_name = 'pg_cron'

$ systemctl restart postgresql

Create a new database `pg_cron` which stores the update schedule:
postgres=# CREATE database pg_cron;

Create jobs for updating the materialized views and to purge the cron log after 3 days:

$ 

Verify the jobs have been successfully created:

pg_cron=# select * from cron.job;
 jobid |      schedule      |                    command                     | nodename  | nodeport |  database  | username | active |   jobname    
-------+--------------------+------------------------------------------------+-----------+----------+------------+----------+--------+--------------
     1 | 13 0 * * *         | DELETE                                        +| localhost |     5432 | pg_cron    | postgres | t      | purge_log
       |                    |     FROM cron.job_run_details                 +|           |          |            |          |        | 
       |                    |     WHERE end_time < now() - INTERVAL '3 days' |           |          |            |          |        | 
     2 | 0,15,30,45 * * * * | REFRESH                                       +| localhost |     5432 | smartmeter | postgres | t      | daily_view
       |                    |   MATERIALIZED VIEW CONCURRENTLY daily_view    |           |          |            |          |        | 
     3 | 1,16,31,46 * * * * | REFRESH                                       +| localhost |     5432 | smartmeter | postgres | t      | monthly_view
       |                    |   MATERIALIZED VIEW CONCURRENTLY monthly_view  |           |          |            |          |        | 
(3 rows)

The materialized views get updated every 15 minutes and the log is purged after 3 days.

Optional: tune TimescaleDB
**************************

Install timescaledb-tune:

$ yaourt -S timescaledb-tune
$ timescaledb-tune -cpus 4 -memory 4GB -yes
$ systemctl restart postgresql

