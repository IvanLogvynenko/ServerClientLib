add_compile_definitions(DEFAULT_PORT=30000)

add_compile_definitions(BUFFER_SIZE=1000)

# set 0 if you don't want to stop the server
add_compile_definitions(CONNECTION_TIMEOUT=15000)
add_compile_definitions(MESSAGE_INCOME_TIMEOUT=500)

add_compile_definitions(MAX_CONNECTION_ATTEMPTS=50)