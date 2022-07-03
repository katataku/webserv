#------------------------ boot database  ----------------------------------------
cp /app/tests/test_data/conf/localhost /etc/nginx/sites-available
ln -s /etc/nginx/sites-available/localhost /etc/nginx/sites-enabled

cp /app/tests/test_data/public/html/upload.html /var/www/html/
cp /app/tests/test_data/public/cgi-bin/file_manager.py /var/www/cgi-bin/

chmod 777 /var/www/cgi-bin/file_manager.py
mkdir /var/www/files
chmod 777 /var/www/files
service fcgiwrap start
echo "Starting nginx";
nginx -g "daemon off;";
