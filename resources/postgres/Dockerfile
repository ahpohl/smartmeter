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
    && git checkout v${PG_CRON_VERSION} \
    && make && make install \
    && cd ~ \
    \
    && apk del .fetch-deps .build-deps \
    && rm -rf /build \
    && sed -r -i "s/[#]*\s*(shared_preload_libraries)\s*=\s*'(.*)'/\1 = 'pg_cron,\2'/;s/,'/'/" /usr/local/share/postgresql/postgresql.conf.sample

#COPY *.sql /docker-entrypoint-initdb.d/
