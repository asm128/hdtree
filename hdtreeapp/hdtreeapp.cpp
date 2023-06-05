#include "hdtreeapp.h"
#include "hdlog.h"

#include <memory>

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

static int32_t hideInput(WinGDI & gdi) {
	SetWindowText(gdi.hInput, TEXT(""));
	ShowWindow(gdi.hInput, SW_HIDE);
	ShowWindow(gdi.hAddCategory, SW_SHOW);
	ShowWindow(gdi.hAddMaterial, SW_SHOW);
	return 0;
}

static int32_t showInput(WinGDI & gdi, INPUT_FIELD	field) {
	fail_if_failed(::hideInput(gdi));

	HWND hButton = (field == INPUT_FIELD::Material) ? gdi.hAddMaterial : gdi.hAddCategory;
	RECT rcClient = {};
	GetClientRect(gdi.hRoot, &rcClient);

	n2u32 posCat = {}, posMat = {}, posClr = {};
	::calcButtonsPosition(rcClient, posCat, posMat, posClr);

	n2u32 pos = (field == INPUT_FIELD::Material) ? posMat : posCat;
	SetWindowPos(gdi.hInput, 0, pos.x, pos.y, SIZE_BUTTON.x, SIZE_BUTTON.y, 0);
	UpdateWindow(gdi.hInput);

	ShowWindow(hButton, SW_HIDE);
	ShowWindow(gdi.hInput, SW_SHOW);
	SetFocus(gdi.hInput);
	return 0;
}

static int32_t imageAdd(HINSTANCE hInstance, HIMAGELIST hList, const TCHAR* filename, int & out_imageIndex) {
	struct Deleter { 
		HBITMAP hBitmap = 0;

		~Deleter() { if(hBitmap) DeleteObject(hBitmap); }

		HBITMAP get_and_clear() { HBITMAP h = hBitmap; hBitmap = (HBITMAP)0; return h; }
	};

	Deleter	bmp; 
	fail_if(0 == (bmp.hBitmap = LoadBitmap(hInstance, filename)));
	fail_if(0 > (out_imageIndex = ImageList_Add(hList, bmp.get_and_clear(), (HBITMAP)0))); 
	return 0;
}

static int32_t createTreeViewImages(WinGDI & gdi) { 
	constexpr int NUM_BITMAPS   = 1;
	fail_if(0 == (gdi.hList = ImageList_Create(SIZE_ICON.x, SIZE_ICON.y, FALSE, NUM_BITMAPS, 0)));

	log_if_failed(imageAdd(gdi.hInstance, gdi.hList, TEXT("CategoryEmpty.bmp")	, gdi.Images.CategoryEmpty));
	log_if_failed(imageAdd(gdi.hInstance, gdi.hList, TEXT("CategoryOpen.bmp")	, gdi.Images.CategoryOpen));
	log_if_failed(imageAdd(gdi.hInstance, gdi.hList, TEXT("CategoryClosed.bmp")	, gdi.Images.CategoryClosed));
	log_if_failed(imageAdd(gdi.hInstance, gdi.hList, TEXT("Material.bmp")		, gdi.Images.Material));
	
	log_if(ImageList_GetImageCount(gdi.hList) < 4); // Fail if not all of the images were added. 

	TreeView_SetImageList(gdi.hTree, gdi.hList, TVSIL_NORMAL); // Associate the image list with the tree-view control. 
	return 0;
}

static int32_t createTree(WinGDI & gdi, RECT & rcClient) {
	RECT rcTree = {};
	::calcTreeRect(rcClient, rcTree);

	constexpr uint32_t STYLE_TREE = WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES;
	fail_if(0 == (gdi.hTree = CreateWindow(WC_TREEVIEW, TEXT("Tree View"), STYLE_TREE, rcTree.left, rcTree.top, rcTree.right - rcTree.left, rcTree.bottom - rcTree.top, gdi.hRoot, 0, gdi.hInstance, 0)));
	return 0;
}

template<typename Tn2>
static HWND createButton(WinGDI & gdi, HWND parent, Tn2 pos, const TCHAR * text) {
	constexpr uint32_t STYLE_BUTTON	= WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON;
	return CreateWindow(TEXT("BUTTON"), text, STYLE_BUTTON, pos.x, pos.y, SIZE_BUTTON.x, SIZE_BUTTON.y, parent, 0, gdi.hInstance, 0);
}

