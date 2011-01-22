#include "DropBox.h"
#include "Editor.h"
#include "StaticPlatform.h"
#include "MovingPlatform.h"

// Window behöver ingen mus längre
// Editor ska ha den 

Editor::Editor() : Window(EDITOR, 900, 300, 200, 600), SNAP_SENSE(30), SNAP_DIST(10)
{
	mLevel = new GameWorld(NULL);
	
	gameArea.top = 0;
	gameArea.bottom = 600;
	gameArea.left = 0;
	gameArea.right = 800;

	// kanske inte behöver ta name som arg?
	TextBox *tPositionX = new TextBox(TEXT_XPOS, "X:", 40, 40, 60, 20);
	TextBox *tPositionY = new TextBox(TEXT_YPOS, "Y:", 40, 70, 60, 20);
	TextBox *tWidth = new TextBox(TEXT_WIDTH, "Width:", 40, 100, 60, 20);
	TextBox *tHeight = new TextBox(TEXT_HEIGHT, "Height:", 40, 130, 60, 20);

	InputBox *iPositionX = new InputBox(INPUT_XPOS, 110, 40, 60, 20);
	InputBox *iPositionY = new InputBox(INPUT_YPOS, 110, 70, 60, 20);
	InputBox *iWidth = new InputBox(INPUT_WIDTH, 110, 100, 60, 20);
	InputBox *iHeight = new InputBox(INPUT_HEIGHT, 110, 130, 60, 20);

	ListBox *listBox = new ListBox(LISTBOX_OBJECTTYPE, 66, 260, 110, 40);

	Button *createButton = new Button(BUTTON_CREATE, "Create", 40, 320, 60, 20, D3DCOLOR_ARGB(255, 90, 140, 140));
	Button *deleteButton = new Button(BUTTON_DELETE, "Delete", 40, 194, 60, 20, D3DCOLOR_ARGB(255, 90, 140, 140));
	Button *saveButton = new Button(BUTTON_SAVE, "Save", 110, 320, 60, 20, D3DCOLOR_ARGB(255, 90, 140, 140));

	DropBox *textureDropBox = new DropBox(DROPBOX_TEXTURE, 66, 165, 110, 20, 20);

	addWindow(tPositionX);
	addWindow(tPositionY);
	addWindow(tWidth);
	addWindow(tHeight);

	addWindow(iPositionX);
	addWindow(iPositionY);
	addWindow(iWidth);
	addWindow(iHeight);

	addWindow(listBox);
	listBox->addItem("Static Platform", 22, D3DCOLOR_ARGB( 255, 230, 230, 230 ));
	listBox->addItem("Moving Platform", 22, D3DCOLOR_ARGB( 255, 200, 200, 200 ));

	addWindow(createButton);
	addWindow(deleteButton);
	addWindow(saveButton);

	addWindow(textureDropBox);
	textureDropBox->addItem("grass_platform", D3DCOLOR_ARGB( 255, 230, 230, 230 ));
	textureDropBox->addItem("brick_platform", D3DCOLOR_ARGB( 255, 200, 200, 200 ));

	strcpy(buffer, ACTIVE_LEVEL.c_str());
	mLevel->loadLevel(buffer);
	activeObject = NULL;

	snapCount = SNAP_SENSE;
	snapDir = ALL;
	snappedObject = NULL;
}
Editor::~Editor()
{
	//mLevel->saveLevel("level_1.txt")
	delete mLevel;
}

void Editor::updateAll(float dt)
{
	Window::updateWindow(dt);
	mousePos = mMouse->getPos();
	
	// mousewheel scaling
	if(activeObject != NULL)
	{
		if(gDInput->mouseDZ() > 0)	{
			activeObject->scale(8, 8);

			// messageHandler(OBJECT_SCALED)
			messageHandler(ACTIVE_OBJECT);
		}
		else if(gDInput->mouseDZ() < 0)	{
			if(activeObject->getHeight() > 58 && activeObject->getWidth() > 58)	{

				activeObject->scale(-8, -8);
				messageHandler(ACTIVE_OBJECT);
			}
		}
	}
		
	// select platform
	if(gDInput->mouseButtonPressed(LEFTBUTTON))	{
		if(mousePos.x > gameArea.left && mousePos.x < gameArea.right && mousePos.y > gameArea.top && mousePos.y < gameArea.bottom)	// gameArea == true
		{			
			// get selectedPlatform
			activeObject = mLevel->getObjectAt(mousePos);
			if(activeObject != NULL)	
			{
				// nollställ
				snapDir = ALL;
				snapCount = SNAP_SENSE;

				// update inputboxes - med activPlatforms värden ;d
				messageHandler(ACTIVE_OBJECT);
			}		
			else
			{
				// reset inputboxes
				resetInputBoxes();
			}
			setActive(false);
		}
		else
		{
			sendMousePress(mousePos.x, mousePos.y);
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
			if(mousePos.x > gameArea.left && mousePos.x < gameArea.right && mousePos.y > gameArea.top && mousePos.y < gameArea.bottom)
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
	updateWindow(dt);
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
			}			
			
		}	
		// sätt till kanten
		else
		{
			if(activeObjectRect.left < 0)
			{
				activeObject->setXY(activeObject->getWidth()/2, activeObject->getY());
			}
			else if(activeObjectRect.right > 800)
			{
				activeObject->setXY(800 - activeObject->getWidth()/2, activeObject->getY());
			}
			else if(activeObjectRect.top < 0)
			{
				activeObject->setXY(activeObject->getX(), activeObject->getHeight()/2);
			}
			else if(activeObjectRect.bottom > 600)
			{
				activeObject->setXY(activeObject->getX(), 600 - activeObject->getHeight()/2);
			}
		}
		messageHandler(ACTIVE_OBJECT);
}

