cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(supermini_i2c_diagnostic)

if(CONFIG_SUPERMINI_I2C_DIAGNOSTIC)
  target_sources(app PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/diagnostic.c)
endif()

