cd /app
make -j
mkdir -p /app/log
./webserv ./test_data/config/webserv/ok/localhost > /app/log/log.txt
