#ifndef __ZDT_H
#define __ZDT_H
#include "stdint.h"
#include "bsp_CAN.h"

#define pi 180.0 // 弧度制

// 电机旋转方向，以输出轴顺时针为正方向
typedef enum
{
	forward = 0,
	backward
} direction;

// 电机角度控制参数
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
#define ALL 0x00
#define J1 0x01
#define J2 0x02
#define J3 0x03
#define J4 0x04
#define J5 0x05
#define J6 0x06

// 定义各轴电机减速比
#define Reduction1 200
#define Reduction2 10
#define Reduction3 10
#define Reduction4 20
#define Reduction5 1
#define Reduction6 7

uint8_t ZDT_CAN_Init(void);
void encoder_Init(uint32_t ID);
uint8_t ZDT_Vel_Control(uint32_t ID, uint8_t dir, uint16_t vel, uint8_t ac, uint8_t m_status);
uint8_t ZDT_Stop(uint32_t ID, uint8_t m_status);
void ZDT_To_Position(position_parameter *parameter);
void ZDT_Read_CurrentPosition(uint8_t ID);
void ZDT_SetOrigin(uint32_t ID);
void ZDT_GoOrigin(uint32_t ID, uint8_t mode, uint8_t m_status);
void ZDT_MultiMotorMotion(uint8_t num, position_parameter *parameter);

#endif
