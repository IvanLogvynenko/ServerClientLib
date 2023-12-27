cd ../
build/server > logs/Server.log &
pid_server=$!
build/client "127.0.0.1" "11999" > logs/Client.log & 
pid_client=$!