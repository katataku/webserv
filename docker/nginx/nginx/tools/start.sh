#------------------------ boot database  ----------------------------------------
cp /app/test_data/config/nginx/localhost /etc/nginx/sites-available
ln -s /etc/nginx/sites-available/localhost /etc/nginx/sites-enabled

cp /app/sample_data/html/* /var/www/html/
cp /app/sample_data/cgi-bin/* /var/www/cgi-bin/

mkdir -p /var/www/images/
cp /app/sample_data/images/* /var/www/images

chmod -R 777 /var/www/cgi-bin/
mkdir /var/www/files
chmod 777 /var/www/files
service fcgiwrap start
echo "Starting nginx";
nginx -g "daemon off;";
