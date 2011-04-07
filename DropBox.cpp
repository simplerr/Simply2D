#include "DropBox.h"

extern Mouse* gMouse;

DropBox::DropBox(WindowHandler* handler, WindowID id, int x, int y, int width, int height, int itemHeight, D3DCOLOR color)
	: Window(handler, id, x, y, width, height, color)
{
	mItems = 0;
	mExpanded = false;
	mItemHeight = itemHeight;
	mSignSide = 20;

	mActivationRect.left = mX - mWidth/2;
	mActivationRect.right = mX + mWidth/2 - mSignSide;
	mActivationRect.top = mY - mHeight/2;
	mActivationRect.bottom = mY + mHeight/2;
}

DropBox::~DropBox()
{
	// dtor
}

void DropBox::update(float dt)
{
	// nothing to do
}

void DropBox::pressed(int mx, int my)
{
	mx = gMouse->getPos().x;
	my = gMouse->getPos().y;
	// expanded?
	if(!mExpanded)
	{
		if((mx > mActivationRect.left && mx < (mActivationRect.right + mSignSide) && my > mActivationRect.top && my < mActivationRect.bottom))
		{			
			// expand it
			mExpanded = true;
			mY += ((mItems * mItemHeight) - mItemHeight);
			mHeight += (mItems * mItemHeight) ;
		}
	}
	else if(mExpanded)
	{
		// if the arrow gets pressed
		if(mx > mActivationRect.left && mx < (mActivationRect.right + mSignSide) && my > mActivationRect.top && my < mActivationRect.bottom)
		{
			mExpanded = false;
			mY = mActivationRect.top + (mActivationRect.bottom - mActivationRect.top)/2;
			mHeight -= (mItems * mItemHeight);
		}
		for(int i = 0; i<mItemList.size();i++)
		{
			if(mx > mItemList[i].getRect().left && mx < mItemList[i].getRect().right && my > mItemList[i].getRect().top && my < mItemList[i].getRect().bottom)
			{	
				mY = mActivationRect.top + (mActivationRect.bottom - mActivationRect.top)/2;
				mHeight -= (mItems * mItemHeight) ;
				mExpanded = false;
				mValue = mItemList[i].itemName;	

				// do what's needed
				callback(getID(), getValue());		
			}
		}
		
	}
}
	
void DropBox::hoover(int mx, int my)
{
	//mMouseOver = true;
}

void DropBox::draw(void)
{
	// sign pilen
	gGraphics->BlitRect(mActivationRect.right + mSignSide/2, mActivationRect.top + mSignSide/2, mSignSide, mSignSide, D3DCOLOR_ARGB(255, 255, 166, 0));
	gGraphics->drawText("V", mActivationRect.right + 4, mActivationRect.top + 2);

	gGraphics->BlitRect(mActivationRect, D3DCOLOR_ARGB(255, 255, 255, 0));
	string tmpValue = getValue();
	strcpy(buffer, tmpValue.c_str());
	gGraphics->drawText(buffer, mActivationRect.left, mActivationRect.top);
	
	if(mExpanded)
	{
		for(int i = 0; i<mItemList.size();i++)
		{
			gGraphics->BlitRect(mItemList[i].x-mSignSide/2, mItemList[i].y, mItemList[i].width, mItemList[i].height, mItemList[i].color);
			
			strcpy(buffer, mItemList[i].itemName.c_str());
			gGraphics->drawText(buffer, mItemList[i].x - mItemList[i].width/2 - mSignSide/2, mItemList[i].y - mItemList[i].height/2);
		}
	}
}

void DropBox::addItem(string name, D3DCOLOR color)
{
	ListItem tmpItem;
	tmpItem.itemName = name;

	if(mItems == 0)
		tmpItem.y = mY + mItemHeight;
	else	
		tmpItem.y =  mY + mItemHeight + mItems *mItemHeight;	
	
	tmpItem.x = mX;
	tmpItem.width = mActivationRect.right - mActivationRect.left;
	tmpItem.height = mItemHeight;

	tmpItem.color = color;

	mItems++;
	mItemList.push_back(tmpItem);
}

void DropBox::setPos(int x, int y)
{
	Window::setPos(x, y);
	RECT tmpRect = getRect();

	mActivationRect = tmpRect;
}

void DropBox::move(int dx, int dy)
{
	Window::move(dx, dy);
	RECT tmpRect = getRect();

	mActivationRect = tmpRect;

	for(int i = 0; i<mItemList.size();i++)
	{
		mItemList[i].x += dx;
		mItemList[i].y += dy;
	}
}

void DropBox::setActive(bool b)
{
	Window::setActive(b);

	if(!b)	{
		if(mExpanded)	{
			mExpanded = false;
			mY = mActivationRect.top + (mActivationRect.bottom - mActivationRect.top)/2;
			mHeight -= (mItems * mItemHeight);
		}
	}
}
/*void DropBox::fixSize(void)
{
	mPosition.left = mPosition.x-(mPosition.width/2);
	mPosition.right = mPosition.x+(mPosition.width/2) + mSignSide;
	mPosition.top = mPosition.y-(mPosition.height/2);
	mPosition.bottom = mPosition.y+(mPosition.height/2);
	//mPosition.width
	//mPosition.x += mSignSide;

	mActivationRect.left = mPosition.left;
	mActivationRect.right = mPosition.right - mSignSide;//+ mSignSide;
	mActivationRect.top = mPosition.top;
	mActivationRect.bottom = mPosition.bottom;

	mRightSign.left = mActivationRect.right;
	mRightSign.right = mActivationRect.right;// + mSignSide;
	mRightSign.top = mActivationRect.top;
	mRightSign.bottom = mActivationRect.bottom;
}*/