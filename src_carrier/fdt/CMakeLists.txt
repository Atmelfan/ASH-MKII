cmake_minimum_required(VERSION 3.7)

set(FDT_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
file(GLOB FDT_SOURCE *.c)

# Raw DTS sources
set(DTS_SOURCES gait_normal.dts gpa_system.dts ${DTS_SOURCES})

# Target to create dtb/dtbs directories
add_custom_target(make_dtb_dir
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/dtb ${CMAKE_BINARY_DIR}/dtbs
        )

# Compile and generate assembly for each source
foreach(dts_src IN ITEMS ${DTS_SOURCES})

    # Extract filename
    GET_FILENAME_COMPONENT(_basename ${dts_src} NAME_WE)

    # Compile FDTs...
    add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/dtb/${_basename}.dtb
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${dts_src} make_dtb_dir
            COMMAND dtc ${CMAKE_CURRENT_SOURCE_DIR}/${dts_src} -O dtb -o ${CMAKE_BINARY_DIR}/dtb/${_basename}.dtb -H epapr
            #COMMAND ${CMAKE_COMMAND} -E echo "Compiling ${_basename}.dtb..."
            VERBATIM
            )

    # Generate assembly files including above
    add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/dtbs/${_basename}.S
            DEPENDS ${CMAKE_BINARY_DIR}/dtb/${_basename}.dtb ${CMAKE_BINARY_DIR}/dtb
            DEPENDS make_dtb_dir
            COMMAND ${CMAKE_C_COMPILER} -E "${CMAKE_CURRENT_SOURCE_DIR}/dtb_templ.S" -o "${CMAKE_BINARY_DIR}/dtbs/${_basename}.S" -DFDT_FILE="${CMAKE_BINARY_DIR}/dtb/${_basename}.dtb" -DFDT_NAME=${_basename}
            #COMMAND ${CMAKE_COMMAND} -E echo "Creating ${_basename}.S..."
            VERBATIM
            )

    # Add assembly file to sources
    set(FDT_BLOB_SOURCE ${CMAKE_BINARY_DIR}/dtbs/${_basename}.S ${FDT_BLOB_SOURCE} )
endforeach()

message(INFO " ${FDT_BLOB_SOURCE}")

# Target to
add_custom_target(fdt_blobs ALL
        DEPENDS ${FDT_BLOB_SOURCE}
        )
set(FDT_SOURCE ${FDT_SOURCE} ${FDT_BLOB_SOURCE} PARENT_SCOPE)



