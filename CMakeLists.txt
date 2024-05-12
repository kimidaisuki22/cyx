cmake_minimum_required(VERSION 3.21.0)
project(cyx VERSION 0.0.1)

# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

include(CTest)

add_library(cyx STATIC)

target_sources(cyx PRIVATE
src/compression/compress_zstd.cpp
src/compression/decompress_zstd.cpp
)

include(cmakes/libs.cmake)

target_include_directories(cyx PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)


option(cyx_BUILD_EXAMPLE "build example for cyx" ON)
option(cyx_INSTALL "install library cyx" ON)
option(cyx_BENCHMARK "install library cyx" ON)

if(${cyx_BUILD_EXAMPLE})
    add_subdirectory(example)
endif()

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()

if(${cyx_BENCHMARK})
    add_subdirectory(benchmark)
endif()

if(${cyx_INSTALL})
include(CPack)
include(GNUInstallDirs)
install(TARGETS cyx
    EXPORT cyx
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION  ${CMAKE_INSTALL_BINDIR}
)
install(EXPORT cyx
    FILE cyx-targets.cmake
    NAMESPACE cyx::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cyx
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmakes/cyx-config.cmake.in
               ${CMAKE_CURRENT_BINARY_DIR}/cyx-config.cmake
               @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cyx-config.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cyx)
endif()
