project(cyx_test)

find_package(GTest CONFIG)

if(NOT GTest_DIR)
return()
endif()

include(GoogleTest)
enable_testing()

set(LIB_MIX  GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main cyx)


function(add_test_executable name)
    add_executable(${name} ${ARGN})
    target_link_libraries(${name} PRIVATE ${LIB_MIX})
    # add_test(
    #     NAME ${name}
    #     COMMAND $<TARGET_FILE:${name}>)

    gtest_add_tests(TARGET ${name})
endfunction()

add_test_executable(filesystem_test src/fs_test.cpp)

add_test_executable(compress_test src/compress_test.cpp)
# add_test_executable(axx_test src/axx_test.cpp)