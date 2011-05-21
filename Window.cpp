#include "Window.h"
#include "Graphics.h"
#include "WindowHandler.h"
#include "Functors.h"

Window::Window(WindowHandler *handler, WindowID id, int x, int y, int width, int height, D3DCOLOR color)
{		
	handler->addWindow(this);

	mID = id;

	mX = handler->getRect().left + x;
	mY = handler->getRect().top + y;
	
	mWidth = width;
	mHeight = height;

	mColor = color;
	mActive = false;
	mInputState = true;
	mVisible = true;
	mOverlaped = false;
}

Window::~Window()
{
	// dtor
}

RECT Window::getRect(void)
{
	RECT rect;

	rect.left = mX - mWidth/2;
	rect.right = mX + mWidth/2;
	rect.top = mY - mHeight/2;
	rect.bottom = mY + mHeight/2;

	return rect;
}

void Window::onDeactive(void)
{
	// to be overwritten
}

