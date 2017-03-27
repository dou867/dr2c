/*
author: dou
*/

#include "UTFParser.h"

const std::vector<wchar_t>& UTFParser::ParseString(char* s)
{
	in = s;
	out.clear();

	while ((current = in[ci]) != 0)
	{
		bitTest = TestBit(current);

		if (cb)
		{
			if (bitTest == 1)
			{
				bytesParsed++;
				cb--;

				if (cb == 0)
					FlushBytes();
			}
			else
			{
				CopyBytes();
				BeginParse();
			}
		}
		else
			BeginParse();

		ci++;
	}

	if (bytesParsed)
		CopyBytes();

	out.push_back(0);
	return out;
}

int UTFParser::StrAdv(char* s)
{
	if (*s == 0)
		return 0;

	int bitTest = TestBit(*s);
	if (bitTest > 1)
	{
		for (int i = 1; i < bitTest; i++)
		{
			if (TestBit(*(s + i)) != 1)
				return 1;
		}
		return bitTest;
	}

	return 1;
}

inline int UTFParser::TestBit(char c)
{
	if ((c & 0xf0) == 0xf0)
		return 4;
	if ((c & 0xe0) == 0xe0)
		return 3;
	if ((c & 0xc0) == 0xc0)
		return 2;
	if ((c & 0x80) == 0x80)
		return 1;
	return 0;
}

inline void UTFParser::BeginParse()
{
	Reset();
	if (bitTest > 1)
	{
		cb = --bitTest;
		bytesParsed++;
	}
	else
	{
		out.push_back((wchar_t)(0x00ff) & in[ci]);
		cb = 0;
	}
}

inline void UTFParser::FlushBytes()
{
	MultiByteToWideChar(CP_UTF8, 0, wBegin, bytesParsed, &w, 1);
	if (w == 0xfffd)
		CopyBytes();
	else
		out.push_back(w);
	Reset();
}

inline void UTFParser::CopyBytes()
{
	for (int i = 0; i < bytesParsed; i++)
		out.push_back((wchar_t)(0x00ff) & wBegin[i]);
}

inline void UTFParser::Reset()
{
	wBegin = in + ci;
	bytesParsed = 0;
}