static LRESULT CALLBACK EditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR /*uIdSubclass*/, DWORD_PTR dwRefData);
static int32_t createButtons(App & app, RECT & rcClient) {
	n2u32	posCat	= {}, posMat = {}, posClr = {};
	::calcButtonsPosition(rcClient, posCat, posMat, posClr);

	WinGDI	& gdi	= app.GDI;
	fail_if(0 == (gdi.hAddCategory	= ::createButton(gdi, gdi.hRoot, posCat, TEXT("New category"))));
	fail_if(0 == (gdi.hAddMaterial	= ::createButton(gdi, gdi.hRoot, posMat, TEXT("New material"))));
	fail_if(0 == (gdi.hClear		= ::createButton(gdi, gdi.hRoot, posClr, TEXT("Clear tree"))));

	constexpr uint32_t STYLE_EDIT	= WS_BORDER | WS_CHILD | WS_VISIBLE | ES_LEFT;// | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL;
	fail_if(0 == (gdi.hInput = CreateWindowEx(0, TEXT("EDIT"), TEXT(""), STYLE_EDIT, posCat.x, posCat.y, SIZE_BUTTON.x * 2, SIZE_BUTTON.y, app.GDI.hRoot, 0, app.GDI.hInstance, 0)));
	SetWindowSubclass(gdi.hInput, &EditProc, 0, (DWORD_PTR)&app);
	ShowWindow(app.GDI.hInput, SW_HIDE);
	return (app.GDI.hAddCategory && app.GDI.hAddMaterial && app.GDI.hClear && app.GDI.hInput) ? 0 : -1;
}

static int32_t createWindow(App & app, RECT & rcClient) {
	WinGDI	& gdi = app.GDI;
	RECT	rcWindow = rcClient;  // dimensions of client area 
	AdjustWindowRect(&rcWindow, WS_OVERLAPPEDWINDOW, 0);

	constexpr uint32_t STYLE_ROOT = WS_VISIBLE | WS_OVERLAPPEDWINDOW; 
	fail_if(0 == (gdi.hRoot = CreateWindow(gdi.WndClass.lpszClassName, TEXT("Caption"), STYLE_ROOT, CW_USEDEFAULT, CW_USEDEFAULT, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, 0, 0, gdi.hInstance, &app)));
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/) {
	// Ensure that the common control DLL is loaded. 
	INITCOMMONCONTROLSEX icc = {sizeof(INITCOMMONCONTROLSEX), ICC_WIN95_CLASSES};
	InitCommonControlsEx(&icc); 

	App	app	= {hInstance};

	exit_if_failed(0 == RegisterClassEx(&app.GDI.WndClass));

	RECT rcClient = {0, 0, 800, 600};  // dimensions of client area 

	exit_if_failed(::createWindow(app, rcClient));

	exit_if_failed(::createTree(app.GDI, rcClient));
	log_if_failed(::createTreeViewImages(app.GDI));

	exit_if_failed(::createButtons(app, rcClient));

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
static int wcstombs (std::string & output, const std::wstring & input) {
	if(0 == input.size())
		return 0;

	int sizeNeededForMultiByte = WideCharToMultiByte(CP_UTF8, 0, &input[0], (int)input.size(), 0, 0, 0, 0);
	fail_if(0 == sizeNeededForMultiByte);

	std::string converted;
	converted.resize(sizeNeededForMultiByte);
	fail_if(0 == WideCharToMultiByte(CP_UTF8, 0, &input[0], (int)input.size(), &converted[0], (int)converted.size(), 0, 0));
	output.append(converted);
	return sizeNeededForMultiByte;
}

static int mbstowcs (std::wstring & output, const std::string & input)	{
	if(0 == input.size())
		return 0;

	std::wstring converted;
	converted.resize(input.size());
	fail_if(0 == MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, &input[0], (int)input.size(), &converted[0], (int)converted.size()));
	converted.resize((uint32_t)wcslen(&converted[0]));
	output.append(converted);
	return (int)converted.size();
}

