#include <stm32f10x.h> 

float firstOrderFilter(float in_data, float a)
{
	static float final = 0;
	final = a*in_data+(1-a)*final;
	return final;
}
