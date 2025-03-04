#pragma once
#pragma warning(disable : 4995)
typedef enum XboxRebootFlag
{
	Title = 0,
	Wait = 1,
	Cold = 2,
	Warm = 4,
	Stop = 8
} XBOX_REBOOT_FLAG;