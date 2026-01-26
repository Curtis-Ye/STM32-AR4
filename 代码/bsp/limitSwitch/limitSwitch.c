#include "stm32f10x.h" // Device header
#include "limitSwitch.h"
// 本项目使用限位开关为常开状态NO，初始状态引脚为高电平，开关触发引脚为低电平

uint8_t limitSwitchMode; // 初始值为0，具体用法为：初始状态时，限位开关全局函数循环。当状态改变，即调用回零函数，暂时跳出全局循环，避免出错。
uint8_t swStatusArr[5] = {0, 0, 0, 0, 0};

// 限位开关初始化
void limitSwitch_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = Switch1 | Switch2 | Switch3 | Switch4 | Switch5 | EmergencySwitch;
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
 * @brief 指定轴到达限位后停止运动
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
			if (swStatusArr[0] != 1)
			{
				ZDT_Stop(J1, 0);
				swStatusArr[0] = 1;
			}
			break;
		case Switch2:
			if (swStatusArr[1] != 1)
			{
				ZDT_Stop(J2, 0);
				swStatusArr[1] = 1;
			}
			break;
		case Switch3:
			if (swStatusArr[2] != 1)
			{
				ZDT_Stop(J3, 0);
				swStatusArr[2] = 1;
			}
			break;
		case Switch4:
			if (swStatusArr[3] != 1)
			{
				ZDT_Stop(J4, 0);
				swStatusArr[3] = 1;
			}
			break;
		case Switch5:
			if (swStatusArr[4] != 1)
			{
				ZDT_Stop(J5, 0);
				swStatusArr[4] = 1;
			}
			break;
		}
		return 0;
	}
	return 1;
}

/**
 * @brief 全局任务--到达限位后停止运动，需要一直循环检测
 * @param void
 * @retval void
 */
void simple_Stop(void)
{
	if (!limitSwitchMode)
	{
		uint8_t i = 1;
		uint8_t limitSwitch_Value[5] = {1, 1, 1, 1, 1}; // 限位开关值数组，初始状态为高电平1
		limitSwitch_Value[0] = limitSwitch_Getstatus(Switch1);
		limitSwitch_Value[1] = limitSwitch_Getstatus(Switch2);
		limitSwitch_Value[2] = limitSwitch_Getstatus(Switch3);
		limitSwitch_Value[3] = limitSwitch_Getstatus(Switch4);
		limitSwitch_Value[4] = limitSwitch_Getstatus(Switch5);
		for (; i <= 5; i++)
		{
			if (limitSwitch_Value[i - 1] == 0 && swStatusArr[i - 1] != 1)
			{
				ZDT_Stop(i, 0);
				swStatusArr[i - 1] = 1;
			}
		}
	}
}

/**
 * @brief 紧急停止，机械臂停止运动
 * @param void
 * @retval void
 */
void Emergency_Stop(void)
{
	if (limitSwitch_Getstatus(EmergencySwitch) == 0)
	{
		ZDT_Stop(0x00, 0);
	}
}
