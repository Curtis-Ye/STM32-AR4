#include <stm32f10x.h>
#include "bsp_CAN.h"
#include "ZDT.h"

uint8_t synchronize[4] = {0x00, 0xFF, 0x66, 0x6B}; // 多机同步指令

/*
	注意！！！！！
	角度模式控制单个电机运动时，多机同步标志设置为0，否则不运动
*/

static void delay_ms(uint32_t ms)
{
	ms = ms * 10240;
	for (uint32_t i = 0; i < ms; i++)
		; // 72MHz系统时钟下，多少个空循环约耗时1ms
}

// CAN总线初始化
uint8_t ZDT_CAN_Init(void)
{
	uint8_t ZDT_status;
	ZDT_status = CAN1_Config();
	return ZDT_status;
}

// 编码器校准
void encoder_Init(uint32_t ID)
{
	uint8_t cmd[3] = {0x06, 0x45, 0x6B};
	myCANSendData(cmd, 3);
}

/**
 * @brief 速度模式
 * @param 结构体指针
 * @retval void
 */
uint8_t ZDT_Vel_Control(velocity_parameter *parameter)
{
	uint8_t cmd[16] = {0};
	uint8_t acc;
	uint16_t motor_vel;
	switch (parameter->ID)
	{
	case J1:
		acc = Reduction1 * (parameter->ac);
		motor_vel = Reduction1 * (parameter->vel);
		break;
	case J2:
		acc = Reduction2 * (parameter->ac);
		motor_vel = Reduction2 * (parameter->vel);
		break;
	case J3:
		acc = Reduction3 * (parameter->ac);
		motor_vel = Reduction3 * (parameter->vel);
		break;
	case J4:
		acc = Reduction4 * (parameter->ac);
		motor_vel = Reduction4 * (parameter->vel);
		break;
	case J5:
		acc = Reduction5 * (parameter->ac);
		motor_vel = Reduction5 * (parameter->vel);
		break;
	case J6:
		acc = Reduction6 * (parameter->ac);
		motor_vel = Reduction6 * (parameter->vel);
		break;
	}
	cmd[0] = parameter->ID;
	cmd[1] = 0xF6;
	cmd[2] = parameter->dir;
	cmd[3] = (uint8_t)(motor_vel >> 8);
	cmd[4] = (uint8_t)(motor_vel >> 0);
	cmd[5] = acc;
	cmd[6] = parameter->m_status;
	cmd[7] = 0x6B;
	uint8_t transmit_mailbox = myCANSendData(cmd, 8);
	return transmit_mailbox;
}

/**
 * @brief 停止
 * @param 电机ID和多机同步标志
 * @retval void
 */
void ZDT_Stop(uint32_t ID, uint8_t m_status)
{
	uint8_t cmd[16] = {0};
	cmd[0] = ID;
	cmd[1] = 0xFE;
	cmd[2] = 0x98;
	cmd[3] = m_status;
	cmd[4] = 0x6B;
	uint8_t transmit_mailbox = myCANSendData(cmd, 5);
}

/**
 * @brief 位置模式
 * @param 结构体指针
 * @retval void
 */
void ZDT_Pos_Control(position_parameter *parameter)
{
	uint32_t pulse_temp;
	uint16_t motor_vel = 0;
	uint8_t dir = forward;
	switch (parameter->ID)
	{
	case J1:
		pulse_temp = Reduction1 * 12800;
		motor_vel = Reduction1 * (parameter->vel);
		break;
	case J2:
		pulse_temp = Reduction2 * 12800;
		motor_vel = Reduction2 * (parameter->vel);
		break;
	case J3:
		pulse_temp = Reduction3 * 12800;
		motor_vel = Reduction3 * (parameter->vel);
		break;
	case J4:
		pulse_temp = Reduction4 * 12800;
		motor_vel = Reduction4 * (parameter->vel);
		break;
	case J5:
		pulse_temp = Reduction5 * 12800;
		motor_vel = Reduction5 * (parameter->vel);
		break;
	case J6:
		pulse_temp = Reduction6 * 12800;
		motor_vel = Reduction6 * (parameter->vel);
		break;
	}
	if (motor_vel > 3000) // 电机速度限幅
		motor_vel = 3000;
	if ((parameter->angle) < 0) // 旋转方向判断
		dir = backward;
	uint32_t pulse = ((parameter->angle) / 360.0f) * pulse_temp;
	uint8_t cmd[16] = {0};
	cmd[0] = parameter->ID;
	cmd[1] = 0xFD;
	cmd[2] = dir;
	cmd[3] = (uint8_t)(motor_vel >> 8);
	cmd[4] = (uint8_t)(motor_vel >> 0);
	cmd[5] = parameter->ac;
	cmd[6] = (uint8_t)(pulse >> 24);
	cmd[7] = (uint8_t)(pulse >> 16);
	cmd[8] = (uint8_t)(pulse >> 8);
	cmd[9] = (uint8_t)(pulse >> 0);
	cmd[10] = parameter->mode;
	cmd[11] = parameter->m_status;
	cmd[12] = 0x6B;
	myCANSendData(cmd, 13);
}

// 读取当前角度
void ZDT_Read_CurrentPosition(uint8_t ID) // 角度换算：读取的十六进制角度值化为十进制*360/65536
{
	uint8_t cmd[16] = {0};
	cmd[0] = ID;
	cmd[1] = 0x36;
	cmd[2] = 0x6B;
	myCANSendData(cmd, 3);
}

// 设置原点
void ZDT_SetOrigin(uint32_t ID)
{
	uint8_t cmd[16] = {0};
	cmd[0] = ID;
	cmd[1] = 0x0A;
	cmd[2] = 0x6D;
	cmd[3] = 0x6B;
	myCANSendData(cmd, 4);
}

// 返回原点
void ZDT_GoOrigin(uint32_t ID, uint8_t mode, uint8_t m_status)
{
	uint8_t cmd[16] = {0};
	cmd[0] = ID;
	cmd[1] = 0x9A;
	cmd[2] = mode;
	cmd[3] = m_status;
	cmd[4] = 0x6B;
	myCANSendData(cmd, 5);
}

/**
 * @brief 多机同步位置模式
 * @param 电机数量和结构体指针
 * @retval void
 */
void ZDT_Multi_PositonMotion(uint8_t num, position_parameter *parameter)
{
	uint8_t i = 0;
	for (i = 0; i < num; i++)
	{
		ZDT_Pos_Control(parameter);
		parameter++;
		delay_ms(10);
	}
	myCANSendData(synchronize, 4);
}

/**
 * @brief 多机同步速度模式
 * @param 电机数量和结构体指针
 * @retval void
 */
void ZDT_Multi_VelocityMotion(uint8_t num, velocity_parameter *parameter)
{
	uint8_t i = 0;
	for (i = 0; i < num; i++)
	{
		ZDT_Vel_Control(parameter);
		parameter++;
		delay_ms(10);
	}
	myCANSendData(synchronize, 4);
}
