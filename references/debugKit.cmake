include(FetchContent)
FetchContent_Declare(
        debugKit
        GIT_REPOSITORY https://github.com/IvanLogvynenko/debugKit
        GIT_TAG 20fb5b1dd0a299eee0de2cf64bdafd56757b0a4e # release 0.0.1.1
)
FetchContent_MakeAvailable(debugKit)
target_compile_options(debugKit PRIVATE -w)
target_include_directories(debugKit SYSTEM INTERFACE ${debugKit_SOURCE_DIR}/include/)