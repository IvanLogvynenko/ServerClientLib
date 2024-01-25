include(FetchContent)
FetchContent_Declare(
        ServerClientLib
        GIT_REPOSITORY https://github.com/IvanLogvynenko/ServerClientLib
        GIT_TAG 4914ec25b88f2320e7cb85b49db95734fcaa2014 #refs/tags/v0.1.1
)
FetchContent_MakeAvailable(ServerClientLib)
target_compile_options(ServerClientLib PRIVATE -w)
target_include_directories(ServerClientLib SYSTEM INTERFACE ${ServerClientLib_SOURCE_DIR}/include/)
