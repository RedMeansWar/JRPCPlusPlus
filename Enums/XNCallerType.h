#pragma once
typedef enum XNCallerType
{
	CallerTypeInvalid = 0,
	CallerTypeTitle = 1,
	CallerTypeSystemApp = 2,
	CallerTypeXBDM = 3,
	CallerTypePersisting= 3,
	CallerTypeTest = 4,
	CallerTypeNumberOfCallerTypes = 4
} XNCALLER_TYPE;
