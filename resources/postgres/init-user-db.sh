#!/bin/bash
set -e

psql -v ON_ERROR_STOP=1 --username "$POSTGRES_USER" --dbname "$POSTGRES_DB" <<-EOSQL
	CREATE USER nodejs WITH PASSWORD 'nodejs';
	CREATE USER grafana WITH PASSWORD 'grafana';
	CREATE DATABASE smartmeter;
  CREATE DATABASE pg_cron;
EOSQL
