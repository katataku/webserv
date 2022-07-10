#------------------------ boot database  ----------------------------------------
cp /app/test_data/config/nginx/localhost /etc/nginx/sites-available
ln -s /etc/nginx/sites-available/localhost /etc/nginx/sites-enabled

cp /app/sample_data/html/upload.html /var/www/html/
cp /app/sample_data/html/index.html /var/www/html/
cp /app/sample_data/cgi-bin/file_manager.py /var/www/cgi-bin/
cp /app/sample_data/cgi-bin/cgi-req-dumper.py /var/www/cgi-bin/

chmod -R 777 /var/www/cgi-bin/
mkdir /var/www/files
chmod 777 /var/www/files
service fcgiwrap start
echo "Starting nginx";
nginx -g "daemon off;";
