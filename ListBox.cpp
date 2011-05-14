#include "ListBox.h"



ListBox::ListBox(WindowHandler* handler, WindowID id, int x, int y, int width, int height, D3DCOLOR color)
				: Window(handler, id, x, y, width, height, color)
{
	mItems = 0;
	mMouseOver = false;
}

ListBox::~ListBox()
{
	// dtor
}

bool ListBox::pressed(int mx, int my)
{
	for(int i = 0; i<mItemList.size();i++)
	{
		if(mx > mItemList[i].getRect().left && mx < mItemList[i].getRect().right && my > mItemList[i].getRect().top && my < mItemList[i].getRect().bottom)
		{		
			mValue = mItemList[i].itemName;			// buttons uses this information!
			break;
		}
		else
			mValue = "none";	// innanför boxen, men inte på ett item
	}

	return true;
}
	
void ListBox::hoover(int mx, int my)
{
	mMouseOver = true;

	for(int i = 0; i < mItems; i++)
	{
		if(gDInput->cursorInsideRect(mItemList[i].getRect()))
			mHooverRect = mItemList[i].getRect();
	}
}

void ListBox::draw(void)
{
	// borde egentligen inta ligga här
	// men update körs bara om det är det aktiva, ska det verkligen vara så?:S Ja - tror det
	//if(!mActive)
	//	mValue = "none";

	// draw the bkgd, unnessessary!
	//gGraphics->BlitRect(mX, mY, mWidth, mHeight, D3DCOLOR_ARGB (255, 30, 200, 150));
	
	// draw items
	for(int i = 0; i<mItemList.size();i++)
	{
		if(mItemList[i].itemName == mValue && mActive)
			gGraphics->BlitRect(mItemList[i].x, mItemList[i].y, mItemList[i].width, mItemList[i].height, D3DCOLOR_ARGB(255, 255, 166, 0));
		else
		{ 
			gGraphics->BlitRect(mItemList[i].x, mItemList[i].y, mItemList[i].width, mItemList[i].height, mItemList[i].color);
			if(mMouseOver)
				gGraphics->BlitRect(mHooverRect, D3DCOLOR_ARGB(255, 255, 166, 255));		
		}

		strcpy(buffer, mItemList[i].itemName.c_str());
		gGraphics->drawText(buffer, mItemList[i].x - mItemList[i].width/2, mItemList[i].y - mItemList[i].height/2);
	}

	mMouseOver = false;
}
void ListBox::addItem(string name, int height, D3DCOLOR color)
{
	ListItem tmpItem;
	tmpItem.itemName = name;
	
	if(mItems == 0)
		tmpItem.y = mY - (mHeight/2) + height/2;//mPosition.top + height/2;
	else
		tmpItem.y = mY - (mHeight/2) + height/2 + mItems *height;
	
	tmpItem.x = mX;
	tmpItem.width = mWidth;
	tmpItem.height = height;
	
	tmpItem.color = color;

	mItems++;
	mItemList.push_back(tmpItem);
}

void ListBox::move(int dx, int dy)
{
	Window::move(dx, dy);

	for(int i = 0; i<mItemList.size();i++)
	{
		mItemList[i].x += dx;
		mItemList[i].y += dy;
	}
}