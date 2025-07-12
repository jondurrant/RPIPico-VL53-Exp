cmake_minimum_required(VERSION 3.13)

set(NAME LVGL) 
set(PICO_BOARD pico2)
set(PICO_PLATFORM rp2350)

include("$ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake")
project(${NAME} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(PICO_CXX_ENABLE_EXCEPTIONS 1)

# Initialize the SDK
pico_sdk_init()


SET(BSP_PATH "${CMAKE_CURRENT_LIST_DIR}/../../lib/bsp" CACHE STRING "Common Lib")
include(bsp.cmake)

SET(LVGL_DIR "${CMAKE_CURRENT_LIST_DIR}/../../lib/lvgl" CACHE STRING "Common Lib")
include(lvgl.cmake)

set(VL53L0X_PATH  "${CMAKE_CURRENT_LIST_DIR}/../../lib/VL53L0X-driver-pico-sdk-cpp/")
include(vl53l0x.cmake)

add_subdirectory(src)


#Set up files for the release packages
install(CODE "execute_process(COMMAND $ENV{HOME}/bin/picoDeploy.sh ${CMAKE_CURRENT_BINARY_DIR}/src/${NAME}.elf)")

# Set up files for the release packages
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/src/${NAME}.uf2
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_GENERATOR "ZIP" "TGZ")
include(CPack)
