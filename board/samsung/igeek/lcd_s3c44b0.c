/*
 * spibash.c
 *
 *  Created on: Jun 11, 2015
 *      Author: darkspr1te
 */
#include "lcd_s3c44b0.h"
#include "font.h"
/*
 * set LCD rotation and pallette below
 * */
//#define CONFIG_LCD_FLIP
//#define CONFIG_LCD_INVERT
/*
 * 
#define PAGE_COUNT 4
#define COLUMN_COUNT 128
* 
#define STARTBYTES 1
*/
#define STARTBYTES 0
#define LCDWIDTH 128
#define LCDHEIGHT 64
int pagemap[] = { 3, 2, 1, 0, 7, 6, 5, 4 };
int newmap[] = {4,5,6,7,0,1,2,3};
#define ORT = 1;

void none_void(void)
{
	int i=0;
	i=1;
}
void drawchar(int *buff, int x, int line, int c) {
	int i=0;
  for (i=0; i<5; i++ ) {
    buff[x + (line*128) ] = font[(c*5)+i];
    x++;
  }
//buff[(x+6)+(line*128)] =0x0;
 }

void init_port(void)
{

	PORTC_LCD_CONFIG_ADDR = PORTC_LCD_CONFIG; //config all port as output
	PORTC_LCD_DATA = 0x0; //all pins to low
	//PCONF =PORTFUART;
	/* wait at least 80us - let's wait 1 ms */
	//udelay (1000);
}

void draw_string(char *buff,int *buffer_data, int line)
{

	short counter=0;


	while (buff[counter] != '\0') {
		  drawchar(buffer_data, (counter*6), line, (buff[counter]));
		  counter++;
	}


}
void shiftOut(int dataPin, int clockPin, int val)
{
        int mask,i;

        PORTC_LCD_DATA = PORTC_LCD_DATA & (0xFFFFFFFF - CLK_PIN);
        PORTC_LCD_DATA = PORTC_LCD_DATA & (0xFFFFFFFF - SDO_PIN);
   //     initMsTimer0(1);//delay 3ms
  //      launchAndWaitTimer0();
        int count = 8;
        mask = 1 << (count - 1);
        for (i = 0; i < count; i++) {   // send 'count' bits to PHY register

        	PORTC_LCD_DATA = PORTC_LCD_DATA & (0xFFFFFFFF - CLK_PIN);
          if (mask & val)
        	  PORTC_LCD_DATA = (PORTC_LCD_DATA  |SDO_PIN);
           else
        	  PORTC_LCD_DATA = PORTC_LCD_DATA & (0xFFFFFFFF - SDO_PIN);
          mask >>= 1;
          PORTC_LCD_DATA = PORTC_LCD_DATA | CLK_PIN; /*rising edge SDIO sampled*/
         }
   //     initMsTimer0(1);//delay 1ms to allow spi to settle before sending anything more
  //      launchAndWaitTimer0();
        PORTC_LCD_DATA = (PORTC_LCD_DATA  |SDO_PIN); // put all lines high after
        PORTC_LCD_DATA = PORTC_LCD_DATA | CLK_PIN;
//        PORTC_LCD_DATA = PORTC_LCD_DATA | CS_PIN;
}
void reset_lcd(void)
{
	PORTC_LCD_DATA = PORTC_LCD_DATA & (0xFFFFFFFF - CS_PIN); //chip select low so LCD listens
	PORTC_LCD_DATA = PORTC_LCD_DATA & (0xFFFFFFFF - RESET_PIN);//reset low so we reset
		/* wait at least 80us - let's wait 1 ms */
	udelay (1000);
	PORTC_LCD_DATA = PORTC_LCD_DATA | RESET_PIN; //bring out of reset , pin high
	PORTC_LCD_DATA = PORTC_LCD_DATA | CS_PIN;//release chip select for now

	//CS HIGH , dont know it's high low yet but label is /cs which is active low
	/* wait at least 80us - let's wait 1 ms */
	udelay (1000);
	PORTC_LCD_DATA = PORTC_LCD_DATA & (0xFFFFFFFF - CS_PIN);//chip select low to LCD listens
	PORTC_LCD_DATA = PORTC_LCD_DATA & (0xFFFFFFFF - RESET_PIN);//reset low we we reset, only required on odd lcd configs

		/* wait at least 80us - let's wait 1 ms */
	udelay (1000);

	PORTC_LCD_DATA = PORTC_LCD_DATA | RESET_PIN; //bring out of reset, pin high, we should be ready now
	//i think the rest not needed
//	PORTC_LCD_DATA = PORTC_LCD_DATA & (0xFFFFFFFF - CS_PIN); //chip select low
//	PORTC_LCD_DATA = PORTC_LCD_DATA |CLK_PIN;
//	PORTC_LCD_DATA = (PORTC_LCD_DATA  |SDO_PIN);
// return 0;
}
void send_data(int cmd)
{
	//void
	PORTC_LCD_DATA = PORTC_LCD_DATA & (0xFFFFFFFF - CS_PIN);
	PORTC_LCD_DATA = PORTC_LCD_DATA | RS_PIN;
	shiftOut(0,0,cmd);
//	PORTC_LCD_DATA = PORTC_LCD_DATA & (0xFFFFFFFF - RS_PIN);

}

