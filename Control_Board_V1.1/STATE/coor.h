#include "stm32f4xx.h"

#define HALT_TIME		((uint8_t)0x05)
#define SPIN_TIME		((uint8_t)0x06)
#define CHECK_TIME	((uint8_t)0x0A)

/****************************************±Í∂®÷∏¡Ó*****************************************/

extern u8 Hand_Shake[8];

extern u8 Spin_XP[8];
extern u8 Spin_XN[8];
extern u8 Spin_YP[8];
extern u8 Spin_YN[8];
extern u8 Spin_ZP[8];
extern u8 Spin_ZN[8];
extern u8 Halt_XP[8];
extern u8 Halt_XN[8];
extern u8 Halt_YP[8];
extern u8 Halt_YN[8];
extern u8 Halt_ZP[8];
extern u8 Halt_ZN[8];

extern u8 Cal_Halt[8];
extern u8 Cal_Spin[8];

extern u8 Check_Halt[8];
extern u8 Check_Spin_P[8];
extern u8 Check_Spin_N[8];
extern u8 Check_Spin_Cal[8];

extern u8 Result[8];
extern u8 Save[8];

void Send_Coor(u8* Data);
void Coor_Halt(u8 Num);
void Coor_Spin(u8 Num);