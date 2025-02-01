#ifndef JRPCPLUSPLUS_H
#define JRPCPLUSPLUS_H

#ifdef JRPCPLUSPLUS_EXPORTS
#define DLL_API __declspec(dllexport)  // Export symbols when building the DLL
#else
#define DLL_API __declspec(dllimport)  // Import symbols when using the DLL
#endif

#include "Xbdm.h"

#endif // !JRPCPLUSPLUS_H
