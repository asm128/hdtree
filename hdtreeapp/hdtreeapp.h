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

	static constexpr n2u32 BUTTON_SIZE = {96, 24};
	static constexpr n2lng MARGIN_SIZE = {2, 2};

	struct App {
		HINSTANCE	hInstance		= {};
		HWND		hRoot			= {};
		HWND		hTree			= {};
		HWND		hInput			= {};
		HWND		hAddCategory	= {};
		HWND		hAddMaterial	= {};
		HWND		hClear			= {};
	};
} // namespace

#endif // HDTREEAPP_H_230604
