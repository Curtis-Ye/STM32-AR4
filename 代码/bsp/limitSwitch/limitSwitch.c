#include "stm32f10x.h" // Device header
#include "limitSwitch.h"
// 本项目定义限位开关为常开状态NO

// 限位开关初始化
void limitSwitch_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = Switch1 | Switch2 | Switch3 | Switch4 | Switch5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LimitSwitch_Pin, &GPIO_InitStructure);
}

/**
 * @brief 获取限位开关状态
 * @param 开关对应IO口
 * @retval 0/1
 */
uint8_t limitSwitch_Getstatus(uint16_t GPIO_Pin)
{
	return GPIO_ReadInputDataBit(LimitSwitch_Pin, GPIO_Pin);
}

/**
 * @brief 到达限位后停止运动
 * @param 开关对应IO口
 * @retval 0/1 如果未回归原点则返回1,否则返回0
 */
uint8_t limitStop(uint16_t GPIO_Pin)
{
	if (limitSwitch_Getstatus(GPIO_Pin) == 0)
	{
		switch (GPIO_Pin)
		{
		case Switch1:
			ZDT_Stop(J1, 0);
			break;
		case Switch2:
			ZDT_Stop(J2, 0);
			break;
		case Switch3:
			ZDT_Stop(J3, 0);
			break;
		case Switch4:
			ZDT_Stop(J4, 0);
			break;
		}
		return 0;
	}
	return 1;
}
