#include "hdtree.h"

#include <Windows.h>
#include <CommCtrl.h>

#ifndef HDTREEAPP_H_230604
#define HDTREEAPP_H_230604

namespace hd
{
	template<typename T>
	struct n2 { T x, y; };

	typedef n2<uint32_t> n2u32;
	typedef n2<long>	 n2lng;

	static constexpr n2u32 SIZE_ICON	= {16, 16};
	static constexpr n2u32 SIZE_BUTTON	= {96, 24};
	static constexpr n2lng SIZE_MARGIN	= {2, 2};

	enum class INPUT_FIELD : uint8_t {
		None	= 0,
		Material,
		Category
	};

	// Holds 
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

	// 
	struct App {
		WinGDI			GDI;

		MaterialTree	Tree;

		INPUT_FIELD		ActiveInput		= {};

		App	(HINSTANCE hInstance) 
			: GDI{hInstance} { 
			WinGDI::defaultWndClass(GDI.WndClass, GDI.hInstance); 
		}
	};
} // namespace

#endif // HDTREEAPP_H_230604
