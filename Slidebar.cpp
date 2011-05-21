#include "Slidebar.h"
#include "TextBox.h"
#include "InputBox.h"
#include "WindowHandler.h"

Slidebar::Slidebar(WindowHandler* handler, WindowID id, string display, int x, int y, int width, int height, float value, float minValue, float maxValue)
	: Window(handler, id, x, y, width, height)
{
	mMinValue = minValue;
	mMaxValue = maxValue;
	mMovingSlider = false;
	mSliderWidth = 40;
	
	if(value > mMaxValue)
		mValue = mMaxValue;
	else if(value < mMinValue)
		mValue = mMinValue;
	else
		mValue = value;


	mSlideBackground.left = getX() - 80;
	mSlideBackground.right = getX() + 80;
	mSlideBackground.top = getY() - 10;
	mSlideBackground.bottom = getY() + 10;

	mTextName = new TextBox(handler, SLIDER_TEXT, display, mSlideBackground.left - 50 - handler->getRect().left,
		getY()- handler->getRect().top, 80, 20);
	mInputBox = new InputBox(handler, SLIDER_INPUT, TYPE_TEXT, mSlideBackground.right + 35 - handler->getRect().left,
		getY()- handler->getRect().top, 50, 20, 4);

	mSliderPos = mValue / (maxValue - minValue) * (mSlideBackground.right - mSlideBackground.left);

	mSlider.left = mSlideBackground.left + mSliderPos - mSliderWidth/2;
	mSlider.right = mSlideBackground.left + mSliderPos + mSliderWidth/2;

	if(mSlider.left < mSlideBackground.left)	{
		mSlider.left = mSlideBackground.left;
		mSlider.right = mSlider.left + mSliderWidth;
	}

	if(mSlider.right > mSlideBackground.right)	{
		mSlider.right = mSlider.right;
		mSlider.left = mSlider.right - mSliderWidth;
	}
	
	mSlider.top = getY() - 10;
	mSlider.bottom = getY()  + 10;

	mInputBox->setValue(value);
}
	
Slidebar::~Slidebar()
{
	// dtor
}

void Slidebar::draw(void)
{
	gGraphics->BlitRect(mSlideBackground, D3DCOLOR_ARGB(255, 255, 0, 0));

	gGraphics->BlitRect(mSlider, D3DCOLOR_ARGB(255, 0, 255, 0));
}

void Slidebar::update(double dt)
{
	if(gDInput->mouseButtonDown(LEFTBUTTON))
	{
		if(gDInput->cursorInsideRect(mSlider) || mMovingSlider)
		{
			if(gDInput->mouseDX() < 0)
			{
				if(mSlider.left > mSlideBackground.left)	{
					mSlider.left += gDInput->mouseDX();
					mSlider.right += gDInput->mouseDX();
					mSliderPos += gDInput->mouseDX();
					mValue =  mMaxValue*((mSliderPos - mSliderWidth/2) / (mSlideBackground.right - mSlideBackground.left));

					if(mSlider.left <= mSlideBackground.left)	{
						mSliderPos = 0;
						mValue = 0;
						gDInput->setCursorX(gDInput->getCursorX() - gDInput->mouseDX()/2);
					}
					mInputBox->setValue(mValue);
				}
				else	{
					mSlider.left = mSlideBackground.left;
					mSlider.right = mSlideBackground.left + mSliderWidth;
					gDInput->setCursorX(gDInput->getCursorX() - gDInput->mouseDX());
					mSliderPos = 0;
					mValue = 0;
				}
			}
			else if(gDInput->mouseDX() > 0)
			{
				if(mSlider.right < mSlideBackground.right)	{
					mSlider.left += gDInput->mouseDX();
					mSlider.right += gDInput->mouseDX();
					mSliderPos += gDInput->mouseDX();
					mValue =  mMaxValue*((mSliderPos + mSliderWidth/2) / (mSlideBackground.right - mSlideBackground.left));

					if(mSlider.right >= mSlideBackground.right)	{
						mSliderPos = mSlideBackground.right - mSlideBackground.left;
						mValue = mMaxValue;
						gDInput->setCursorX(gDInput->getCursorX() - gDInput->mouseDX()/2);
					}
					mInputBox->setValue(mValue);
				}
				else	{
					mSlider.left = mSlideBackground.right - mSliderWidth;
					mSlider.right = mSlideBackground.right;
					gDInput->setCursorX(gDInput->getCursorX() - gDInput->mouseDX());
					mSliderPos = mSlideBackground.right - mSlideBackground.left;
					mValue = mMaxValue;
				}
			}

			gDInput->setCursorY(gDInput->getCursorY() - gDInput->mouseDY());
		}
	}

	if(gDInput->mouseButtonReleased(LEFTBUTTON))
		mMovingSlider = false;
}

bool Slidebar::pressed(int mx, int my)
{
	if(gDInput->cursorInsideRect(mSlider))
		mMovingSlider = true;

	return true;
}