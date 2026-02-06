#include <stm32f10x.h>
#include "stm32f10x_conf.h"
#include "Init.h"
#include "Mytask.h"
extern uint8_t USART1_Status;

int main(void)
{
	projectInit();
	createMyTask();
}
