#pragma once

#define OFFSET_MAIN	0x1435036D0

// 检查指针地址是否有效
#define IsValidPtr(x) (x != NULL && (DWORD_PTR)x >= 0x10000 && (DWORD_PTR)x < 0x00007FFFFFFEFFFF)

class Client {
public:
	char pad_0x0000[0x390]; //0x0000
	__int32 m_clientState; //0x0390	
	char pad_0x0394[0xC8]; //0x0394
	__int32 m_gameType; //0x045C
}; //Size: 0x0058

class Main
{
public:
	char _0x0000[0x0020]; //0x0000
	unsigned char m_IsDedicatedServer; //0x0020 
	unsigned char m_IsDedicatedServerData; //0x0021 
	unsigned char m_IsPaused; //0x0022 
	char _0x0023[0x002D]; //0x0023
	Client* m_Client; //0x0050 

	static Main* GetInstance() {
		return *(Main**)(OFFSET_MAIN);
	}
};//Size=0x0058