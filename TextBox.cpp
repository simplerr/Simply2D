#include "TextBox.h"

TextBox::TextBox(WindowHandler* handler, WindowID id, string display, int x, int y, int width, int height, D3DCOLOR color)
	:Window(handler, id, x, y, width, height, color)
{
	mDisplayText = display;
}

TextBox::~TextBox()
{
	// nada
}

void TextBox::draw()
{	
	if(mVisible)
	{
		//char tmp[256];
		strcpy(buffer, mDisplayText.c_str());

		gGraphics->BlitRect(mX, mY, mWidth, mHeight, mColor);
		gGraphics->drawText(buffer, mX-mWidth/2+5, mY-mHeight/2, D3DCOLOR_ARGB(255,0,0,0));
	}
}
