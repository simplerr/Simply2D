#include "WindowHandler.h"
#include "Mouse.h"

extern Mouse* gMouse;

WindowHandler::WindowHandler()		
{
	mX = 0;
	mY = 0;
	mWidth = 1400;	// screen width
	mHeight = 900;	// screen height

	mActiveWindow = NULL;
}

WindowHandler::WindowHandler(int x, int y, int width, int height)
{
	mX = x;
	mY = y;
	mWidth = width;
	mHeight = height;

	mActiveWindow = NULL;
}
	
WindowHandler::~WindowHandler()
{
	for(int i = 0;i<mWindowList.size();i++)
	{
		delete mWindowList[i];	
	}
	mWindowList.clear();
}

void WindowHandler::update(double dt)
{
	// update the active window
	if(mActiveWindow != NULL)
		mActiveWindow->update(dt);

	RECT winRect;

	// find out which window was pressed
	if(gDInput->mouseButtonPressed(LEFTBUTTON))
	{
		bool insideAny = false;

		for(int i = 0; i < mWindowList.size(); i++)
		{
			winRect = mWindowList[i]->getRect();

			if(gMouse->inside(winRect))	
			{
			
				// old active wont be active anymore
				if(mActiveWindow != NULL)
					mActiveWindow->setActive(false);

				mActiveWindow = mWindowList[i];
				mActiveWindow->setActive(true);
				mWindowList[i]->pressed();

				insideAny = true;
			}
		}

		if(!insideAny)
			if(mActiveWindow != NULL)	{
				mActiveWindow->setActive(false);
				mActiveWindow = NULL;
			}
	}
	else	// just check for hoover
	{
		for(int i = 0; i < mWindowList.size(); i++)
		{
			winRect = mWindowList[i]->getRect();

			if(gMouse->inside(winRect))	
			{			
				mWindowList[i]->hoover();
			}
		}
	}
}

void WindowHandler::draw(void)
{	
	for(int i = 0; i < mWindowList.size(); i++)
	{
		mWindowList[i]->draw();
	}

	// perhaps draw a bkgd?
}

void WindowHandler::addWindow(Window* window)
{
	if(window != NULL)	{
		mWindowList.push_back(window);
	}
}

void WindowHandler::removeWindow(Window* window)
{
	int i = 0;
	vector<Window*>::iterator itr =  mWindowList.begin();
	while(itr != mWindowList.end() && i < mWindowList.size())
	{
		if(mWindowList[i]->getID() == window->getID())
		{
			delete mWindowList[i];
			itr = mWindowList.erase(itr);

			// set the active window to NULL if it was erased
			if(mActiveWindow != NULL)	{
				if(mActiveWindow->getID() == window->getID())
					mActiveWindow = NULL;
			}
			break;
		}
		else	{
			itr++;
			i++;
		}
	}
}

RECT WindowHandler::getRect(void)
{
	RECT rect;

	rect.left = mX - mWidth/2;
	rect.right = mX + mWidth/2;
	rect.top = mY - mHeight/2;
	rect.bottom = mY + mHeight/2;

	return rect;
}

// hack warning, but it does the job
void WindowHandler::keyPressed(WPARAM wParam)
{
	if(mActiveWindow != NULL)
		mActiveWindow->wm_keydown(wParam);
}