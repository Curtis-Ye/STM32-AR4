#include <stm32f10x.h> 
#include "bsp_usart.h"
#include "wit.h"

uint8_t witGet;

void Wit_Init(void)
{
	USART1_Init (115200);
}

void witGetData(wit_GetData *Data,int16_t *witangle,int16_t *witacc,int16_t *witomega)
{
	Data->Roll  = (int16_t)(((uint16_t)witangle[1]<<8)|witangle[0])/32768.0f*180.0f;
	Data->Pitch = (int16_t)(((uint16_t)witangle[3]<<8)|witangle[2])/32768.0f*180.0f;
	Data->Yaw   = (int16_t)(((uint16_t)witangle[5]<<8)|witangle[4])/32768.0f*180.0f;
	Data->Xacc  = (int16_t)(((uint16_t)witacc[1]<<8)|witacc[0])/32768.0f*16.0f;
	Data->Yacc  = (int16_t)(((uint16_t)witacc[3]<<8)|witacc[2])/32768.0f*16.0f;
	Data->Zacc  = (int16_t)(((uint16_t)witacc[5]<<8)|witacc[4])/32768.0f*16.0f;
	Data->Xomega= (int16_t)(((uint16_t)witomega[1]<<8)|witomega[0])/32768.0f*2000.0f;
	Data->Yomega= (int16_t)(((uint16_t)witomega[3]<<8)|witomega[2])/32768.0f*2000.0f;
	Data->Zomega= (int16_t)(((uint16_t)witomega[5]<<8)|witomega[4])/32768.0f*2000.0f;
}

//void setZaxis(void)
//{
//	uint8_t Zaxis[3]={0xFF,0xAA,0x52};
//	if(Get_KeyRight_Status()==0)
//	{
//		USART1_SendDatas(Zaxis,3);
//	}	
//}

void getAngleData(int16_t *witangle)
{
	static uint8_t i = 0;
	static uint8_t angleMode = disable;
	if(angleMode==disable)
	{
		if(witGet==0x55)
		{
			angleMode=enable;
		}
	}
	else if(angleMode==enable)
	{
		if(witGet==0x53)
		{
			angleMode=angle;
		}
	}
	else if(angleMode==angle)
	{
		witangle[i]=witGet;
			i++;
			if(i==6)
			{
				i=0;
				angleMode=disable;
			}
	}
}

void getAccData(int16_t *witacc)
{
	static uint8_t i = 0;
	static uint8_t accMode = disable;
	if(accMode==disable)
	{
		if(witGet==0x55)
		{
			accMode=enable;
		}
	}
	else if(accMode==enable)
	{
		if(witGet==0x51)
		{
			accMode=acc;
		}
	}
	else if(accMode==acc)
	{
		witacc[i]=witGet;
			i++;
			if(i==6)
			{
				i=0;
				accMode=disable;
			}
	}
}

void getWomegaData(int16_t *witomega)
{
	static uint8_t i = 0;
	static uint8_t waccMode = disable;
	if(waccMode==disable)
	{
		if(witGet==0x55)
		{
			waccMode=enable;
		}
	}
	else if(waccMode==enable)
	{
		if(witGet==0x52)
		{
			waccMode=wacc;
		}
	}
	else if(waccMode==wacc)
	{
		witomega[i]=witGet;
			i++;
			if(i==6)
			{
				i=0;
				waccMode=disable;
			}
	}
}
