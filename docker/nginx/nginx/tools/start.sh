#------------------------ boot database  ----------------------------------------
chmod 777 /var/www/localhost/cgi-bin/test.py
mkdir /var/www/localhost/files
chmod 777 /var/www/localhost/files
service fcgiwrap start
echo "Starting nginx";
nginx -g "daemon off;";
