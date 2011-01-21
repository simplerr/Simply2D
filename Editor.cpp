#include "DropBox.h"
#include "Editor.h"
#include "StaticPlatform.h"
#include "MovingPlatform.h"

// Window behöver ingen mus längre
// Editor ska ha den 

Editor::Editor() : SNAP_SENSE(30), SNAP_DIST(10)
{
	mLevel = new GameWorld(NULL);
	gameArea = new Window("gamearea", 400, 300, 800, 600, D3DCOLOR_ARGB(0, 0, 0, 0));
	sideBar = new Window("sidebar", 900, 300, 200, 600, D3DCOLOR_ARGB(50, 100,0,0));

	// kanske inte behöver ta name som arg?
	TextBox *tPositionX = new TextBox("posx", "X:", 40, 40, 60, 20);
	TextBox *tPositionY = new TextBox("posy", "Y:", 40, 70, 60, 20);
	TextBox *tWidth = new TextBox("width", "Width:", 40, 100, 60, 20);
	TextBox *tHeight = new TextBox("height", "Height:", 40, 130, 60, 20);

	InputBox *iPositionX = new InputBox("iposx", 110, 40, 60, 20);
	InputBox *iPositionY = new InputBox("iposy", 110, 70, 60, 20);
	InputBox *iWidth = new InputBox("iwidth", 110, 100, 60, 20);
	InputBox *iHeight = new InputBox("iheight", 110, 130, 60, 20);

	ListBox *listBox = new ListBox("listbox", 66, 260, 110, 40);

	Button *createButton = new Button("Create", "createobject", 40, 210, 60, 20, D3DCOLOR_ARGB(255, 0, 150, 150));
	Button *deleteButton = new Button("Delete", "deleteobject", 112, 210, 60, 20, D3DCOLOR_ARGB(255, 0, 150, 150));

	DropBox *textureDropBox = new DropBox("texturedropbox", 66, 165, 110, 20, 20);

	sideBar->addWindow(tPositionX);
	sideBar->addWindow(tPositionY);
	sideBar->addWindow(tWidth);
	sideBar->addWindow(tHeight);

	sideBar->addWindow(iPositionX);
	sideBar->addWindow(iPositionY);
	sideBar->addWindow(iWidth);
	sideBar->addWindow(iHeight);

	sideBar->addWindow(listBox);
	listBox->addItem("Grass Platform", 22, D3DCOLOR_ARGB(255, 30, 120, 150));
	listBox->addItem("Brick Platform", 22, D3DCOLOR_ARGB(255, 200, 0, 150));

	sideBar->addWindow(createButton);
	sideBar->addWindow(deleteButton);

	sideBar->addWindow(textureDropBox);
	textureDropBox->addItem("grass_platform");
	textureDropBox->addItem("brick_platform", D3DCOLOR_ARGB(255, 0, 255, 150));
	textureDropBox->addItem("stone_platform");
	textureDropBox->addItem("gold_platform", D3DCOLOR_ARGB(255, 0, 255, 150));

	//mLevel->loadLevel("level_1.txt");
	activeObject = NULL;
	create = "none";
	itemToCreate = "none";
	snapCount = SNAP_SENSE;
	snapDir = ALL;
	snappedObject = NULL;
	//buildBaseLevel();
}
Editor::~Editor()
{
	// dtor
	//mLevel->saveLevel("level_1.txt");
	delete sideBar;
	delete gameArea;
	delete mLevel;
}

