#include "TextBox.h"

TextBox::TextBox(WindowID id, string value, int x, int y, int width, int height, D3DCOLOR color)
	:Window(id, x, y, width, height, color)
{
	
	mValue = value;
}

TextBox::~TextBox()
{
	// nada
}

int TextBox::renderAll()
{	
	//char tmp[256];
	strcpy(buffer, mValue.c_str());

	gGraphics->BlitRect(mPosition.x, mPosition.y, mPosition.width, mPosition.height, mColor);
	gGraphics->drawText(buffer, mPosition.x-mPosition.width/2+5, mPosition.y-mPosition.height/2, D3DCOLOR_ARGB(255,0,0,0));

	return 1;
}

int TextBox::wm_lbuttondown(int x, int y)
{
	return 1;
}
