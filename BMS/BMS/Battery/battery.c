#include "adc.h"
#include "battery.h"

#define CHARGER_DETECT_PORT 0
#define CHARGER_DETECT_PIN  25
#define BATVOLTAGELOW 		2278
#define BATVOLTAGEHIG 		2580

#define CHARGER_CONNECTED 		1
#define CHARGER_NOT_CONNECTED 	0
#define BATADC 					3

static U16 GetBatteryLevel(void);

void battery_init(void) {
	ADCInit(6);
}

U8 IsBatteryCriticallyLow(void) {
	U16 BatVoltage;

	BatVoltage = ADC0Read(BATADC);
	if (BatVoltage < BATVOLTAGELOW)
		return 1;
	else
		return 0;
}

U8 IsBatteryLow(void) {
	U16 BatVoltage, BatLowVoltage;
	BatVoltage = ADC0Read(BATADC);
	BatLowVoltage = ((BATVOLTAGEHIG - BATVOLTAGELOW) * 20) / 100;
	if (BatVoltage < (BATVOLTAGELOW + BatLowVoltage))
		return 1;
	else {
		return 0;
	}
}

static U16 GetBatteryLevel(void) {
	U16 BatVoltage;
	U16 BatLevel = (BATVOLTAGEHIG - BATVOLTAGELOW);

	BatVoltage = ADC0Read(BATADC);
	if (BatVoltage > BATVOLTAGEHIG) {
		BatLevel = 100;
		return BatLevel;
	}
	if (BatVoltage > BATVOLTAGELOW) {
		BatLevel = (((BatVoltage - BATVOLTAGELOW) * 100) / BatLevel);
	} else {
		BatLevel = 0;
	}
	return BatLevel;
}

U16 BatteryLevel(void) {
	double level[] = { 0, 5, 10, 15, 20, 30, 45, 60, 75, 90, 100 };
	U16 BatteryLev;
	U8 i;

	BatteryLev = GetBatteryLevel();
	for (i = 0; i < ((sizeof(level) / sizeof(double)) - 1); i++) {
		if ((BatteryLev > level[i]) && (BatteryLev <= level[i + 1])) {
			return level[i + 1];
		}
	}

	return 0;
}
/****************************************************************************
 End of File
 ********************************************************************************/
