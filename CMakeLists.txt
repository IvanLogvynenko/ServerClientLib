cmake_minimum_required(VERSION 3.16)
project(ServerClientLib VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wconversion -Werror -fsanitize=address")

find_package(debugKit)
include("default_values.cmake")

add_library(ServerClientLib
    src/Server.cpp
    src/Client.cpp
    src/Connection.cpp
)
target_link_libraries(ServerClientLib PUBLIC debugKit)
target_include_directories(ServerClientLib PRIVATE include)

include(GNUInstallDirs)
install(TARGETS ServerClientLib
        EXPORT ServerClientLibTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ServerClientLib)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/ServerClientLibConfig.cmake"
    COMPATIBILITY SameMajorVersion
)
install(EXPORT ServerClientLibTargets
        FILE ServerClientLibTargets.cmake
        NAMESPACE ServerClientLib::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ServerClientLib
)
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/ServerClientLibConfig.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ServerClientLib
)

if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/install_manifest.txt")
    add_custom_target(uninstall
        COMMAND xargs rm < "${CMAKE_CURRENT_BINARY_DIR}/install_manifest.txt"
        COMMENT "Удаление установленных файлов"
    )
else()
    add_custom_target(uninstall
        COMMAND echo "install_manifest.txt not found. Uninstall impossible."
    )
endif()
