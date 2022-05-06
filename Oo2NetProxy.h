#pragma once
#include <windows.h>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
using namespace std;

typedef unsigned __int64(__fastcall* OodleNetwork1UDP_State_Uncompact)(void* to, void* from);
typedef unsigned __int64(__fastcall* OodleNetwork1UDP_State_Size)();
typedef unsigned __int64(__fastcall* OodleNetwork1_Shared_Size)(unsigned char htbits);
typedef unsigned __int64(__fastcall* OodleNetwork1_Shared_SetWindow)(void* to, unsigned char htbits, char* from, __int64 dic_size);
typedef bool(__fastcall* OodleNetwork1UDP_Decode)(void* state, void* shared, char* src, __int64 srcLen, char* dst, __int64 dstLen);

class Oo2NetProxy
{
public:
	static void __declspec(dllexport) Init(char* path);
	static void __declspec(dllexport) Decode(char* in, __int64 inLen, char* out, __int64 outLen);

};

void* OodleNetwork1UDP_State;
void* OodleNetwork1UDP_Shared;
OodleNetwork1UDP_Decode oodleNetwork1UDP_Decode;