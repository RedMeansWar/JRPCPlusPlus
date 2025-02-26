#include "JRPCPlusPlus.h"

#pragma comment(lib, "comsuppw.lib")

#define XAM L"xam.xex"
#define KERNEL L"xboxkrnl.exe"

namespace JRPC_Client
{
	#pragma region Connections
	bool JRPC_Client::JRPC::Connect(IXboxConsole** Console, std::wstring XboxNameOrIp = L"default")
	{
		IXboxConsole* _console = nullptr;
		IXboxManager* _manager = nullptr;
		BSTR defaultConsole = nullptr;

		CoInitialize(nullptr);

		HRESULT hr = CoCreateInstance(__uuidof(XboxManager), nullptr, CLSCTX_ALL, __uuidof(IXboxManager), (void**)&_manager);
		if (FAILED(hr))
		{
			std::wcerr << L"Failed to create XboxManager instance." << std::endl;
			return false;
		}

		if (XboxNameOrIp == L"default")
		{
			hr = _manager->get_DefaultConsole(&defaultConsole);
			if (FAILED(hr) || defaultConsole == nullptr)
			{
				std::wcerr << L"Failed to retrieve default console." << std::endl;
				_manager->Release();
				return false;
			}

			XboxNameOrIp = defaultConsole;
			SysFreeString(defaultConsole);
		}

		hr = _manager->OpenConsole(_bstr_t(XboxNameOrIp.c_str()), &_console);
		_manager->Release();

		if (FAILED(hr) || !_console)
		{
			std::wcerr << L"Failed to open Xbox console connection." << std::endl;
			return false;
		}

		int retry = 0;
		bool connected = false;
		ConnectionId = 0;

		while (!connected)
		{
			try
			{
				hr = _console->OpenConnection(nullptr, &ConnectionId);
				if (SUCCEEDED(hr))
				{
					connected = true;
				}
				else
				{
					if (hr == 0x82DA0100 || hr == 0x82DA0001 || hr == 0x82DA0108)
					{
						if (retry >= 3)
						{
							*Console = _console;
							return false;
						}

						retry++;
						Sleep(100);
					}
					else
					{
						*Console = _console;
						return false;
					}
				}
			}
			catch(...)
			{
				std::wcerr << L"Exception occurred while opening connection." << std::endl;
				*Console = _console;
				return false;
			}
		} 

		*Console = _console;
		return true;
	}

	void JRPC_Client::JRPC::Connect(IXboxConsole** Console)
	{
		Connect(Console, L"default");
	}

	bool JRPC_Client::JRPC::IsConnected()
	{
		return XBDM_CONNECTED;
	}

	#pragma endregion

	#pragma region Commands
	std::wstring JRPC::SendCommand(IXboxConsole* Console, std::wstring Command)
	{
		if (ConnectionId == NULL || Console == nullptr)
		{
			std::wcerr << L"IXboxConsole argument did not connect using JRPC's connect function." << std::endl;
			return;
		}

		BSTR* response = nullptr;

		try
		{
			_bstr_t bstrCommandVal(Command.c_str());

			Console->SendTextCommand(ConnectionId, bstrCommandVal, response);

			std::wstring responseStr(*response);
			size_t error = responseStr.find(L"error=");

			if (error != std::wstring::npos)
			{
				throw std::runtime_error(std::string(responseStr.begin() + 11, responseStr.end()));
			}

			if (responseStr.find(L"DEBUG") != std::wstring::npos)
			{
				throw std::runtime_error("JRPC is not installed on the current console.");
			}

			SysFreeString(bstrCommandVal);
		}
		catch (const _com_error& ex)
		{
			HRESULT hr = ex.Error();
			_bstr_t bstrDescription = ex.Description();

			std::cerr << "COM Exception Code: " << std::hex << hr << std::endl;
			std::cerr << "COM Exception Description: " << (const char*)bstrDescription << std::endl;

			if (hr == static_cast<HRESULT>(0x82DA0007))
			{
				throw std::runtime_error("JRPC is not installed on the current console.");
			}
			else
			{
				throw std::runtime_error
				(
					"COM Exception: Error Code - " + std::to_string(hr) + ", Description - " + std::string((const char*)bstrDescription)
				);
			}
		}
	}
	#pragma endregion

