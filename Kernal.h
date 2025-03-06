#include "Windows.h"
#include "Structures/String.h"
#include "Enums/FirmwareReentry.h"

#define __isync() __emit(0x4C00012C)
#define doSync(addr) \ __dcbst(0, addr); \ __sync(); \ __isync()

extern "C"
{
    void RtlInitAnsiString(String* pDestinationString, const char* sourceString);
    void HalReturnToFirmware(FirmwareReentry powerDownMode);
    void HalSendSMCMessage(void* pInput, void* pOutput);
    HRESULT ObCreateSymbolicLink(String* pLinkName, String* pDevicePath);

    UINT32 ExCreateThreadType(HANDLE* Handle, UINT32 StackSize, UINT32* ThreadId, void* ApiThreadStartup, R)
}