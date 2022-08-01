cd /app
make
mkdir -p /app/log
./webserv ./test_data/config/webserv/ok/default.conf > /app/log/log.txt &
sleep 1

#[options]
#-b: --benchmark ベンチマークモード。ディレイを無効にする。
siege -b http://localhost/ > /app/log/siege.log &

# topを1秒ごとに20回繰り返す
#-b: バッチモード（対話的な入力を受け付けないで、結果を全て出力）
#-d: 実行間隔（秒）
#-n: 実行回数（回）
top -b -n 1 |grep MEM > /app/log/top.log #ラベル行出力用
top -b -d 1 -n 500 |grep webserv >> /app/log/top.log
pkill siege
sleep 1
