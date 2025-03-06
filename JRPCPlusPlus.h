#ifndef JRPCPLUSPLUS_H
#define JRPCPLUSPLUS_H
#define NO_WARNINGS

#include <windows.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <type_traits>
#include <typeindex>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <iomanip>
#include <typeinfo>
#include <thread>
#include <cstdint> 
#include <algorithm>
#include <comutil.h>
#include <comdef.h>
#include <wtypes.h>
#include <any>
#include <xbdm.h>
#include <XDevkit.h>
#include "Enums/LEDState.h"
#include "Enums/XboxColor.h"
#include "Enums/XboxRebootFlag.h"
#include "Enums/TemperatureType.h"
#include "Enums/XboxThreadType.h"
#include "Enums/XNotifyType.h"
#include "Enums/PowerLEDMode.h"
#include "Enums/ExCreateThreadFlag.h"
#include "Enums/FirmwareReentry.h"
#include "Enums/XNCallerType.h"
#include "XbdmCommands.h"

#ifdef JRPCPLUSPLUS_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

namespace JRPC_Client
{
	typedef BOOL(*DLAUNCH_GET_OPT_VALUE_BY_NAME)(const char* optionName, UINT32* optionValue);
	typedef BOOL(*DLAUNCH_SET_OPT_VALUE_BY_NAME)(const char* optionName, UINT32* optionValue);

	static class JRPC
	{
		public:
			bool						IsConnected();
			bool						IsDevKit();
			bool						InXenia();
			bool						Connect(IXboxConsole* Console, std::wstring XboxNameOrIp = L"default");
			void						Disconnect(IXboxConsole* Console);
			void						Reconnect(IXboxConsole* Console);
			void						SetLEDs(IXboxConsole* Console, LEDState TopLeft, LEDState TopRight, LEDState BottomLeft, LEDState BottomRight);
			std::wstring				SendCommand(IXboxConsole* Console, std::wstring Command);
			std::wstring				GetCPUKey(IXboxConsole* Console);
			std::wstring				GetConsoleType(IXboxConsole* Console);
			std::wstring				GetName(IXboxConsole* Console);
			std::wstring				GetXUID(IXboxConsole* Console);
			std::wstring				GetDMVersion(IXboxConsole* Console);
			UINT32						GetCurrentTitleId(IXboxConsole* Console);
			UINT32						XamGetCurrentTitleId(IXboxConsole* Console);
			UINT32						GetTemperature(IXboxConsole* Console, TemperatureType TemperatureType);
			std::wstring				GetKeneralVersion(IXboxConsole* Console);
			void						FreezeConsole(IXboxConsole* Console, bool Freeze);
			void						ShutDownConsole(IXboxConsole* Console);
			void						RebootConsole(IXboxConsole* Console, XboxRebootFlag Flag);
			void						Reboot(IXboxConsole* Console, XboxRebootFlag Flag);
			bool						SetFanSpeed(IXboxConsole* Console, int Fan, int Speed);
			void						GetUserDefaultProfile(IXboxConsole* Console);
			void						SetUserDefaultProfile(IXboxConsole* Console, long XUID);
			void						GetSignInState(IXboxConsole* Console);
			void						QuickSignIn(IXboxConsole* Console);
			void						SetConsoleColor(IXboxConsole* Console, XboxColor color);
			void						Push();
			void						XNotify(IXboxConsole* Console, std::wstring Message, XNotifyType Type);
			void						XNotify(IXboxConsole* Console, std::wstring Message, int Type);
			void						CallVoid(IXboxConsole* Console, UINT32 Address, std::vector<void*>& Arguments);
			void						CallVoid(IXboxConsole* Console, std::wstring Module, int Ordinal, std::vector<void*>& Arguments);
			void						CallVoid(IXboxConsole* Console, XboxThreadType Type, UINT32 Address, std::vector<void*>& Arguments);
			void						CallVoid(IXboxConsole* Console, XboxThreadType Type, std::wstring Module, int Ordinal, std::vector<void*>& Arguments);
			std::wstring				CallString(IXboxConsole* Console, UINT32 Address, std::vector<void*>& Arguments);
			std::wstring				CallString(IXboxConsole* Console, std::wstring Module, int Ordinal, std::vector<void*>& Arguments);
			std::wstring				CallString(IXboxConsole* Console, XboxThreadType Type, UINT32 Address, std::vector<void*>& Arguments);
			std::wstring				CallString(IXboxConsole* Console, XboxThreadType Type, std::wstring Module, int Ordinal, std::vector<void*>& Arguments);
			UINT32						ResolveFunction(IXboxConsole* Console, std::wstring Module, UINT32 Ordinal);
			INT32						UIntToInt(UINT Value);;
			std::wstring				ToHexWString(const std::wstring& WString);
			std::string					ToHexString(const std::string& String);
			std::vector<unsigned char>	ToByteArray(const std::wstring& WString);
			std::vector<unsigned char>	ToByteArray(const std::string& String);
			void*						ResolveMemoryFunction(IXboxConsole* Console, std::wstring Module, UINT32 Orindal);
			void						Thread(PTHREAD_START_ROUTINE StartAddress, std::vector<void*>& Arguments);
			void						ThreadEx(PTHREAD_START_ROUTINE StartAddress, std::vector<void*>& Arguments, ExCreateThreadFlag creationFlag);

