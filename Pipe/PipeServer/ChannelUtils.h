#pragma once
#include <windef.h>

class CChannelUtils
{
public:
	// int转换为byte[]
	static void IntToBytes(int value, BYTE* ary);

	// byte[]转换为int
	static int BytesToInt(BYTE* ary, int offset);

};