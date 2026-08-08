#include "main_app.h"

typedef enum
{
    FAN_OFF,
    FAN_10,
    FAN_20,
    FAN_30,
    FAN_40,
    FAN_50,
    FAN_60,
    FAN_70,
    FAN_80,
    FAN_90,
    FAN_100
} fan_state_t;

uint8_t fan_decision(uint16_t temperature);
void fan_speed_config(uint8_t duty_cycle);
void fan_control_auto(void);
