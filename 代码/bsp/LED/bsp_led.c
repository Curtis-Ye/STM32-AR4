/***********************************************************************************************************************************
 ** 【文件名称】  led.c
 ** 【淘    宝】  魔女开发板      https://demoboard.taobao.com
 ***********************************************************************************************************************************
 ** 【文件功能】  实现LED指示灯常用的初始化函数、功能函数
 **                 
 ** 【移植说明】  
 **             
 ** 【更新记录】  
 **
***********************************************************************************************************************************/  
#include "bsp_led.h"


    

void Led_Init(void)
{    
    GPIO_InitTypeDef GPIO_InitStructure;                    // 定义一个GPIO_InitTypeDef类型的结构体
    
    // 使能相关时钟
    RCC_APB2PeriphClockCmd(LED_BLUE_CLK , ENABLE);          // 使能LED_BLUE所用引脚的端口时钟
    
    // 配置LED_BLUE引脚工作模式
    GPIO_InitStructure.GPIO_Pin   = LED_BLUE_PIN;           // 选择要控制的GPIO引脚       
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;       // 设置引脚模式为通用推挽输出   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       // 设置引脚速率为50MHz            
    GPIO_Init(LED_BLUE_GPIO , &GPIO_InitStructure);         // 调用库函数，初始化GPIO    
   

    LED_BLUE_GPIO ->BSRR = LED_BLUE_PIN ;                   // 点亮LED_BLUE，低电平点亮
                               
    printf("LED 指示灯            配置完成\r");     
}



