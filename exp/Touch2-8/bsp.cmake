if (DEFINED BSP_PATH)
	message("Using Given BSP_PATH '${BSP_PATH}')")
else ()
	set(BSP_PATH "${CMAKE_CURRENT_LIST_DIR}lib/bsp/")
    message("Using local BSP_PATH '${BSP_PATH}')")
endif ()

add_library(bsp STATIC)
target_sources(bsp PUBLIC
  ${BSP_PATH}/bsp_battery.c
  ${BSP_PATH}/bsp_cst328.c
  ${BSP_PATH}/bsp_dma_channel_irq.c
  ${BSP_PATH}/bsp_i2c.c
  ${BSP_PATH}/bsp_i2s.c
  ${BSP_PATH}/bsp_lcd_brightness.c
  ${BSP_PATH}/bsp_pcf85063.c
  ${BSP_PATH}/bsp_qmi8658.c
  ${BSP_PATH}/bsp_st7789.c
)

# Add include directory
target_include_directories(bsp PUBLIC 
    ${BSP_PATH}
)

pico_generate_pio_header(bsp 
	${BSP_PATH}/bsp_i2s.pio
	)


target_link_libraries(bsp PUBLIC 
    pico_stdlib 
    hardware_spi 
    hardware_i2c 
    hardware_pwm 
    hardware_adc 
    hardware_dma
    hardware_pio
    hardware_irq
    )