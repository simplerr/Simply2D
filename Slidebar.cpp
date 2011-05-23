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
	
	// no invalid values
	if(value > mMaxValue)
		mValue = mMaxValue;
	else if(value < mMinValue)
		mValue = mMinValue;
	else
		mValue = value;

	// coordinates of the slider background
	mSlideBackground.left = getX() - 80;
	mSlideBackground.right = getX() + 80;
	mSlideBackground.top = getY() - 10;
	mSlideBackground.bottom = getY() + 10;

	// window controls
	mTextName = new TextBox(handler, SLIDER_TEXT, display, mSlideBackground.left + 60 - handler->getRect().left,
		getY()- handler->getRect().top-25, 120, 20);
	mInputBox = new InputBox(handler, SLIDER_INPUT, TYPE_TEXT, mSlideBackground.right + 35 - handler->getRect().left,
		getY()- handler->getRect().top, 50, 20, 4);

	// sliders position, X% of the span * slidewidth + offset
	mSliderPos = float((mValue / (maxValue - minValue)) * (mSlideBackground.right - mSlideBackground.left - mSliderWidth) + mSliderWidth/2);

	// substract the minValue, 100-200 range is 0-100 on the slidebar
	mSliderPos -= (minValue / (maxValue - minValue)) * (mSlideBackground.right - mSlideBackground.left - mSliderWidth);

	// sliderects coordinates
	mSlider.left = mSlideBackground.left + mSliderPos - mSliderWidth/2;
	mSlider.right = mSlideBackground.left + mSliderPos + mSliderWidth/2;
	mSlider.top = getY() - 10;
	mSlider.bottom = getY()  + 10;

	// the inputbox value, ie the sliders real value
	mInputBox->setValue(mValue);
}
	
Slidebar::~Slidebar()
{
	// dtor
}

void Slidebar::draw(void)
{
	// background of the slider
	gGraphics->BlitRect(mSlideBackground, D3DCOLOR_ARGB(255, 255, 0, 0));

	// the slider
	gGraphics->BlitRect(mSlider, D3DCOLOR_ARGB(255, 0, 255, 0));
}

void Slidebar::update(double dt)
{
	static char temp[10]; // ugly hack
	if(gDInput->mouseButtonDown(LEFTBUTTON))
	{
		if(gDInput->cursorInsideRect(mSlider) || mMovingSlider)
		{
			if(gDInput->mouseDX() < 0)
			{
				// slider has a valid position
				if(mSliderPos > 0 + mSliderWidth/2)	{
					moveSlider(gDInput->mouseDX());

					// set the value, dx in 0-1 range * value span
					mValue +=  (mMaxValue - mMinValue)*((gDInput->mouseDX()) / (mSlideBackground.right - mSlideBackground.left - mSliderWidth)); //  - mSliderWidth på vänster sidan?:O
					callback(getID(), WindowMessage(mValue));


					// stop invalid movement
					if(mSliderPos <= 0 + mSliderWidth/2)	{
						stopSlider(LEFT);
					}
					
					sprintf(temp, "%.2f", mValue);	// hack >_>
					string temp2 = string(temp);
					mInputBox->setValue(temp2);
				}
				else	{
					stopSlider(LEFT);
				}
			}
			else if(gDInput->mouseDX() > 0)
			{
				if(mSliderPos < (mSlideBackground.right - mSlideBackground.left - mSliderWidth/2))	{
					moveSlider(gDInput->mouseDX());

					// set the value, dx in 0-1 range * value span
					mValue +=  (mMaxValue - mMinValue)*((gDInput->mouseDX()) / (mSlideBackground.right - mSlideBackground.left - mSliderWidth));
					callback(getID(), WindowMessage(mValue));

					// stop invalid movement
					if(mSliderPos >= (mSlideBackground.right - mSlideBackground.left - mSliderWidth/2))	{
						stopSlider(RIGHT);
					}
					sprintf(temp, "%.2f", mValue);	// hack >_>
					string temp2 = string(temp);
					mInputBox->setValue(temp2);
				}
				else	{
					stopSlider(RIGHT);
				}
			}

			// dont allow Y movement while sliding
			gDInput->setCursorY(gDInput->getCursorY() - gDInput->mouseDY());
		}
	}

	if(gDInput->mouseButtonReleased(LEFTBUTTON))
		mMovingSlider = false;
}

// hack - needed?
bool Slidebar::pressed(int mx, int my)
{
	if(gDInput->cursorInsideRect(mSlider))
		mMovingSlider = true;

	return true;
}

void Slidebar::moveSlider(float dx)
{
	mSlider.left += dx;
	mSlider.right += dx;
	mSliderPos += dx;
}

void Slidebar::stopSlider(direction dir)
{
	if(dir == LEFT)
	{
		mSlider.left = mSlideBackground.left;
		mSlider.right = mSlideBackground.left + mSliderWidth;
		gDInput->setCursorX(gDInput->getCursorX() - gDInput->mouseDX());
		mSliderPos = mSliderWidth/2;
		mValue = mMinValue;
	}
	else if(dir == RIGHT)
	{
		mSlider.left = mSlideBackground.right - mSliderWidth;
		mSlider.right = mSlideBackground.right;
		gDInput->setCursorX(gDInput->getCursorX() - gDInput->mouseDX());
		mSliderPos = mSlideBackground.right - mSlideBackground.left - mSliderWidth/2;
		mValue = mMaxValue;
	}
}

void Slidebar::setValue(float value)
{
	char temp[10];

	mValue = value;

	sprintf(temp, "%.2f", mValue);	// hack >_>
	string temp2 = string(temp);
	mInputBox->setValue(temp2);

	// sliders position, X% of the span * slidewidth + offset
	mSliderPos = float((mValue / (mMaxValue - mMinValue)) * (mSlideBackground.right - mSlideBackground.left - mSliderWidth) + mSliderWidth/2);

	// substract the minValue, 100-200 range is 0-100 on the slidebar
	mSliderPos -= (mMinValue / (mMaxValue - mMinValue)) * (mSlideBackground.right - mSlideBackground.left - mSliderWidth);
	
	// sliderects coordinates
	mSlider.left = mSlideBackground.left + mSliderPos - mSliderWidth/2;
	mSlider.right = mSlideBackground.left + mSliderPos + mSliderWidth/2;
}