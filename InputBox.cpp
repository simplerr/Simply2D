#include "InputBox.h"

InputBox::InputBox(Window *parent, WindowID id, int x, int y, int width, int height, D3DCOLOR color)
		: Window(parent, id, x, y, width, height, color)
{
	showCaret = false;
}

InputBox::~InputBox()
{
	// nada
}

void InputBox::updateWindow(float dt)
{
	static float dtsum = 0;
	dtsum +=dt;

	if(dtsum >= .6)	{
		showCaret = true;
	}
	if(dtsum >= 1.2)	{
		dtsum = 0;
		showCaret = false;
	}
}

int InputBox::renderAll()
{
	if(mVisible)
	{
		//static char tmp [256];
		strcpy(buffer, mValue.c_str());

		gGraphics->BlitFullRectBordered(mX, mY, mWidth, mHeight, mColor);
		gGraphics->drawText(">", mX-mWidth/2+5, mY-mHeight/2, D3DCOLOR_ARGB(255,255,165,0));
		gGraphics->drawText(buffer, mX-mWidth/2+15, mY-mHeight/2, D3DCOLOR_ARGB(255,0,0,0));

		if(mActive)
		{
			if(showCaret)
				gGraphics->drawText("|", mX-mWidth/2+(14+caretPos*9), mY-mHeight/2, D3DCOLOR_ARGB(255,0,0,0));	
		}
	}

	return 1;
}

int InputBox::wm_lbuttondown(int x, int y)
{
	caretPos = mValue.size();
	return 1;
}

int InputBox::wm_keydown(WPARAM wParam)
{
	char input = wParam;
	
	if(mActive)
	{
		switch(input)
		{
		// enter
		case '\r':		{
							mParent->messageHandler(getID(), mValue);
							break;
						}
		// backspace
		case '\b':		{
							if(caretPos > 0)	
							{
								mValue.erase(caretPos-1, 1);
								caretPos -=1;
							}
							break;
						
						}
		// left
		case VK_LEFT:	{
							if(caretPos > 0)
								caretPos -= 1;
							break;
						}
		// right
		case VK_RIGHT:	{
							if(caretPos < mValue.size())
								caretPos +=1;
							break;
						}
		// delete
		case VK_DELETE:	{
							if(caretPos < mValue.size())
							{
								mValue.erase(caretPos, 1);
							}

							break;
						}
		// input
		default:		{
							if(mValue.size() < 4)
							{
								string strInput = string(1,input);		
								mValue.insert(caretPos, strInput);	
								caretPos +=1;
							}						
						}
		}
	}
	return 1;
}