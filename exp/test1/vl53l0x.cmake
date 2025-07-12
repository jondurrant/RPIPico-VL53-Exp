if (DEFINED VL53L0X_PATH)
	message("Using Given VL53L0X_PATH '${VL53L0X_PATH}')")
else ()
	set(VL53L0X_PATH  "${CMAKE_CURRENT_LIST_DIR}/lib/VL53L0X-driver-pico-sdk-cpp/")
    message("Using local VL53L0X_PATH '${VL53L0X_PATH}')")
endif ()

add_library(VL53L0X STATIC)

target_sources(VL53L0X PUBLIC 
	${VL53L0X_PATH}/VL53L0X.cpp
)


# Add include directory
target_include_directories(VL53L0X PUBLIC 
    ${VL53L0X_PATH}/
)

# Add the standard library to the build
target_link_libraries(VL53L0X PUBLIC 
	pico_stdlib 
	hardware_i2c
)

