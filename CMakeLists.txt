if(CONFIG_SUPERMINI_I2C_DIAGNOSTIC)
  zephyr_library()
  zephyr_library_sources(src/diagnostic.c)
endif()

