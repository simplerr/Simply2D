#include "Window.h"
#include "Graphics.h"

Window::Window(WindowID id, int x, int y, int width, int height, D3DCOLOR color)
{
	mPosition.x = x;
	mPosition.y = y;
	mPosition.width = width;
	mPosition.height = height;
	updateRectToNewXY();	// fixa top,left,right,bottom ..

	mColor = color;
	mID = id;
	mActiveWinID = NOT_SET;
	mActive = false;
	mActiveWin = this;
	inputState = true;
	mVisible = true;
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
	// bad ptr
	if(!w)	{
		MessageBox(0, "bad wnd ptr", 0, 0);	
		return -1;
	}
	w->mPosition.x = this->mPosition.left + w->mPosition.x;
	w->mPosition.y = this->mPosition.top + w->mPosition.y;
	w->updateRectToNewXY();
	w->setParent(this);

	mSubWins.push_back(w);

	// updatera childs?

	return 1;
}

// ska ta argument
// string msg
// #defines?
void Window::sendMousePress(int mx, int my)	// bra namn? har ju bara med musen att göra >_>
{
		// ta reda på vilket window som ska köra wm_lbuttondown()!
		Window *found;
		found = findChildAtCoord(mx, my);

		if(found != NULL)	{
			mActiveWin->mActive = false;
			mActiveWin = found;
			mActiveWin->mActive = true;
			mActiveWin->wm_lbuttondown(mx, my);
			mActiveWinID = mActiveWin->getID();
		}
		else	{
			mActiveWinID = NOT_SET;
			mActiveWin->mActive = false;
		}
}

int Window::renderAll(void)
{
	gGraphics->BlitRect(mPosition.x, mPosition.y, mPosition.width, mPosition.height, mColor);

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

void Window::updateRectToNewXY(void)
{
	mPosition.left = mPosition.x-(mPosition.width/2);
	mPosition.right = mPosition.x+(mPosition.width/2);
	mPosition.top = mPosition.y-(mPosition.height/2);
	mPosition.bottom = mPosition.y+(mPosition.height/2);
}

/****************************************************************************
 
 findchildatcoord: returns the top-most child window at coord (x,y); 
 recursive.
 
 ****************************************************************************/
Window *Window::findChildAtCoord(int x, int y)
{
  int child = 0;
  Window *found = NULL;

  for (int i = 0;i < mSubWins.size();i++)
  {
	  // kollar om windowet är synligt nu också!
	  if(x > mSubWins[i]->mPosition.left && x < mSubWins[i]->mPosition.right && y > mSubWins[i]->mPosition.top && y < mSubWins[i]->mPosition.bottom && mSubWins[i]->mVisible)	{
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
	  if(x > mPosition.left && x < mPosition.right && y > mPosition.top && y < mPosition.bottom)	{
			found = this;
	  }
	  // else -> inget window
  }
  return found;
}

void Window::keyPressed(WPARAM wParam)
{
	if(inputState)
		mActiveWin->wm_keydown(wParam);
}

string Window::getValue(WindowID id)
 {
	for (int i = 0;i < mSubWins.size();i++)
	{
		if(mSubWins[i]->getID() == id)		// kanske strcmp() istället?
			return mSubWins[i]->mValue;
	}

	return "nothing";
 }
void Window::setValue(WindowID id, string value)
 {
	for (int i = 0;i < mSubWins.size();i++)
	{		
		
		if(mSubWins[i]->getID() == id)	// getID();
		{
			mSubWins[i]->mValue = value;
		}
	}
 }

void Window::setVisibility(WindowID id, bool value)
{
	for (int i = 0;i < mSubWins.size();i++)
	{
		if(mSubWins[i]->getID() == id)
			mSubWins[i]->mVisible = value;
	}
}

RECT Window::getRect(void)
{
	RECT tmpRect;
	tmpRect.left = mPosition.left;
	tmpRect.right = mPosition.right;
	tmpRect.top = mPosition.top;
	tmpRect.bottom = mPosition.bottom;

	return tmpRect;
}

void Window::updateRect(void)
{
	mPosition.left = mPosition.x-(mPosition.width/2);
	mPosition.right = mPosition.x+(mPosition.width/2);
	mPosition.top = mPosition.y-(mPosition.height/2);
	mPosition.bottom = mPosition.y+(mPosition.height/2);
}

