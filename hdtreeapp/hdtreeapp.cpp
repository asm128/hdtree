#include "hdtree.h"
#include "hdtreeview.h"

#include <windows.h>
#include <CommCtrl.h>

static LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, Message, wParam, lParam);
}

static void defaultWndClass(WNDCLASSEX & wndClass, HINSTANCE hInstance) {
	wndClass				= {sizeof(WNDCLASSEX)};

	wndClass.lpfnWndProc	= WndProc;
	wndClass.hInstance		= hInstance;
	wndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wndClass.lpszClassName	= TEXT("HD_Window");
	wndClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hIconSm		= LoadIcon(NULL, IDI_APPLICATION);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/) {
    // Ensure that the common control DLL is loaded. 
    INITCOMMONCONTROLSEX icc = {sizeof(INITCOMMONCONTROLSEX), ICC_WIN95_CLASSES};
    InitCommonControlsEx(&icc); 

	WNDCLASSEX wndClass;
	defaultWndClass(wndClass, hInstance);
	if(!RegisterClassEx(&wndClass)) {
		MessageBox(0, TEXT("Window Registration Failed!"), TEXT("Error!"), MB_ICONEXCLAMATION|MB_OK);
		return EXIT_FAILURE;
	}

	HWND hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, wndClass.lpszClassName, TEXT("Caption"), WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, 0, 0, hInstance, 0);
	if(hwnd == NULL) {
		MessageBox(0, TEXT("Window Creation Failed!"), TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK);
		return EXIT_FAILURE;
	}

	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg); 
	}
	return int(msg.wParam);
}

#define print_text_test_result(call, text) printf("\n%i <- %s with %s", call, #call, text);

// console build executes a test on the tree object instead of 
int main () {
	hd::MaterialTree	tree;

	constexpr const char*	testStrings[] = 
		{ "1"
		, "c"
		, "aa"
		, "bb"
		, "cc"
		, "ac"
		, "bb"
		, "ca"
		, "acb"
		, "bbc"
		, "cab"
		, "b"
		, "a"
		, "acb"
		, "bbc"
		, "zab"
		, "aabz"
		, "cabasd"
		, "j"
		, "b"
		, "x"
		, "b"
		, "iii"
		, "i"
		, "addins"
		, "appcompat"
		, "BCW5.INI"
		, "bfsvc.exe"
		, "Boot"
		, "Branding"
		, "apppatch"
		, "AppReadiness"
		, "Containers"
		, "CSC"
		, "Cursors"
		, "DDACLSys.log"
		, "debug"
		, "Ascd_ProcessLog.ini"
		, "Ascd_tmp.ini"
		, "audio.log"
		, "bcastdvr"
		, "CbsTemp"
		, "comsetup.log"
		, "diagerr.xml"
		, "diagnostics"
		, "DiagTrack"
		, "diagwrn.xml"
		, "GameBarPresenceWriter"
		, "GearBox.ini"
		, "gethelp_audiotroubleshooter_latestpackage.zip"
		, "glide2x.dll"
		, "Help"
		, "HelpPane.exe"
		, "hh.exe"
		, "IdentityCRL"
		, "IE11_main.log"
		, "IME"
		, "ImmersiveControlPanel"
		, "INF"
		, "InfusedApps"
		, "InputMethod"
		, "DigitalLocker"
		, "DirectX.log"
		, "Downloaded Installations"
		, "DPINST.LOG"
		, "DtcInstall.log"
		, "ehome"
		, "en"
		, "en-US"
		, "es-ES"
		, "explorer.exe"
		, "glide3x.dll"
		, "Globalization"
		, "L2Schemas"
		, "LAN.log"
		, "LiveKernelReports"
		, "Logs"
		, "Migration"
		, "Minidump"
		, "ModemLogs"
		, "msdfmap.ini"
		, "MusiccityDownload.exe"
		, "notepad.exe"
		, "OCR"
		, "ODBCINST.INI"
		, "Offline Web Pages"
		, "Panther"
		, "Performance"
		, "PFRO.log"
		, "MAMCityDownload.ocx"
		, "MASetupCaller.dll"
		, "mib.bin"
		, "Microsoft.NET"
		, "PLA"
		, "PolicyDefinitions"
		, "Prefetch"
		, "PrintDialog"
		, "Professional.xml"
		, "progress.ini"
		, "Provisioning"
		, "py.exe"
		, "pyshellext.amd64.dll"
		, "pyw.exe"
		, "regedit.exe"
		, "Registration"
		, "RemotePackages"
		, "rescache"
		, "Resources"
		, "RtlExUpd.dll"
		, "SchCache"
		, "schemas"
		, "security"
		, "ServiceProfiles"
		, "Speech"
		, "Speech_OneCore"
		, "splwow64.exe"
		, "System"
		, "system.ini"
		, "System32"
		, "twain_32.dll"
		, "UpdateAssistant"
		, "Vss"
		, "WaaS"
		, "Web"
		, "win.ini"
		, "WindowsUpdate.log"
		, "winhlp32.exe"
		, "WinSxS"
		, "WLXPGSS.SCR"
		, "ServiceState"
		, "servicing"
		, "SystemApps"
		, "Temp"
		, "TextInput"
		, "tracing"
		, "TSSysprep.log"
		, "twain_32"
		, "Setup"
		, "setupact.log"
		, "setuperr.log"
		, "ShellComponents"
		, "ShellExperiences"
		, "SystemResources"
		, "SystemTemp"
		, "SysWOW64"
		, "TAPI"
		, "Tasks"
		, "ShellNew"
		, "SKB"
		, "SoftwareDistribution"
		, "WMSysPr9.prx"
		, "write.exe"
		};

	for(const char* textToTest : testStrings)
		print_text_test_result(tree.AddCategory(textToTest), textToTest);

	for(const char* textToTest : testStrings)
		print_text_test_result(tree.Categories[0].AddMaterial(textToTest), textToTest);

	printf("\n\nCategory results:\n");
	for(auto cat : tree.Categories)
		printf("\n%s", cat.Name.c_str());

	printf("\n\nMaterial results:\n");
	for(auto mat : tree.Categories[0].Materials)
		printf("\n%s", mat.c_str());

	return EXIT_SUCCESS;
}