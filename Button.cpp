#include "Button.h"

Button::Button(WindowID id, string value, int x, int y, int width, int height, D3DCOLOR color)
			:Window(id, x, y, width, height, color)
{
	mValue = value;
	pressed = false;
	// eh inget att g�ra?:d
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
			sprintf(buffer, "x: %i, y: %i", mPosition.x, mPosition.y);
			MessageBox(0, buffer, 0, 0);
			pressed = true;
			mValue = "pressed";
			mParent->messageHandler(getID(), mValue);
		}
	}

	return 1;
}

int Button::renderAll(void)
{
	if(mVisible)
	{
		if(pressed)	{
			gGraphics->BlitRect(mPosition.x, mPosition.y, mPosition.width, mPosition.height, D3DCOLOR_ARGB(255, 255, 166, 0));
			pressed = false;
			mValue = "notpressed";
		}
		else
			gGraphics->BlitRect(mPosition.x, mPosition.y, mPosition.width, mPosition.height, D3DCOLOR_ARGB(255, 200, 100, 30));
		
		//char tmp [256];
		//strcpy(tmp, mName.c_str());
		gGraphics->drawText("Knapp!", mPosition.x-mPosition.width/2+5, mPosition.y-mPosition.height/2, D3DCOLOR_ARGB(255,0,0,0));
	}

	return 1;
}