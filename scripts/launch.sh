cd ../
build/server > logs/Server.log &
pid_server=$!
build/client $1 > logs/Client.log & 
pid_client=$!