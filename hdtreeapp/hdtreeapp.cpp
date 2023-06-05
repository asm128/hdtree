#include "hdtreeapp.h"

#include <CommCtrl.h>

using namespace hd;

static void calcTreeRect(const RECT & rcClient, RECT & rcTree) { 
	rcTree = {SIZE_MARGIN.x, SIZE_MARGIN.y, rcClient.right - SIZE_MARGIN.x, long(rcClient.bottom - SIZE_BUTTON.y - SIZE_MARGIN.y * 2)}; 
}

static void calcButtonsPosition(const RECT & rcClient, n2u32 & xCat, n2u32 & xMat, n2u32 & xClr) {
	xCat.x = SIZE_MARGIN.x;
	xMat.x = (rcClient.right >> 1) - (SIZE_BUTTON.x >> 1);
	xClr.x = rcClient.right - SIZE_MARGIN.x - SIZE_BUTTON.x;

	xCat.y = xMat.y = xClr.y = rcClient.bottom - SIZE_MARGIN.y - SIZE_BUTTON.y;
}

#define log_if(condition)							\
	if(condition) do {								\
		OutputDebugString(TEXT(#condition "\n"));	\
		printf("%s\n", #condition);			\
	} while(0)

#define fail_with_val_if(val, condition)			\
	if(condition) do {								\
		OutputDebugString(TEXT(#condition "\n"));	\
		printf("%s\n", #condition);					\
		return (val);								\
	} while(0)

#define log_if_failed(condition)	log_if(0 > (int32_t)(condition))
#define fail_if(condition)			fail_with_val_if(-1, condition)
#define fail_if_failed(condition)	fail_if(0 > (int32_t)(condition))
#define exit_if_failed(condition)	fail_with_val_if(EXIT_FAILURE, condition)

static int32_t hideInput(App & app) {
	SetWindowText(app.GDI.hInput, TEXT(""));
	ShowWindow(app.GDI.hInput, SW_HIDE);
	ShowWindow(app.GDI.hAddCategory, SW_SHOW);
	ShowWindow(app.GDI.hAddMaterial, SW_SHOW);
	return 0;
}

static int32_t showInput(App & app, INPUT_FIELD	field) {
	fail_if_failed(::hideInput(app));

	HWND hButton = (field == ::INPUT_FIELD::Material) ? app.GDI.hAddMaterial : app.GDI.hAddCategory;
	RECT rcClient = {};
	GetClientRect(app.GDI.hRoot, &rcClient);

	n2u32 posCat = {}, posMat = {}, posClr = {};
	::calcButtonsPosition(rcClient, posCat, posMat, posClr);

	n2u32 pos = (field == ::INPUT_FIELD::Material) ? posMat : posCat;
	SetWindowPos(app.GDI.hInput, 0, pos.x, pos.y, SIZE_BUTTON.x, SIZE_BUTTON.y, 0);
	UpdateWindow(app.GDI.hInput);

	ShowWindow(hButton, SW_HIDE);
	ShowWindow(app.GDI.hInput, SW_SHOW);
	SetFocus(app.GDI.hInput);
	return 0;
}

static int32_t imageAdd(HINSTANCE hInstance, HIMAGELIST hList, const TCHAR* filename, int & out_imageIndex) {
	HBITMAP hBitmap = LoadBitmap(hInstance, filename); 
	fail_if(0 == hBitmap);
	out_imageIndex = ImageList_Add(hList, hBitmap, (HBITMAP)NULL); 
	DeleteObject(hBitmap); 
	return (out_imageIndex >= 0) ? 0 : -1;
}

static int32_t createTreeViewImages(App & app) { 
	HIMAGELIST hList = 0;
	constexpr int NUM_BITMAPS   = 1;
	fail_if(0 == (hList = ImageList_Create(SIZE_ICON.x, SIZE_ICON.y, FALSE, NUM_BITMAPS, 0)));

	log_if_failed(imageAdd(app.GDI.hInstance, hList, TEXT("CategoryEmpty.bmp")	, app.GDI.Images.CategoryEmpty));
	log_if_failed(imageAdd(app.GDI.hInstance, hList, TEXT("CategoryOpen.bmp")	, app.GDI.Images.CategoryOpen));
	log_if_failed(imageAdd(app.GDI.hInstance, hList, TEXT("CategoryClosed.bmp")	, app.GDI.Images.CategoryClosed));
	log_if_failed(imageAdd(app.GDI.hInstance, hList, TEXT("Material.bmp")		, app.GDI.Images.Material));
	
	log_if(ImageList_GetImageCount(hList) < 4); // Fail if not all of the images were added. 

	TreeView_SetImageList(app.GDI.hTree, hList, TVSIL_NORMAL); // Associate the image list with the tree-view control. 
	return 0;
}

static int32_t createTree(App & app, RECT & rcClient) {
	RECT rcTree = {};
	::calcTreeRect(rcClient, rcTree);

	constexpr uint32_t STYLE_TREE = WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES;
	app.GDI.hTree = CreateWindow(WC_TREEVIEW, TEXT("Tree View"), STYLE_TREE, rcTree.left, rcTree.top, rcTree.right - rcTree.left, rcTree.bottom - rcTree.top, app.GDI.hRoot, 0, app.GDI.hInstance, 0);
	return app.GDI.hTree ? 0 : -1;
}

template<typename Tn2>
static HWND createButton(App & app, HWND parent, Tn2 pos, const TCHAR * text) {
	constexpr uint32_t STYLE_BUTTON	= WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON;
	return CreateWindow(TEXT("BUTTON"), text, STYLE_BUTTON, pos.x, pos.y, SIZE_BUTTON.x, SIZE_BUTTON.y, parent, 0, app.GDI.hInstance, 0);
}

static LRESULT CALLBACK EditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR /*uIdSubclass*/, DWORD_PTR dwRefData);
static int32_t createButtons(App & app, RECT & rcClient) {
	n2u32 posCat = {}, posMat = {}, posClr = {};
	::calcButtonsPosition(rcClient, posCat, posMat, posClr);
	fail_if(0 == (app.GDI.hAddCategory	= ::createButton(app, app.GDI.hRoot, posCat, TEXT("New category"))));
	fail_if(0 == (app.GDI.hAddMaterial	= ::createButton(app, app.GDI.hRoot, posMat, TEXT("New material"))));
	fail_if(0 == (app.GDI.hClear		= ::createButton(app, app.GDI.hRoot, posClr, TEXT("Clear tree"))));

	constexpr uint32_t STYLE_EDIT	= WS_BORDER | WS_CHILD | WS_VISIBLE | ES_LEFT;// | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL;
	app.GDI.hInput			= CreateWindowEx(0, TEXT("EDIT"), TEXT(""), STYLE_EDIT, posCat.x, posCat.y, SIZE_BUTTON.x * 2, SIZE_BUTTON.y, app.GDI.hRoot, 0, app.GDI.hInstance, 0);
	SetWindowSubclass(app.GDI.hInput, &EditProc, 0, (DWORD_PTR)&app);
	ShowWindow(app.GDI.hInput, SW_HIDE);
	return (app.GDI.hAddCategory && app.GDI.hAddMaterial && app.GDI.hClear && app.GDI.hInput) ? 0 : -1;
}

static int32_t createWindow(App & app, RECT & rcClient) {
	RECT rcWindow = rcClient;  // dimensions of client area 
	AdjustWindowRect(&rcWindow, WS_OVERLAPPEDWINDOW, 0);

	constexpr uint32_t STYLE_ROOT = WS_VISIBLE | WS_OVERLAPPEDWINDOW;
	fail_if(0 == (app.GDI.hRoot = CreateWindow(app.GDI.WndClass.lpszClassName, TEXT("Caption"), STYLE_ROOT, CW_USEDEFAULT, CW_USEDEFAULT, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, 0, 0, app.GDI.hInstance, &app)));
	return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/) {
	// Ensure that the common control DLL is loaded. 
	INITCOMMONCONTROLSEX icc = {sizeof(INITCOMMONCONTROLSEX), ICC_WIN95_CLASSES};
	InitCommonControlsEx(&icc); 

	App	app	= {hInstance};

	exit_if_failed(0 == RegisterClassEx(&app.GDI.WndClass));

	RECT rcClient = {0, 0, 800, 600};  // dimensions of client area 

	exit_if_failed(::createWindow (app, rcClient));
	exit_if_failed(::createTree   (app, rcClient));
	exit_if_failed(::createButtons(app, rcClient));
	log_if_failed(::createTreeViewImages(app));

	//exit_with_error_if(0 > ::createInput  (app, rcClient));

	MSG msg		= {};
	msg.wParam	= EXIT_FAILURE;
	while(GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return int(msg.wParam);
}

#ifdef UNICODE
static int wcstombs (std::string & output, const std::wstring & input)	{
	if(0 == input.size())
		return 0;

	int sizeNeededForMultiByte = WideCharToMultiByte(CP_UTF8, 0, &input[0], (int)input.size(), 0, 0, 0, 0);
	fail_if(0 == sizeNeededForMultiByte);

	std::string converted;
	converted.resize(sizeNeededForMultiByte);
	WideCharToMultiByte(CP_UTF8, 0, &input[0], (int)input.size(), &converted[0], (int)converted.size(), 0, 0);
	output.append(converted);
	return sizeNeededForMultiByte;
}
#endif

static LRESULT CALLBACK EditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR /*uIdSubclass*/, DWORD_PTR dwRefData) {
	App & app = *(App*)dwRefData;
	switch(uMsg) {
	case WM_CHAR:
		switch(wParam) {
		case VK_ESCAPE:	
			log_if_failed(::hideInput(app));
			return 0;
		case VK_RETURN: {
			size_t length = GetWindowTextLength(hWnd);
			if(0 == length)
				app.NewName.clear();
			else {
				app.NewName.resize(length + 1);
				GetWindowText(hWnd, &app.NewName[0], (int)app.NewName.size());
			}
#ifdef UNICODE
			std::string newName		= {};
			::wcstombs(newName, app.NewName);
#else
			std::string & newName	= app.NewName;
#endif
			log_if_failed(app.Tree.AddCategory(newName));
			log_if_failed(::hideInput(app));
			return 0;
		}
		}
	}
	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK hd::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch(uMsg) {
	case WM_GETMINMAXINFO:	// This message prevents the window from becoming too small.
		((::MINMAXINFO*)lParam)->ptMinTrackSize	= {320, 200};
		return 0;
	case WM_NCCREATE: { 
		const CREATESTRUCTA	& createStruct	= *(const CREATESTRUCTA*)lParam;
		if(0 == createStruct.hwndParent) {
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)createStruct.lpCreateParams);
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	}

	App	* app = (App*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if(0 == app)
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	switch(uMsg) {
	case WM_COMMAND: {
			 if((HWND)lParam == app->GDI.hAddCategory) { app->ActiveInput = INPUT_FIELD::Category; ::showInput(*app, app->ActiveInput); }
		else if((HWND)lParam == app->GDI.hAddMaterial) { app->ActiveInput = INPUT_FIELD::Material; ::showInput(*app, app->ActiveInput); }
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_SIZE: {
		RECT rcClient = {};  // dimensions of client area 
		GetClientRect(app->GDI.hRoot, &rcClient);

		n2u32	posCat = {}, posMat = {}, posClr = {};
		::calcButtonsPosition(rcClient, posCat, posMat, posClr);
		SetWindowPos(app->GDI.hAddCategory	, 0, posCat.x, posCat.y, SIZE_BUTTON.x, SIZE_BUTTON.y, 0);
		SetWindowPos(app->GDI.hAddMaterial	, 0, posMat.x, posMat.y, SIZE_BUTTON.x, SIZE_BUTTON.y, 0);
		SetWindowPos(app->GDI.hClear		, 0, posClr.x, posClr.y, SIZE_BUTTON.x, SIZE_BUTTON.y, 0);
		SetWindowPos(app->GDI.hInput		, 0, posMat.x, posMat.y, SIZE_BUTTON.x, SIZE_BUTTON.y, 0);

		RECT	rcTree = {};
		::calcTreeRect(rcClient, rcTree);
		SetWindowPos(app->GDI.hTree, 0, rcTree.left, rcTree.top, rcTree.right - rcTree.left, rcTree.bottom - rcTree.top, 0);
		break;
	}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
