#include "Button.h"

Button::Button(WindowID id, string display, int x, int y, int width, int height, D3DCOLOR color)
			:Window(id, x, y, width, height, color)
{
	mValue = "none";
	mDisplayText = display;
	pressed = false;
	mFlashTime = .08;
	// eh inget att g�ra?:d
}
Button::~Button()
{
	// dtor
}

void Button::updateWindow(float dt)
{
	static float dtsum;

	if(dtsum >= mFlashTime)	{
		dtsum = 0;
		pressed = false;
	}
	else
		dtsum += dt;
}

int Button::wm_lbuttondown(int x, int y)
{
	if(mVisible)
	{
		if(!pressed)	{
			pressed = true;
			//mValue = "pressed";
			mParent->messageHandler(getID());//, mValue);
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
			//pressed = false;
			//mValue = "notpressed";
		}
		else
			gGraphics->BlitRect(mPosition.x, mPosition.y, mPosition.width, mPosition.height, mColor);
		
		strcpy(buffer, mDisplayText.c_str());
		gGraphics->drawText(buffer, mPosition.x-mPosition.width/2+5, mPosition.y-mPosition.height/2, D3DCOLOR_ARGB(255,0,0,0));
	}

	return 1;
}