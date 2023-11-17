#include <reg51.h>
#include "Eeprom_.h"
#include "I2C_.h"
#include "INTRINS.H"

unsigned char bdata b;
sbit LSB1=b ^ 0;
sbit MSB1=b ^ 7;

void Eeprom_Trans(unsigned char x)				/*sending 8 bit adress to RTC  */
{
	unsigned char j;
	b=x;
	for(j=0;j<8;j++)
	{
		SCL=0;
		DELAY;
		SDA=MSB1;
		b<<=1;				/* sending bit by bit */
		_nop_();
		SCL=1;
		DELAY;
		SCL=0;
	}
	SDA=1;			 /*acknowledgement	*/
	SCL=1;
	DELAY;
	SCL=0;
	DELAY;
}

unsigned char Eeprom_Recv(unsigned char ACK_Flag)				
{
	unsigned char j;
	
	for(j=0;j<8;j++)
	{
		SCL=0;
		DELAY;
		LSB1 =SDA;
		if(j<7)
		b<<=1;				/* sending bit by bit */
		_nop_();
		SCL=1;
		DELAY;
		SCL=0;
	}
	if (ACK_Flag)
	{
		SDA=1;				  /* acknowledgement*/
		SCL=0;
		DELAY;
		SDA=0;
		SCL=1;
		DELAY;																						   
		SCL=0;
		DELAY;
	}
else
	{
		SDA=1;				  /* acknowledgement*/
		SCL=0;
		DELAY;
		SDA=1;
		SCL=1;
		DELAY;																						   
		SCL=0;
		DELAY;
	}
	return b;
}

void I2C_Write(unsigned char Slave_Write_Address , unsigned char Location, unsigned char val)
{
	IIC_Start();
	IIC_send_adr(Slave_Write_Address & 0xFE);
	IIC_send_adr(Location);
	Eeprom_Trans(val);
	IIC_Stop();
}


unsigned char I2C_Read(unsigned char Slave_Write_Address , unsigned char Location,unsigned char Slave_Read_Address)
{
	unsigned char kk;
	IIC_Start();
	IIC_send_adr(Slave_Write_Address & 0xFE );
	IIC_send_adr(Location);
	IIC_Start();
	IIC_send_adr(Slave_Read_Address | 0X01);
	kk=Eeprom_Recv(1);
	IIC_Stop();
	return kk;
}