#include "hdtree.h"

#include <Windows.h>

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

	// Holds 
	struct TVImages {
	    int			Material		= 0;
	    int			CategoryEmpty	= 0;
	    int			CategoryOpen	= 0;
	    int			CategoryClosed	= 0;
	};

	LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
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

	// 
	struct App {
		HINSTANCE		hInstance		= {};
		HWND			hRoot			= {};
		HWND			hTree			= {};
		HWND			hAddCategory	= {};
		HWND			hAddMaterial	= {};
		HWND			hClear			= {};
		HWND			hInput			= {};

		TVImages		Images			= {};

		MaterialTree	Tree;
		
		WNDCLASSEX		WndClass		= {};
#ifdef UNICODE
		std::wstring	NewName			= {};
#else
		std::string		NewName			= {};
#endif
		App	(HINSTANCE _hInstance) 
			: hInstance(_hInstance) { 
			defaultWndClass(WndClass, hInstance); 
		}
	};
} // namespace

#endif // HDTREEAPP_H_230604
