#pragma once
#include <functional>

// 每包数据头大小
const int PACKGE_HEADER_SIZE = 4;

// 管道缓冲区大小
const int BUFFER_MAX_LEN = 1024;

// 读取回调
using READ_CALLBACK = std::function < void(char* data, int len) > ;

// 管道关闭回调
using EXIT_CALLBACK = std::function < void() > ;
