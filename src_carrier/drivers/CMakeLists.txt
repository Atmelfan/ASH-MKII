
add_subdirectory(i2c)
add_subdirectory(spi)
set(DRIVERS_SRC
        ${I2C_SRC}
        ${SER_SRC}
        ${SPI_SRC}
        ${CMAKE_CURRENT_SOURCE_DIR}/pca9685.h
        ${CMAKE_CURRENT_SOURCE_DIR}/pca9685.c
        ${CMAKE_CURRENT_SOURCE_DIR}/mpu6000.h
        ${CMAKE_CURRENT_SOURCE_DIR}/mpu6000.c
        #PARENT_SCOPE
        )
set(DRIVERS_SRC ${DRIVERS_SRC} PARENT_SCOPE)
message(INFO "${DRIVERS_SRC}")