	#pragma region Console Information
	std::wstring JRPC::GetCPUKey(IXboxConsole* Console)
	{
		std::wstring command = L"consolefeatures ver=2 type=10 params=\"A\\0\\A\\0\\\"";
		std::wstring text = SendCommand(Console, command);

		size_t spacePos = text.find(L" ");
		if (spacePos != std::wstring::npos)
		{
			return text.substr(spacePos + 1);
		}

		return L"";
	}

	std::wstring JRPC::GetConsoleType(IXboxConsole* Console)
	{
		std::wstring command = L"consolefeatures ver=2 type=17 params=\"A\\0\\A\\0\\\"";
		std::wstring text = SendCommand(Console, command);

		size_t spacePos = text.find(L" ");
		if (spacePos != std::wstring::npos)
		{
			return text.substr(spacePos + 1);
		}

		return L"";
	}
	
	std::wstring JRPC::GetName(IXboxConsole* Console)
	{
		std::wstring consoleName = L"";
		BSTR xboxName = nullptr;

		if (!IsConnected())
		{
			return L"";
		}

		try { consoleName = Console->get_Name(&xboxName); }
		catch (...) { }
	}

	std::wstring JRPC::GetKeneralVersion(IXboxConsole* Console)
	{
		std::wstring command = L"consolefeatures ver=2 type=13 params=\"A\\0\\A\\0\\\"";
		std::wstring text = SendCommand(Console, command);

		size_t spacePos = text.find(L" ");
		if (spacePos != std::wstring::npos)
		{
			return text.substr(spacePos + 1);
		}

		return L"";
	}
	#pragma endregion


	#pragma region Console Features
	void JRPC::FreezeConsole(IXboxConsole* Console, bool Freeze)
	{
		if (Freeze)
		{
			SendCommand(Console, L"stop");
		}

		SendCommand(Console, L"go");
	}

	void JRPC::ShutDownConsole(IXboxConsole* Console)
	{
		try
		{
			std::wstring command = L"consolefeatures ver=2 type=11 params=\"A\\0\\A\\0\\\"";
			SendCommand(Console, command);
		}
		catch (...)
		{
			throw new std::exception("Failed to shutdown console.");
		}
	}

	void JRPC::RebootConsole(IXboxConsole* Console, RebootFlag Flag)
	{
		BSTR consoleName = nullptr;

		try
		{
			Console->Reboot(consoleName, NULL, NULL, (int)Flag);
		}
		catch (...)
		{
			throw new std::exception("Failed to reboot console.");
		}
	}

	void JRPC::Reboot(IXboxConsole* Console, RebootFlag Flag)
	{
		RebootConsole(Console, Flag);
	}

	bool JRPC::SetFanSpeed(IXboxConsole* Console, int Fan, int Speed)
	{
		UINT32 address = ResolveFunction(Console, KERNEL, 41);
		std::vector<UINT8> numArray(16, 0);
		std::vector<UINT8> numArray1(16, 16);

		switch (Fan)
		{
			case 1: numArray[0] = 148; break;
			case 2: numArray[0] = 137; break;
			default: return false;
		}

		if (Speed > 100)
		{
			Speed = 100;
		}

		if (Speed < 0)
		{
			Speed = 55;
		}

		numArray[1] = Speed >= 45 ? static_cast<UINT8>(Speed | 128) : static_cast<UINT8>(127);
		

	}
	#pragma endregion


