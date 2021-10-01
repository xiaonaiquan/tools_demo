#include "ChannelUtils.h"


void CChannelUtils::IntToBytes(int value, BYTE* ary)
{
	ary[3] = (BYTE)((value & 0xFF000000) >> 24);
	ary[2] = (BYTE)((value & 0x00FF0000) >> 16);
	ary[1] = (BYTE)((value & 0x0000FF00) >> 8);
	ary[0] = (BYTE)((value & 0x000000FF));
}

int CChannelUtils::BytesToInt(BYTE* ary, int offset) {
	int value;
	value = (int)((ary[offset] & 0xFF)
		| ((ary[offset + 1] << 8) & 0xFF00)
		| ((ary[offset + 2] << 16) & 0xFF0000)
		| ((ary[offset + 3] << 24) & 0xFF000000));
	return value;
}