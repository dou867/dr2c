/*
author: dou
*/

#pragma once
#include <Windows.h>
#include <vector>

class UTFParser
{
public:
	const std::vector<wchar_t>& ParseString(char* s);
	inline int TestBit(char c);
	int StrAdv(char* s);

private:
	inline void Reset();
	inline void FlushBytes();
	inline void CopyBytes();
	inline void BeginParse();
	char* in = nullptr;
	int bytesParsed = 0;
	char* wBegin = nullptr;
	wchar_t w = 0;
	std::vector<wchar_t> out;
	char cb = 0;
	int ci = 0;
	char current = 0;
	char bitTest = 0;
};