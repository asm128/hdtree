#include "hdtree.h"
#include "hdn2.h"

#ifdef _WIN32
#	include <Windows.h>
#	include <CommCtrl.h>
#endif

#ifndef HDTREEAPP_H_230604
#define HDTREEAPP_H_230604

namespace hd
{
	static constexpr n2u32 SIZE_ICON	= {16, 16};
	static constexpr n2u32 SIZE_BUTTON	= {96, 24};
	static constexpr n2lng SIZE_MARGIN	= {2, 2};

	enum class INPUT_FIELD : uint8_t {
		None	= 0,
		Material,
		Category
	};

#ifdef _WIN32
	struct TVImages {
		int32_t	Material		= 0;
		int32_t	CategoryEmpty	= 0;
		int32_t	CategoryOpen	= 0;
		int32_t	CategoryClosed	= 0;
	};

	LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	struct WinGDI {
		HINSTANCE	hInstance		= {};
		HWND		hRoot			= {};
		HWND		hTree			= {};
		HWND		hAddCategory	= {};
		HWND		hAddMaterial	= {};
		HWND		hClear			= {};
		HWND		hInput			= {};
		HIMAGELIST	hList			= 0;

		TVImages	Images			= {};

		WNDCLASSEX	WndClass		= {};

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
	};
#endif

	// 
	struct App {
#ifdef _WIN32
		WinGDI			GDI;

		App(HINSTANCE hInstance) : GDI{hInstance} { WinGDI::defaultWndClass(GDI.WndClass, GDI.hInstance); }
#endif
		MaterialTree	Tree;

		INPUT_FIELD		ActiveInput		= {};

	};
} // namespace

#endif // HDTREEAPP_H_230604
