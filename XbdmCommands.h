#pragma once

#ifndef XBDM_COMMANDS_H

#include <string>

namespace JRPC_Client
{
    class XbdmCommands
    {
        public:
            const wchar_t* ADMIN_PW = L"adminpw";
            const wchar_t* ALT_ADDR = L"altaddr";
            const wchar_t* AUTH_USE = L"authuser";
            const wchar_t* BOX_ID = L"boxid";
            const wchar_t* BREAK = L"break";
            const wchar_t* BYE = L"bye";
            const wchar_t* CAP_CONTROL = L"capcontrol";
            const wchar_t* CAP_CTRL = L"capctrl";
            const wchar_t* CONTINUE = L"continue";
            const wchar_t* CRASH_DUMP = L"crashdump";
            const wchar_t* D3D_OP_CODE = L"d3dopcode";
            const wchar_t* DBG_NAME = L"dbgname";
            const wchar_t* DBG_OPTIONS = L"dbgoptions";
            const wchar_t* DEBUGGER = L"debugger";
            const wchar_t* DEBUG_MODE = L"debugmode";
            const wchar_t* DEDICATE = L"dedicate";
            const wchar_t* DEF_TITLE = L"deftitle";
            const wchar_t* DIR_LIST = L"dirlist";
            const wchar_t* DM_VERSION = L"dmversion";
            const wchar_t* DRIVE_FREE_SPACE = L"drivefreespace";
            const wchar_t* DVD_BULK = L"dvdblk";
            const wchar_t* DVD_PERF = L"dvdperf";
            const wchar_t* FILE_E_OF = L"fileeof";
            const wchar_t* FLASH = L"flash";
            const wchar_t* FMT_FAT = L"fmtfat";
            const wchar_t* FUNC_CALL = L"funccall";
            const wchar_t* GET_CONTEXT = L"getcontext";
            const wchar_t* GET_FILE = L"getfile";
            const wchar_t* GET_FILE_ATTRIBUTES = L"getfileattributes";
            const wchar_t* GET_GAMMA = L"getgamma";
            const wchar_t* GET_MEM = L"getmem";
            const wchar_t* GET_MEM2 = L"getmem2";
            const wchar_t* GET_PALETTE = L"getpalette";
            const wchar_t* GET_PID = L"getpid";
            const wchar_t* GET_SUM = L"getsum";
            const wchar_t* GET_SURF = L"getsurf";
            const wchar_t* GET_USER_PRIV = L"getuserpriv";
            const wchar_t* GET_UTIL_DRIVE_INFO = L"getutildrvinfo";
            const wchar_t* GO = L"go";
            const wchar_t* GPU_COUNT = L"gpucount";
            const wchar_t* HALT = L"halt";
            const wchar_t* IRT_SWEEP = L"irtsweep";
            const wchar_t* IS_BREAK = L"isbreak";
            const wchar_t* IS_DEBUGGER = L"isdebugger";
            const wchar_t* IS_STOPPED = L"isstopped";
            const wchar_t* KD = L"kd";
            const wchar_t* KEY_X_CHG = L"keyxchg";
            const wchar_t* LOCK_MODE = L"lockmode";
            const wchar_t* LOP = L"lop";
            const wchar_t* MAGIC_BOOT = L"magicboot";
            const wchar_t* MEM_TRACK = L"memtrack";
            const wchar_t* MK_DIR = L"mkdir";
            const wchar_t* MM_GLOBAL = L"mmglobal";
            const wchar_t* MOD_LONG = L"modlong";
            const wchar_t* MOD_SELECTIONS = L"modsections";
            const wchar_t* MODULES = L"modules";
            const wchar_t* NO_STOP_ON = L"nostopon";
            const wchar_t* NOTIFY = L"notify";
            const wchar_t* NOTIFY_AT = L"modifyat";
            const wchar_t* PDB_INFO = L"pdbinfo";
            const wchar_t* PS_SNAP = L"pssnap";
            const wchar_t* QUERY_PC = L"querypc";
            const wchar_t* REBOOT = L"reboot";
            const wchar_t* RENAME = L"rename";
            const wchar_t* RESUME = L"resume";
            const wchar_t* SCREENSHOT = L"screenshot";
            const wchar_t* SEND_FILE = L"sendfile";
            const wchar_t* SERV_NAME = L"servname";
            const wchar_t* SET_CONFIG = L"setconfig";
            const wchar_t* SET_CONTEXT = L"setcontext";
            const wchar_t* SET_FILE_ATTRIBUTES = L"setfileattributes";
            const wchar_t* SET_MEM = L"setmem";
            const wchar_t* SET_SYS_TIME = L"setsystime";
            const wchar_t* SIGN_CONTENT = L"signcontent";
            const wchar_t* STOP = L"stop";
            const wchar_t* STOP_ON = L"stopon";
            const wchar_t* SUSPENDW = L"suspend";
            const wchar_t* SYS_FILE_UPD = L"sysfileupd";
            const wchar_t* SYS_TIME = L"systime";
            const wchar_t* THREAD_INFO = L"threadinfo";
            const wchar_t* THREADS = L"threads";
            const wchar_t* TITLE = L"title";
            const wchar_t* USER = L"user";
            const wchar_t* USER_LIST = L"userlist";
            const wchar_t* VS_SNAP = L"vssnap";
            const wchar_t* WALK_MEM = L"walkmem";
            const wchar_t* WRITE_FILE = L"writefile";
            const wchar_t* XBE_INFO = L"xbeinfo";
            const wchar_t* XTL_INFO = L"xtlinfo";
    };
};

#endif // !XBDM_COMMANDS_H
