#include "Button.h"
#include <string>
#include "Window.h"
#include <boost\function.hpp>
#include <boost\bind.hpp>
#include "Sound.h"
#include "C:\Program Files (x86)\irrKlang-1.3.0\include\irrKlang.h"


extern Sound* gSound;
extern Mouse* gMouse;

Button::Button(WindowHandler *handler, WindowID id, string display, int x, int y, int width, int height, D3DCOLOR color)
			:Window(handler, id, x, y, width, height, color)
{
	mDisplayText = display;
	mMouseOver = false;
	mFont = true;

	mNormalTexture = NULL;
	mHooverTexture = NULL;
	mSoundSource = "none";
}

Button::Button(WindowHandler *handler, WindowID id, string display, int x, int y, int width, int height, bool b, char* normalTexture, char* hooverTexture, bool font, D3DCOLOR color)
	:Window(handler, id, x, y, width, height, color)	
{
	mDisplayText = display;
	mMouseOver = false;
	mFont = font;

	mNormalTexture = gGraphics->loadTexture(normalTexture);
	mHooverTexture = gGraphics->loadTexture(hooverTexture);
}

Button::~Button()
{
	// dtor
	ReleaseCOM(mNormalTexture);
	ReleaseCOM(mHooverTexture);
	callback = NULL;
}

void Button::update(float dt)
{
	// cursor inside? then show hoover fx
	/*int mx = gMouse->getScreenPos().x;
	int my = gMouse->getScreenPos().y;
	RECT bRect = getRect();

	if(mx > bRect.left && mx < bRect.right && my > bRect.top && mx < bRect.bottom)
		mMouseOver = true;
	else
		mMouseOver = false;*/
}

bool Button::pressed(int mx, int my)
{
	if(callback != NULL)	{
		if(mSoundSource != "none")	
			gSound->mEngine->play2D(mSoundSource.c_str());
		
		if(!callback(getID(), getValue()))
			return false;
	}
	return true;
}
void Button::hoover(int mx, int my)
{
	mMouseOver = true;
}

void Button::draw(void)
{
	if(getVisible())
	{
		// draw the bkgd of button
		if(mMouseOver)	{
			if(mNormalTexture == NULL)
				gGraphics->BlitRect(mX, mY, mWidth, mHeight, D3DCOLOR_ARGB(255, 255, 166, 0));
			else	{
				gGraphics->BlitTexture(mHooverTexture, getRect());

			}

			// so it wont be true forever
			mMouseOver = false;
		}
		else	{
			if(mNormalTexture == NULL)
				gGraphics->BlitRect(mX, mY, mWidth, mHeight, mColor);
			else
				gGraphics->BlitTexture(mNormalTexture, getRect());
		}

		// draw the font 
		if(mFont && !getOverlap())	{
			strcpy(buffer, mDisplayText.c_str());
			gGraphics->drawText(buffer, mX-mWidth/2+5, mY-mHeight/2, D3DCOLOR_ARGB(255,0,0,0));
		}
		
		overlaped(false);
	}
}

void Button::setPressSound(string source)
{
	mSoundSource = source;
}