#include "DropBox.h"
#include "Editor.h"
#include "StaticPlatform.h"
#include "MovingPlatform.h"

// Window behöver ingen mus längre
// Editor ska ha den 

Editor::Editor() : Window(NULL, EDITOR, 1100, 400, 200, 800), SNAP_SENSE(30), SNAP_DIST(10)
{
	
	mLevel = new GameWorld(NULL);
	
	gameArea.top = 0;
	gameArea.bottom = GAME_HEIGHT;
	gameArea.left = 0;
	gameArea.right = GAME_WIDTH;

	/*
	addWindow(tPositionX);
	addWindow(tPositionY);
	addWindow(tWidth);
	addWindow(tHeight);
	addWindow(tStartX);
	addWindow(tStartY);
	addWindow(tEndX);
	addWindow(tEndY);
	addWindow(tSpeed);
	addWindow(iPositionX);
	addWindow(iPositionY);
	addWindow(iWidth);
	addWindow(iHeight);
	addWindow(iStartX);
	addWindow(iStartY);
	addWindow(iEndX);
	addWindow(iEndY);
	addWindow(iSpeed);
	addWindow(createButton);
	addWindow(deleteButton);
	addWindow(saveButton);
	addWindow(textureDropBox);
	addWindow(listBox);
	*/

	strcpy(buffer, ACTIVE_LEVEL.c_str());
	mLevel->loadLevel(buffer);
	activeObject = NULL;

	snapCount = SNAP_SENSE;
	snapDir = ALL;
	snappedObject = NULL;

	createObjectTextPos = 220;
	mPrevActiveObjectType = NO_OBJECT;
}
Editor::~Editor()
{
	//mLevel->saveLevel("level_1.txt")
	delete mLevel;
}

