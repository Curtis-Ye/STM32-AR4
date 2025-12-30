#include <stm32f10x.h>	    // 头文件引用(标准库); 内核、芯片外设....；(stm32f10x.conf.h, 对标准库头文件进行调用)
#include "stm32f10x_conf.h" // 头文件引用(标准库); 内核、芯片外设....；(stm32f10x.conf.h, 对标准库头文件进行调用)
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_usart.h"
#include "ZDT.h"
#include "Filter.h"
#include "limitSwitch.h"
#include "bsp_w25qxx.h"	     // Flash存储器
#include "bsp_lcd_ili9341.h" // LCD

position_parameter parameter333 = {J1, 100, 100, 30, 1, 0};

static void delay_ms(uint32_t ms)
{
	ms = ms * 10240;
	for (uint32_t i = 0; i < ms; i++)
		; // 72MHz系统时钟下，多少个空循环约耗时1ms
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	     // 中断分组，组2:抢占级0~3,子优先级0~3 ; 全局只设置一次，尽量放在显眼的地方
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // 使能AFIO辅助功能IO时钟
	AFIO->MAPR |= 0x2 << 24;			     // 设置调试模式：使用SWD;  000_全开, 010_只开SWD, 100:全关; 作用:关闭JTAG只保留SWD, 即释放PB3、PB4、PA15，只需PA13、PA14
	USART1_Init(115200);				     // 串口初始化
	Led_Init();					     // LED 初始化; 板载LED的重要作用，配合代码调试，能方便地定位错误
	LED_BLUE_OFF;					     // 蓝灯，这里用作USB连接指示：亮-连接成功，灭-未连接
	limitSwitch_Init();				     // 限位开关初始化

	// LCD显示屏相关初始化
	W25qx_Init();
	LCD_Init();

	if (ZDT_CAN_Init()) // CAN初始化
	{
		LCD_String(0, 48, "正在初始化...", 24, WHITE, BLACK);
	}
	delay_ms(1000);

	LCD_String(0, 48, "                ", 24, WHITE, BLACK); // 清空上条显示内容

	// LCD屏幕显示内容
	LCD_String(30, 8, "Locus六轴机械臂", 24, WHITE, BLACK);
	LCD_Line(0, 40, 239, 40, WHITE);
	LCD_String(0, 48, "各关节旋转角度:", 16, WHITE, BLACK);
	LCD_String(0, 68, "J1:", 16, WHITE, BLACK);
	LCD_String(0, 88, "J2:", 16, WHITE, BLACK);
	LCD_String(0, 108, "J3:", 16, WHITE, BLACK);
	LCD_String(0, 128, "J4:", 16, WHITE, BLACK);
	LCD_String(0, 148, "J5:", 16, WHITE, BLACK);
	LCD_String(0, 168, "J6:", 16, WHITE, BLACK);

	while (1)
	{
		delay_ms(1);		  // 延时
		static uint16_t time = 0; // 用于计算过了多少时间
		if (time++ % 1000 == 0)	  // 每1秒对外发送一次报文，方便测试
		{
			LED_BLUE_TOGGLE; // 规律地闪烁LED，方便观察代码是否正常地跑
		}
		ZDT_To_Position(&parameter333);
		if (myCANReceiveFlag())
		{
			uint32_t ID;
			uint8_t len;
			uint8_t data[8];
			myCANReceiveData(&ID, &len, data);
		}
	}
}