void Editor::updateAll(float dt)
{
	mousePos = mMouse->getPos();
	
	// mousewheel scaling
	if(activeObject != NULL)
	{
		if(gDInput->mouseDZ() > 0)	{
			activeObject->scale(8, 8);

			updateInputBoxes();
			updateDragRects();
		}
		else if(gDInput->mouseDZ() < 0)	{
			if(activeObject->getHeight() > 58 && activeObject->getWidth() > 58)	{

				activeObject->scale(-8, -8);
				updateInputBoxes();
				updateDragRects();
			}
		}
	}
		
	// select platform
	if(gDInput->mouseButtonPressed(LEFTBUTTON))	{
			
		WindowRect areaInfo = gameArea->getInfo();
		if(mousePos.x > areaInfo.left && mousePos.x < areaInfo.right && mousePos.y > areaInfo.top && mousePos.y < areaInfo.bottom)	// gameArea == true
		{			
			// get selectedPlatform
			activeObject = mLevel->getObjectAt(mousePos);
			if(activeObject != NULL)	
			{
				// nollställ
				snapDir = ALL;
				snapCount = SNAP_SENSE;

				// update inputboxes - med activPlatforms värden ;d
				updateInputBoxes();
				sideBar->setVisibility("Delete", true);
			}		
			else
			{
				// reset inputboxes
				resetInputBoxes();
				sideBar->setVisibility("Delete", false);
			}
			sideBar->setActive(false);
		}
		else
		{
			sideBar->handleMessages(MOUSEPRESS, mousePos.x, mousePos.y);
			itemToCreate = sideBar->getValue("listbox");
			create = sideBar->getValue("Create");			
			delet = sideBar->getValue("Delete");

			if(delet == "pressed")	{
				if(activeObject != NULL)	{
					mLevel->deleteStaticObject(activeObject->getID());	// ska lägga till för dynamic också!		
					resetInputBoxes();
				}
			}
			
			// listbox item vald
			if(itemToCreate != "none")
			{
				if(create == "pressed")	
				{					
					if(itemToCreate == "Brick Platform")
					{
						StaticPlatform *platform = new StaticPlatform(500, 300, 100, 100, "misc\\textures\\brick_platform.bmp");
						mLevel->addStaticObject(platform);
					}
					else if(itemToCreate == "Grass Platform")
					{
						StaticPlatform *platform = new StaticPlatform(500, 300, 100, 100, "misc\\textures\\grass_platform.bmp");
						mLevel->addStaticObject(platform);
					}
					// aktiv plattform = den nya?
				}
			}
		}
		// initiera/updatera dragAreas
		if(activeObject != NULL)
		{
			updateDragRects();
		}
	}
	// flytta/resize markerad plattform
	if(activeObject != NULL)
	{
		// if mousedown
		if(gDInput->mouseButtonDown(LEFTBUTTON))
		{
			WindowRect areaInfo = gameArea->getInfo();
			if(mousePos.x > areaInfo.left && mousePos.x < areaInfo.right && mousePos.y > areaInfo.top && mousePos.y < areaInfo.bottom)
			{			
				if(mousePos.x > dragLeft.left && mousePos.x < dragLeft.right && mousePos.y > dragLeft.top && mousePos.y < dragLeft.bottom)
					resizePlatform(DRAGLEFT);
				else if(mousePos.x > dragRight.left && mousePos.x < dragRight.right && mousePos.y > dragRight.top && mousePos.y < dragRight.bottom)
					resizePlatform(DRAGRIGHT);
				else if(mousePos.x > dragTop.left && mousePos.x < dragTop.right && mousePos.y > dragTop.top && mousePos.y < dragTop.bottom)
					resizePlatform(DRAGUP);
				else if(mousePos.x > dragBottom.left && mousePos.x < dragBottom.right && mousePos.y > dragBottom.top && mousePos.y < dragBottom.bottom)
					resizePlatform(DRAGDOWN);
				else
					movePlatform();
				// dragRects och inputBoxes updateras i funktionerna
			}
		}	
	}
	sideBar->updateWindow(dt);
}
// körs när man tar tag i markerad plattform
void Editor::movePlatform(void)
{		
	RECT activeObjectRect = activeObject->getRect();
		// updatera koordinater
		float dx = gDInput->mouseDX();
		float dy = gDInput->mouseDY();

		if(activeObjectRect.left <= 0 || activeObjectRect.right >= 800)
				mMouse->setMousePos(mousePos.x - dx, mousePos.y);
			if(activeObjectRect.top <= 0 || activeObjectRect.bottom >= 600)
				mMouse->setMousePos(mousePos.x, mousePos.y - dy);

		// kolla begränsningar - true -> gameArea + ingen krock med annan plattform!
		if(activeObjectRect.left >= 0 && activeObjectRect.right <= 800 && activeObjectRect.top >= 0 && activeObjectRect.bottom <= 600)
		{
			// så musen inte rör sig vid kanten
			
		
			if(!objectSnapping(activeObject, dx, dy))
			{
				if(snapDir == ALL)	// skulle kännas bättre med NONE
				{
					activeObject->move(dx, dy);							
				}
				else if(snapDir == LEFT || snapDir == RIGHT)
				{
					// dx kan vara båda + att om den inte är nära snapped ska den röra sig fritt
					if(snapCount >= SNAP_SENSE || snapCount <= -SNAP_SENSE  || stillSnapped() == false)	// temporär, gäller alla platformer
					{		
						activeObject->move(dx, dy);
						snapDir = ALL;
					}
					else	{
						snapCount += dx;
					}					

					// ska kunna röra plattformen lodrätt
					activeObject->move(0, dy);	
					// musens ska inte röra på sig!
					mMouse->setMousePos(mousePos.x - dx, mousePos.y);
				}
				else if(snapDir == UP || snapDir == DOWN)
				{
					if(snapCount >= SNAP_SENSE || snapCount <= -SNAP_SENSE || stillSnapped() == false)
					{
						activeObject->move(dx, dy);
						snapDir = ALL;
					}
					else	{
						snapCount += dy;
					}

					activeObject->move(dx, 0);
					// musens ska inte röra på sig!
					mMouse->setMousePos(mousePos.x, mousePos.y - dy);
				}
				sideBar->setVisibility("Delete", true);		
			}			
			
		}	
		// sätt till kanten
		else
		{
			if(activeObjectRect.left < 0)
			{
				activeObject->setXY(activeObject->getWidth()/2, activeObject->getY());

				/*activeObject->rect.left = 0;
				activeObject->rect.right = activeObject->rect.left + activeObject->width;
				activeObject->xpos = activeObject->rect.left + activeObject->width/2;*/
			}
			else if(activeObjectRect.right > 800)
			{
				activeObject->setXY(800 - activeObject->getWidth()/2, activeObject->getY());
				/*activeObject->rect.right = 800;
				activeObject->rect.left = activeObject->rect.right - activeObject->width;
				activeObject->xpos = activeObject->rect.left + activeObject->width/2;*/
			}
			else if(activeObjectRect.top < 0)
			{
				activeObject->setXY(activeObject->getX(), activeObject->getHeight()/2);
				/*activeObject->rect.top = 0;
				activeObject->rect.bottom = activeObject->rect.top + activeObject->height;
				activeObject->ypos = activeObject->rect.top + activeObject->height/2;*/
			}
			else if(activeObjectRect.bottom > 600)
			{
				activeObject->setXY(activeObject->getX(), 600 - activeObject->getHeight()/2);
				/*activeObject->rect.bottom = 600;
				activeObject->rect.top = activeObject->rect.bottom - activeObject->height;
				activeObject->ypos = activeObject->rect.top + activeObject->height/2;*/
			}
		}
		updateInputBoxes();
		updateDragRects();
}

