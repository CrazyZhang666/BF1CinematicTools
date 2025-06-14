// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

// 线程休眠时间
#define ThreadSleep(ms) (std::this_thread::sleep_for(std::chrono::milliseconds(ms)))

std::string offlineName = "ChangeYouName";

PVOID fpGetComputerNameA = NULL;

// Hook系统函数 GetComputerNameA
BOOL WINAPI HKGetComputerNameA(LPSTR lpBuffer, LPDWORD lpnSize)
{
	// 一个汉字占3字节，最大5个汉字，长度为16字符
	strcpy(lpBuffer, offlineName.c_str());

	return TRUE;
}

// 开始Hook
void DetourHookOn()
{
	// 恢复之前状态，避免反复拦截
	DetourRestoreAfterWith();
	// 开始劫持（开始事务）
	DetourTransactionBegin();
	// 刷新当前的线程
	DetourUpdateThread(GetCurrentThread());

	/////////////////////

	// 将拦截的函数附加到原函数的地址上,这里可以拦截多个函数

	fpGetComputerNameA = DetourFindFunction("kernel32.dll", "GetComputerNameA");
	DetourAttach(&fpGetComputerNameA, HKGetComputerNameA);

	/////////////////////

	// 提交修改，立即HOOk（结束事务）
	DetourTransactionCommit();
}

// 卸载Hook
void DetourHookOff()
{
	// 开始事务
	DetourTransactionBegin();
	// 更新线程信息
	DetourUpdateThread(GetCurrentThread());

	/////////////////////

	// 将拦截的函数从原函数的地址上解除，这里可以解除多个函数

	DetourDetach(&fpGetComputerNameA, HKGetComputerNameA);

	/////////////////////

	// 结束事务
	DetourTransactionCommit();
}

// 核心方法
void Core()
{
	/////////////////////////////////////////

	// 获取数据文件夹路径    
	PWSTR programDataPath;
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_ProgramData, 0, NULL, &programDataPath);
	if (!SUCCEEDED(hr))
		return;

	// 构建 玩家名称 文件路径
	std::filesystem::path namePath = std::filesystem::path(programDataPath) / "BF1CinematicTools" / "Config" / "PlayerName.txt";

	// 以二进制模式打开文件以确保正确处理所有字符
	std::ifstream fileRead(namePath, std::ios::in | std::ios::binary);
	// 检查文件是否成功打开
	if (fileRead.is_open())
	{
		std::string content;
		// 读取第一行文本 
		std::getline(fileRead, content);
		// 关闭文件  
		fileRead.close();

		// 判断是否为空字符串
		auto isValidName = !content.empty();
		// 判断玩家自定义用户名是否有效
		if (isValidName)
		{
			// 复制为玩家名称
			offlineName = content;
		}
	}

	ThreadSleep(50);

	// 开始Hook
	DetourHookOn();

	ThreadSleep(50);

	/////////////////////////////////////////

	bool isGameReady = false;

	// 检查战地1窗口是否已经完全启动
	// 12 = ClientState_Ingame
	while (!isGameReady)
	{
		Main* pMain = Main::GetInstance();
		if (IsValidPtr(pMain))
		{
			Client* pClient = pMain->m_Client;
			if (IsValidPtr(pClient))
			{
				if (pClient->m_clientState == 12)
				{
					isGameReady = true;
				}
			}
		}

		ThreadSleep(50);
	}

	// 加载 电影工具Dll
	ThreadSleep(1000);
	{
		// 构建 电影工具DLL 文件路径
		std::filesystem::path dllPath = std::filesystem::path(programDataPath) / "BF1CinematicTools" / "CTBF1" / "CT_BF1.dll";
		// 检查文件是否存在
		if (!std::filesystem::exists(dllPath))
			return;

		// 加载 电影工具Dll
		LoadLibraryW(dllPath.wstring().c_str());
	}

	// 加载 马恩Dll
	ThreadSleep(1000);
	{
		// 构建 马恩DLL 文件路径
		std::filesystem::path dllPath = std::filesystem::path(programDataPath) / "BF1CinematicTools" / "Marne" / "Marne.dll";
		// 检查文件是否存在
		if (!std::filesystem::exists(dllPath))
			return;

		// 加载 马恩Dll
		LoadLibraryW(dllPath.wstring().c_str());
	}
}

// Dll主线程
DWORD WINAPI MainThread(LPVOID lpThreadParameter)
{
	// 核心方法
	Core();

	return TRUE;
}

// Dll加载入口
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		if (HANDLE handle = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MainThread, hModule, NULL, NULL))
			CloseHandle(handle);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}