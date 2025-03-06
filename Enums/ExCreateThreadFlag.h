#pragma once
typedef enum ExCreateThreadFlag
{
	Suspended = 1 << 0,
	System = 1 << 1,
	TlsStatic = 1 << 3,
	Priority1 = 1 << 6,
	Priority2 = 1 << 5,
	ReturnKThead = 1 << 7,
	TitleExecution = 1 << 8,
	Hidden = 1 << 10,
	Core0 = 1 << 24,
	Core1 = 1 << 25,
	Core2 = 1 << 26,
	Core3 = 1 << 27,
	Core4 = 1 << 28,
	Core5 = 1 << 29
} EX_CREATE_THREAD_FLAG;
