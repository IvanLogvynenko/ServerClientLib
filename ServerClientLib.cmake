include(FetchContent)
FetchContent_Declare(
        ServerClientLib
        GIT_REPOSITORY https://github.com/IvanLogvynenko/ServerClientLib
        GIT_TAG c7b22282ae2357379391a0fe86f9a665461b7df9 #refs/tags/v0.1.0
)
FetchContent_MakeAvailable(ServerClientLib)
target_compile_options(ServerClientLib PRIVATE -w)
target_include_directories(ServerClientLib SYSTEM INTERFACE ${ServerClientLib_SOURCE_DIR}/include/)
