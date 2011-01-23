#include "DropBox.h"

DropBox::DropBox(Window *parent, WindowID id, int x, int y, int width, int height, int itemHeight, D3DCOLOR color)
	: Window(parent, id, x, y, width, height, color)
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

void DropBox::updateWindow(float dt)
{
	// nothing to do
}
int DropBox::wm_lbuttondown(int x, int y)
{
	RECT tmpRect = getRect();
	// ta reda på om boxen är expandad
	// trycker man innanför activeItem.RECT
	if(!mExpanded)
	{
		if((x > mActivationRect.left && x < (mActivationRect.right + mSignSide) && y > mActivationRect.top && y < mActivationRect.bottom))
		{
			
			// expandera drop boxen
			mExpanded = true;
			mY += ((mItems * mItemHeight) - mItemHeight);
			mHeight += (mItems * mItemHeight) - mItemHeight;
		}
	}
	else if(mExpanded)
	{
		// om man trycker på sidesign 
		if(x > mActivationRect.right && x < (mActivationRect.right + mSignSide) && y > mActivationRect.top && y <(mActivationRect.top + mSignSide))
		{
			mExpanded = false;
			mY = mActivationRect.top + (mActivationRect.bottom - mActivationRect.top)/2;
			mHeight -= (mItems * mItemHeight) - mItemHeight;
		}
		for(int i = 0; i<mItemList.size();i++)
		{
			if(x > mItemList[i].getRect().left && x < mItemList[i].getRect().right && y > mItemList[i].getRect().top && y < mItemList[i].getRect().bottom)
			{	
				mY = mActivationRect.top + (mActivationRect.bottom - mActivationRect.top)/2;// mPosition.bottom = mActivationRect.bottom;
				mHeight -= (mItems * mItemHeight) - mItemHeight;
				mExpanded = false;
				mValue = mItemList[i].itemName;	
				mParent->messageHandler(mID, mValue);			
			}
		}
		
	}
	return 1;
}
int DropBox::wm_keydown(WPARAM wParam)
{
	// onödigt, rensa upp sen!
	return 1;
}
int DropBox::renderAll(void)
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
	return 1;
}

void DropBox::addItem(string name, D3DCOLOR color)
{
	ListItem tmpItem;
	tmpItem.itemName = name;
	
	//if(mItems == 0)
	//	tmpItem.y = mY - (mHeight/2) + mItemHeight/2;//mPosition.top + height/2;
	//else
	//	tmpItem.y = mY - (mHeight/2) + mItemHeight/2 + mItems *mItemHeight;

	if(mItems == 0)
		tmpItem.y = mY + mItemHeight;// - (mHeight/2) + mItemHeight/2;
	else	
		tmpItem.y =  mY + mItemHeight + mItems *mItemHeight;
	
	
	
	tmpItem.x = mX;
	tmpItem.width = mActivationRect.right - mActivationRect.left;//mWidth;
	tmpItem.height = mItemHeight;
	
	/*tmpItem.rect.left = tmpItem.x - tmpItem.width/2;
	tmpItem.rect.right = tmpItem.x + tmpItem.width/2;
	tmpItem.rect.top = tmpItem.y - tmpItem.height/2;
	tmpItem.rect.bottom = tmpItem.y + tmpItem.height/2;*/
	tmpItem.color = color;

	//mY += mItemHeight/2;
	//mHeight += mItemHeight;

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

void DropBox::onDeactive(void)
{
	mExpanded = false;
	mY = mActivationRect.top + (mActivationRect.bottom - mActivationRect.top)/2;
	mHeight -= (mItems * mItemHeight) - mItemHeight;
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