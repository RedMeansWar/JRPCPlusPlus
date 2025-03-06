#pragma once
typedef enum LEDState
{
	Off = 0x00,
	Red = 0x08,
	Green = 0x80,
	Orange = 0x88,
	Default = 0x80
} LED_STATE;