void send_cmd(int cmd)
{
	//void
	PORTC_LCD_DATA = PORTC_LCD_DATA & (0xFFFFFFFF - CS_PIN);
	PORTC_LCD_DATA = PORTC_LCD_DATA & (0xFFFFFFFF - RS_PIN);
	shiftOut(0,0,cmd);
	PORTC_LCD_DATA = PORTC_LCD_DATA |CS_PIN;
//	PORTC_LCD_DATA = PORTC_LCD_DATA | RS_PIN;
}
void lcd_init(void)
{
	// LCD bias select
	  send_cmd(CMD_SET_BIAS_7);
	  // ADC select
	/* wait at least 80us - let's wait 1 ms */

	udelay (1000);
#ifdef CONFIG_LCD_FLIP
  send_cmd(CMD_SET_ADC_NORMAL);
#else
  send_cmd(CMD_SET_ADC_REVERSE); /*flips up/down */
#endif  
	  // SHL select
	/* wait at least 80us - let's wait 1 ms */
	udelay(1000);
	//send_cmd(CMD_SET_DISP_REVERSE); /*reverses color*/

	udelay (1000);
#ifdef CONFIG_LCD_FLIP
	  send_cmd(CMD_SET_COM_NORMAL);
#else
	  send_cmd(CMD_SET_COM_REVERSE );
#endif
	  // Initial display line
		/* wait at least 80us - let's wait 1 ms */
	udelay (1000);
	  send_cmd(CMD_SET_DISP_START_LINE);
		/* wait at least 80us - let's wait 1 ms */
	udelay (1000);
	  // turn on voltage converter (VC=1, VR=0, VF=0)
	  send_cmd(CMD_SET_POWER_CONTROL | 0x4);
	  // wait for 50% rising
		/* wait at least 80us - let's wait 1 ms */
	udelay (1000);

	  // turn on voltage regulator (VC=1, VR=1, VF=0)
	  send_cmd(CMD_SET_POWER_CONTROL | 0x6);
	  // wait >=50ms
		/* wait at least 80us - let's wait 1 ms */
	udelay (1000);
	  // turn on voltage follower (VC=1, VR=1, VF=1)
	  send_cmd(CMD_SET_POWER_CONTROL | 0x7);
	  // wait
		/* wait at least 80us - let's wait 1 ms */
	udelay (1000);

	  // set lcd operating voltage (regulator resistor, ref voltage resistor)
	  send_cmd(CMD_SET_RESISTOR_RATIO | 0x6);
	/* wait at least 80us - let's wait 1 ms */
	udelay (1000);

	  send_cmd(CMD_DISPLAY_ON);
	  	/* wait at least 80us - let's wait 1 ms */
	udelay (1000);
		/* wait at least 80us - let's wait 1 ms */
	udelay (1000);
	  	/* wait at least 80us - let's wait 1 ms */
	udelay (1000);
	/* wait at least 80us - let's wait 1 ms */
	udelay (1000);
	
	
}