void Editor::buildGUI(void)
{
	
	// kanske inte behöver ta name som arg?
	tPositionX = new TextBox(this, TEXT_XPOS, "X:", 40, 40, 60, 20);
	tPositionY = new TextBox(this, TEXT_YPOS, "Y:", 40, 70, 60, 20);
	tWidth = new TextBox(this, TEXT_WIDTH, "Width:", 40, 100, 60, 20);
	tHeight = new TextBox(this, TEXT_HEIGHT, "Height:", 40, 130, 60, 20);

	tStartX = new TextBox(this, TEXT_STARTX, "Start X:", 40, 160, 60, 20);
	tStartY = new TextBox(this, TEXT_STARTY, "Start Y:", 40, 190, 60, 20);
	tEndX = new TextBox(this, TEXT_ENDX, "End X:", 40, 220, 60, 20);
	tEndY = new TextBox(this, TEXT_ENDY, "End Y:", 40, 250, 60, 20);
	tSpeed = new TextBox(this, TEXT_SPEED, "Speed:", 40, 280, 60, 20);

	iPositionX = new InputBox(this, INPUT_XPOS, 110, 40, 60, 20);
	iPositionY = new InputBox(this, INPUT_YPOS, 110, 70, 60, 20);
	iWidth = new InputBox(this, INPUT_WIDTH, 110, 100, 60, 20);
	iHeight = new InputBox(this, INPUT_HEIGHT, 110, 130, 60, 20);

	iStartX = new InputBox(this, INPUT_STARTX, 110, 160, 60, 20);
	iStartY = new InputBox(this, INPUT_STARTY, 110, 190, 60, 20);
	iEndX = new InputBox(this, INPUT_ENDX, 110, 220, 60, 20);
	iEndY = new InputBox(this, INPUT_ENDY, 110, 250, 60, 20);
	iSpeed = new InputBox(this, INPUT_SPEED, 110, 280, 60, 20);

	listBox = new ListBox(this, LISTBOX_OBJECTTYPE, 66, 260, 110, 40);

	createButton = new Button(this, BUTTON_CREATE, "Create", 40, 320, 60, 20, D3DCOLOR_ARGB(255, 90, 140, 140));
	deleteButton = new Button(this, BUTTON_DELETE, "Delete", 40, 194, 60, 20, D3DCOLOR_ARGB(255, 90, 140, 140));
	saveButton = new Button(this, BUTTON_SAVE, "Save", 110, 320, 60, 20, D3DCOLOR_ARGB(255, 90, 140, 140));

	textureDropBox = new DropBox(this, DROPBOX_TEXTURE, 76, 165, 130, 20, 20);

	listBox->addItem("Static Platform", 22, D3DCOLOR_ARGB( 255, 230, 230, 230 ));
	listBox->addItem("Moving Platform", 22, D3DCOLOR_ARGB( 255, 200, 200, 200 ));

	textureDropBox->addItem("grass_platform", D3DCOLOR_ARGB( 255, 200, 200, 200 ));
	textureDropBox->addItem("brick_platform", D3DCOLOR_ARGB( 255, 230, 230, 230 ));

	tStartX->setVisibility(false);
	tStartY->setVisibility(false);
	tEndX->setVisibility(false);
	tEndY->setVisibility(false);
	tSpeed->setVisibility(false);
	iStartX->setVisibility(false);
	iStartY->setVisibility(false);
	iEndX->setVisibility(false);
	iEndY->setVisibility(false);
	iSpeed->setVisibility(false);

	resetInputBoxes();
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
		if(mousePos.x > gameArea.left && mousePos.x < gameArea.right && mousePos.y > gameArea.top)	// gameArea == true
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

				// static eller dynamic platform?
				if(activeObject->getType() == MOVING_PLATFORM && mPrevActiveObjectType != MOVING_PLATFORM)
				{
					

					tStartX->setVisibility(true);
					tStartY->setVisibility(true);
					tEndX->setVisibility(true);
					tEndY->setVisibility(true);
					tSpeed->setVisibility(true);
					iStartX->setVisibility(true);
					iStartY->setVisibility(true);
					iEndX->setVisibility(true);
					iEndY->setVisibility(true);
					iSpeed->setVisibility(true);

					textureDropBox->move(0, +150);
					deleteButton->move(0, +150);
					createButton->move(0, +150);
					listBox->move(0, +150);
					saveButton->move(0, +150);
					createObjectTextPos += 150;

					mPrevActiveObjectType = MOVING_PLATFORM;				
				}	
				else if(activeObject->getType() == STATIC_PLATFORMA && mPrevActiveObjectType != STATIC_PLATFORMA)	
				{
					tStartX->setVisibility(false);
					tStartY->setVisibility(false);
					tEndX->setVisibility(false);
					tEndY->setVisibility(false);
					tSpeed->setVisibility(false);
					iStartX->setVisibility(false);
					iStartY->setVisibility(false);
					iEndX->setVisibility(false);
					iEndY->setVisibility(false);
					iSpeed->setVisibility(false);

					if(mPrevActiveObjectType != NO_OBJECT)
					{
						textureDropBox->move(0, -150);
						deleteButton->move(0, -150);
						createButton->move(0, -150);
						listBox->move(0, -150);
						saveButton->move(0, -150);
						createObjectTextPos -= 150;
					}

					mPrevActiveObjectType = STATIC_PLATFORMA;
				}
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

		if(activeObjectRect.left <= 0 || activeObjectRect.right >= GAME_WIDTH)
				mMouse->setMousePos(mousePos.x - dx, mousePos.y);
			if(activeObjectRect.top <= 0 || activeObjectRect.bottom >= GAME_HEIGHT)
				mMouse->setMousePos(mousePos.x, mousePos.y - dy);

		// kolla begränsningar - true -> gameArea + ingen krock med annan plattform!
		if(activeObjectRect.left >= 0 && activeObjectRect.right <= GAME_WIDTH && activeObjectRect.top >= 0 && activeObjectRect.bottom <= GAME_HEIGHT)
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
			else if(activeObjectRect.right > GAME_WIDTH)
			{
				activeObject->setXY(GAME_WIDTH - activeObject->getWidth()/2, activeObject->getY());
			}
			else if(activeObjectRect.top < 0)
			{
				activeObject->setXY(activeObject->getX(), activeObject->getHeight()/2);
			}
			else if(activeObjectRect.bottom > GAME_HEIGHT)
			{
				activeObject->setXY(activeObject->getX(), GAME_HEIGHT - activeObject->getHeight()/2);
			}
		}
		messageHandler(ACTIVE_OBJECT);
}

