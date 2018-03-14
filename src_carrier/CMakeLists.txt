cmake_minimum_required(VERSION 3.7)
project(src_carrier)

set(LIBOCM3 ../libopencm3)

set(CMAKE_C_COMPILER "/usr/bin/arm-none-eabi-gcc")
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/STM32F415RGT6.ld)
set(DEVICE STM32F4)
#set(CMAKE_VERBOSE_MAKEFILE on)

#I have no fucking idea how any of this works.
#Half of these commands do exactly nothing except break seemingly unrelated commands on removal...


set(GCC_LINKERFLAGS "--static -nostartfiles")
set(GCC_LINKERFLAGS "${GCC_LINKERFLAGS} -T${LINKER_SCRIPT}" )
#set(GCC_LINKERFLAGS ${GCC_LINKERFLAGS} -Wl,-Map=$(*).map )
set(GCC_LINKERFLAGS "${GCC_LINKERFLAGS} -Wl,--gc-sections" )
#set(GCC_LINKERFLAGS "${GCC_LINKERFLAGS} -Wl,--print-gc-sections" ) #Annoying as hell
set(GCC_LINKERFLAGS "${GCC_LINKERFLAGS}" )


set(GCC_LINKLIBRARIES "-lopencm3_stm32f4")
set(GCC_LINKLIBRARIES "${GCC_LINKLIBRARIES} --specs=rdimon.specs -Wl,--start-group -lgcc -lc -lm -Wl,--end-group  -lrdimon" )

set(GCC_FPFLAGS "-mfloat-abi=hard -mfpu=fpv4-sp-d16" )
set(GCC_ARCHFLAGS "-mthumb -mcpu=cortex-m4 ${GCC_FPFLAGS}" )

set(GCC_CFLAGS "-g -Os -Wextra -Wshadow -Wimplicit-function-declaration")
set(GCC_CFLAGS "${GCC_CFLAGS} -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes")
set(GCC_CFLAGS "${GCC_CFLAGS} -fno-common -ffunction-sections -fdata-sections")

set(GCC_CXXFLAGS "-g -O0")
set(GCC_CXXFLAGS "${GCC_CXXFLAGS} -Wextra -Wshadow -Wredundant-decls  -Weffc++")
set(GCC_CXXFLAGS "${GCC_CXXFLAGS} -fno-common -ffunction-sections -fdata-sections")

set(GCC_CPPFLAGS "${GCC_ARCHFLAGS} -MD -Wall -Wundef -I${LIBOCM3}/include -D${DEVICE}" )
#message("${GCC_ARCHFLAGS} -MD -Wall -Wundef -I${LIBOCM3}/include")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_CFLAGS} ${GCC_CCPFLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CXXFLAGS} ${GCC_CCPFLAGS}")


set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c leg.c leg.h jetson.c jetson.h)
include_directories(${LIBOCM3}/include)
link_directories(${LIBOCM3}/lib )
add_definitions( ${GCC_CPPFLAGS})


add_subdirectory(math)
add_subdirectory(drivers)
add_subdirectory(fdt)
include_directories(FDT_INCLUDES)

add_executable(src_carrier ${SOURCE_FILES} ${FDT_SOURCE})
set_target_properties(src_carrier PROPERTIES OUTPUT_NAME "carrier.elf")
target_link_libraries(src_carrier ${GCC_LINKLIBRARIES} ${GCC_LINKERFLAGS} ${GCC_ARCHFLAGS})
#set_target_properties(src_carrier PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})

