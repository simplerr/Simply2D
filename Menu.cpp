#include "Menu.h"
#include "Mouse.h"
#include "Sound.h"

extern Mouse* gMouse;
extern Sound* gSound;

Menu::Menu(std::string menuName, navigationType a_navigation, bool useFonts, int itemAmount, int a_spacing)
{
	mName = menuName;
	
	// mellanrummet mellan menuitems
	spacing = a_spacing;
	navigation = a_navigation;

	numbersOfItems = itemAmount;
	mUseFonts = useFonts;
}

Menu::Menu(std::string menuName, navigationType a_navigation, MenuType type, int breakCount, bool useFonts, int itemAmount, int a_spacing)
{
	mName = menuName;
	spacing = a_spacing;
	navigation = a_navigation;
	numbersOfItems = itemAmount;
	mUseFonts = useFonts;
	mType = type;
	mBreakCount = breakCount;
}

Menu::~Menu()
{
	ReleaseCOM(backgroundTexture);
	
	std::list<MenuItem>::iterator i = mMenuItemList.begin();
	while( i != mMenuItemList.end())
	{	
		ReleaseCOM(i->standardTexture);
		ReleaseCOM(i->onSelectTexture);
		i++;
	}
	// dtor
}

void Menu::setMenuBackground(char *textureSource, int x, int y, int width, int height)
{
	backgroundTexture = gGraphics->loadTexture(textureSource);

	backgroundRect.left = x-(width/2);
	backgroundRect.right = x+(width/2);
	backgroundRect.top = y-(height/2);
	backgroundRect.bottom = y+(height/2);

	bkgdX = x;
	bkgdY = y;

	idCounter = 0;
}

void Menu::addMenuItem(MenuItem newItem)
{
	mMenuItemList.push_back(newItem);
}

void Menu::addMenuItem(std::string itemName, char *textureSource)
{
	static int newID = 0;
	MenuItem tempMenuItem;	
	tempMenuItem.itemName = itemName;
	tempMenuItem.state = STANDARD;
	tempMenuItem.standardTexture = gGraphics->loadTexture(textureSource);
	tempMenuItem.onSelectTexture = tempMenuItem.standardTexture;
	tempMenuItem.ID = newID;
	tempMenuItem.pressable = true;

	newID++;
	mMenuItemList.push_back(tempMenuItem);
}

void Menu::addMenuItem(std::string itemName, char *standardTextureSource, char *onSelectTextureSource)
{
	static int newID = 0;
	MenuItem tempMenuItem;
	tempMenuItem.itemName = itemName;
	tempMenuItem.state = STANDARD;
	tempMenuItem.standardTexture = gGraphics->loadTexture(standardTextureSource);
	tempMenuItem.onSelectTexture = gGraphics->loadTexture(onSelectTextureSource);
	tempMenuItem.ID = newID;
	tempMenuItem.pressable = true;

	newID++;
	mMenuItemList.push_back(tempMenuItem);
}

void Menu::buildMenu(int itemWidth, int itemHeight)
{
	int menuSize = mMenuItemList.size();
	int itemNumber = menuSize/2;
	float isfloat = (float)menuSize/2;

	int width, height, centerX, centerY, diffY;
	width = backgroundRect.right - backgroundRect.left;
	height = backgroundRect.bottom - backgroundRect.top;
	centerX = backgroundRect.left + (width/2);

	// hantera udda antal menuItems
	if((int)isfloat != isfloat)
	{
		// decimal tal, dvs udda antal items
		std::list<MenuItem>::iterator i = mMenuItemList.begin();	
		while( i != mMenuItemList.end())
		{	 
			if(itemNumber != 0)
			{				
				// avstånd i Y från mitten
				diffY = itemNumber*(spacing +itemHeight);
				// menuItems Y koordinat
				centerY = bkgdY - diffY;			
			}
			else if(itemNumber == 0)	{	// behövs inte, är itemNumber = 0 så är diffY = 0!
				// x och y = bkgds x och y
				centerY = bkgdY;
			}

			i->itemRect.left = centerX-(itemWidth/2);
			i->itemRect.right = centerX+(itemWidth/2);
			i->itemRect.top = centerY-(itemHeight/2);
			i->itemRect.bottom = centerY+(itemHeight/2); 

			i++;
			itemNumber--;
		}		
	}	
	// hantera jämna tal menu items
	else
	{
		std::list<MenuItem>::iterator i = mMenuItemList.begin();	
		while( i != mMenuItemList.end())
		{	 
			if(itemNumber == 0)					
				itemNumber--;	

			if(itemNumber > 0)
				diffY = (itemNumber * (spacing + itemHeight)) - 0.5*(spacing + itemHeight);
			else if(itemNumber < 0)
				diffY = (itemNumber * (spacing + itemHeight)) + 0.5*(spacing + itemHeight);

			centerY = bkgdY - diffY;
				
			i->itemRect.left = centerX-(itemWidth/2);
			i->itemRect.right = centerX+(itemWidth/2);
			i->itemRect.top = centerY-(itemHeight/2);
			i->itemRect.bottom = centerY+(itemHeight/2); 

			i++;
			itemNumber--;
		}
	}
}

void Menu::draw(void)
{	
	gGraphics->BlitTexture(backgroundTexture, backgroundRect, 0xFFFFFFFF, 0);

	std::list<MenuItem>::iterator i = mMenuItemList.begin();
	while( i != mMenuItemList.end())
	{	
		if(i->state == STANDARD)
			gGraphics->BlitTexture(i->standardTexture, i->itemRect, 0xFFFFFFFF, 0);
		else if(i->state = SELECTED)
			gGraphics->BlitTexture(i->onSelectTexture, i->itemRect, 0xFFFFFFFF, 0);

		if(mUseFonts)
			gGraphics->drawText((char*)i->itemName.c_str(), i->itemRect.left + 5, i->itemRect.top + (i->itemRect.bottom - i->itemRect.top)/2-10);
		i++;
	}	
}

