#pragma once
typedef enum PowerLEDMode
{
	PowerLEDOff = 0x03,
	PowerLEDOn = 0x01,
	PowerLEDBlink = 0x10,
	PowerLEDDefault = 0x02
} POWER_LED_MODE;
