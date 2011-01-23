#include "ListBox.h"

ListBox::ListBox(Window *parent, WindowID id, int x, int y, int width, int height, D3DCOLOR color)
				: Window(parent, id, x, y, width, height, color)
{
	items = 0;
}

ListBox::~ListBox()
{
	// dtor
}
// körs bara om den är aktiv!
void ListBox::updateWindow(float dt)
{
	// har nog inget att göra
}

int ListBox::wm_lbuttondown(int x, int y) 
{
	// ta reda på vilken listItem som blev tryckt
	// spara dess namn i activeItemName
	char buffer[256];

	for(int i = 0; i<itemList.size();i++)
	{
		if(x > itemList[i].rect.left && x < itemList[i].rect.right && y > itemList[i].rect.top && y < itemList[i].rect.bottom)
		{		
			mValue = itemList[i].itemName;			// Button ska bara köra vald item!
			mParent->messageHandler(getID(), mValue);
			return 1;
		}
		else
			mValue = "none";	// innanför boxen, men inte på ett item
	}
	return 1;
}

int ListBox::wm_keydown(WPARAM wParam)
{
	// man ska kunna gå upp och ned
	// ska fungera som i menyn
	return 1;
}

int ListBox::renderAll(void)
{
	// borde egentligen inta ligga här
	// men updateWindow körs bara om det är det aktiva, ska det verkligen vara så?:S Ja - tror det
	if(!mActive)
		mValue = "none";

	// rita ut "bakgrunden"
	gGraphics->BlitRect(mX, mY, mWidth, mHeight, D3DCOLOR_ARGB (255, 30, 200, 150));
	// rita ut alla items
	for(int i = 0; i<itemList.size();i++)
	{
		if(itemList[i].itemName == mValue && mActive)
			gGraphics->BlitRect(itemList[i].x, itemList[i].y, itemList[i].width, itemList[i].height, D3DCOLOR_ARGB(255, 255, 166, 0));
		else
			gGraphics->BlitRect(itemList[i].x, itemList[i].y, itemList[i].width, itemList[i].height, itemList[i].color);
			
		//char tmp[256];
		strcpy(buffer, itemList[i].itemName.c_str());
		gGraphics->drawText(buffer, itemList[i].x - itemList[i].width/2, itemList[i].y - itemList[i].height/2);
	}

	return 1;
}
void ListBox::addItem(string name, int height, D3DCOLOR color)
{
	ListItem tmpItem;
	tmpItem.itemName = name;
	
	if(items == 0)
		tmpItem.y = mY - (mHeight/2) + height/2;//mPosition.top + height/2;
	else
		tmpItem.y = mY - (mHeight/2) + height/2 + items *height;
	
	tmpItem.x = mX;
	tmpItem.width = mWidth;
	tmpItem.height = height;
	
	tmpItem.rect.left = tmpItem.x - tmpItem.width/2;
	tmpItem.rect.right = tmpItem.x + tmpItem.width/2;
	tmpItem.rect.top = tmpItem.y - tmpItem.height/2;
	tmpItem.rect.bottom = tmpItem.y + tmpItem.height/2;
	tmpItem.color = color;

	items++;
	itemList.push_back(tmpItem);
}
