/***********************************************************************************************************************************
 ** ���ļ����ơ�  led.c
 ** ����    ����  ħŮ������      https://demoboard.taobao.com
 ***********************************************************************************************************************************
 ** ���ļ����ܡ�  ʵ��LEDָʾ�Ƴ��õĳ�ʼ�����������ܺ���
 **
 ** ����ֲ˵����
 **
 ** �����¼�¼��
 **
 ***********************************************************************************************************************************/
#include "bsp_led.h"

void Led_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; // ����һ��GPIO_InitTypeDef���͵Ľṹ��

    // ʹ�����ʱ��
    RCC_APB2PeriphClockCmd(LED_BLUE_CLK, ENABLE); // ʹ��LED_BLUE�������ŵĶ˿�ʱ��

    // ����LED_BLUE���Ź���ģʽ
    GPIO_InitStructure.GPIO_Pin = LED_BLUE_PIN;       // ѡ��Ҫ���Ƶ�GPIO����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // ��������ģʽΪͨ���������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // ������������Ϊ50MHz
    GPIO_Init(LED_BLUE_GPIO, &GPIO_InitStructure);    // ���ÿ⺯������ʼ��GPIO

    LED_BLUE_GPIO->BSRR = LED_BLUE_PIN; // ����LED_BLUE���͵�ƽ����

    printf("LED ָʾ��            �������\r");
}

void LED_ON(void)
{
	  GPIO_ResetBits(LED_BLUE_GPIO,LED_BLUE_PIN);
}

void LED_OFF(void)
{
	  GPIO_SetBits(LED_BLUE_GPIO,LED_BLUE_PIN);
}
