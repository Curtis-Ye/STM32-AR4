#ifndef __WIT_H
#define __WIT_H

#include "stdint.h"

typedef struct
{
	float Roll;
	float Pitch;
	float Yaw;
	float Xacc;
	float Yacc;
	float Zacc;
	float Xomega;
	float Yomega;
	float Zomega;
}wit_GetData;

typedef enum
{
	disable,
	enable,
	angle,
	acc,
	wacc
}witMode;

void Wit_Init(void);
void witGetData(wit_GetData *angle,int16_t *witangle,int16_t *witacc,int16_t *witomega);
void setZaxis(void);
void setZaxis(void);
void getAngleData(int16_t *witangle);
void getWomegaData(int16_t *witomega);
void getAccData(int16_t *witacc);
#endif
