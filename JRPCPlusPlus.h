#ifndef JRPCPLUSPLUS_H
#define JRPCPLUSPLUS_H
#define NO_WARNINGS

#ifdef JRPCPLUSPLUS_EXPORTS
#define DLL_API __declspec(dllexport)  // Export symbols when building the DLL
#else
#define DLL_API __declspec(dllimport)  // Import symbols when using the DLL
#endif

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include "Xbdm.h"
#include "XbdmDefinitions.h"
#include "Enums/TemperatureType.h"
#include "Enums/RebootFlag.h"
#include "Enums/XNotifyType.h"
#include "Enums/TrayState.h"
#include "Enums/XboxColor.h"
#include "Enums/LEDState.h"

namespace JRPC_Client
{
	class DLL_API JRPC : public XBDM::DevConsole
	{
		public:
			bool					IsConnected();
			void					SetLeds(LEDState TopLeft, LEDState TopRight, LEDState BottomLeft, LEDState BottomRight);
			void					SendTextCommand(std::string Command);
			void					FreezeConsole(bool& Freeze);
			void					ShutdownConsole();
			void					Shutdown();
			void					RestartConsole(RebootFlag Flag);
			void					RebootConsole(RebootFlag Flag);
			void					XNotify(std::string Message, int Type);
			void					XNotify(std::string Message, XNotifyType Type = XNotifyType::FlashingXboxConsole);
			void					SetConsoleColor(XboxColor Color);
			std::string				GetCPUKey();
			std::string				CPUKey();
			std::string				IPAddress();
			std::string				IP();
			std::string				KernalVersion();
			std::string				DashboardVersion();
			std::string				GetConsoleType();
			std::string				ConsoleType();
			std::string				GetType();
			std::string				GetName();
			std::string				GetXUID();
			std::string				GetDMVersion();
			UINT32					GetCurrentTitleId();
			UINT32					XamGetCurrentTitleId();
			UINT32					GetTemperature(TemperatureType TemperatureType);
			template <typename T>	T Call(const std::string& Command);
			void*					CallArgs(bool& SystenThread, UINT32 Type, std::type_info& t, const std::string& module);

		private:
			UINT32											Void = 0U;
			UINT32											Int = 1U;
			UINT32											String = 2U;
			UINT32											Float = 3U;
			UINT32											Byte = 4U;
			UINT32											IntArray = 5U;
			UINT32											FloatArray = 6U;
			UINT32											ByteArray = 7U;
			UINT32											Uint64 = 8U;
			UINT32											Uint64Array = 9U;
			std::unordered_map<std::type_info, INT32>		ValueTypeSizeMap;
			std::unordered_map<std::type_info, INT32>		StructPrimitiveSizeMap;
			static std::unordered_set<std::type_info>		ValidReturnTypes;
			BYTE											SMCMessage[16];
			BYTE											SMCReturn[16];

			std::string					ToHexString(const std::string& String);
	};
}

#endif // !JRPCPLUSPLUS_H