#include "Button.h"

Button::Button(char* name, string value, int x, int y, int width, int height, D3DCOLOR color)
			:Window(name, x, y, width, height, color)
{
	mValue = value;
	// eh inget att göra?:d
}
Button::~Button()
{
	// dtor
}

int Button::wm_lbuttondown(int x, int y)
{
	if(mVisible)
	{
		if(!pressed)	{
			pressed = true;
			mValue = "pressed";
		}
	}

	return 1;
}

int Button::renderAll(void)
{
	if(mVisible)
	{
		if(pressed)	{
			gGraphics->BlitRect(mPosition.x, mPosition.y, mPosition.width, mPosition.height, mColor);
			pressed = false;
			mValue = "notpressed";
		}
		else
			gGraphics->BlitRect(mPosition.x, mPosition.y, mPosition.width, mPosition.height, D3DCOLOR_ARGB(255, 200, 100, 30));
		
		//char tmp [256];
		//strcpy(tmp, mName.c_str());
		gGraphics->drawText(mName, mPosition.x-mPosition.width/2+5, mPosition.y-mPosition.height/2, D3DCOLOR_ARGB(255,0,0,0));
	}

	return 1;
}