int Editor::renderAll()
{
	mLevel->drawLevel();
	Window::renderAll();

	gGraphics->drawText("Active object:", GAME_WIDTH +10, 7);
	gGraphics->drawText("Create object:", GAME_WIDTH +10, createObjectTextPos);

	// displays the orange effect
	if(activeObject != NULL)	{
		gGraphics->BlitRect(activeObject->getRect(), D3DCOLOR_ARGB(150, 255, 166, 0));

		// diplays the drag areas in different colours
		gGraphics->BlitRect(dragLeft, D3DCOLOR_ARGB(220, 130, 166, 255));
		gGraphics->BlitRect(dragRight, D3DCOLOR_ARGB(220, 130, 166, 255));
		gGraphics->BlitRect(dragTop, D3DCOLOR_ARGB(220, 130, 166, 255));
		gGraphics->BlitRect(dragBottom, D3DCOLOR_ARGB(220, 130, 166, 255));

		// displays the path of the moving object
		if(activeObject->getType() == MOVING_PLATFORM)
		{
			MovingPlatform *tmpPlatform = dynamic_cast<MovingPlatform*>(activeObject);
			RECT activeObjectRect = tmpPlatform->getRect();
			POINT endPos = tmpPlatform->getEndPos();

			RECT pathRect;
			pathRect.left = activeObjectRect.right;
			pathRect.right = endPos.x - tmpPlatform->getWidth()/2;
			pathRect.top = tmpPlatform->getY() - 5;
			pathRect.bottom = tmpPlatform->getY() + 5;

			gGraphics->BlitRect(pathRect, D3DCOLOR_ARGB(150, 0, 166, 255));

			// displays the end pos, and the drag rect
			gGraphics->BlitRect(movingObjectPathRect, D3DCOLOR_ARGB(150, 255, 166, 0));
		}
	}
	return 1;
}

void Editor::resetInputBoxes(void)
{
	char buffer[10] = " ";				
	iPositionX->setValue(buffer); 				
	iPositionY->setValue(buffer); 				
	iWidth->setValue(buffer); 				
	iHeight->setValue(buffer);

	iStartX->setValue(buffer); 				
	iStartY->setValue(buffer); 				
	iEndX->setValue(buffer); 				
	iEndY->setValue(buffer);
	iSpeed->setValue(buffer);

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
				if(x <= GAME_WIDTH && x >= 0)
					activeObject->setXY(x, activeObject->getY());
				else
					activeObject->setXY(GAME_WIDTH, activeObject->getY());

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
				if(y <= GAME_HEIGHT && y >= 0)
					activeObject->setXY(activeObject->getX(), y);
				else
					activeObject->setXY(activeObject->getX(), GAME_HEIGHT);

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
			string value = listBox->getValue();
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
				if(activeObject->getType() == STATIC_PLATFORMA)
					mLevel->deleteStaticObject(activeObject->getID());	// ska lägga till för dynamic också!
				else if(activeObject->getType() == MOVING_PLATFORM)
					mLevel->deleteDynamicObject(activeObject->getID());
				resetInputBoxes();
				activeObject = NULL;
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
			iPositionX->setValue(buffer); 

			sprintf(buffer, "%i", (int)activeObject->getY());
			iPositionY->setValue(buffer); 

			sprintf(buffer, "%i", activeObject->getWidth());
			iWidth->setValue(buffer); 

			sprintf(buffer, "%i", activeObject->getHeight());
			iHeight->setValue(buffer);

			// update texture source
			sprintf(buffer, "%s", activeObject->getTextureSource());
			if(strcmp(buffer, "misc\\textures\\grass_platform.bmp") == 0)
				textureDropBox->setValue("grass_platform");
			else if(strcmp(buffer, "misc\\textures\\brick_platform.bmp") == 0)
				textureDropBox->setValue("brick_platform");

			// if moving platform
			// updatera start, end och speed

			if(activeObject->getType() == MOVING_PLATFORM)
			{
				MovingPlatform *tmpPlatform = dynamic_cast<MovingPlatform*>(activeObject);
				
				POINT tmpPoint = tmpPlatform->getStartPos();
				sprintf(buffer, "%i", tmpPoint.x);
				iStartX->setValue(buffer);

				sprintf(buffer, "%i", tmpPoint.y);
				iStartY->setValue(buffer);

				tmpPoint = tmpPlatform->getEndPos();

				sprintf(buffer, "%i", tmpPoint.x);
				iEndX->setValue(buffer);

				sprintf(buffer, "%i", tmpPoint.y);
				iEndY->setValue(buffer);

				sprintf(buffer, "%.2f", tmpPlatform->getSpeed());
				iSpeed->setValue(buffer);

				// tmpPoint = endPos
				movingObjectPathRect.left = tmpPoint.x - tmpPlatform->getWidth()/2;
				movingObjectPathRect.right = tmpPoint.x + tmpPlatform->getWidth()/2;
				movingObjectPathRect.top = tmpPoint.y - tmpPlatform->getHeight()/2;
				movingObjectPathRect.bottom = tmpPoint.y + tmpPlatform->getHeight()/2;
			}

			updateDragRects();
			break;
		}
	}
	
}