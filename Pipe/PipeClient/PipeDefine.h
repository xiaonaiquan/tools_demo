#pragma once
#include <functional>

// ÿ������ͷ��С
const int PACKGE_HEADER_SIZE = 4;

// �ܵ���������С
const int BUFFER_MAX_LEN = 1024;

// ��ȡ�ص�
using READ_CALLBACK = std::function < void(char* data, int len) > ;

// �ܵ��رջص�
using EXIT_CALLBACK = std::function < void() > ;
