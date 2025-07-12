/**
 * Jon Durrant.
 *
 *
 */

#include "pico/stdlib.h"
#include <stdio.h>

#include "VL53L0X.h"


#define I2C_SDA 4
#define I2C_SCL 5


int main(){


	stdio_init_all();

	sleep_ms(2000);
	printf("start\n");

	// Initialize I2C0 at 400kHz
	i2c_init(i2c0, 400 * 1000);
	gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
	gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
	gpio_pull_up(I2C_SDA);
	gpio_pull_up(I2C_SCL);


	// Initialize VL53L0X sensor
	VL53L0X sensor;
	sensor.init();
	sensor.setTimeout(500);

	sensor.setMeasurementTimingBudget(300000);

	// Start continuous back-to-back mode (take readings as fast as possible)
	sensor.startContinuous();

	for (;;){
		printf("millimeters: %d\n", sensor.readRangeContinuousMillimeters()-40);
		sleep_ms(1000);
	}


}
