#include "WindowHandler.h"
#include "Mouse.h"
#include "CameraManager.h"

extern CameraManager* gCameraManager;
extern Mouse* gMouse;

WindowHandler::WindowHandler()		
{
	mX = 0;
	mY = 0;
	mWidth = 1400;	// screen width
	mHeight = 900;	// screen height

	mActiveWindow = NULL;
	mNextPrimaryID = 0;
}

WindowHandler::WindowHandler(int x, int y, int width, int height)
{
	mX = x;
	mY = y;
	mWidth = width;
	mHeight = height;

	mActiveWindow = NULL;
	mNextPrimaryID = 0;
}
	
WindowHandler::~WindowHandler()
{
	for(int i = 0;i<mWindowList.size();i++)
	{
		delete mWindowList[i];	
	}
	mWindowList.clear();
}


void WindowHandler::addWindow(Window* window)
{
	if(window != NULL)	{
		window->setPrimaryID(mNextPrimaryID);
		mWindowList.push_back(window);
	}
	mNextPrimaryID++;
}

void WindowHandler::removeWindow(Window* window)
{
	int i = 0;
	vector<Window*>::iterator itr =  mWindowList.begin();
	while(itr != mWindowList.end() && i < mWindowList.size())
	{
		if(mWindowList[i]->getPrimaryID() == window->getPrimaryID())
		{
			delete mWindowList[i];
			itr = mWindowList.erase(itr);

			// set the active window to NULL if it was erased
			if(mActiveWindow != NULL)	{
				if(mActiveWindow->getPrimaryID() == window->getPrimaryID())
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


void WindowHandler::update(double dt)
{
	/* update the active window */ 
	if(mActiveWindow != NULL)
		mActiveWindow->update(dt);

	RECT winRect;

	// find out which window was pressed
	if(gDInput->mouseButtonPressed(LEFTBUTTON))
	{
		bool insideAny = false;

		/* loop through the window list*/
		for(int i = 0; i < mWindowList.size(); i++)
		{
			winRect = mWindowList[i]->getRect();

			/* a window was pressed */
			if(gMouse->insideWindow(winRect))	
			{
				/* there is a active window*/
				if(mActiveWindow != NULL)
				{					
					/* the window that was pressed was the active one, call the press() function */
					if(mActiveWindow->getPrimaryID() == mWindowList[i]->getPrimaryID())	{
						mWindowList[i]->pressed(gMouse->getScreenPos().x, gMouse->getScreenPos().y);
					}
					/* not the active one and the mouse isn't inside the active window*/
					else if(!gMouse->insideWindow(mActiveWindow->getRect()))	{
						mActiveWindow->setActive(false);	// deactivate
						mActiveWindow = mWindowList[i];		// point at the new active window
						mActiveWindow->setActive(true);		// activate it
						mWindowList[i]->pressed(gMouse->getScreenPos().x, gMouse->getScreenPos().y);	// send press
					}
				}
				/* the active window was NULL */
				else	
				{
					mActiveWindow = mWindowList[i];
					mActiveWindow->setActive(true);
					mWindowList[i]->pressed(gMouse->getScreenPos().x, gMouse->getScreenPos().y); 
				}
				
				/* indicates that some window was pressed*/
				insideAny = true;
			}
		}

		/* the mouse press wasn't inside any window, deactivate the active one! */
		if(!insideAny)
			if(mActiveWindow != NULL)	{
				mActiveWindow->setActive(false);
				mActiveWindow = NULL;
			}
	}
	/* no mouse press, only which window that needs to call hoover() */
	else
	{
		/* loop through window list */
		for(int i = 0; i < mWindowList.size(); i++)
		{
			winRect = mWindowList[i]->getRect();

			/* an active window */
			if(mActiveWindow != NULL)
			{
				/* inside a window but not inside the active one */
				if(gMouse->insideWindow(winRect) && !gMouse->insideWindow(mActiveWindow->getRect()) && mActiveWindow->getPrimaryID() != mWindowList[i]->getPrimaryID())
					mWindowList[i]->hoover(gMouse->getScreenPos().x , gMouse->getScreenPos().y);
				/* inside the active window */
				else if(mActiveWindow->getPrimaryID() == mWindowList[i]->getPrimaryID() && gMouse->insideWindow(winRect))
					mWindowList[i]->hoover(gMouse->getScreenPos().x , gMouse->getScreenPos().y);
			}
			/* no active window */
			else	
			{
				/* inside a window */
				if(gMouse->insideWindow(winRect))	
				{			
					mWindowList[i]->hoover(gMouse->getScreenPos().x , gMouse->getScreenPos().y);
				}
			}
		}
	}
}

void WindowHandler::draw(void)
{	
	RECT activeRect, iterRect;
	if(mActiveWindow != NULL)	
		activeRect = mActiveWindow->getRect();

	for(int i = 0; i < mWindowList.size(); i++)
	{
		iterRect = mWindowList[i]->getRect();

		if(mActiveWindow != NULL && mActiveWindow->getPrimaryID() != mWindowList[i]->getPrimaryID())	{
			if(activeRect.left < iterRect.right && activeRect.right > iterRect.left && activeRect.top < iterRect.bottom && activeRect.bottom > iterRect.top)	{
				mWindowList[i]->overlaped(true);
			}
		}	
		mWindowList[i]->draw();
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