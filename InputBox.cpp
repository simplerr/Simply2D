#include "InputBox.h"

InputBox::InputBox(char* name, int x, int y, int width, int height, D3DCOLOR color)
		: Window(name, x, y, width, height, color)
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
	//static char tmp [256];
	strcpy(buffer, mValue.c_str());

	gGraphics->BlitFullRectBordered(mPosition.x, mPosition.y, mPosition.width, mPosition.height, mColor);
	gGraphics->drawText(">", mPosition.x-mPosition.width/2+5, mPosition.y-mPosition.height/2, D3DCOLOR_ARGB(255,255,165,0));
	gGraphics->drawText(buffer, mPosition.x-mPosition.width/2+15, mPosition.y-mPosition.height/2, D3DCOLOR_ARGB(255,0,0,0));

	if(mActive)
	{
		if(showCaret)
			gGraphics->drawText("|", mPosition.x-mPosition.width/2+(14+caretPos*9), mPosition.y-mPosition.height/2, D3DCOLOR_ARGB(255,0,0,0));	
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
							// används inte och lär inte göra det!
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