	#pragma region Memory
	UINT32 JRPC::ResolveFunction(IXboxConsole* Console, std::wstring Module, UINT32 Ordinal)
	{
		std::size_t moduleString = Module.length();
		std::wostringstream oss;

		oss << L"consolefeatures ver=2 type=9 params=\"A\\0\\A\\2\\"
			<< L"\\" << ToHexWString(Module)
			<< L"\\" << Ordinal
			<< L"\\\"";

		std::wstring text = SendCommand(Console, oss.str());

		size_t spacePos = text.find(L" ");
		if (spacePos == std::wstring::npos)
		{
			return 0;
		}

		return std::stoul(text.substr(spacePos + 1), nullptr, 16);
	}

	std::wstring JRPC::CallArgs(IXboxConsole* Console, bool SystemThread, UINT32 Type, const std::type_info& T, const std::wstring Module, int Ordinal, UINT32 Address, UINT32 ArraySize, const std::vector<std::any>& Arguments)
	{
		if (!IsValidReturnType(T))
		{
			throw std::invalid_argument("JRPC only supports: bool, byte, short, int, long, ushort, uint, ulong, float, double");
		}

		UINT32 connectionTimeout = Console->put_ConversationTimeout(4000000U);
		Console->put_ConnectTimeout(connectionTimeout);

		std::wostringstream command;
		command << L"consolefeatures ver=2 type=" << Type
				<< (SystemThread ? L" system" : L"")
				<< (Module.empty() ? L"" : L" module=\"" + Module + L"\" ord=" + std::to_wstring(Ordinal))
				<< L" as=" << ArraySize << L" params=\"A\\"
				<< std::hex << Address << L"\\A\\" << Arguments.size() << L"\\";

		if (Arguments.size() > 37)
		{
			throw std::invalid_argument("You can't use more than 37 parameters in a call.");
		}

		for (const auto arg : Arguments)
		{
			bool flag = false;
			if (arg.type() == typeid(JRPC::Int))
			{
				command << "Int\\" << std::any_cast<UINT32>(arg) << "\\";
			}
			else if (arg.type() == typeid(INT) || arg.type() == typeid(BOOLEAN) || arg.type() == typeid(UINT8))
			{
				if (arg.type() == typeid(BOOLEAN))
				{
					command << "Int/" << std::any_cast<BOOLEAN>(arg) << "\\";
				}
				else
				{
					command << "Int\\" << std::any_cast<INT>(arg) << "\\";
				}
			}
			else if (arg.type() == typeid(std::vector<INT>))
			{
				const auto& vec = std::any_cast<std::vector<int>>(arg);
				command << "ByteArray/" << vec.size() * sizeof(int) << "\\";

				for (const auto& elem : vec)
				{
					command << std::hex << elem << "\\";
				}
			}
			else if (arg.type() == typeid(std::string))
			{
				const auto& str = std::any_cast<std::string>(arg);

			}
			else if (arg.type() == typeid(std::string))
			{
				const auto& str = std::any_cast<std::string>(arg);
				command << "ByteArray/" << str.size() << "\\" << str.c_str();
			}
			else if (arg.type() == typeid(double))
			{
				command << "Float\\" << std::any_cast<double>(arg) << "\\";
			}
			else if (arg.type() == typeid(float))
			{
				command << "Float\\" << std::any_cast<float>(arg) << "\\";
			}
			else if (arg.type() == typeid(std::vector<float>))
			{
				const auto& vec = std::any_cast<std::vector<float>>(arg);
				command << "ByteArray/" << vec.size() * sizeof(float) << "\\";
				for (const auto& elem : vec)
				{
					command << std::hex << elem << "\\";
				}
			}
			else if (arg.type()  == typeid(std::vector<UINT8>))
			{
				const auto& vec = std::any_cast<std::vector<uint8_t>>(arg);
				command << "ByteArray/" << vec.size() << "\\";
				for (const auto& elem : vec)
				{
					command << std::hex << static_cast<int>(elem) << "\\";
				}
			}
			else
			{
				command << "Uint64\\" << std::any_cast<uint64_t>(arg) << "\\";
			}
		}

		command << "\"";

		std::wstring text = command.str().c_str();
		std::wstring text1 = SendCommand(Console, text);

		std::wstring text2 = L"buf_addr=";
		while (text1.contains(text2))
		{
			Sleep(250);
			std::wstringstream cmd;
			UINT32 parsedSubstring = ParseHexValue(text1.substr(text1.find(text2) + text2.length()).c_str());

			cmd << L"consolefeatures " << text2 << "0x" << parsedSubstring;
			text1 = SendCommand(Console, cmd.str());
		}

		Console->put_ConversationTimeout(2000U);
		Console->put_ConnectTimeout(5000U);

		switch (Type)
		{
			case 1U:
				UINT32 num = ParseHexValue(text1.substr(text1.find(L" ") + 1).c_str());
				if (T == typeid(UINT32))
				{
					//return num;
				}

				break;
		}

	}

