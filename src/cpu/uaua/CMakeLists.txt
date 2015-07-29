cmake_minimum_required(VERSION 2.8)

cpu_setup("uaua")

function(uaua_add_executable exe)
  cpu_add_executable(${exe} ${ARGN})
  cpu_target_link_common_libraries(${exe})
endfunction()

uaua_add_executable(uaua main.cc)
uaua_add_executable(nobasi nobasi.cc)