void Menu::update(POINT mousePos)
{
	//updateSelectedItem(mousePos);
	if(navigation == MOUSE)
	{
		std::list<MenuItem>::iterator i = mMenuItemList.begin();
		while( i != mMenuItemList.end())
		{
			// mouse is inside
			if(mousePos.x < i->itemRect.right && mousePos.x > i->itemRect.left && mousePos.y < i->itemRect.bottom && mousePos.y > i->itemRect.top)
			{
				i->state = SELECTED;
				if(gMouse->buttonPressed(LEFTBUTTON) && i->pressable)	{
					gSound->mEngine->play2D("misc\\sound\\menu_click.wav");
					if(!callback(i->itemName))
						break;
				}		
			}
			else
				i->state = STANDARD;

			i++;
		}
	}
	else if(navigation == ARROWKEYS)
	{		
		// updatera idCounter				
		if(gDInput->keyPressed(DIK_W)){
			idCounter--;
		}
		else if(gDInput->keyPressed(DIK_S))	{
			idCounter++;
		}
	
		// kolla om man är utanför menyn
		if(idCounter < 0)
			idCounter = numbersOfItems-1;	// tänk 0-X
		else if(idCounter > numbersOfItems-1)
			idCounter = 0;			

		std::list<MenuItem>::iterator i = mMenuItemList.begin();
		while( i != mMenuItemList.end())
		{								
			if(i->ID == idCounter){				
				i->state = SELECTED;
			}
			else
				i->state = STANDARD;

			// a item was pressed
			if(gDInput->keyPressed(DIK_RETURN))
			{
				if(i->ID == idCounter && i->pressable)	
				{
					if(!callback(i->itemName))
						break;
				}					
			}

			i++;
		}
	}

}

void Menu::buildMenu2(int itemWidth, int itemHeight)
{
	/* stuff neeeded */
	int		menuSize;
	int		widthInItems;
	int		heightInItems;
	int		loopX;
	int		loopY;
	int		dX;
	int		dY;
	int		curX;
	int		curY;
	int		resX, resY;
	int		itemNumber;

	/* number of items */
	menuSize = mMenuItemList.size();

	/* find out width and height counted in items */
	widthInItems = menuSize % mBreakCount;

	/*
	*  this is neccessary to get a correct height
	*  has to do with integers rounding down
	*  will only occur when there's an uneven amount of menu items
	*  adds mBreakCount since adding 1 isn't suffice with large menus
	*/
	if(menuSize % mBreakCount != 0)	{
		heightInItems = (menuSize + mBreakCount) / mBreakCount;
		if(heightInItems != 1)
			widthInItems = mBreakCount;
	}
	else	{
		heightInItems = (menuSize) / mBreakCount;
		if(heightInItems != 0)
			widthInItems = mBreakCount;
	}

	if(menuSize == 1)	{
		widthInItems = 0;
		heightInItems = 0;
	}

	/* if the first row is filled, then we now that the width = break count */
	//if(heightInItems != 0)	// (heightInItems != 1)
	//	widthInItems = mBreakCount;

	itemNumber = 0;

	/* loopa through all items */
	std::list<MenuItem>::iterator i = mMenuItemList.begin();
	while( i != mMenuItemList.end())
	{
		loopX = itemNumber %  mBreakCount;
		loopY = itemNumber /  mBreakCount;

		/* even amount of items in the widest x row */
		if(widthInItems % 2 == 0 && widthInItems != 0)	{
			dX = (loopX * (spacing + itemWidth));
			dX -= 0.5*(spacing + itemWidth);

			if(menuSize == 1)
				dX = 0;

			resX = bkgdX - dX - widthInItems*(spacing + itemWidth)/2 ;//+ (spacing + itemWidth)/2;
		}
		/* uneven */
		else	{
			dX = loopX*(spacing +itemWidth);

			if(menuSize == 1)
				resX = bkgdX;
			else
				resX = bkgdX - dX - widthInItems*(spacing + itemWidth)/2 + (spacing + itemWidth)/2;;
		}

		/* even amount of items in the widest y row */
		if(heightInItems % 2 == 0 && heightInItems != 0)	{
			dY = (loopY * (spacing + itemHeight));
			dY -= 0.5*(spacing + itemHeight);	
			
			if(menuSize == 1)
				dY = 0;

			resY = bkgdY - dY - heightInItems*(spacing + itemHeight)/2;
		}
		/* uneven */
		else	{		
				dY = loopY*(spacing +itemHeight);

				if(menuSize == 1)
					resY = bkgdY;
				else
					resY = bkgdY - dY - heightInItems*(spacing + itemHeight)/2 + (spacing + itemHeight)/2;
		}

		/* center the menu items */
		//centerX -= 

		i->itemRect.left = resX-(itemWidth/2);
		i->itemRect.right = resX+(itemWidth/2);
		i->itemRect.top = resY-(itemHeight/2);
		i->itemRect.bottom = resY+(itemHeight/2); 

		itemNumber--;
		i++;
	}
}

void Menu::setPressable(std::string name, bool b)
{
	std::list<MenuItem>::iterator i = mMenuItemList.begin();
	while( i != mMenuItemList.end())
	{
		if(i->itemName == name)
			i->pressable = b;
		i++;
	}
}

void Menu::removeItem(std::string itemName)
{	
	std::list<MenuItem>::iterator i = mMenuItemList.begin();
	while( i != mMenuItemList.end())
	{
		if(i->itemName == itemName)	{
			mMenuItemList.erase(i);
			buildMenu2();
			return;
		}
		i++;
	}
}