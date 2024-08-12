include(FetchContent)
FetchContent_Declare(
        ServerClientLib
        GIT_REPOSITORY https://github.com/IvanLogvynenko/ServerClientLib
        GIT_TAG main
)
FetchContent_MakeAvailable(ServerClientLib)
target_compile_options(ServerClientLib PRIVATE -w)
target_include_directories(ServerClientLib SYSTEM INTERFACE ${ServerClientLib_SOURCE_DIR}/include/)
