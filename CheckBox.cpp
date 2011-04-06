#include "CheckBox.h"

CheckBox::CheckBox(WindowHandler* handler, WindowID id, string display, int x, int y, int width, int height, D3DCOLOR checkedColor, D3DCOLOR color)
	:Window(handler, id, x, y, width, height, color)
{
	mDisplayText = display;
	mCheckedColor = checkedColor;

	mChecked = false;
}

CheckBox::~CheckBox()
{
	// dtor
}

void CheckBox::pressed(int mx, int my)
{
	if(!mChecked)	{
		mChecked = true;
		//callback();
	}
	else if(mChecked)	{
		mChecked = false;
		//callback();
	}
}

void CheckBox::draw(void)
{
	RECT tmpRect = getRect();

	// draw the margin
	gGraphics->BlitRect(tmpRect, D3DCOLOR_ARGB(255, 0, 0, 0));

	// draw the check area
	tmpRect.left += 2;
	tmpRect.right -= 2;
	tmpRect.top += 2;
	tmpRect.bottom -= 2;

	if(mChecked)
		gGraphics->BlitRect(tmpRect, mCheckedColor);
	else if(!mChecked)
		gGraphics->BlitRect(tmpRect, D3DCOLOR_ARGB(255, 255, 255, 255));

	// draw the text in front of the checkbox
	sprintf(buffer, "%s", mDisplayText.c_str());
	gGraphics->drawText(buffer, getX() - 100, getY() - getWidth()/2 - 2);
}

