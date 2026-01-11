#include <stm32f10x.h>
#include "origin.h"

#define SWITCHNUM 5

/**
 * @brief 所有轴回归原点并设置原点
 * @param void
 * @retval 1 如果回归原点则返回1
 */
uint8_t All_Go_Origin(void)
{
        velocity_parameter origin[5] = {{J1, 1, 30, 10, 1}, {J2, 1, 30, 10, 1}, {J3, 1, 30, 10, 1}, {J4, 1, 30, 10, 1}, {J5, 1, 30, 10, 1}};
        uint8_t originArr[5] = {0, 0, 0, 0, 0};
        uint8_t SwitchArr[SWITCHNUM];
        uint8_t m_state = 1;
        ZDT_Multi_VelocityMotion(5, origin);
        while (m_state)
        {
                SwitchArr[0] = limitStop(Switch1);
                SwitchArr[1] = limitStop(Switch2);
                SwitchArr[2] = limitStop(Switch3);
                SwitchArr[3] = limitStop(Switch4);
                SwitchArr[4] = limitStop(Switch5);
                for (uint8_t i = 0; i < SWITCHNUM; i++)
                {
                        if (SwitchArr[i] != originArr[i])
                        {
                                m_state = 1;
                        }
                        else
                        {
                                m_state = 0;
                        }
                }
        }
        for (uint8_t j = 1; j <= 5; j++)
        {
                ZDT_SetOrigin(j);
        }
        return 1;
}

/**
 * @brief 其中一轴回归原点并设置原点
 * @param 电机ID
 * @retval 1 如果回归原点则返回1
 */
uint8_t Go_Origin(uint32_t ID)
{
        uint8_t limitSwitch_State;
        uint16_t GPIO_Pin;
        velocity_parameter origin = {ID, 1, 30, 10, 1};
        ZDT_Multi_VelocityMotion(1, &origin);
        switch (ID)
        {
        case J1:
                GPIO_Pin = Switch1;
                break;
        case J2:
                GPIO_Pin = Switch2;
                break;
        case J3:
                GPIO_Pin = Switch3;
                break;
        case J4:
                GPIO_Pin = Switch4;
                break;
        case J5:
                GPIO_Pin = Switch5;
                break;
        }
        while (limitSwitch_State)
        {
                limitSwitch_State = limitStop(GPIO_Pin);
        }
        ZDT_SetOrigin(ID);
        return 1;
}
