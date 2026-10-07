#ifndef CONSENTIUM_CONFIG_H
#define CONSENTIUM_CONFIG_H

// Public, user-facing constants for ConsentiumThings sketches.
// Internal board/library settings live in src/internal/BoardConfig.h.

// Analog input for battery / external analog devices
#define ADC_IN 34 // ADC1_CH6

// 0-10 V voltage bus channels (use with readVoltageBus)
#define VIN_1 0
#define VIN_2 1
#define VIN_3 2
#define VIN_4 3

// 4-20 mA current bus channels (use with readCurrentBus)
#define CIN_1 0
#define CIN_2 1
#define CIN_3 2
#define CIN_4 3

// Data precision for pushData() / airSync()
#define LOW_PRE 2
#define MID_PRE 4
#define HIGH_PRE 7

#endif // CONSENTIUM_CONFIG_H

