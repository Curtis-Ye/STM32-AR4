#include "Init.h"

void projectInit(void)
{
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);      // 中断分组，组2:抢占级0~3,子优先级0~3 ; 全局只设置一次，尽量放在显眼的地方
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // 使能AFIO辅助功能IO时钟
        AFIO->MAPR |= 0x2 << 24;                             // 设置调试模式：使用SWD;  000_全开, 010_只开SWD, 100:全关; 作用:关闭JTAG只保留SWD, 即释放PB3、PB4、PA15，只需PA13、PA14
        USART1_Init(115200);                                 // 串口初始化
        Led_Init();                                          // LED 初始化; 板载LED的重要作用，配合代码调试，能方便地定位错误
        LED_BLUE_OFF;                                        // 蓝灯，这里用作USB连接指示：亮-连接成功，灭-未连接
        limitSwitch_Init();                                  // 限位开关初始化
        ZDT_CAN_Init();                                      // CAN初始化
        ZDT_Delay_ms(1000);
}