void Editor::renderAll()
{
	//gameArea->renderAll();
	mLevel->drawLevel();
	sideBar->renderAll();

	if(activeObject != NULL)	{
		// orange markering
		//gGraphics->BlitRect(activeObject->xpos, activeObject->ypos, activeObject->rect.right - activeObject->rect.left+7,
		//activeObject->rect.bottom - activeObject->rect.top+7, D3DCOLOR_ARGB(150, 255, 166, 0));

		gGraphics->BlitRect(activeObject->getRect(), D3DCOLOR_ARGB(150, 255, 166, 0));

		//drag areas
		gGraphics->BlitRect(dragLeft, D3DCOLOR_ARGB(220, 130, 166, 255));
		gGraphics->BlitRect(dragRight, D3DCOLOR_ARGB(220, 130, 166, 255));
		gGraphics->BlitRect(dragTop, D3DCOLOR_ARGB(220, 130, 166, 255));
		gGraphics->BlitRect(dragBottom, D3DCOLOR_ARGB(220, 130, 166, 255));

	}
}

void Editor::keyPressed(WPARAM wParam)
{
	if(activeObject != NULL)
	{
		RECT activeObjectRect = activeObject->getRect();
		sideBar->keyPressed(wParam);
		
		char *tmp = new char;		// mem leak
		string tmp2;
		int x, y, width, height;

		//////////////////////////////////
		//// * x ska sättas till gameAreaWIDTH + platformWIDHT/2 etc.....
		////
		//////////////////////////////////
		tmp2 = sideBar->getValue("iposx");
		sprintf(tmp, "%s", tmp2.c_str());
		x = atoi(tmp);
		if(x <= 800 && x >= 0)
			activeObject->setXY(x, activeObject->getY());
		else
			activeObject->setXY(800, activeObject->getY());

		tmp2 = sideBar->getValue("iposy");
		sprintf(tmp, "%s", tmp2.c_str());
		y = atoi(tmp);
		if(y <= 600 && y >= 0)
			activeObject->setXY(activeObject->getX(), y);
		else
			activeObject->setXY(activeObject->getX(), 600);

		tmp2 = sideBar->getValue("iwidth");
		sprintf(tmp, "%s", tmp2.c_str());
		width = atoi(tmp);
		activeObject->setWidth(width);

		tmp2 = sideBar->getValue("iheight");
		sprintf(tmp, "%s", tmp2.c_str());
		height = atoi(tmp);
		activeObject->setHeight(height);

		updateDragRects();
	}	
}

