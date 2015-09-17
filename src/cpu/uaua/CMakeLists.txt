cmake_minimum_required(VERSION 2.8)

cpu_setup("uaua")

function(uaua_add_executable exe)
  cpu_add_executable(${exe} ${ARGN})
  cpu_target_link_common_libraries(${exe})
endfunction()

function(uaua_add_test exe)
    uaua_add_executable(${exe} ${exe}.cc)
    cpu_target_link_libraries(${exe} gtest gtest_main)
    if(NOT ARGV1)
        cpu_add_test(${exe})
    endif()
endfunction()

uaua_add_executable(uaua main.cc)

cpu_add_runner(run.sh)

uaua_add_test(ai_test)
