#include "Oo2NetProxy.h"

void Oo2NetProxy::Init(char* runtimedata_file)
{
	//std::ifstream input(path, std::ios::binary);
	//std::vector<char> runtimedata_file(std::istreambuf_iterator<char>(input), {});
	int magic = (int)runtimedata_file[0] | (((int)runtimedata_file[1] | (((int)runtimedata_file[2] | ((int)runtimedata_file[3] << 8)) << 8)) << 8);
	if (magic != 287528961) //->this is the code, but the method copy 32 bytes (not 8)
	{
		return;//TODO: error
	}
	unsigned char htbits = runtimedata_file[0x8];//it's encoded in 4 bytes, but we know we'll need only 1
	__int32 dic_size = *(reinterpret_cast<__int32*>(&runtimedata_file[0xc]));//(int)runtimedata_file[0xc] | (((int)runtimedata_file[0xc + 1] | (((int)runtimedata_file[0xc + 2] | ((int)runtimedata_file[0xc + 3] << 8)) << 8)) << 8);
	__int32 statecompacted_size = *(reinterpret_cast<__int32*>(&runtimedata_file[0x18]));//(int)runtimedata_file[0x18] | (((int)runtimedata_file[0x18 + 1] | (((int)runtimedata_file[0x18 + 2] | ((int)runtimedata_file[0x18 + 3] << 8)) << 8)) << 8);

	char* runtimedata_data = &runtimedata_file[32];

	char* state_comp = (char*)malloc(statecompacted_size);
	char* window = (char*)malloc(dic_size);
	if (!state_comp || !window)
		return;//TODO err
	memcpy(window, runtimedata_data, dic_size);
	memcpy(state_comp, &runtimedata_data[dic_size], statecompacted_size);

	HINSTANCE hGetProcIDDLL = LoadLibrary(L"lib\\oo2net_9_win64.dll");
	if (!hGetProcIDDLL) {
		std::cout << "Couldn't load lib\\oo2net_9_win64.dll" << std::endl;
		return; // TODO error
	}

	OodleNetwork1UDP_State_Uncompact oodleNetwork1UDP_State_Uncompact = (OodleNetwork1UDP_State_Uncompact)GetProcAddress(hGetProcIDDLL, "OodleNetwork1UDP_State_Uncompact");
	OodleNetwork1UDP_State_Size oodleNetwork1UDP_State_Size = (OodleNetwork1UDP_State_Size)GetProcAddress(hGetProcIDDLL, "OodleNetwork1UDP_State_Size");
	OodleNetwork1_Shared_Size oodleNetwork1_Shared_Size = (OodleNetwork1_Shared_Size)GetProcAddress(hGetProcIDDLL, "OodleNetwork1_Shared_Size");
	OodleNetwork1_Shared_SetWindow oodleNetwork1_Shared_SetWindow = (OodleNetwork1_Shared_SetWindow)GetProcAddress(hGetProcIDDLL, "OodleNetwork1_Shared_SetWindow");
	oodleNetwork1UDP_Decode = (OodleNetwork1UDP_Decode)GetProcAddress(hGetProcIDDLL, "OodleNetwork1UDP_Decode");
	
	
	if (!oodleNetwork1UDP_State_Uncompact ||
		!oodleNetwork1UDP_State_Size ||
		!oodleNetwork1_Shared_Size ||
		!oodleNetwork1_Shared_SetWindow ||
		!oodleNetwork1UDP_Decode) {
		std::cout << "Couln't find all required exports" << std::endl;
		return; // TODO error
	}
	__int64 state_size = oodleNetwork1UDP_State_Size();
	__int64 shared_size = oodleNetwork1_Shared_Size(htbits);

	OodleNetwork1UDP_State = malloc(state_size);
	OodleNetwork1UDP_Shared = malloc(shared_size);
	if (!OodleNetwork1UDP_State || !OodleNetwork1UDP_Shared)
		return;//TODO err

	if (!oodleNetwork1UDP_State_Uncompact(OodleNetwork1UDP_State, state_comp))
		return;//TODO error

	oodleNetwork1_Shared_SetWindow(OodleNetwork1UDP_Shared, htbits, window, dic_size);

	return;//TODO Success
}

void Oo2NetProxy::Decode(char* in, __int64 inLen, char* out, __int64 outLen)
{
	char* inCopy = (char*)malloc(inLen);
	char* outBuff = (char*)malloc(outLen);
	if (!inCopy || !outBuff || !out) return;//TODO error
	memcpy(inCopy, in, inLen);
	bool a = oodleNetwork1UDP_Decode(OodleNetwork1UDP_State, OodleNetwork1UDP_Shared, inCopy, inLen, outBuff, outLen);
	memcpy(out, outBuff, outLen);
}
