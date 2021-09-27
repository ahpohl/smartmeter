FROM timescale/timescaledb:latest-pg13

ARG PG_CRON_VERSION=1.4.1
RUN set -ex \
    && apk add --no-cache --virtual .fetch-deps \
                ca-certificates \
                git \
                openssl \
                openssl-dev \
                tar \
    && mkdir -p /build/ \
    && git clone https://github.com/citusdata/pg_cron.git /build/pg_cron \
    \
    && apk add --no-cache --virtual .build-deps \
                coreutils \
                dpkg-dev dpkg \
                gcc \
                libc-dev \
                make \
                cmake \
                clang \
                llvm \
                util-linux-dev \
    \
    # Build current version \
    && cd /build/pg_cron && rm -fr build \
    && git checkout -b v${PG_CRON_VERSION} \
    && make && make install \
    && cd ~ \
    \
    && apk del .fetch-deps .build-deps \
    && rm -rf /build \
    && sed -r -i "s/[#]*\s*(shared_preload_libraries)\s*=\s*'(.*)'/\1 = 'pg_cron,\2'/;s/,'/'/" /usr/local/share/postgresql/postgresql.conf.sample \
    && sed -i "/^shared_preload_libraries.*/a cron.database_name = 'pg_cron'" /usr/local/share/postgresql/postgresql.conf.sample

COPY init-user-db.sh /docker-entrypoint-initdb.d/003_init-user-db.sh
COPY smartmeter.sql /docker-entrypoint-initdb.d/004_smartmeter.sql
COPY archive.sql /docker-entrypoint-initdb.d/005_archive.sql
COPY aggregates.sql /docker-entrypoint-initdb.d/006_aggregates.sql
COPY views.sql /docker-entrypoint-initdb.d/007_views.sql
COPY pg_cron.sql /docker-entrypoint-initdb.d/008_pgcron.sql
COPY custom.sql /docker-entrypoint-initdb.d/009_custom.sql
