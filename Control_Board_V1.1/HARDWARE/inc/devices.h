#include "stm32f4xx.h"

#define DEVICE_1					((uint8_t)0x01)
#define DEVICE_2					((uint8_t)0x02)

#define POWER1 PEout(9)	// DS0
#define POWER2 PAout(15)	// DS1	

#define ON 	0	// DS0
#define OFF 1	// DS1	

extern u8 Hand_Shake[8];

void Power_Init(void);
void Connect_Device(u8 Device);
