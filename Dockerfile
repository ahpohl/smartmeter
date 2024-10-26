FROM alpine:latest

RUN apk add git build-base linux-headers mosquitto-dev

WORKDIR /app

ADD https://api.github.com/repos/ahpohl/smartmeter/git/refs/heads/master smartmeter-version.json
#RUN git clone https://github.com/ahpohl/smartmeter.git
COPY . /app/smartmeter
RUN cd /app/smartmeter && make PREFIX=/usr install

ENTRYPOINT smartmeter --config /etc/smartmeter.conf
