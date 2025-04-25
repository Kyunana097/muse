#include "oled.h"
#include "oledfont.h"  	 

//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 

/**********************************************
//IIC Start I2C发送起始
**********************************************/
void IIC_Start()
{
	OLED_SCLK_Set() ;
	OLED_SDIN_Set();
	OLED_SDIN_Clr();
	OLED_SCLK_Clr();
}

/**********************************************
//IIC Stop	I2C发送结束
**********************************************/
void IIC_Stop()
{
	OLED_SCLK_Set() ;
	OLED_SDIN_Clr();
	OLED_SDIN_Set();
}

/**********************************************
//IIC wait	I2C等待接受应答
**********************************************/
void IIC_Wait_Ack()
{
	OLED_SCLK_Set() ;
	OLED_SCLK_Clr();
}

/**********************************************
// IIC Write byte	写入字节
**********************************************/
void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	unsigned char m,da;
	da=IIC_Byte;	//要写入的字节
	OLED_SCLK_Clr();
	for(i=0;i<8;i++)	//循环八次进行掩码	
	{
			m=da;
		//	OLED_SCLK_Clr();
		m=m&0x80;				//1000 0000 从最高位开始取位 直至取完
		if(m==0x80)				//若该位为1
		{OLED_SDIN_Set();}		//传1
		else OLED_SDIN_Clr();	//为0传0
			da=da<<1;			//转下一位
		OLED_SCLK_Set();		//本次写入结束
		OLED_SCLK_Clr();
		}
}

/**********************************************
// IIC Write Command 写命令
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
   IIC_Start();
   Write_IIC_Byte(0x78);            //Slave address,SA0=0 从机地址0x78
	IIC_Wait_Ack();	
   Write_IIC_Byte(0x00);			//write command
	IIC_Wait_Ack();	
   Write_IIC_Byte(IIC_Command); 
	IIC_Wait_Ack();	
   IIC_Stop();
}
/**********************************************
// IIC Write Data 写数据
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
   IIC_Start();
   Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
	 IIC_Wait_Ack();	
   Write_IIC_Byte(0x40);			//write data
	 IIC_Wait_Ack();	
   Write_IIC_Byte(IIC_Data);
	 IIC_Wait_Ack();	
   IIC_Stop();
}
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)							//输入0写命令(OLED_CMD) 1写数据(OLED_DATA)
	{
		Write_IIC_Data(dat);
	}
	else 
	{	
		Write_IIC_Command(dat);
	}
}


//坐标设置
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
} 

//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //更新显示
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反色显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(Char_Size ==16)
		{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
		}
		else 
		{	
			OLED_Set_Pos(x,y);
			for(i=0;i<6;i++)
			OLED_WR_Byte(F6x8[c][i],OLED_DATA);
		}
}		

//显示一个字符号串
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}

/**
 * @brief 显示 0~1000 的数值（自动计算位数，最小内存）
 * @param x: X 坐标（0~127）
 * @param y: Y 坐标（0~7）
 * @param value: 要显示的值（0~1000）
 * @param size: 字体大小（8 或 16）
 */
void OLED_ShowValue(u8 x, u8 y, u32 value, u8 size) 
{
	u8 i = 0;
	char buffer[5]; // 存储 "0"~"1000"（4 位 + '\0'）

	// 将数字转换为字符串
	buffer[0] = (value / 1000) + '0';       // 千位（0 或 1）
	buffer[1] = ((value / 100) % 10) + '0'; // 百位
	buffer[2] = ((value / 10) % 10) + '0';  // 十位
	buffer[3] = (value % 10) + '0';         // 个位
	buffer[4] = '\0';                       // 字符串结束符

	// 跳过前导零（可选，如 "0100" → "100"）
	while (i < 3 && buffer[i] == '0') i++;

	// 显示字符串
	OLED_ShowString(x, y, (u8*)&buffer[i], size);
}

//初始化SSD1306					    
void OLED_Init(void)
{ 	
 
	OLED_WR_Byte(0xAE,OLED_CMD);//--display off
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	OLED_WR_Byte(0x81,OLED_CMD); // contract control
	OLED_WR_Byte(0xFF,OLED_CMD);//--128   
	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//
	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0x80,OLED_CMD);//
	
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//
	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//
	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//
	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
}  