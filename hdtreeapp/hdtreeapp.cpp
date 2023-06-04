#include "hdtree.h"
#include "hdtreeview.h"

#include <windows.h>
#include <CommCtrl.h>


template<typename T>
struct n2 { T x, y; };

typedef n2<uint32_t> n2u32;
typedef n2<long>	 n2lng;

static constexpr n2u32 BUTTON_SIZE = {96, 24};
static constexpr n2lng MARGIN_SIZE = {2, 2};

struct App {
	HINSTANCE	hInstance		= {};
	HWND		hRoot			= {};
	HWND		hAddCategory	= {};
	HWND		hAddMaterial	= {};
	HWND		hClear			= {};
	HWND		hTree			= {};
};

static void calcButtonsPosition(const RECT & rcClient, n2u32 & xCat, n2u32 & xMat, n2u32 & xClr) {
	xCat.x = MARGIN_SIZE.x;
	xMat.x = (rcClient.right >> 1) - (BUTTON_SIZE.x >> 1);
	xClr.x = rcClient.right - MARGIN_SIZE.x * 2 - BUTTON_SIZE.x;

	xCat.y = xMat.y = xClr.y = rcClient.bottom - MARGIN_SIZE.y - BUTTON_SIZE.y;
							   
}							   

static void calcTreeRect(const RECT & rcClient, RECT & rcTree) { 
	rcTree = {MARGIN_SIZE.x, MARGIN_SIZE.y, rcClient.right - MARGIN_SIZE.x * 2, long(rcClient.bottom - BUTTON_SIZE.y - MARGIN_SIZE.y * 2)}; 
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch(uMsg) {
	case WM_GETMINMAXINFO:	// This message prevents the window from becoming too small.
		((::MINMAXINFO*)lParam)->ptMinTrackSize	= {320, 200};
		return 0;
	case WM_NCCREATE: { 
		const CREATESTRUCTA						& createStruct			= *(const CREATESTRUCTA*)lParam;
		if(0 == createStruct.hwndParent) {
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)createStruct.lpCreateParams);
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	}


	App	* app = (App*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if(0 == app)
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	if(hWnd == app->hRoot) {
		switch(uMsg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_SIZE: {
			RECT rcClient = {};  // dimensions of client area 
			GetClientRect(app->hRoot, &rcClient);


			n2u32	posCat = {}, posMat = {}, posClr = {};
			RECT	rcTree = {};

			::calcButtonsPosition(rcClient, posCat, posMat, posClr);
			::calcTreeRect(rcClient, rcTree);

			SetWindowPos(app->hAddCategory	, 0, posCat.x, posCat.y, BUTTON_SIZE.x, BUTTON_SIZE.y, 0);
			SetWindowPos(app->hAddMaterial	, 0, posMat.x, posMat.y, BUTTON_SIZE.x, BUTTON_SIZE.y, 0);
			SetWindowPos(app->hClear		, 0, posClr.x, posClr.y, BUTTON_SIZE.x, BUTTON_SIZE.y, 0);
			SetWindowPos(app->hTree			, 0, rcTree.left, rcTree.top, rcTree.right - rcTree.left, rcTree.bottom - rcTree.top, 0);
			break;
		}
		}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
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
	RECT	rcClient	= {0, 0, 800, 600};  // dimensions of client area 
	RECT	rcWindow	= rcClient;  // dimensions of client area 
	AdjustWindowRect(&rcWindow, WS_OVERLAPPEDWINDOW, 0);

	App		app			= {hInstance};

	constexpr uint32_t STYLE_ROOT	= WS_VISIBLE | WS_OVERLAPPEDWINDOW;
	app.hRoot = CreateWindow(wndClass.lpszClassName, TEXT("Caption"), STYLE_ROOT, CW_USEDEFAULT, CW_USEDEFAULT, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, 0, 0, hInstance, &app);

	if(app.hRoot == NULL) {
		MessageBox(0, TEXT("Window Creation Failed!"), TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK);
		return EXIT_FAILURE;
	}
	UpdateWindow(app.hRoot);

	RECT rcTree = {};
	n2u32 posCat = {}, posMat = {}, posClr = {};
	::calcButtonsPosition(rcClient, posCat, posMat, posClr);
	::calcTreeRect(rcClient, rcTree);

	constexpr uint32_t STYLE_BUTTON	= WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON;
	constexpr uint32_t STYLE_TREE	= WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES;
	app.hAddCategory	= CreateWindow(TEXT("BUTTON"), TEXT("New category")	, STYLE_BUTTON, posCat.x, posCat.y, BUTTON_SIZE.x, BUTTON_SIZE.y, app.hRoot, 0, hInstance, 0);
	app.hAddMaterial	= CreateWindow(TEXT("BUTTON"), TEXT("New material")	, STYLE_BUTTON, posMat.x, posMat.y, BUTTON_SIZE.x, BUTTON_SIZE.y, app.hRoot, 0, hInstance, 0);
	app.hClear			= CreateWindow(TEXT("BUTTON"), TEXT("Clear tree")	, STYLE_BUTTON, posClr.x, posClr.y, BUTTON_SIZE.x, BUTTON_SIZE.y, app.hRoot, 0, hInstance, 0);

	app.hTree			= CreateWindow(WC_TREEVIEW, TEXT("Tree View"), STYLE_TREE, rcTree.left, rcTree.top, rcTree.right - rcTree.left, rcTree.bottom - rcTree.top, app.hRoot, 0, hInstance, 0); 

	MSG msg		= {};
	msg.wParam	= EXIT_FAILURE;
	while(GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg); 
	}
	return int(msg.wParam);
}

#define print_text_test_result(call, text) printf("\n%i <- %s with %s", call, #call, text);

// console build executes a test on the tree object instead of 
int main () {
	hd::MaterialTree		tree;

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