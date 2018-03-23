
set(PLATFORM_SRC dev.c dev.h i2c.h serial.h spi.h)

set(PLATFORM_DEFAULT "gpa_tx2_carrier")
if (NOT PLATFORM_SELECT)
    message(WARNING "No platform selected. Defaulting to ${PLATFORM_DEFAULT}!")
    set(PLATFORM_SELECT ${PLATFORM_DEFAULT})
else()
    message(INFO " Platform: ${PLATFORM_SELECT} ")
endif ()

add_subdirectory(${PLATFORM_SELECT})

add_library(platform ${PLATFORM_SRC} ${BOARD_SRC})