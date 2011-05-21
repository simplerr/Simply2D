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

bool CheckBox::pressed(int mx, int my)
{
	if(!getChecked())	{
		setChecked(true);
		if(callback != NULL)
			callback(getID());
	}
	else if(getChecked())	{
		setChecked(false);
		if(callback != NULL)
			callback(getID());
	}

	// doesn't need to return anything else,
	// in facts it's only buttons that can change state
	return true;
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

	if(getChecked())
		gGraphics->BlitRect(tmpRect, mCheckedColor);
	else if(!getChecked())
		gGraphics->BlitRect(tmpRect, D3DCOLOR_ARGB(255, 255, 255, 255));

	// draw the text in front of the checkbox
	sprintf(buffer, "%s", mDisplayText.c_str());
	gGraphics->drawText(buffer, getX() - 100, getY() - getWidth()/2 - 2);
}

bool CheckBox::getChecked(void)
{
	return mChecked;
}

void CheckBox::setChecked(bool checked)
{
	mChecked = checked;
}
