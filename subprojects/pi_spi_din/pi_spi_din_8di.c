#include <spi.h>
#include <wiringPi.h> 
//#include <wiringPiSPI.h>

#include "pi_spi_din.h"

uint8_t pi_spi_din_8di_read(enum chip_enable ce, uint8_t address)
{
	address = address > 3 ? 3 : address;
	
	uint8_t data[3] =
	{
		0x41 | (address << 1),	// Read command
		0x09, 					// GPIO register
		0x00
	};
	
	/*digitalWrite(chip_select, LOW);
	wiringPiSPIDataRW(0, data, 3);
	digitalWrite(chip_select, HIGH);*/
	
	spi_transfer(ce, data, 3);
	
	return data[2];
}

uint8_t pi_spi_din_8di_read_single(enum chip_enable ce, uint8_t address, uint8_t channel)
{
	channel = channel > 7 ? 7 : channel;
	
	return pi_spi_din_8di_read(ce, address) >> channel & 0x01;
}

void pi_spi_din_8di_init(enum chip_enable ce, uint8_t address)
{
	address = address > 3 ? 3 : address;
	
	uint8_t data[] =
	{
		0x40 | (address << 1),	// Write command
		0x00, 					// IODIR register
		0xFF,					// IODIR is input
		0xFF					// IPOL is inverted
	};
	
	/*digitalWrite(chip_select, LOW);
	wiringPiSPIDataRW(0, data, 4);
	digitalWrite(chip_select, HIGH);*/
	
	spi_transfer(ce, data, 4);
	
	// Enable hardware addressing
	data[0] = 0x40 | (address << 1);	// Write command
	data[1] = 0x05;						// IOCON register
	data[2] = 0x08;						// HAEN
	
	/*digitalWrite(chip_select, LOW);
	wiringPiSPIDataRW(0, data, 3);
	digitalWrite(chip_select, HIGH);*/
	
	spi_transfer(ce, data, 3);
}
