#include "hdtreeapp.h"

// I moved these here because they don't rely on Windows types.
void hd::calcTreeRect(const n2u32 & targetSize, RECT & rcTree) { 
	rcTree = {SIZE_MARGIN.x, SIZE_MARGIN.y, long(targetSize.x - SIZE_MARGIN.x), long(targetSize.y - SIZE_BUTTON.y - SIZE_MARGIN.y * 2)}; 
}

void hd::calcButtonsPosition(const n2u32 & targetSize, n2u32 & xCat, n2u32 & xMat, n2u32 & xClr) {
	xCat.x = SIZE_MARGIN.x;
	xMat.x = (targetSize.x >> 1) - (SIZE_BUTTON.x >> 1);
	xClr.x = targetSize.x - SIZE_MARGIN.x - SIZE_BUTTON.x;

	xCat.y = xMat.y = xClr.y = targetSize.y - SIZE_MARGIN.y - SIZE_BUTTON.y;
}