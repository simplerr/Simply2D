#include "InputBox.h"
#include <sstream>
#include "boost\lexical_cast.hpp"

InputBox::InputBox(WindowHandler* handler, WindowID id, DataType dataType, int x, int y, int width, int height, int maxlen, D3DCOLOR color)
		: Window(handler, id, x, y, width, height, color)
{
	mShowCaret = false;
	mMaxLength = maxlen;
	mDataType = dataType;
	setValue(" ");
}

InputBox::~InputBox()
{
	// dtor
}

void InputBox::update(double dt)
{
	static float dtsum = 0;
	dtsum +=dt;

	if(dtsum >= .6)	{
		mShowCaret = true;
	}
	if(dtsum >= 1.2)	{
		dtsum = 0;
		mShowCaret = false;
	}
}

void InputBox::draw()
{
	if(mVisible)
	{
		gGraphics->BlitFullRectBordered(mX, mY, mWidth, mHeight, mColor);
		gGraphics->drawText(">", mX-mWidth/2+5, mY-mHeight/2, D3DCOLOR_ARGB(255,255,165,0));
		gGraphics->drawText((char*)getString().c_str(), mX-mWidth/2+15, mY-mHeight/2, D3DCOLOR_ARGB(255,0,0,0));

		if(mActive)
		{
			if(mShowCaret)
				gGraphics->drawText("|", mX-mWidth/2+(12+mCaretPos*8), mY-mHeight/2, D3DCOLOR_ARGB(255,0,0,0));		
		}
	}
}

bool InputBox::pressed(int mx, int my)
{
	mCaretPos = getString().size();
	return true;
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
							if(callback != NULL)
								callback(getID());
							break;
						}
		// backspace
		case '\b':		{
							if(mCaretPos > 0)	
							{
								// erase letter
								mValue.erase(mCaretPos-1, 1);
								mCaretPos -=1;
							}
							break;
						
						}
		// left
		case VK_LEFT:	{
							if(mCaretPos > 0)
								mCaretPos -= 1;
							break;
						}
		// right
		case VK_RIGHT:	{
							if(mCaretPos < getString().size())
								mCaretPos +=1;
							break;
						}
		// delete
		case VK_DELETE:	{
							if(mCaretPos < getString().size())
							{
								// erase letter
								mValue.erase(mCaretPos, 1);
							}

							break;
						}
		// input
		default:		{
							if(getString().size() < mMaxLength)
							{
								// just for git
								string strInput = string(1,input);	

								if(strInput == "¾")
									mValue.insert(mCaretPos, ".");
								else if((int)input > 0)	{
									if(isdigit(input) || isalpha(input))	
										mValue.insert(mCaretPos, strInput);
									else
										return 1;
								}						
								else
									return 1;

								mCaretPos +=1;
							}						
						}
		}
	}
	return 1;
}

void InputBox::setActive(bool b)
{
	Window::setActive(b);

	mShowCaret = b;
}

void InputBox::setValue(string value)
{
	mValue = value;
}

void InputBox::setValue(int value)
{
	mValue = boost::lexical_cast<string>(value);
}

void InputBox::setValue(float value)
{
	mValue = boost::lexical_cast<string>(value);
}

/*template<class T>
T InputBox::getValue(void)
{
	if(mDataType == TYPE_INT)	{
		int returnInt = boost::lexical_cast<int>(mValue);
		return returnInt;
	}
	else if(mDataType == TYPE_TEXT)
		return mValue;
	else if(mDataType == TYPE_FLOAT)
		return boost::lexical_cast<float>(mValue);
	else
		return false;
}*/

int InputBox::getInt(void)
{
	return boost::lexical_cast<int>(mValue);
}

float InputBox::getFloat(void)
{
	return boost::lexical_cast<float>(mValue);
}

string InputBox::getString(void)
{
	return mValue;
}
