#pragma once
#include <windef.h>

class CChannelUtils
{
public:
	// intת��Ϊbyte[]
	static void IntToBytes(int value, BYTE* ary);

	// byte[]ת��Ϊint
	static int BytesToInt(BYTE* ary, int offset);

};