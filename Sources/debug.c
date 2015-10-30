#define __DEBUG_C_
#include "includes.h"
int flagkey1=0;
int flagkey2=0;
int flagkey3=0;
int flagkey4=0;
void set_key(void)
{
	if(key1==0)
	{
		delay_us(30);
		if(key1==0)	
		{
			while(!key1){}
			if(flagkey1==0)
			{
				flagkey1=1;
				flagkey2=0;
				flagkey3=0;
				flagkey4=0;
			}
			else flagkey1++;
			if(flagkey1>=3) flagkey1=0;
			LCD_Fill(0x00);
			/*功能待添加*/
		}
	}
	
	if(key2==0)
	{
		delay_us(30);
		if(key2==0)	
		{
			while(!key2){}
			flagkey1=0;
			flagkey2=1;
			flagkey3=0;
			flagkey4=0;
			LCD_Fill(0x00);
			/*功能待添加*/
		}
	}
	
	if(key3==0)
	{
		delay_us(30);
		if(key3==0)	
		{
			while(!key3) {}
			flagkey1=0;
			flagkey2=0;
			flagkey3=1;
			flagkey4=0;
			LCD_Fill(0x00);
			maxep=0;maxecp=0;
			maxen=0;maxecn=0;
			/*功能待添加*/
		}
	}
	
	if(key4==0)
	{
		delay_us(30);
		if(key4==0)	
		{
			while(!key4){}
			flagkey1=0;
			flagkey2=0;
			flagkey3=0;
			flagkey4=1;
			dis_count=0;
			LCD_Fill(0x00);
			/*功能待添加*/
		}
	}

}
