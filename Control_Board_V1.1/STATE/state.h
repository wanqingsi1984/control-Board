#ifndef __STATE_H
#define __STATE_H
#include "stm32f4xx.h"
/****************************************System State Define*****************************************/
typedef struct
{
	u8	System_State;
	u8	Sub_State;
	int	Time;
	u8	Device_1;
	u8	Device_2;
	u8	Position_X;
	u8	Position_Y;
	u8	Axle;
	u8	Error_Type;
}State_TypeDef;

#define CONNECT						((uint8_t)0x00)
#define COORDINATE				((uint8_t)0x01)
#define RESULT						((uint8_t)0x02)


/****************************************Device State Define*****************************************/
#define NOT_CONNECTED			((uint8_t)0x00)
#define CONNECTING				((uint8_t)0x01)
#define CON_SUCC					((uint8_t)0x02)
#define CON_FAIL					((uint8_t)0x03)
#define COORDINATING			((uint8_t)0x04)
#define COOR_SUCC					((uint8_t)0x05)
#define COOR_FAIL					((uint8_t)0x06)
#define SAVE_SUCC					((uint8_t)0x07)


/****************************************Position Value Define****************************************/
#define POSITION0					((uint8_t)0x00)
#define POSITION1					((uint8_t)0x01)
#define POSITION2					((uint8_t)0x02)
#define POSITION3					((uint8_t)0x03)
#define UNKNOW_POSITION		((uint8_t)0x04)
#define MOVING0						((uint8_t)0x05)
#define MOVING1						((uint8_t)0x06)
#define MOVING2						((uint8_t)0x07)
#define MOVING3						((uint8_t)0x08)

/****************************************Axle Value Define****************************************/
#define AXLE_XP					((uint8_t)0x00)
#define AXLE_XN					((uint8_t)0x01)
#define AXLE_YP					((uint8_t)0x02)
#define AXLE_YN					((uint8_t)0x03)

/****************************************Error Type Define****************************************/
#define NO_ERROR							((uint8_t)0x00)
#define ERROR_KEY							((uint8_t)0x01)
#define ERROR_TIMEOUT					((uint8_t)0x02)
#define ERROR_COOR_FAIL				((uint8_t)0x04)
#define ERROR_NO_CONNECTED		((uint8_t)0x08)
#define SINGLE_DEVICE					((uint8_t)0x10)

extern State_TypeDef Current_State;
extern State_TypeDef Display_State;

void DealWith_State(void);
void Display_Function(void);

#endif
