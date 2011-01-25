#include "CheckBox.h"

CheckBox::CheckBox(Window *parent, WindowID id, string display, int x, int y, int width, int height, D3DCOLOR checkedColor, D3DCOLOR color)
	:Window(parent, id, x, y, width, height, color)
{
	mDisplayText = display;
	mCheckedColor = checkedColor;

	mChecked = false;
}

CheckBox::~CheckBox()
{
	// dtor
}

int CheckBox::wm_lbuttondown(int x, int y)
{
	if(!mChecked)	{
		mChecked = true;
		mParent->messageHandler(getID(), "True");
	}
	else if(mChecked)	{
		mChecked = false;
		mParent->messageHandler(getID(), "False");
	}
	return 1;
}

int CheckBox::renderAll(void)
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

	// draw the text infront
	sprintf(buffer, "%s", mDisplayText.c_str());
	gGraphics->drawText(buffer, getX() - 100, getY() - getWidth()/2 - 2);

	return 1;
}

