cd /app
make -j
mkdir -p /app/log
./webserv ./test_data/config/webserv/ok/default.conf > /app/log/log.txt