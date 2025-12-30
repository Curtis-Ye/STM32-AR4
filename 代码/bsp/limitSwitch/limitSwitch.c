#include "stm32f10x.h" // Device header
#include "limitSwitch.h"

// 定义限位开关为常开状态NO
// 限位开关初始化
void limitSwitch_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = switch1 | switch2 | switch3 | switch4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(limitSwitch_Pin, &GPIO_InitStructure);
}

// 获取开关电平，由于开关常开即未触发时断开，IO口采用上拉输入，未输入状态为高电平，开关按下时为低电平
uint8_t limitSwitch_Getstatus(uint16_t GPIO_Pin)
{
	return GPIO_ReadInputDataBit(limitSwitch_Pin, GPIO_Pin);
}
