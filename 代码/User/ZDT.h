#ifndef __ZDT_H
#define __ZDT_H
#include "stdint.h"
#include "bsp_CAN.h"

#define pi 180.0 // 弧度制

// 电机速度模式控制参数
typedef struct
{
	uint32_t ID;
	uint8_t dir;
	uint16_t vel;
	uint8_t ac;
	uint8_t m_status;
} velocity_parameter;

// 电机角度模式控制参数
typedef struct
{
	uint32_t ID;	  // 电机ID
	uint16_t vel;	  // 速度
	uint8_t ac;	  // 加速度
	float angle;	  // 目标角度
	uint8_t mode;	  // 运行模式：绝对/相对
	uint8_t m_status; // 多机同步标志
} position_parameter;

// 定义各个轴电机ID
typedef enum
{
	ALL,
	J1,
	J2,
	J3,
	J4,
	J5,
	J6
} motor_ID;

// 定义各轴电机减速比
#define Reduction1 50
#define Reduction2 10
#define Reduction3 10
#define Reduction4 10
#define Reduction5 1
#define Reduction6 7

uint8_t ZDT_CAN_Init(void);
void encoder_Init(uint32_t ID);
uint8_t ZDT_Vel_Control(velocity_parameter *parameter);
void ZDT_Stop(uint32_t ID, uint8_t m_status);
void ZDT_Pos_Control(position_parameter *parameter);
void ZDT_Read_CurrentPosition(uint8_t ID);
void ZDT_SetOrigin(uint32_t ID);
void ZDT_GoOrigin(uint32_t ID, uint8_t mode, uint8_t m_status);
void ZDT_Multi_PositonMotion(uint8_t num, position_parameter *parameter);
void ZDT_Multi_VelocityMotion(uint8_t num, velocity_parameter *parameter);
void ZDT_Delay_ms(uint32_t ms);
#endif