void Editor::buildBaseLevel(void)
{	
	/*mLevel->addPlatform(400, 520, 800, 100, GRASS_PLATFORM);
	mLevel->addPlatform(600, 300, 100, 100, GRASS_PLATFORM);
	mLevel->addPlatform(250, 300, 100, 100, BRICK_PLATFORM);*/
	
}

void Editor::updateInputBoxes(void)
{
	// updatera inputboxarna 
	char buffer[256];

	sprintf(buffer, "%i", (int)activeObject->getX());
	sideBar->setValue("iposx", buffer); 

	sprintf(buffer, "%i", (int)activeObject->getY());
	sideBar->setValue("iposy", buffer); 

	sprintf(buffer, "%i", activeObject->getWidth());
	sideBar->setValue("iwidth", buffer); 

	sprintf(buffer, "%i", activeObject->getHeight());
	sideBar->setValue("iheight", buffer);	
}
void Editor::resetInputBoxes(void)
{
	char buffer[10] = " ";				
	sideBar->setValue("iposx", buffer); 				
	sideBar->setValue("iposy", buffer); 				
	sideBar->setValue("iwidth", buffer); 				
	sideBar->setValue("iheight", buffer);
}

// de ska vara en procentuell del av activeObject
// de ska inte renderas
// hoovrar man över dem ska cursor bytas
void Editor::updateDragRects(void)
{
	RECT activeObjectRect = activeObject->getRect();

	dragLeft.left = activeObjectRect.left;
	dragLeft.right = dragLeft.left + 20;
	dragLeft.top = activeObjectRect.top + 20;
	dragLeft.bottom = activeObjectRect.bottom - 20;

	dragRight.right = activeObjectRect.right;
	dragRight.left = dragRight.right - 20;
	dragRight.top = activeObjectRect.top + 20;
	dragRight.bottom = activeObjectRect.bottom - 20;

	dragTop.top = activeObjectRect.top;
	dragTop.bottom = dragTop.top + 20;
	dragTop.left = activeObjectRect.left + 20;
	dragTop.right = activeObjectRect.right - 20;

	dragBottom.bottom = activeObjectRect.bottom;
	dragBottom.top = dragBottom.bottom - 20;
	dragBottom.left = activeObjectRect.left + 20;
	dragBottom.right = activeObjectRect.right - 20;

	/*dragLeft.left = activeObject->rect.left;
	dragLeft.right = dragLeft.left + .2*activeObject->width;
	dragLeft.top = activeObject->rect.top + .2*activeObject->height;
	dragLeft.bottom = activeObject->rect.bottom - .2*activeObject->height;

	dragRight.right = activeObject->rect.right;
	dragRight.left = dragRight.right - .2*activeObject->width;
	dragRight.top = activeObject->rect.top + .2*activeObject->height;
	dragRight.bottom = activeObject->rect.bottom - .2*activeObject->height;

	dragTop.top = activeObject->rect.top;
	dragTop.bottom = dragTop.top + .2*activeObject->height;
	dragTop.left = activeObject->rect.left + .2*activeObject->width;
	dragTop.right = activeObject->rect.right - .2*activeObject->width;

	dragBottom.bottom = activeObject->rect.bottom;
	dragBottom.top = dragBottom.bottom - .2*activeObject->height;
	dragBottom.left = activeObject->rect.left + .2*activeObject->width;
	dragBottom.right = activeObject->rect.right - .2*activeObject->width;*/
}

