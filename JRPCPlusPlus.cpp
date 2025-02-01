#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>
#include <typeindex>
#include <memory>
#include "pch.h"
#include "JRPCPlusPlus.h"
#include "Xbdm.h"
#include "XbdmDefinitions.h"
#include <iomanip>

#pragma region Variables
#pragma endregion

#pragma region Checks
bool JRPC_Client::JRPC::IsConnected()
{
	if (XBDM::DevConsole::OpenConnection())
		return true;
	return false;
}

void JRPC_Client::JRPC::SetLeds(LEDState TopLeft, LEDState TopRight, LEDState BottomLeft, LEDState BottomRight)
{
	std::ostringstream command;
	std::string response;

	command << "consolefeatures ver=2 type14 =params=\"A\\0\\A\\4\\" << JRPC_Client::JRPC::Int << "\\"
		<< (UINT32)TopLeft << "\\" << JRPC_Client::JRPC::Int << "\\" << (UINT32)TopRight << "\\" << JRPC_Client::JRPC::Int
		<< (UINT32)BottomLeft << "\\" << JRPC_Client::JRPC::Int << "\\" << (UINT32)BottomRight << "\\\"";

	XBDM::DevConsole::SendCommand(command.str(), response);
}

void JRPC_Client::JRPC::XNotify(std::string Message, int Type)
{
	std::ostringstream command;
	std::string response;

	command << "consolefeatures ver=2 type=12 params=\"A\\0\\A\\2\\" << JRPC_Client::JRPC::String << "/"
		<< Message.length() << "\\" << ToHexString(Message) << "\\" << JRPC_Client::JRPC::Int << "\\" <<
		Type << "\\\"";

	XBDM::DevConsole::SendCommand(command.str(), response);
}

void JRPC_Client::JRPC::XNotify(std::string Message, XNotifyType Type = XNotifyType::FlashingXboxConsole)
{
	XNotify(Message, (int)Type);
}
#pragma endregion

#pragma region Private Methods
std::string JRPC_Client::JRPC::ToHexString(const std::string& str)
{
	std::ostringstream oss;
	for (char c : str)
	{
		oss << std::setw(2) << std::setfill('0') << std::hex << (static_cast<unsigned int>(static_cast<unsigned char>(c)));
	}
	return oss.str();
}
#pragma endregion
