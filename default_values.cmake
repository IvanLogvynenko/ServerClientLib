add_compile_definitions(DEFAULT_PORT=37373)

add_compile_definitions(BUFFER_SIZE=50)
add_compile_definitions(DEFAULT_MESSAGE="")

# set 0 if you don't want to stop the server
add_compile_definitions(HANDLING_TIMEOUT=500)
add_compile_definitions(DEFAULT_TIMEOUT=500)

add_compile_definitions(MAX_CONNECTION_ATTEMPTS=50)