		private:
			DWORD							ConnectionId;
			DWORD							Void = 0U;
			DWORD							Int = 1U;
			DWORD							String = 2U;
			DWORD							Float = 3U;
			DWORD							Byte = 4U;
			DWORD							IntArray = 5U;
			DWORD							FloatArray = 6U;
			DWORD							ByteArray = 7U;
			DWORD							Uint64 = 8U;
			DWORD							Uint64Array = 9U;

			void							Log(std::string message);
			bool							IsValidReturnValue(const std::type_info& T);
			bool							IsValidReturnType(const std::type_info& T);
			unsigned int					ParseHexValue(const std::wstring& input);
			std::wstring					ConvertToHex(UINT32 value);
			std::wstring					ReplaceWString(std::wstring source, const std::wstring& from, const std::wstring& to);
			std::wstring					CallArgs(IXboxConsole* Console, bool SystemThread, UINT32 Type, const std::type_info& T, const std::wstring Module, int Ordinal, UINT32 Address, UINT32 ArraySize, const std::vector<std::any>& Arguments);

			std::unordered_map<std::type_index, int>	StructPrimitiveSizeMap;

			std::unordered_map<std::type_index, int>	ValueTypeSizeMap =
			{
				{ typeid(bool), 4 },
				{ typeid(char), 1 },
				{ typeid(short), 2 },
				{ typeid(int), 4 },
				{ typeid(long long), 8 },
				{ typeid(unsigned short), 2 },
				{ typeid(unsigned int), 4 },
				{ typeid(unsigned long long), 8 },
				{ typeid(float), 4 },
				{ typeid(double), 8 }
			};

			std::unordered_set<std::type_index>			ValidReturnTypes = 
			{
				typeid(void),
				typeid(bool),
				typeid(char),
				typeid(short),
				typeid(int),
				typeid(long long),
				typeid(unsigned short),
				typeid(unsigned int),
				typeid(unsigned long long),
				typeid(float),
				typeid(double),
				typeid(std::string),
				typeid(bool[]),
				typeid(char[]),
				typeid(short[]),
				typeid(int[]),
				typeid(long long[]),
				typeid(unsigned short[]),
				typeid(unsigned int[]),
				typeid(unsigned long long[]),
				typeid(float[]),
				typeid(double[]),
				typeid(std::string[])
			};

			std::vector<unsigned char>					SMCMessage[16];
			std::vector<unsigned char>					SMCReturn[16];

		public:
			template <typename T>
			T Call(IXboxConsole* Console, UINT32 Address, const std::vector<void*>& Arguments);

			template <typename T>
			T Call(IXboxConsole* Console, const std::wstring& Module, int Ordinal, const std::vector<void*>& Arguments);

			template <typename T>
			T Call(IXboxConsole* Console, XboxThreadType Type, UINT32 Address, const std::vector<void*>& Arguments);

			template <typename T>
			T Call(IXboxConsole* Console, XboxThreadType Type, std::wstring Module, int Ordinal, const std::vector<void*>& Arguments);

			template <typename T>
			T* CallArray(IXboxConsole* Console, UINT32 Address, UINT32 ArraySize, const std::vector<void*>& Arguments);

			template <typename T>
			T* CallArray(IXboxConsole* Console, std::wstring Module, int Ordinal, UINT32 ArraySize, const std::vector<void*>& Arguments);
			
			template <typename T>
			T* CallArray(IXboxConsole* Console, XboxThreadType Type, UINT32 Address, UINT32 ArraySize, const std::vector<void*>& Arguments);
			
			template <typename T>
			T* CallArray(IXboxConsole* Console, XboxThreadType Type, std::wstring Module, int Ordinal, UINT32 ArraySize, const std::vector<void*>& Arguments);

			template <typename T>
			T CallArgs(IXboxConsole* Console, bool SystemThread, UINT32 Type, std::type_info& t, std::string module, int ordinal, UINT32 Address, UINT32 ArraySize, const std::vector<std::string>& Arguments);

			template <typename T>
			T Peek(IXboxConsole* Console, UINT64 Address);

			template <typename T>
			void Poke(IXboxConsole* Console, UINT64 Address, const T& Data);

			template <typename T>
			void Write(IXboxConsole* Console, void* Destination, const T& Data);
	};
}

#endif // !JRPCPLUSPLUS_H