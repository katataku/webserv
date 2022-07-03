#------------------------ boot database  ----------------------------------------
cp /app/tests/test_data/conf/localhost /etc/nginx/sites-available
ln -s /etc/nginx/sites-available/localhost /etc/nginx/sites-enabled

cp /app/tests/test_data/public/html/upload.html /var/www/localhost
cp /app/tests/test_data/public/cgi-bin/file_manager.py /var/www/localhost/cgi-bin 

chmod 777 /var/www/localhost/cgi-bin/file_manager.py
mkdir /var/www/localhost/files
chmod 777 /var/www/localhost/files
service fcgiwrap start
echo "Starting nginx";
nginx -g "daemon off;";