	void JRPC::CallVoid(IXboxConsole* Console, UINT32 Address, std::vector<void*>& Arguments)
	{

	}
	#pragma endregion

	#pragma region Convert
	std::wstring JRPC::ConvertToHex(UINT32 value)
	{
		std::wstringstream ss;
		ss << std::hex << std::uppercase << value;
		return ss.str();
	}

	std::wstring JRPC::ToHexWString(const std::wstring& WString)
	{
		std::wstringstream hexStream;
		for (size_t i = 00; i < WString.length(); ++i)
		{
			hexStream << std::setw(2) << std::setfill('0') << std::hex << (static_cast<unsigned char>(WString[i]));
		}

		return hexStream.str();
	}

	std::string JRPC::ToHexString(const std::string& String)
	{
		std::stringstream hexStream;
		for (size_t i = 00; i < String.length(); ++i)
		{
			hexStream << std::setw(2) << std::setfill('0') << std::hex << (static_cast<unsigned char>(String[i]));
		}

		return hexStream.str();
	}

	std::vector<unsigned char> JRPC::ToByteArray(const std::wstring& WString)
	{
		std::vector<unsigned char> array(WString.length() + 1);
		for (size_t i = 0; i < WString.length(); ++i)
		{
			array[i] = static_cast<unsigned char>(WString[i]);
		}

		return array;
	}

	std::vector<unsigned char> JRPC::ToByteArray(const std::string& String)
	{
		std::vector<unsigned char> array(String.length() + 1);
		for (size_t i = 0; i < String.length(); ++i)
		{
			array[i] = static_cast<unsigned char>(String[i]);
		}

		return array;
	}

	INT32 JRPC::UIntToInt(UINT32 Value)
	{
		UINT8 byteArray[4];
		std::memcpy(byteArray, &Value, sizeof(Value));

		INT32 result;
		std::memcpy(&result, byteArray, sizeof(result));

		return result;
	}
	#pragma endregion

	#pragma region Misc
	bool JRPC::IsValidReturnValue(const std::type_info& T)
	{
		return (T == typeid(bool) || T == typeid(int) || T == typeid(UINT32) || T == typeid(long) ||
			T == typeid(USHORT) || T == typeid(float) || T == typeid(double) || T == typeid(char));
	}

	bool JRPC::IsValidReturnType(const std::type_info& T)
	{
		return ValidReturnTypes.contains(T);
	}


	unsigned int JRPC::ParseHexValue(const std::wstring& input)
	{
		std::wstring hexStr = input;
		if (hexStr.find(L"0x") == 0 || hexStr.find(L"0X") == 0)
		{
			hexStr = hexStr.substr(2);
		}

		unsigned int val;

		std::wstringstream wss;
		wss << std::hex << hexStr;
		wss >> val;

		return val;
	}
	#pragma endregion

	#pragma region DashLaunch
	#pragma endregion
}