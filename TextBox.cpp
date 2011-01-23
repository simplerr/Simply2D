#include "TextBox.h"

TextBox::TextBox(Window *parent, WindowID id, string display, int x, int y, int width, int height, D3DCOLOR color)
	:Window(parent, id, x, y, width, height, color)
{
	mDisplayText = display;
}

TextBox::~TextBox()
{
	// nada
}

int TextBox::renderAll()
{	
	if(mVisible)
	{
		//char tmp[256];
		strcpy(buffer, mDisplayText.c_str());

		gGraphics->BlitRect(mX, mY, mWidth, mHeight, mColor);
		gGraphics->drawText(buffer, mX-mWidth/2+5, mY-mHeight/2, D3DCOLOR_ARGB(255,0,0,0));
	}

	return 1;
}

int TextBox::wm_lbuttondown(int x, int y)
{
	return 1;
}
