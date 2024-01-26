include(FetchContent)
FetchContent_Declare(
        ServerClientLib
        GIT_REPOSITORY https://github.com/IvanLogvynenko/ServerClientLib
        GIT_TAG f03135f45e93d2d1bbbcd4e007af1a38e4e14381 #refs/tags/v0.1.1
)
FetchContent_MakeAvailable(ServerClientLib)
target_compile_options(ServerClientLib PRIVATE -w)
target_include_directories(ServerClientLib SYSTEM INTERFACE ${ServerClientLib_SOURCE_DIR}/include/)
