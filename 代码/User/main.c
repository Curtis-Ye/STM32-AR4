#include <stm32f10x.h>	    // 头文件引用(标准库); 内核、芯片外设....；(stm32f10x.conf.h, 对标准库头文件进行调用)
#include "stm32f10x_conf.h" // 头文件引用(标准库); 内核、芯片外设....；(stm32f10x.conf.h, 对标准库头文件进行调用)
#include "bsp_led.h"
#include "Jcode.h"
#include "FreeRTOS.h"
#include "task.h"
extern uint8_t USART1_Status;

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	     // 中断分组，组2:抢占级0~3,子优先级0~3 ; 全局只设置一次，尽量放在显眼的地方
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // 使能AFIO辅助功能IO时钟
	AFIO->MAPR |= 0x2 << 24;			     // 设置调试模式：使用SWD;  000_全开, 010_只开SWD, 100:全关; 作用:关闭JTAG只保留SWD, 即释放PB3、PB4、PA15，只需PA13、PA14
	USART1_Init(115200);				     // 串口初始化
	Led_Init();					     // LED 初始化; 板载LED的重要作用，配合代码调试，能方便地定位错误
	LED_BLUE_OFF;					     // 蓝灯，这里用作USB连接指示：亮-连接成功，灭-未连接
	limitSwitch_Init();				     // 限位开关初始化
	ZDT_CAN_Init();					     // CAN初始化
	ZDT_Delay_ms(1000);
	while (1)
	{
		simple_Stop();		// 限位开关保护任务
		if (USART1_Status == 1) // 串口处理任务
		{
			USART1_Data_process();
		}
		ZDT_Delay_ms(1);		  // 延时
		static uint16_t time = 0; // 用于计算过了多少时间
		if (time++ % 1000 == 0)	  // 每1秒对外发送一次报文，方便测试
		{
			LED_BLUE_TOGGLE; // 规律地闪烁LED，方便观察代码是否正常地跑
		}
		if (myCANReceiveFlag())
		{
			uint32_t ID;
			uint8_t len;
			uint8_t data[8];
			myCANReceiveData(&ID, &len, data);
		}
	}
}
