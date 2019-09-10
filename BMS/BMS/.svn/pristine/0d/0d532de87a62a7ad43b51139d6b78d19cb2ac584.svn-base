#include "lpc17xx.h"
#include "lpc17xx_gpio.h"
#include "type.h"
#include "i2c.h"

#define KEYPAD1_PORT 3
#define  KEYPAD1_PIN (1 << 26)

#define KEYPAD2_PORT 3
#define  KEYPAD2_PIN (1 << 25)

#define KEYPAD3_PORT 2
#define  KEYPAD3_PIN (1 << 13)

#define KEYPAD4_PORT 2
#define  KEYPAD4_PIN (1 << 12)



U8 ReadKeyPress1(){

	GPIO_SetDir(KEYPAD1_PORT,KEYPAD1_PIN,0);
	return (!(GPIO_ReadValue(KEYPAD1_PORT)&KEYPAD1_PIN));
}

U8 ReadKeyPress2(){

	GPIO_SetDir(KEYPAD2_PORT,KEYPAD2_PIN,0);
	return (!(GPIO_ReadValue(KEYPAD2_PORT)&KEYPAD2_PIN));
}

U8 ReadKeyPress3(){

	GPIO_SetDir(KEYPAD3_PORT,KEYPAD3_PIN,0);
	return (!(GPIO_ReadValue(KEYPAD3_PORT)&KEYPAD3_PIN));
}

U8 ReadKeyPress4(){

	GPIO_SetDir(KEYPAD4_PORT,KEYPAD4_PIN,0);
	return (!(GPIO_ReadValue(KEYPAD4_PORT)&KEYPAD4_PIN));
}
/*
int tca8418_write(int address, int reg, int value)
{
	int rc;
//	i2c_lpc_ctrl(address);

  //--- wr START + CONTROL
  rc = i2c_lpc_ctrl(address & 0xFE); //-- Now WR (RD/WI = 0)
   if(rc != I2C_NO_ERR)
   return rc;

//	i2c_lpc_wr_byte(0x68);
	i2c_lpc_wr_byte(reg);
	i2c_lpc_wr_byte(value);
	return 0;
}


int tca8418_read(int slave_address,int command, char * buf, int num)
{
	   int rc;
    //--- wr START + CONTROL
      rc = i2c_lpc_ctrl(slave_address & 0xFE); //-- Now WR (RD/WI = 0)
      if(rc != I2C_NO_ERR)
       return rc;

			i2c_lpc_wr_byte(command);

       //--- wr START + CONTROL again - for read start
      rc = i2c_lpc_ctrl(slave_address | 0x01); //-- Now RD (RD/WI = 1)
      if(rc != I2C_NO_ERR)
       return rc;

      rc = i2c_lpc_rx_to_buf(buf,num);
      if(rc != I2C_NO_ERR)
       return rc;

      i2c_lpc_stop();     //---- Set STOP ---
     return I2C_NO_ERR;

}

void tca8414_init(void)
{
//	char data[1]={0xFF};
	char buf[1]={0xFF};
 	//int ret;
 	int reg,error=0;

    tca8418_write(TCA8418_ADDRESS, REG_CFG, CFG_INT_CFG | CFG_OVR_FLOW_IEN | CFG_KE_IEN);

//  tca8418_write(TCA8418_ADDRESS, REG_GPIO_INT_EN3, 0x01);
    ret = tca8418_read(TCA8418_ADDRESS, REG_CFG, buf,1);

 Assemble a mask for row and column registers
//	reg  =  ~(~0 << 6);
//	reg += (~(~0 << 8)) << 8;

	reg  =  ~(~0 << 9);
	reg += (~(~0 << 9)) << 9;



	 Set registers to keypad mode
	error |= tca8418_write(TCA8418_ADDRESS, REG_KP_GPIO1, reg);
	error |= tca8418_write(TCA8418_ADDRESS, REG_KP_GPIO2, reg >> 8);
	error |= tca8418_write(TCA8418_ADDRESS, REG_KP_GPIO3, reg >> 16);

	 Enable column debouncing
	error |= tca8418_write(TCA8418_ADDRESS, REG_DEBOUNCE_DIS1, reg);
	error |= tca8418_write(TCA8418_ADDRESS, REG_DEBOUNCE_DIS2, reg >> 8);
	error |= tca8418_write(TCA8418_ADDRESS, REG_DEBOUNCE_DIS3, reg >> 16);

}
*/
