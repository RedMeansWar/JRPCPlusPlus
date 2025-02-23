#include "JRPCPlusPlus.h"

#pragma comment(lib, "comsuppw.lib")

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

	#pragma endregion


	#pragma region Console Features

	#pragma endregion


	#pragma region Memory
	UINT32 JRPC::ResolveFunction(IXboxConsole& Console, std::wstring& Module, UINT32 Ordinal)
	{
		std::wstring command = L"consolefeatures ver=" + JRPCVersion + L" type=9 params=\"A\\0\\A\\2\\" +
			std::to_wstring(Module.length()) + L"\\" +
			ToHexWString(Module) + L"\\" +
			std::to_wstring(Ordinal) + L"\\\"";

		std::wstring response = SendCommand(&Console, command);
		std::size_t spacePos = response.find(' ');

		if (spacePos != std::wstring::npos)
		{
			std::wstring hexAddress = response.substr(spacePos + 1);

			try
			{
				return std::stoul(hexAddress, nullptr, 16);
			}
			catch (const std::invalid_argument& ex)
			{
				throw std::runtime_error("Failed to parse function address: " + std::string(ex.what()));
			}
		}

		throw std::runtime_error("Invalid response format: function address not found.");
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
	#pragma endregion

	#pragma region Misc
	bool JRPC::IsValidReturnValue(const std::type_info& t)
	{
		return (t == typeid(bool) || t == typeid(int) || t == typeid(UINT32) || t == typeid(long) ||
			t == typeid(USHORT) || t == typeid(float) || t == typeid(double) || t == typeid(char));
	}
	#pragma endregion

}