int Editor::renderAll()
{
	mLevel->drawLevel();
	Window::renderAll();

	gGraphics->drawText("Active object:", 810, 7);
	gGraphics->drawText("Create object:", 810, 220);

	if(activeObject != NULL)	{
		gGraphics->BlitRect(activeObject->getRect(), D3DCOLOR_ARGB(150, 255, 166, 0));

		//drag areas
		gGraphics->BlitRect(dragLeft, D3DCOLOR_ARGB(220, 130, 166, 255));
		gGraphics->BlitRect(dragRight, D3DCOLOR_ARGB(220, 130, 166, 255));
		gGraphics->BlitRect(dragTop, D3DCOLOR_ARGB(220, 130, 166, 255));
		gGraphics->BlitRect(dragBottom, D3DCOLOR_ARGB(220, 130, 166, 255));

	}
	return 1;
}

void Editor::resetInputBoxes(void)
{
	char buffer[10] = " ";				
	setValue(INPUT_XPOS, buffer); 				
	setValue(INPUT_YPOS, buffer); 				
	setValue(INPUT_WIDTH, buffer); 				
	setValue(INPUT_HEIGHT, buffer);
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
	messageHandler(ACTIVE_OBJECT);
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

void Editor::messageHandler(WindowID sender, string data)
{
	char temp[256];

	switch(sender)
	{
	case INPUT_XPOS:
		{
			if(activeObject != NULL)	{
				int x;

				sprintf(temp, "%s", data.c_str());
				x = atoi(temp);
				if(x <= 800 && x >= 0)
					activeObject->setXY(x, activeObject->getY());
				else
					activeObject->setXY(800, activeObject->getY());

				updateDragRects();
			}
			break;
		}
	case INPUT_YPOS:
		{
			if(activeObject != NULL)
			{
				int y;

				sprintf(temp, "%s", data.c_str());
				y = atoi(temp);
				if(y <= 600 && y >= 0)
					activeObject->setXY(activeObject->getX(), y);
				else
					activeObject->setXY(activeObject->getX(), 600);

				updateDragRects();
			}
			break;
		}
	case INPUT_WIDTH:
		{
			if(activeObject != NULL)
			{
				int width;

				sprintf(temp, "%s", data.c_str());
				width = atoi(temp);
				activeObject->setWidth(width);

				updateDragRects();
			}
			break;
		}
	case INPUT_HEIGHT:
		{	
			if(activeObject != NULL)
			{
				int height;

				sprintf(temp, "%s", data.c_str());
				height = atoi(temp);
				activeObject->setHeight(height);

				updateDragRects();
			}
			break;
		}
	case DROPBOX_TEXTURE:
		{
			if(activeObject != NULL)
			{
				strcpy(buffer, data.c_str());
				if(strcmp(buffer, "grass_platform") == 0)
					activeObject->setTextureSource("misc\\textures\\grass_platform.bmp");
				if(strcmp(buffer, "brick_platform") == 0)
					activeObject->setTextureSource("misc\\textures\\brick_platform.bmp");
			}
			break;
		}
	case BUTTON_CREATE:
		{
			// listbox item vald
			string value = getValue(LISTBOX_OBJECTTYPE);
			if(value != "none")
			{								
					if(value == "Static Platform")
					{
						StaticPlatform *platform = new StaticPlatform(500, 300, 100, 100, "misc\\textures\\brick_platform.bmp");
						mLevel->addStaticObject(platform);
					}
					else if(value == "Moving Platform")
					{				
						POINT start;
						POINT end;
						start.x = 200;
						start.y = 300;
						end.x = 600;
						end.y = 300;
						MovingPlatform *platform = new MovingPlatform(200, 300, 100, 100, "misc\\textures\\grass_platform.bmp", start, end, NULL);
						mLevel->addDynamicObject(platform);
					}
					// aktiv plattform = den nya?
			}
			break;
		}
	case BUTTON_DELETE:
		{		
			if(activeObject != NULL)	{			
				mLevel->deleteStaticObject(activeObject->getID());	// ska lägga till för dynamic också!		
				resetInputBoxes();
			}
			break;
		}
	case BUTTON_SAVE:
		{
			strcpy(buffer, ACTIVE_LEVEL.c_str());
			mLevel->saveLevel(buffer);
			break;
		}
	case ACTIVE_OBJECT:
		{
			// updatera inputboxarna 
			char buffer[256];

			sprintf(buffer, "%i", (int)activeObject->getX());
			setValue(INPUT_XPOS, buffer); 

			sprintf(buffer, "%i", (int)activeObject->getY());
			setValue(INPUT_YPOS, buffer); 

			sprintf(buffer, "%i", activeObject->getWidth());
			setValue(INPUT_WIDTH, buffer); 

			sprintf(buffer, "%i", activeObject->getHeight());
			setValue(INPUT_HEIGHT, buffer);

			// update texture source
			sprintf(buffer, "%s", activeObject->getTextureSource());
			if(strcmp(buffer, "misc\\textures\\grass_platform.bmp") == 0)
				setValue(DROPBOX_TEXTURE, "grass_platform");
			else if(strcmp(buffer, "misc\\textures\\brick_platform.bmp") == 0)
				setValue(DROPBOX_TEXTURE, "brick_platform");

			updateDragRects();
			break;
		}
	}
	
}