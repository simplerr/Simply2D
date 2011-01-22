#include "DropBox.h"

DropBox::DropBox(WindowID id, int x, int y, int width, int height, int itemHeight, D3DCOLOR color)
	: Window(id, x, y, width, height, color)
{
	mItems = 0;
	mValue = "none";
	mExpanded = false;
	mItemHeight = itemHeight;
	mSignSide = 20;
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
	// ta reda på om boxen är expandad
	// trycker man innanför activeItem.RECT
	//MessageBox(0, "msg!", 0, 0);
	if(!mExpanded)
	{
		if(x > mActivationRect.left && x < (mActivationRect.right + mSignSide) && y > mActivationRect.top && y < mActivationRect.bottom)
		{
			// expandera drop boxen
			mExpanded = true;
			mPosition.bottom += (mItems * mItemHeight) - mItemHeight;
		}
	}
	else if(mExpanded)
	{
		// om man trycker på sidesign 
		if(x > mActivationRect.right && x < (mActivationRect.right + mSignSide) && y > mActivationRect.top && y <(mActivationRect.top + mSignSide))
		{
			mExpanded = false;
			mPosition.bottom = mActivationRect.bottom;
		}
		for(int i = 0; i<mItemList.size();i++)
		{
			if(x > mItemList[i].rect.left && x < mItemList[i].rect.right && y > mItemList[i].rect.top && y < mItemList[i].rect.bottom)
			{	
				mPosition.bottom = mActivationRect.bottom;
				mExpanded = false;
				mValue = mItemList[i].itemName;	
				mParent->messageHandler(getID(), mValue);
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
	if(!mActive)	{
		mExpanded = false;
		mPosition.bottom = mActivationRect.bottom;	
	}
	// sign pilen
	gGraphics->BlitRect(mActivationRect.right + mSignSide/2, mActivationRect.top + mSignSide/2, mSignSide, mSignSide, D3DCOLOR_ARGB(255, 255, 166, 0));
	gGraphics->drawText("V", mActivationRect.right + 4, mActivationRect.top + 2);

	if(!mExpanded)
	{	
		// bakgrund + text för mValue
		gGraphics->BlitRect(mActivationRect, D3DCOLOR_ARGB(255, 70, 166, 150));
		string tmpValue = getOwnValue();
		strcpy(buffer, tmpValue.c_str());
		//WindowRect tmpRect = getInfo();
		//gGraphics->drawText(buffer, tmpRect.x - mPosition.width/2, tmpRect.y - mPosition.height/2);
		gGraphics->drawText(buffer, mActivationRect.left, mActivationRect.top);
	}
	else if(mExpanded)
	{
		for(int i = 0; i<mItemList.size();i++)
		{
			gGraphics->BlitRect(mItemList[i].x, mItemList[i].y, mItemList[i].width, mItemList[i].height, mItemList[i].color);
			
			strcpy(buffer, mItemList[i].itemName.c_str());
			gGraphics->drawText(buffer, mItemList[i].x - mItemList[i].width/2, mItemList[i].y - mItemList[i].height/2);
		}
	}
	return 1;
}

void DropBox::addItem(string name, D3DCOLOR color)
{
	ListItem tmpItem;
	tmpItem.itemName = name;
	
	if(mItems == 0)
		tmpItem.y = mPosition.top + mItemHeight/2;
	else
		tmpItem.y = mPosition.top + mItemHeight/2 + mItems *mItemHeight;
	
	tmpItem.x = mPosition.x;
	tmpItem.width = mPosition.width;
	tmpItem.height = mItemHeight;
	
	tmpItem.rect.left = tmpItem.x - tmpItem.width/2;
	tmpItem.rect.right = tmpItem.x + tmpItem.width/2;
	tmpItem.rect.top = tmpItem.y - tmpItem.height/2;
	tmpItem.rect.bottom = tmpItem.y + tmpItem.height/2;
	tmpItem.color = color;

	mItems++;
	mItemList.push_back(tmpItem);
}

void DropBox::updateRectToNewXY(void)
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
}