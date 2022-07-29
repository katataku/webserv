cd /app
mkdir -p /app/log
./webserv ./test_data/config/webserv/ok/default.conf > /app/log/log.txt &
sleep 1

./tests/stress_test/stress_test.sh > /app/log/siege.log 