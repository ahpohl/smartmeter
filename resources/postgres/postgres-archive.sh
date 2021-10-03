#!/bin/bash

RSYNC_PASSWORD="synosync"
export RSYNC_PASSWORD
PGDATA="/var/lib/postgres/data"
WALPATH=$1
WALFILE=$2

rsync -aq --ignore-existing $PGDATA/$WALPATH admin@syno.fritz.box::NetBackup/wal-docker/$WALFILE
exit $?
