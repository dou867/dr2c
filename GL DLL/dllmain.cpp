/*
author: dou
*/

// dllmain.cpp : Defines the entry point for the DLL application.
#include <windows.h>
#include <map>
#include "GL DLL.h"

using namespace std;

void changeVP(void* addr, int size);
void hookAttach(char* addr, void* hook);
void replaceCall(char* addr, void* target);
void replaceJT(char* addr, void* replace);
void replaceOffset(DWORD* addr, DWORD replace);

int(*pf)(char*, char*, ...) = formatStr;
int(*pf2)(char*, int, char*, va_list) = formatStr;

map<DWORD, char*> m = {
	{ 0x4951cb,u8"得到" },
	{ 0x49525c,u8"失去" },
	{ 0x4281ac,u8"更多..." },
	{ 0x46d036,u8"音效" },
	{ 0x46d0ce,u8"音乐" },
	{ 0x47007d,u8"死角" },
	{ 0x494edf,u8"增加" },
	{ 0x494fad,u8"降低" },
};

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		hookAttach((char*)0x45f7ec, warpStr);
		hookAttach((char*)0x43e384, getLootName);
		replaceCall((char*)0x43d4f3, AddFGString);
		replaceJT((char*)0x4a9ca8, pf);
		hookAttach((char*)0x4a5540, pf2);
		//replaceCall((void*)0x48efa7, getStatName);
		//replaceCall((void*)0x48f030, getStatName);
		replaceCall((char*)0x494fa0, getStatName);
		replaceCall((char*)0x494ed2, getStatName);
		replaceCall((char*)0x49507b, getStatName);
		//hookAttach((void*)0x45E894, strTrs);
		for(pair<DWORD, char*> p : m)
		{
			replaceOffset((DWORD*)p.first, (DWORD)p.second);
		}
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

inline void changeVP(void* addr, int size)
{
	HANDLE h = GetCurrentProcess();
	DWORD prevProtect = 0;
	VirtualProtectEx(h, addr, size, PAGE_EXECUTE_READWRITE, &prevProtect);
}

void hookAttach(char* addr, void* hook)
{
	changeVP(addr, 5);
	*addr = 0x68;
	*(DWORD*)(addr + 1) = (DWORD)hook;
	*(addr + 5) = 0xC3;
}

void replaceCall(char* addr, void* replace)
{
	if (*(unsigned char*)addr != 0xE8)
		return;
	changeVP(addr, 5);
	*(DWORD*)(addr + 1) = (DWORD)replace - (DWORD)addr - 5;
}

void replaceJT(char* addr, void* replace)
{
	if (*(unsigned short*)addr != 0x25ff)
		return;
	DWORD* dest = *(DWORD**)(addr + 2);
	*dest = (DWORD)replace;
}

void replaceOffset(DWORD* addr, DWORD replace)
{
	changeVP(addr, 4);
	*addr = replace;
}