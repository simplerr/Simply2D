#include "Menu.h"

Menu::Menu(std::string menuName, navigationType a_navigation, bool useFonts, int itemAmount, int a_spacing)
{
	mName = menuName;
	
	// mellanrummet mellan menuitems
	spacing = a_spacing;
	navigation = a_navigation;

	numbersOfItems = itemAmount;
	mUseFonts = useFonts;
}

Menu::~Menu()
{
	ReleaseCOM(backgroundTexture);
	
	std::list<MenuItem>::iterator i = mMenuItemList.begin();
	while( i != mMenuItemList.end())
	{	
		ReleaseCOM(i->standardTexture);
		ReleaseCOM(i->onSelectTexture);
		ReleaseCOM(i->onPressTexture);
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
	tempMenuItem.onPressTexture = tempMenuItem.standardTexture;		// ingen hoover eller press effekt? peka på standard texturen då!
	tempMenuItem.onSelectTexture = tempMenuItem.standardTexture;
	tempMenuItem.ID = newID;

	newID++;
	mMenuItemList.push_back(tempMenuItem);
}

void Menu::addMenuItem(std::string itemName, char *standardTextureSource, char *onSelectTextureSource, char *onPressTextureSource)
{
	static int newID = 0;
	MenuItem tempMenuItem;
	tempMenuItem.itemName = itemName;
	tempMenuItem.state = STANDARD;
	tempMenuItem.standardTexture = gGraphics->loadTexture(standardTextureSource);
	tempMenuItem.onSelectTexture = gGraphics->loadTexture(onSelectTextureSource);
	tempMenuItem.onPressTexture = gGraphics->loadTexture(onPressTextureSource);	
	tempMenuItem.ID = newID;

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
		else if(i->state == PRESSED)
			gGraphics->BlitTexture(i->onPressTexture, i->itemRect, 0xFFFFFFFF, 0);
		else if(i->state = SELECTED)
			gGraphics->BlitTexture(i->onSelectTexture, i->itemRect, 0xFFFFFFFF, 0);

		if(mUseFonts)
			gGraphics->drawText((char*)i->itemName.c_str(), i->itemRect.left + (i->itemRect.right - i->itemRect.left)/2-20, i->itemRect.top + (i->itemRect.bottom - i->itemRect.top)/2-10);
		i++;
	}

	
}

void Menu::update(POINT mousePos)
{
	updateSelectedItem(mousePos);
}

void Menu::updateSelectedItem(POINT mousePos)
{
	if(navigation == MOUSE)
	{
		std::list<MenuItem>::iterator i = mMenuItemList.begin();
		while( i != mMenuItemList.end())
		{
			if(mousePos.x < i->itemRect.right && mousePos.x > i->itemRect.left && mousePos.y < i->itemRect.bottom && mousePos.y > i->itemRect.top)	{				
				i->state = SELECTED;
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
			i++;
		}
	}
}

bool Menu::buttonPressed(POINT mousePos, std::string pressedButton)
{
	// navigera med musen?
	if(navigation == MOUSE)
	{	
			std::list<MenuItem>::iterator i = mMenuItemList.begin();
			while( i != mMenuItemList.end())
			{
				if(mousePos.x < i->itemRect.right && mousePos.x > i->itemRect.left && mousePos.y < i->itemRect.bottom && mousePos.y > i->itemRect.top && i->itemName == pressedButton)	{				
					i->state = PRESSED;
					return true;
				}
				i++;
			}
	}
	// navigera med tangentbordet
	else if(navigation == ARROWKEYS)
	{
		if(gDInput->keyPressed(DIK_RETURN))
		{
			std::list<MenuItem>::iterator i = mMenuItemList.begin();
			while( i != mMenuItemList.end())
			{
				if(i->ID == idCounter && i->itemName == pressedButton)	{				
					i->state = PRESSED;
					return true;
				}
				i++;
			}
		}
	}
		return false;		
}
