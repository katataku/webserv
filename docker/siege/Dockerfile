FROM debian:stable-slim

WORKDIR /app

RUN apt-get update && apt-get install -y \
	make \
	g++ \
	procps \
	siege

COPY ./tools/start.sh /usr/local/bin
# prevent container from terminating
ENTRYPOINT ["tail", "-f", "/dev/null"]
