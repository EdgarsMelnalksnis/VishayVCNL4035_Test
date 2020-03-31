#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include "vcnl4035.h"

#define BIT(n) 1u<<n
#define DEV_ADDR 0x60
static void i2c_write(uint8_t,uint16_t);
int file_i2c; 

enum vcnl4035_regs {
	ALS_CONF = 0,	// 0x0
	ALS_THDH,	// 0x1
	ALS_THDL,	// 0x2
	PS_CONF,	// 0x3
	PS_CONF_MS,	// 0x4
	PS_CANC,	// 0x5
	PS_THDL,	// 0x6
	PS_THDH,	// 0x7
	PS1_DATA,	// 0x8
	PS2_DATA,	// 0x9
	PS3_DATA,	// 0xa
	ALS_DATA,	// 0xb
	WHITE_DATA,	// 0xc
	INT_FLAG,	// 0xd
	ID,		// 0xe
};

int main()
{
	
	int length;
	unsigned char buffer[60] = {0};

	
	//----- OPEN THE I2C BUS -----
	char *filename = (char*)"/dev/i2c-1";
	if ((file_i2c = open(filename, O_RDWR)) < 0)
	{
		//ERROR HANDLING: you can check errno to see what went wrong
		printf("Failed to open the i2c bus");
		return;
	}
	
	int addr = 0x60;          //<<<<<The I2C address of the slave
	if (ioctl(file_i2c, I2C_SLAVE, addr) < 0)
	{
		printf("Failed to acquire bus access and/or talk to slave.\n");
		//ERROR HANDLING; you can check errno to see what went wrong
		return;
	}ok, 
	
	
	//----- READ BYTES -----
	length = 4;			//<<< Number of bytes to read
	if (read(file_i2c, buffer, length) != length)		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		//ERROR HANDLING: i2c transaction failed
		printf("Failed to read from the i2c bus.\n");
	}
	else
	{
		printf("Data read: %s\n", buffer);
	}        

	
	//----- WRITE BYTES -----
	buffer[0] = 0x01;
	buffer[1] = 0x02;
	length = 2;			//<<< Number of bytes to write
	if (write(file_i2c, buffer, length) != length)		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
	}
	
	
	i2c_write( PS_CONF, BIT(14) | BIT(15));
	i2c_write( PS_CONF_MS, BIT(0) | BIT(3));

	i2c_write( PS_CONF_MS, BITL(3) | BITH(0));
	i2c_write( PS_CONF, (4 << 1) | (1 << 6) | (2 << 8) 
			| BITH(2) | BITH(3) | BITH(6));
	i2c_write(ALS_CONF, (2 << 5));
	i2c_write(PS_THDL, 0x1ff);
	
	
	
	
	
return 1;
}


static void i2c_write(uint8_t reg,uint16_t value)
{
	uint8_t dev_addr=DEV_ADDR;
uint8_t buff[4] = {dev_addr,reg,value & 0xff,value>>8};

if (write(file_i2c, buff, 4) != 4)		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
	}
}