#endif

static HTREEITEM addTreeItem(App & app, const std::string & itemText, HTREEITEM hParent, HTREEITEM hPrev, const TVImages & tvImages) { 
	TVITEM		tvi		= {}; 
	HWND		hTree	= app.GDI.hTree;
	HTREEITEM	hItem	= {};
	{ // add item
		TVINSERTSTRUCT tvins = {}; 
		tvi.mask			= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM; 
#ifdef UNICODE
		std::wstring text;
		mbstowcs(text, itemText);
#else
		std::string text = itemText;
#endif
		tvi.pszText			= &text[0];
		tvi.cchTextMax		= (int)text.size();
		tvi.iImage			= (hParent == TVI_ROOT) ? tvImages.CategoryEmpty : tvImages.Material; 
		tvi.iSelectedImage	= (hParent == TVI_ROOT) ? tvImages.CategoryEmpty : tvImages.Material; 
		tvi.lParam			= (LPARAM)&app; 

		tvins.item			= tvi; 
		tvins.hInsertAfter	= hPrev; 
		tvins.hParent		= hParent; 
		fail_with_val_if((HTREEITEM)0, (0 == (hItem = (HTREEITEM)SendMessage(hTree, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvins))));	// Add the item to the tree-view control. 
	}

	if (hParent != TVI_ROOT) { // The new item is a child item. Give the parent item a closed folder bitmap to indicate it now has child items. 
		tvi.mask			= TVIF_IMAGE | TVIF_SELECTEDIMAGE; 
		tvi.hItem			= hParent;
		tvi.iImage			= tvImages.CategoryClosed; 
		tvi.iSelectedImage	= tvImages.CategoryClosed; 
		TreeView_SetItem(hTree, &tvi); 
	} 
	return hItem;
} 

static int32_t refreshTree(App & app) {
	for(uint32_t iCat = 0; iCat < app.Tree.Categories.size(); ++iCat) {
		const Category & category = app.Tree.Categories[iCat];

		HTREEITEM hCat;
		log_if(0 == (hCat = addTreeItem(app, category.Name, (HTREEITEM)TVI_ROOT, (HTREEITEM)TVI_LAST, app.GDI.Images)));

		for(uint32_t iMat = 0; iMat < category.Materials.size(); ++iMat) {
			log_if(0 == addTreeItem(app, category.Materials[iMat], hCat, (HTREEITEM)TVI_LAST, app.GDI.Images));
		}
	}
	return 0;
}

static LRESULT CALLBACK EditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR /*uIdSubclass*/, DWORD_PTR dwRefData) {
	App & app = *(App*)dwRefData;
	switch(uMsg) {
	case WM_CHAR:
		switch(wParam) {
		case VK_ESCAPE:	
			log_if_failed(::hideInput(app.GDI));
			return 0;
		case VK_RETURN: {
			size_t length = GetWindowTextLength(hWnd);
#ifdef UNICODE
			std::wstring tName = {};
#else
			std::string	tName = {};
#endif
			if(0 == length)
				tName.clear();
			else {
				tName.resize(length + 1);
				GetWindowText(hWnd, &tName[0], (int)tName.size());
			}
#ifdef UNICODE
			std::string newName = {};
			::wcstombs(newName, tName);
#else
			std::string & newName = tName;
#endif
			std::string categoryName = app.Tree.Categories.size() ? app.Tree.Categories[app.Tree.Categories.size() - 1].Name : std::string{};	// #TODO: grab this from the GDI or from a member in App.

			log_if_failed((app.ActiveInput == INPUT_FIELD::Category) ? app.Tree.AddCategory(newName) : app.Tree.AddMaterial(categoryName, newName));
			log_if_failed(::hideInput(app.GDI));
			log_if_failed(::refreshTree(app));
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
		if((HWND)lParam == app->GDI.hAddCategory || (HWND)lParam == app->GDI.hAddMaterial) {
			app->ActiveInput = ((HWND)lParam == app->GDI.hAddCategory) ? INPUT_FIELD::Category: INPUT_FIELD::Material; 
			log_if_failed(::showInput(app->GDI, app->ActiveInput));
		}
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
