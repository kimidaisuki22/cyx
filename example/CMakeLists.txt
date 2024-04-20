

function(add_exe name)
    add_executable(example_${name} src/example_${name}.cpp)
    target_link_libraries(example_${name}
        cyx
    )
endfunction()

add_exe(compression)
