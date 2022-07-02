FROM debian:buster

RUN apt-get update \
	&& apt-get -y upgrade \
	&& apt-get -y install wget python3 nginx-full fcgiwrap

RUN mkdir /var/www/localhost
RUN mkdir /var/www/localhost/cgi-bin
COPY srcs/localhost /etc/nginx/sites-available
RUN ln -s /etc/nginx/sites-available/localhost /etc/nginx/sites-enabled

COPY ./tools/start.sh /usr/local/bin
COPY ./srcs/upload.html /var/www/localhost
COPY ./srcs/file_manager.py /var/www/localhost/cgi-bin
CMD bash /usr/local/bin/start.sh;
