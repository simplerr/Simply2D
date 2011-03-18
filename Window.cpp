#include "Window.h"
#include "Graphics.h"

Window::Window(Window *parent, WindowID id, int x, int y, int width, int height, D3DCOLOR color)
{
	if(parent == NULL)	{	
		mParent = NULL;
		mX = x;
		mY = y;
		nextID = 0;
		primaryID = 0;
	}
	else	{
			
		mParent = parent;
		mParent->addWindow(this);//mSubWins.push_back(this);

		RECT parentRect = parent->getRect();
		mX = parentRect.left + x;
		mY = parentRect.top + y;
	}
	
	mWidth = width;
	mHeight = height;

	mColor = color;
	mID = id;
	mActiveWinID = NOT_SET;
	mActive = false;
	mActiveWin = this;
	mInputState = true;
	mVisible = true;
	mValue = "none";

	//sprintf(buffer, "value: %s, x: %i, y: %i, width: %i, height: %i", mValue.c_str(), mX, mY, mWidth, mHeight);
}

Window::~Window()
{
	// deletes
	for(int i = 0;i<mSubWins.size();i++)
	{
		delete mSubWins[i];	
	}
	mSubWins.clear();
}

int Window::addWindow(Window *w)
{
	w->setPrimaryID(primaryID);
	mSubWins.push_back(w);
	primaryID++;

	return 1;
}

int Window::removewindow(Window *w)
{
	// om det window man tar bort är det aktiva måste mActiveWin sättas till NULL!
	// om det window man tar bort är det aktiva måste mActiveWin sättas till NULL!

	//eh 
	int i = 0;
	vector<Window*>::iterator itr =  mSubWins.begin();
	while(itr != mSubWins.end() && i < mSubWins.size())
	{
		if(mSubWins[i]->getPrimaryID() == w->getPrimaryID())
		{
			delete mSubWins[i];		// viktigt att deleta innan!
			itr = mSubWins.erase(itr);
			// if the erased window was the active one
			if(mActiveWin != NULL)	{
				if(mActiveWin->getPrimaryID() == w->getPrimaryID())
					mActiveWin = NULL;
			}
			break;
		}
		else	{
			itr++;
			i++;
		}
	}

	return 1;
}

void Window::sendMousePress(int mx, int my)	// bra namn? har ju bara med musen att göra >_>
{
		// ta reda på vilket window som ska köra wm_lbuttondown()!
		Window *found;
		found = findChildAtCoord(mx, my);

		if(found != NULL)	{
			if(mActiveWin != NULL)
				mActiveWin->mActive = false;
			//mActiveWin->onDeactive();
			mActiveWin = found;
			mActiveWin->mActive = true;
			mActiveWin->wm_lbuttondown(mx, my);
			mActiveWinID = mActiveWin->getID();
		}
		else	{
			mActiveWinID = NOT_SET;
			mActiveWin->mActive = false;
			//mActiveWin->onDeactive();
		}
}

int Window::renderAll(void)
{
	gGraphics->BlitRect(mX, mY, mWidth, mHeight, mColor);

	for(int i = 0;i<mSubWins.size();i++)
	{
		mSubWins[i]->renderAll();
	}
	return 1;
}

// hur ett Baswindow hanterar ett vänsterklick!
int Window::wm_lbuttondown(int x, int y)
{
	// inget att göra just nu!

	return 1;
}
// kan tas bort!
int Window::wm_keydown(WPARAM wParam)
 {
	 return 1;
 }

/****************************************************************************
 
 findchildatcoord: returns the top-most child window at coord (x,y); 
 recursive.
 
 ****************************************************************************/
Window *Window::findChildAtCoord(int x, int y)
{
  int child = 0;
  Window *found = NULL;
  RECT tmpRect;

  for (int i = 0;i < mSubWins.size();i++)
  {
	  // kollar om windowet är synligt nu också!
	  tmpRect = mSubWins[i]->getRect();
	  if(x > tmpRect.left && x < tmpRect.right && y > tmpRect.top && y < tmpRect.bottom && mSubWins[i]->getVisible())	{
		    child = i;
			break; // viktigt med break, annars kommer child att skrivas över med -1
	  }
	  else
		  child = -1;
  }

  // ett child!
  if(child >= 0)	{
	  if(mSubWins[child]->windowCount() > 0)
		 found = mSubWins[child]->findChildAtCoord(x,y);
	  else	
		 found = mSubWins[child];			 	  
  }
  // inget child!
  else	{
	  // ett window
	  tmpRect = getRect();	  
	  if(x > tmpRect.left && x < tmpRect.right && y > tmpRect.top && y < tmpRect.bottom)	{
			found = this;
	  }
	  // else -> inget window
  }
  return found;
}

void Window::keyPressed(WPARAM wParam)
{
	if(mInputState)
		mActiveWin->wm_keydown(wParam);
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
