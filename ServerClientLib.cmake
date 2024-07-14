include(FetchContent)
FetchContent_Declare(
        ServerClientLib
        GIT_REPOSITORY https://github.com/IvanLogvynenko/ServerClientLib
        GIT_TAG master
)
FetchContent_MakeAvailable(ServerClientLib)
target_compile_options(ServerClientLib PRIVATE -w)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE DEBUG)
endif()

if (CMAKE_BUILD_TYPE==DEBUG) 
    add_compile_definitions(LOGGING)
    add_compile_definitions(FULL_LOGGING)
    add_compile_definitions(ERROR)
else()
    if (CMAKE_BUILD_TYPE==RELEASE)
        add_compile_definitions(LOGGING)
    endif()
endif()

target_include_directories(ServerClientLib SYSTEM INTERFACE ${ServerClientLib_SOURCE_DIR}/include/)
