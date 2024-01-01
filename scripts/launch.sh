if [ $# -gt 1 ]; then 
    echo "Usage: $0 [port(opt)]"
fi

if [ $# -eq 0 ]; then 
    echo "Default port will be used"
else 
    if [ $1 -lt 1024 ]; then
        echo "Usage of port that is less then 1024 is not recommended as many services are using ports from 1-1024" && exit 1
    else 
        echo "Provided port is valid"
    fi
fi

# Check if build dir exists
if [ ! -x "../build" ]; then
    mkdir ../build
fi

# Check if the server executable exists
if [ ! -x "../build/server" ]; then
    echo "Server executable not found or not executable."
    {
        cd ../build
        if [ ! -x "Makefile" ]; then
            cmake ..
            if [ $? -ne 0 ]; then
                echo "Error: CMake configuration failed."
                exit 1
            fi
        fi
        make server
        if [ $? -ne 0 ]; then
            echo "Error: Make failed. Check the build logs for details."
            exit 1
        fi
    }
fi

# Check if the client executable exists
if [ ! -x "../build/client" ]; then
    echo "Client executable not found or not executable."
    {
        cd ../build
        make client
        if [ $? -ne 0 ]; then
            echo "Error: Make failed. Check the build logs for details."
            exit 1
        fi
    }
fi

# Check if log dir exists
if [ ! -x "../logs" ]; then
    mkdir ../logs
fi

cd ../
echo "Execution start"

build/server $1 > logs/Server.log &
pid_server=$!

echo "Server started. Starting client"

build/client "127.0.0.1" $1 > logs/Client.log & 
pid_client=$!

echo "Awaiting execution to finish"

wait $pid_server
wait $pid_client

echo "Finished"