#ifndef __BATTERY_H 
#define __BATTERY_H

#include "battery.h"
#include "type.h"

U8 IsBatteryCriticallyLow(void);
U8 IsBatteryLow(void);
U16 BatteryLevel(void);

#endif
