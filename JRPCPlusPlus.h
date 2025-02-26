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
#include "XbdmCommands.h"
#include "Enums/LEDState.h"
#include "Enums/RebootFlag.h"
#include "Enums/TemperatureType.h"
#include "Enums/TrayState.h"
#include "Enums/XboxColor.h"
#include "Enums/XNotifyType.h"

#ifdef JRPCPLUSPLUS_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

namespace JRPC_Client
{
	static class JRPC
	{
		public:
			bool						IsConnected();
			bool						IsDevKit();
			bool						InXenia();
			bool						Connect(IXboxConsole** Console, std::wstring XboxNameOrIp = L"default");
			void						Connect(IXboxConsole** Console);
			void						Disconnect(IXboxConsole* Console);
			void						Reconnect(IXboxConsole* Console);
			void						SetLEDs(IXboxConsole* Console, LEDState TopLeft, LEDState TopRight, LEDState BottomLeft, LEDState BottomRight);
			std::wstring				SendCommand(IXboxConsole* Console, std::wstring Command);
			std::wstring				GetCPUKey(IXboxConsole* Console);
			std::wstring				GetConsoleType(IXboxConsole* Console);
			std::wstring				GetName(IXboxConsole* Console);
			std::wstring				GetXUID(IXboxConsole* Console);
			std::wstring				GetDMVersion(IXboxConsole* Console);
			std::wstring				GetCurrentTitleId(IXboxConsole* Console);
			std::wstring				XamGetCurrentTitleId(IXboxConsole* Console);
			std::wstring				GetKeneralVersion(IXboxConsole* Console);
			std::wstring				GetTemperature(IXboxConsole* Console, TemperatureType TemperatureType);
			void						FreezeConsole(IXboxConsole* Console, bool Freeze);
			void						ShutDownConsole(IXboxConsole* Console);
			void						RebootConsole(IXboxConsole* Console, RebootFlag Flag);
			void						Reboot(IXboxConsole* Console, RebootFlag Flag);
			bool						SetFanSpeed(IXboxConsole* Console, int Fan, int Speed);
			void						GetUserDefaultProfile(IXboxConsole* Console);
			void						SetUserDefaultProfile(IXboxConsole* Console, long XUID);
			void						GetSignInState(IXboxConsole* Console);
			void						QuickSignIn(IXboxConsole* Console);
			void						SetConsoleColor(IXboxConsole* Console, XboxColor color);
			void						Push();
			void						CallVoid(IXboxConsole* Console, UINT32 Address, std::vector<void*>& Arguments);
			void						CallVoid(IXboxConsole* Console, std::wstring Module, int Ordinal, std::vector<void*>& Arguments);
			void						CallVoid(IXboxConsole* Console, ThreadType Type, UINT32 Address, std::vector<void*>& Arguments);
			void						CallVoid(IXboxConsole* Console, ThreadType Type, std::wstring Module, int Ordinal, std::vector<void*>& Arguments);
			std::wstring				CallString(IXboxConsole* Console, UINT32 Address, std::vector<void*>& Arguments);
			std::wstring				CallString(IXboxConsole* Console, std::wstring Module, int Ordinal, std::vector<void*>& Arguments);
			std::wstring				CallString(IXboxConsole* Console, ThreadType Type, UINT32 Address, std::vector<void*>& Arguments);
			std::wstring				CallString(IXboxConsole* Console, ThreadType Type, std::wstring Module, int Ordinal, std::vector<void*>& Arguments);
			UINT32						ResolveFunction(IXboxConsole* Console, std::wstring Module, UINT32 Ordinal);
			INT32						UIntToInt(UINT Value);;
			std::wstring				ToHexWString(const std::wstring& WString);
			std::string					ToHexString(const std::string& String);
			std::vector<unsigned char>	ToByteArray(const std::wstring& WString);
			std::vector<unsigned char>	ToByteArray(const std::string& String);


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

			bool							IsValidReturnValue(const std::type_info& T);
			bool							IsValidReturnType(const std::type_info& T);
			std::wstring					ConvertToHex(UINT32 value);
			unsigned int					ParseHexValue(const std::wstring& input);
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
			T Call(IXboxConsole* Console, ThreadType Type, UINT32 Address, const std::vector<void*>& Arguments);

			template <typename T>
			T Call(IXboxConsole* Console, ThreadType Type, std::wstring Module, int Ordinal, const std::vector<void*>& Arguments);

			template <typename T>
			T* CallArray(IXboxConsole* Console, UINT32 Address, UINT32 ArraySize, const std::vector<void*>& Arguments);

			template <typename T>
			T* CallArray(IXboxConsole* Console, std::wstring Module, int Ordinal, UINT32 ArraySize, const std::vector<void*>& Arguments);
			
			template <typename T>
			T* CallArray(IXboxConsole* Console, ThreadType Type, UINT32 Address, UINT32 ArraySize, const std::vector<void*>& Arguments);
			
			template <typename T>
			T* CallArray(IXboxConsole* Console, ThreadType Type, std::wstring Module, int Ordinal, UINT32 ArraySize, const std::vector<void*>& Arguments);

			template <typename T>
			T CallArgs(IXboxConsole* Console, bool SystemThread, UINT32 Type, std::type_info& t, std::string module, int ordinal, UINT32 Address, UINT32 ArraySize, const std::vector<std::string>& Arguments);

			template <typename T>
			T Peek(IXboxConsole* Console, UINT64 Address);

			template <typename T>
			void Poke(IXboxConsole* Console, UINT64 Address, const T& Data);
	};
}

enum class ThreadType
{
	System,
	User
};

#endif // !JRPCPLUSPLUS_H