/*
 * easier to blank buffer then write buffer
 */
void clear_screen(void)
{
	int c, p;
	send_cmd(CMD_DISPLAY_OFF);
	for (p=0 ; p < 8;p++)
		{
		send_cmd(CMD_SET_PAGE |p);
		for (c=0;c >128; c++)
		{
			send_cmd(CMD_SET_COLUMN_LOWER | (c & 0xf));
			send_cmd(CMD_SET_COLUMN_UPPER | ((c >> 4) & 0xf));
			send_data(0xff);
		}
	}
	send_cmd(CMD_DISPLAY_ON);
}
void write_buffer(int *buffer_data) {
  int c, y,col,maxcol;
  y=0;c=0;
  col=0;
  
  send_cmd(CMD_SET_DISP_START_LINE);

  for(y = 0; y < 8; y++) {
	send_cmd(CMD_SET_PAGE | pagemap[y]);
    send_cmd(CMD_SET_COLUMN_LOWER | ((col+STARTBYTES) & 0xf));
    send_cmd(CMD_SET_COLUMN_UPPER | (((col+STARTBYTES) >> 4) & 0xf));
    send_cmd(CMD_RMW); //set read-modify-write to reduce load on mpu/cpu
 //   send_cmd(0x55);
 maxcol = LCDWIDTH -1 ;
    for(c = 0; c < 128; c++) {
     send_data(buffer_data[(128*y)+c]);
     // for(; col <= maxcol; col++) {

    //  send_data(buffer_data[(128*y)+col]);

    	}
    }

  send_cmd(CMD_RMW_CLEAR);
}

void empty_buffer(int *buffer_data) {
	 int c, y;
	  y=0;c=0;
	  send_cmd(CMD_SET_DISP_START_LINE);

	  for(y = 0; y < 8; y++) {
		send_cmd(CMD_SET_PAGE | pagemap[y]);
	    send_cmd(CMD_SET_COLUMN_LOWER | (0x1 & 0xf));
	    send_cmd(CMD_SET_COLUMN_UPPER | ((0x1 >> 4) & 0xf));
	    send_cmd(CMD_RMW); //set read-modify-write to reduce load on mpu/cpu
	    for(c = 1; c < 129; c++) {
	    	buffer_data[(128*y)+c]=0x00;
	 	    	}
	      	  }

	  send_cmd(CMD_RMW_CLEAR);
}

#define start_page 1
#define start_column 1


void Set_Column_Address(unsigned char add)
{
    send_cmd((0x10|(add>>4)));
	send_cmd((0x0f&add));
	return;
}
void Set_Page_Address(unsigned char add)
{
    add=0xb0|add;
    send_cmd(add);
	return;
}

void write_num(int *n,int k,int station_dot)
{
	int i; 
			for(i=0;i<8;i++)
				{
				send_data(*(n+16*k+i));
				}
				
	Set_Page_Address(start_page+1)	;
    Set_Column_Address(start_column+station_dot*8);	
			for(i=8;i<16;i++)
				{
				send_data(*(n+16*k+i));
				}
}
void Initial_Dispay_Line(int line)
{
    line|=0x40;
    send_cmd(line);
	return;
}

void display_picture(int pic[])
{
   int i,j,tmp_offset;
   tmp_offset =3;//when 
	//Initial_Dispay_Line(0x40);
		Initial_Dispay_Line(0x40);
	for(i=0;i<0x08;i++)
	{
	Set_Page_Address(newmap[i]);
	Set_Column_Address(0x00+tmp_offset);
	//send_cmd(CMD_SET_PAGE | pagemap[i]);
  //  send_cmd(CMD_SET_COLUMN_LOWER | ((0x1+tmp_offset) & 0xf));
  //  send_cmd(CMD_SET_COLUMN_UPPER | (((0x1+tmp_offset) >> 4) & 0xf));
        for(j=0;j<0x84;j++)
		{
		    send_data(pic[i*0x84+j]);
		}
		//this lcd is 132 wide so add on 3 bits to end as our structure is set for 128 support
		//todo add param for image/array size
	//	send_data(0);
		//send_data(0);
	//	send_data(0);
	}
    return;
    
 
}

