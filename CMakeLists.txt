cmake_minimum_required(VERSION 3.1)

project(
  vcan_unit_test
  VERSION 1.0
  LANGUAGES C)

include_directories(vcan_unit_test
  vcan
  vcanfd
  xcore_vcan
  fifo
)

add_executable(vcan_unit_test
  main.c
  vcan/vcan.c
  vcanfd/vcanfd.c
  xcore_vcan/xcore_vcan.c)