void Editor::resizePlatform(DragRect drag)
{
	// updatera movements
	float dx = gDInput->mouseDX();
	float dy = gDInput->mouseDY();

	RECT activeObjectRect = activeObject->getRect();

	// när man förminskar så borde dragRecten förminskas från andra håller
	// dvs att right ska sättas först och sedan ska left vara relativ till den

	if(drag == DRAGLEFT)
	{
		if(activeObject->getWidth() >= 50)	{
			activeObjectRect.left += dx;
			activeObject->scale(-dx, 0);
			activeObject->setXY(activeObjectRect.left + activeObject->getWidth()/2, activeObject->getY());
		}
	}	
	else if(drag == DRAGRIGHT)
	{
		if(activeObject->getWidth() >= 50)	{
			activeObjectRect.right += dx;
			activeObject->scale(dx, 0);		// drar man musen åt höger ökar bredden
			activeObject->setXY(activeObjectRect.left + activeObject->getWidth()/2, activeObject->getY());
		}
	}
	else if(drag == DRAGUP)
	{
		if(activeObject->getHeight() >= 50)	{
			activeObjectRect.top += dy;
			activeObject->scale(0, -dy);
			activeObject->setXY(activeObject->getX(), activeObjectRect.top + activeObject->getHeight()/2);
		}
	}
	else if(drag == DRAGDOWN)
	{	
		if(activeObject->getHeight() >= 50)	{
			activeObjectRect.bottom += dy;
			activeObject->scale(0, dy);
			activeObject->setXY(activeObject->getX(), activeObjectRect.top + activeObject->getHeight()/2);
		}
	}
	updateDragRects();
	updateInputBoxes();
}

bool Editor::objectSnapping(Object *object, float dx, float dy)
{
	Object testObject = *activeObject;
	RECT activeObjectRect = activeObject->getRect();
	RECT snapObjectRect;
	bool toReturn = false;
	int snapDist;

	// om aktiv plattform inte är i någon annan plattform
	if(mLevel->objectIntersection(activeObject) == NULL)
	{	
		// till höger
		//testPlatform.rect.right += SNAP_DIST;
		testObject.move(SNAP_DIST, 0);
		snappedObject = mLevel->objectIntersection(&testObject);
		if(snappedObject != NULL)
		{
			// snap
			if(dx > 0)
			{
				// ta reda på hur långt den ska snappa
				snapObjectRect = snappedObject->getRect();
				snapDist = snapObjectRect.left - activeObjectRect.right;
				mMouse->setMousePos(mousePos.x+snapDist, mousePos.y);
				activeObject->move(snapDist, 0);

				snapCount = 0;
				snapDir = LEFT;
				return true;
			}
		}

		// till vänster
		testObject = *activeObject;
		testObject.move(-SNAP_DIST, 0);
		snappedObject = mLevel->objectIntersection(&testObject);
		if(snappedObject != NULL)
		{
			// snap
			if(dx < 0)
			{
				snapObjectRect = snappedObject->getRect();
				snapDist = activeObjectRect.left - snapObjectRect.right;
				mMouse->setMousePos(mousePos.x-snapDist, mousePos.y);
				activeObject->move(-snapDist, 0);

				snapCount = 0;
				snapDir = RIGHT;
				return true;
			}
		}

		// under
		testObject = *activeObject;
		testObject.move(0, SNAP_DIST);
		snappedObject = mLevel->objectIntersection(&testObject);
		if(snappedObject != NULL)
		{
			// snap
			if(dy > 0)
			{
				snapObjectRect = snappedObject->getRect();
				snapDist = snapObjectRect.top - activeObjectRect.bottom;
				mMouse->setMousePos(mousePos.x, mousePos.y + snapDist);
				activeObject->move(0, snapDist);

				snapCount = 0;
				snapDir = DOWN;
				return true;
			}
		}

		// över
		testObject = *activeObject;
		testObject.move(0, -SNAP_DIST);
		snappedObject = mLevel->objectIntersection(&testObject);
		if(mLevel->objectIntersection(&testObject))
		{
			// snap
			if(dy < 0)
			{
				snapObjectRect = snappedObject->getRect();
				snapDist = activeObjectRect.top - snapObjectRect.bottom;
				mMouse->setMousePos(mousePos.x, mousePos.y - snapDist);
				activeObject->move(0, -snapDist);

				snapCount = 0;
				snapDir = UP;
				return true;
			}
		}
	}
	
	return false;
}

bool Editor::stillSnapped(void)
{		
	if(snappedObject != NULL)
	{
		RECT activeObjectRect = activeObject->getRect();
		RECT snappedObjectRect = snappedObject->getRect();
		if(activeObjectRect.top <= snappedObjectRect.bottom && activeObjectRect.bottom >= snappedObjectRect.top && activeObjectRect.right >= snappedObjectRect.left &&
			activeObjectRect.left <= snappedObjectRect.right && activeObject->getID() != snappedObject->getID())
			return true;
	}

	return false;
}