FROM amd64/debian:buster

RUN apt-get update \
  && apt-get -y upgrade \
  && apt-get -y install wget python3 nginx-full fcgiwrap

RUN mkdir /var/www/cgi-bin

COPY ./tools/start.sh /usr/local/bin
CMD bash /usr/local/bin/start.sh;
