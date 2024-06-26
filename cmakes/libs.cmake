find_package(zstd CONFIG REQUIRED)
target_link_libraries(cyx PRIVATE $<IF:$<TARGET_EXISTS:zstd::libzstd_shared>,zstd::libzstd_shared,zstd::libzstd_static>)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(cyx PRIVATE spdlog::spdlog)