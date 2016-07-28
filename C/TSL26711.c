// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// TSL26711
// This code is designed to work with the TSL26711_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/products

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

void main()
{
	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-1";
	if((file = open(bus, O_RDWR)) < 0)
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, TSL26711 I2C address is 0x39(57)
	ioctl(file, I2C_SLAVE, 0x39);

	// Select proximity time register OR with command register(0x02 | 0x80)
	// Ptime = 2.72 ms(0xFF)
	char config[2] = {0};
	config[0] = 0x02 | 0x80;
	config[1] = 0xFF;
	write(file, config, 2);
	// Select wait time register OR with command register(0x03 | 0x80)
	// Wtime = 2.72 ms(0xFF)
	config[0] = 0x03 | 0x80;
	config[1] = 0xFF;
	write(file, config, 2);
	// Select pulse count register OR with command register(0x0E | 0x80)
	// Pulse count = 32(0x20)
	config[0] = 0x0E | 0x80;
	config[1] = 0x20;
	write(file, config, 2);
	// Select control register OR with command register(0x0F | 0x80)
	// 100 mA LED strength, proximtiy uses CH1 diode, 1x PGAIN, 1x AGAIN(0x20)
	config[0] = 0x0F | 0x80;
	config[1] = 0x20;
	write(file, config, 2);
	// Select enable register OR with command register(0x00 | 0x80)
	// Set Power ON, proximity and wait enabled(0x0D)
	config[0] = 0x00 | 0x80;
	config[1] = 0x0D;
	write(file, config, 2);
	sleep(1);

	// Read 2 bytes of data from register (0x18 | 0x80)
	// proximity lsb, proximity msb
	char reg[1] = {0x18 | 0x80} ;
	write(file, reg, 1);
	char data[2] = {0};
	if(read(file, data, 2) != 2)
	{
		printf("Erorr : Input/output Erorr \n");
	}
	else
	{
		// Convert the data
		int proximity = (data[1] * 256 + data[0]);

		// Output data to screen
		printf("Proximity Of the Device : %d \n", proximity);
	}
}