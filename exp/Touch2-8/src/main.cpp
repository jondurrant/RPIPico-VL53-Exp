#include <stdio.h>
#include "pico/stdlib.h"

#define I2C_SDA 6
#define I2C_SCL 7
#define I2C_PORT i2c1



#include "hardware/clocks.h"
extern "C"{
#include "bsp_i2c.h"
#include "lv_port.h"
#include "bsp_battery.h"
#include "bsp_lcd_brightness.h"

}
#include "Widgets.h"
#include "VL53L0X.h"


VL53L0X *pSensor = NULL;

void set_cpu_clock(uint32_t freq_khz)
{
    set_sys_clock_khz(freq_khz, true);
    clock_configure(
        clk_peri,
        0,
        CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS,
        freq_khz * 1000,
        freq_khz * 1000);
}

int main()
{
    stdio_init_all();

	// Initialize I2C0 at 400kHz
	i2c_init(I2C_PORT, 400 * 1000);
	gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
	gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
	gpio_pull_up(I2C_SDA);
	gpio_pull_up(I2C_SCL);

	//Setup Sensor
	pSensor = new VL53L0X(I2C_PORT);
	pSensor->init();
	pSensor->setTimeout(500);
	pSensor->setMeasurementTimingBudget(300000);
	pSensor->startContinuous();


    bsp_battery_init(100);
    set_cpu_clock(220 * 1000);
    bsp_i2c_init();
    lv_port_init();
    bsp_lcd_brightness_init();
    bsp_lcd_brightness_set(50);

    Widgets_Init();
    while (true)
    {
        lv_timer_handler();
        sleep_ms(1);
    }
}
