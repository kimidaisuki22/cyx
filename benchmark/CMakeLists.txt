find_package(benchmark CONFIG)

if(NOT benchmark_DIR)
return()
endif()

function(add_bench target)

add_executable(benchmark_${target} src/benchmark_${target}.cpp)
target_link_libraries(benchmark_${target} PRIVATE benchmark::benchmark benchmark::benchmark_main cyx)

endfunction()

add_bench(raw_arena)