#include "Button.h"

Button::Button(Window *parent, WindowID id, string display, int x, int y, int width, int height, D3DCOLOR color)
			:Window(parent, id, x, y, width, height, color)
{
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
	if(getVisible())
	{
		if(!pressed)	{
			pressed = true;
			//mValue = "pressed";
			mParent->messageHandler(mID);
		}
	}

	return 1;
}

int Button::renderAll(void)
{
	//sprintf(buffer, "value: %s, x: %i, y: %i, width: %i, height: %i", mValue.c_str(), mX, mY, mWidth, mHeight);
	//MessageBox(0, buffer, 0, 0);
	if(getVisible())
	{
		if(pressed)	{
			gGraphics->BlitRect(mX, mY, mWidth, mHeight, D3DCOLOR_ARGB(255, 255, 166, 0));
			//pressed = false;
			//mValue = "notpressed";
		}
		else
			gGraphics->BlitRect(mX, mY, mWidth, mHeight, mColor);
		
		strcpy(buffer, mDisplayText.c_str());
		gGraphics->drawText(buffer, mX-mWidth/2+5, mY-mHeight/2, D3DCOLOR_ARGB(255,0,0,0));
	}

	return 1;
}