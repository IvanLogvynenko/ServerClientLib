include(FetchContent)
FetchContent_Declare(
        ServerClientLib
        GIT_REPOSITORY https://github.com/IvanLogvynenko/ServerClientLib
        GIT_TAG 83fe4589dc1640687ab4580be2ad085ac01fcd2c #refs/tags/v0.1.1
)
FetchContent_MakeAvailable(ServerClientLib)
target_compile_options(ServerClientLib PRIVATE -w)
target_include_directories(ServerClientLib SYSTEM INTERFACE ${ServerClientLib_SOURCE_DIR}/include/)
