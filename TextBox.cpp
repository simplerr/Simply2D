#include "TextBox.h"

TextBox::TextBox(WindowHandler* handler, WindowID id, string displayText, int x, int y, int width, int height, D3DCOLOR color)
	:Window(handler, id, x, y, width, height, color)
{
	mDisplayText = displayText;
}

TextBox::~TextBox()
{
	// nada
}

void TextBox::draw()
{	
	if(mVisible)
	{
		strcpy(buffer, mDisplayText.c_str());

		gGraphics->BlitRect(mX, mY, mWidth, mHeight, mColor);
		gGraphics->drawText(buffer, mX-mWidth/2+5, mY-mHeight/2, D3DCOLOR_ARGB(255,0,0,0));
	}
}

void TextBox::setDisplayText(string text)
{
	mDisplayText = text;
}

string TextBox::getValue(void)
{
	return mDisplayText;
}
