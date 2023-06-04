#include "hdtreeapp.h"

#include <CommCtrl.h>

using namespace hd;

static void calcTreeRect(const RECT & rcClient, RECT & rcTree) { 
	rcTree = {MARGIN_SIZE.x, MARGIN_SIZE.y, rcClient.right - MARGIN_SIZE.x, long(rcClient.bottom - BUTTON_SIZE.y - MARGIN_SIZE.y * 2)}; 
}

static void calcButtonsPosition(const RECT & rcClient, n2u32 & xCat, n2u32 & xMat, n2u32 & xClr) {
	xCat.x = MARGIN_SIZE.x;
	xMat.x = (rcClient.right >> 1) - (BUTTON_SIZE.x >> 1);
	xClr.x = rcClient.right - MARGIN_SIZE.x - BUTTON_SIZE.x;

	xCat.y = xMat.y = xClr.y = rcClient.bottom - MARGIN_SIZE.y - BUTTON_SIZE.y;
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

	if(hWnd == app->hTree) {}
	else if(hWnd == app->hAddCategory) {}
	else if(hWnd == app->hAddMaterial) {}
	else if(hWnd == app->hClear) {}
	else if(hWnd == app->hInput) {}
	else if(hWnd == app->hRoot) {
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

HWND createButton(App & app, n2u32 pos, const TCHAR * text) {
	constexpr uint32_t STYLE_BUTTON	= WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON;
	return CreateWindow(TEXT("BUTTON"), text, STYLE_BUTTON, pos.x, pos.y, BUTTON_SIZE.x, BUTTON_SIZE.y, app.hRoot, 0, app.hInstance, 0);
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

	App		app			= {hInstance};

	RECT	rcClient	= {0, 0, 800, 600};  // dimensions of client area 
	RECT	rcWindow	= rcClient;  // dimensions of client area 
	AdjustWindowRect(&rcWindow, WS_OVERLAPPEDWINDOW, 0);
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

	app.hAddCategory	= ::createButton(app, posCat, TEXT("New category"));
	app.hAddMaterial	= ::createButton(app, posMat, TEXT("New material"));
	app.hClear			= ::createButton(app, posClr, TEXT("Clear tree"));

	constexpr uint32_t STYLE_TREE = WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES;
	app.hTree			= CreateWindow(WC_TREEVIEW, TEXT("Tree View"), STYLE_TREE, rcTree.left, rcTree.top, rcTree.right - rcTree.left, rcTree.bottom - rcTree.top, app.hRoot, 0, hInstance, 0); 

	MSG msg		= {};
	msg.wParam	= EXIT_FAILURE;
	while(GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg); 
	}
	return int(msg.wParam);
}
