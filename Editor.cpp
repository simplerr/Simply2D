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

	activeObject = new ActiveObject();

	snapCount = SNAP_SENSE;
	snapDir = ALL;
	snappedObject = NULL;
	movingEndPos = false;
	movingSpawnPos = false;
	showPaths = false;
	tryLevel = false;

	createObjectTextPos = 290;
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
	tPositionX = new TextBox(this, TEXT_XPOS, "X:", 40, 120, 60, 20);
	tPositionY = new TextBox(this, TEXT_YPOS, "Y:", 40, 150, 60, 20);
	tWidth = new TextBox(this, TEXT_WIDTH, "Width:", 40, 180, 60, 20);
	tHeight = new TextBox(this, TEXT_HEIGHT, "Height:", 40, 210, 60, 20);
	tStartX = new TextBox(this, TEXT_STARTX, "Start X:", 40, 240, 60, 20);
	tStartY = new TextBox(this, TEXT_STARTY, "Start Y:", 40, 270, 60, 20);
	tEndX = new TextBox(this, TEXT_ENDX, "End X:", 40, 300, 60, 20);
	tEndY = new TextBox(this, TEXT_ENDY, "End Y:", 40, 330, 60, 20);
	tSpeed = new TextBox(this, TEXT_SPEED, "Speed:", 40, 360, 60, 20);
	tSpawnX = new TextBox(this, TEXT_SPAWNX, "X:", 40, 40, 60, 20);
	tSpawnY = new TextBox(this, TEXT_SPAWNY, "Y:", 40, 70, 60, 20);

	iPositionX = new InputBox(this, INPUT_XPOS, 110, 120, 60, 20);
	iPositionY = new InputBox(this, INPUT_YPOS, 110, 150, 60, 20);
	iWidth = new InputBox(this, INPUT_WIDTH, 110, 180, 60, 20);
	iHeight = new InputBox(this, INPUT_HEIGHT, 110, 210, 60, 20);
	iStartX = new InputBox(this, INPUT_STARTX, 110, 240, 60, 20);
	iStartY = new InputBox(this, INPUT_STARTY, 110, 270, 60, 20);
	iEndX = new InputBox(this, INPUT_ENDX, 110, 300, 60, 20);
	iEndY = new InputBox(this, INPUT_ENDY, 110, 330, 60, 20);
	iSpeed = new InputBox(this, INPUT_SPEED, 110, 360, 60, 20);
	iSpawnX = new InputBox(this, INPUT_SPAWNX, 110, 40, 60, 20);
	iSpawnY = new InputBox(this, INPUT_SPAWNY, 110, 70, 60, 20);

	// sets the values to the spawnPos!
	POS spawnPos = mLevel->getSpawn();
	sprintf(buffer, "%i", (int)mLevel->getSpawn().x);
	iSpawnX->setValue(buffer);
	sprintf(buffer, "%i", (int)mLevel->getSpawn().y);
	iSpawnY->setValue(buffer);

	listBox = new ListBox(this, LISTBOX_OBJECTTYPE, 66, 340, 110, 40);

	createButton = new Button(this, BUTTON_CREATE, "Create", 40, 390, 60, 20, D3DCOLOR_ARGB(255, 90, 140, 140));
	deleteButton = new Button(this, BUTTON_DELETE, "Delete", 40, 270, 60, 20, D3DCOLOR_ARGB(255, 90, 140, 140));
	saveButton = new Button(this, BUTTON_SAVE, "Save", 110, 390, 60, 20, D3DCOLOR_ARGB(255, 90, 140, 140));
	bTryLevel = new Button(this, BUTTON_TRYLEVEL, "Test", 40, 600, 60, 20, D3DCOLOR_ARGB(255, 90, 140, 140));

	textureDropBox = new DropBox(this, DROPBOX_TEXTURE, 76, 240, 130, 20, 20);

	pathCheckBox = new CheckBox(this, CHECKBOX_SHOWPATH, "Show paths: ", 110, 575, 16, 16);

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

int Editor::updateAll(float dt)
{
	if(tryLevel)
		return -1;

	Window::updateWindow(dt);
	mousePos = mMouse->getPos();

	//mLevel->updateLevel(dt);
	
	MovingPlatform *tmpPlatform;

	// mousewheel scaling
	if(activeObject->mObject != NULL)
	{
		if(gDInput->mouseDZ() > 0)	{
			activeObject->mObject->scale(8, 8);

			// messageHandler(OBJECT_SCALED)
			messageHandler(ACTIVE_OBJECT);
		}
		else if(gDInput->mouseDZ() < 0)	{
			if(activeObject->mObject->getHeight() > 58 && activeObject->mObject->getWidth() > 58)	{

				activeObject->mObject->scale(-8, -8);
				messageHandler(ACTIVE_OBJECT);
			}
		}
	}
		
	// select platform
	if(gDInput->mouseButtonPressed(LEFTBUTTON))	{
		if(mousePos.x > gameArea.left && mousePos.x < gameArea.right && mousePos.y > gameArea.top)	// gameArea == true
		{	
			Object *tmpObject;
			tmpObject = NULL;
			//if(!activeObject->mObject->getActive())
			//	tmpObject = mLevel->getObjectAt(mousePos);
			if(activeObject->mMovingPlatform)	{
				if(!(mousePos.x > activeObject->mMovingPlatform->getEndPosRect().left && mousePos.x < activeObject->mMovingPlatform->getEndPosRect().right
				&& mousePos.y > activeObject->mMovingPlatform->getEndPosRect().top && mousePos.y < activeObject->mMovingPlatform->getEndPosRect().bottom))
				tmpObject = mLevel->getObjectAt(mousePos);
			}
			else	{
				tmpObject = mLevel->getObjectAt(mousePos);
				//MessageBox(0, "hh", 0, 0);
			}

			if(tmpObject != NULL)
			{
				activeObject->setObject((tmpObject));
				tmpObject = NULL;
			}
			else if(activeObject->mMovingPlatform)	{
				if(!(mousePos.x > activeObject->mMovingPlatform->getEndPosRect().left && mousePos.x < activeObject->mMovingPlatform->getEndPosRect().right
				&& mousePos.y > activeObject->mMovingPlatform->getEndPosRect().top && mousePos.y < activeObject->mMovingPlatform->getEndPosRect().bottom))
					activeObject->clear();
			}
			else
				activeObject->clear();


			/*tmpPlatform = dynamic_cast<MovingPlatform*>(activeObject);
			// if not pathDragRect
			if(activeObject == NULL || !(activeObject->mObject->getType() == MOVING_PLATFORM && mousePos.x > tmpPlatform->getEndPosRect().left && mousePos.x < tmpPlatform->getEndPosRect().right
				&& mousePos.y > tmpPlatform->getEndPosRect().top && mousePos.y < tmpPlatform->getEndPosRect().bottom))	{
					activeObject = mLevel->getObjectAt(mousePos);
			}*/
			if(activeObject->mObject != NULL)	
			{
				movingSpawnPos = false;
				// nollställ
				snapDir = ALL;
				snapCount = SNAP_SENSE;

				// update inputboxes - med activPlatforms värden ;d
				messageHandler(ACTIVE_OBJECT);

				// static eller dynamic platform?
				if(activeObject->mObject->getType() == MOVING_PLATFORM && mPrevActiveObjectType != MOVING_PLATFORM)
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
				else if(activeObject->mObject->getType() == STATIC_PLATFORMA && mPrevActiveObjectType != STATIC_PLATFORMA)	
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
				// spawn kod
				RECT spawnRect = mLevel->getSpawnRect();
				if(mousePos.x > spawnRect.left && mousePos.x < spawnRect.right && mousePos.y > spawnRect.top && mousePos.y < spawnRect.bottom)	{
					movingSpawnPos = true;
				}
				else
					movingSpawnPos = false;
				
				// reset inputboxes
				resetInputBoxes();
			}
			setActive(false);
		}
		else
		{
			movingSpawnPos = false;
			sendMousePress(mousePos.x, mousePos.y);
		}
		// initiera/updatera dragAreas
		if(activeObject->mObject != NULL)
		{
			updateDragRects();
		}
	}
	// move, resize and change endPos of active object
	if(gDInput->mouseButtonDown(LEFTBUTTON))
	{
		// if mousedown
		if(activeObject->mObject != NULL)
		{
			RECT activeObjectRect = activeObject->mObject->getRect();
			if(mousePos.x > gameArea.left && mousePos.x < gameArea.right && mousePos.y > gameArea.top && mousePos.y < gameArea.bottom)
			{	
				// change end-pos
				if(activeObject->mObject->getType() == MOVING_PLATFORM)
				{
					if(mousePos.x > activeObject->mMovingPlatform->getEndPosRect().left && mousePos.x < activeObject->mMovingPlatform->getEndPosRect().right
						&& mousePos.y > activeObject->mMovingPlatform->getEndPosRect().top && mousePos.y < activeObject->mMovingPlatform->getEndPosRect().bottom)	{						
							moveEndPos();
							movingEndPos = true;
					}
					else 
							movingEndPos = false;
				}
				else
					movingEndPos = false;
				// resize
				if(mousePos.x > dragLeft.left && mousePos.x < dragLeft.right && mousePos.y > dragLeft.top && mousePos.y < dragLeft.bottom && !movingEndPos)	// FIX
					resizePlatform(DRAGLEFT);
				else if(mousePos.x > dragRight.left && mousePos.x < dragRight.right && mousePos.y > dragRight.top && mousePos.y < dragRight.bottom && !movingEndPos)
					resizePlatform(DRAGRIGHT);
				else if(mousePos.x > dragTop.left && mousePos.x < dragTop.right && mousePos.y > dragTop.top && mousePos.y < dragTop.bottom && !movingEndPos)
					resizePlatform(DRAGUP);
				else if(mousePos.x > dragBottom.left && mousePos.x < dragBottom.right && mousePos.y > dragBottom.top && mousePos.y < dragBottom.bottom && !movingEndPos)
					resizePlatform(DRAGDOWN);
				// move
				else if(mousePos.x > activeObjectRect.left && mousePos.x < activeObjectRect.right && mousePos.y > activeObjectRect.top && mousePos.y < activeObjectRect.bottom && !movingEndPos)	
					movePlatform();
			}
		}	
		if(movingSpawnPos)	{
			moveSpawnPos();
			messageHandler(MOVE_SPAWNPOS);
				
		}
	}
	return 1;
}
// körs när man tar tag i markerad plattform
void Editor::movePlatform(void)
{		
	RECT activeObjectRect = activeObject->mObject->getRect();
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
			
		
			if(!objectSnapping(activeObject->mObject, dx, dy))
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
				activeObject->mObject->setXY(activeObject->mObject->getWidth()/2, activeObject->mObject->getY());
			}
			else if(activeObjectRect.right > GAME_WIDTH)
			{
				activeObject->mObject->setXY(GAME_WIDTH - activeObject->mObject->getWidth()/2, activeObject->mObject->getY());
			}
			else if(activeObjectRect.top < 0)
			{
				activeObject->mObject->setXY(activeObject->mObject->getX(), activeObject->mObject->getHeight()/2);
			}
			else if(activeObjectRect.bottom > GAME_HEIGHT)
			{
				activeObject->mObject->setXY(activeObject->mObject->getX(), GAME_HEIGHT - activeObject->mObject->getHeight()/2);
			}
		}
		messageHandler(ACTIVE_OBJECT);
}

int Editor::renderAll()
{
	if(showPaths)
		mLevel->drawEditorLevel();
	else if(!showPaths)
		mLevel->drawLevel();
	Window::renderAll();

	POS spawnPos = mLevel->getSpawn();

	gGraphics->BlitRect(spawnPos.x, spawnPos.y, USER_WIDTH, USER_HEIGHT, D3DCOLOR_ARGB(220, 220, 40, 0));

	gGraphics->drawText("Spawn:", GAME_WIDTH +10, 7);
	gGraphics->drawText("Active object:", GAME_WIDTH +10, 90);
	gGraphics->drawText("Create object:", GAME_WIDTH +10, createObjectTextPos);

	// displays the orange effect
	if(activeObject->mObject != NULL)	{
		gGraphics->BlitRect(activeObject->mObject->getRect(), D3DCOLOR_ARGB(150, 255, 166, 0));

		// diplays the drag areas in different colours
		gGraphics->BlitRect(dragLeft, D3DCOLOR_ARGB(220, 130, 166, 255));
		gGraphics->BlitRect(dragRight, D3DCOLOR_ARGB(220, 130, 166, 255));
		gGraphics->BlitRect(dragTop, D3DCOLOR_ARGB(220, 130, 166, 255));
		gGraphics->BlitRect(dragBottom, D3DCOLOR_ARGB(220, 130, 166, 255));

		// displays the path of the moving object
		if(activeObject->mObject->getType() == MOVING_PLATFORM && !showPaths)
		{
			RECT activeObjectRect = activeObject->mObject->getRect();
			POS endPos = activeObject->mMovingPlatform->getEndPos();

			RECT pathRect;
			if(endPos.x > activeObject->mObject->getX())
			{
				pathRect.left = activeObjectRect.right;
				pathRect.right = endPos.x - activeObject->mObject->getWidth()/2;
				pathRect.top = activeObject->mObject->getY() - 5;
				pathRect.bottom = activeObject->mObject->getY() + 5;
			}
			else if(endPos.x < activeObject->mObject->getX())
			{
				pathRect.left = endPos.x + activeObject->mObject->getWidth()/2;
				pathRect.right = activeObjectRect.left;
				pathRect.top = activeObject->mObject->getY() - 5;
				pathRect.bottom = activeObject->mObject->getY() + 5;
			}
			else
				pathRect = activeObjectRect;

			gGraphics->BlitRect(pathRect, D3DCOLOR_ARGB(150, 0, 166, 255));

			// displays the end pos, and the drag rect of the active object
			gGraphics->BlitRect(activeObject->mMovingPlatform->getEndPosRect(), D3DCOLOR_ARGB(150, 255, 166, 0));
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
	RECT activeObjectRect = activeObject->mObject->getRect();

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

	RECT activeObjectRect = activeObject->mObject->getRect();

	// när man förminskar så borde dragRecten förminskas från andra håller
	// dvs att right ska sättas först och sedan ska left vara relativ till den

	if(drag == DRAGLEFT)
	{
		if(activeObject->mObject->getWidth() >= 50)	{
			activeObjectRect.left += dx;
			activeObject->mObject->scale(-dx, 0);
			activeObject->mObject->setXY(activeObjectRect.left + activeObject->mObject->getWidth()/2, activeObject->mObject->getY());
			
		}
	}	
	else if(drag == DRAGRIGHT)
	{
		if(activeObject->mObject->getWidth() >= 50)	{
			activeObjectRect.right += dx;
			activeObject->mObject->scale(dx, 0);		// drar man musen åt höger ökar bredden
			activeObject->mObject->setXY(activeObjectRect.left + activeObject->mObject->getWidth()/2, activeObject->mObject->getY());
		}
	}
	else if(drag == DRAGUP)
	{
		if(activeObject->mObject->getHeight() >= 50)	{
			activeObjectRect.top += dy;
			activeObject->mObject->scale(0, -dy);
			activeObject->mObject->setXY(activeObject->mObject->getX(), activeObjectRect.top + activeObject->mObject->getHeight()/2);
		}
	}
	else if(drag == DRAGDOWN)
	{	
		if(activeObject->mObject->getHeight() >= 50)	{
			activeObjectRect.bottom += dy;
			activeObject->mObject->scale(0, dy);
			activeObject->mObject->setXY(activeObject->mObject->getX(), activeObjectRect.top + activeObject->mObject->getHeight()/2);
		}
	}
	messageHandler(ACTIVE_OBJECT);
}

bool Editor::objectSnapping(Object *object, float dx, float dy)
{
	Object testObject = *activeObject->mObject;
	RECT activeObjectRect = activeObject->mObject->getRect();
	RECT snapObjectRect;
	bool toReturn = false;
	int snapDist;

	// om aktiv plattform inte är i någon annan plattform
	if(mLevel->objectIntersection(activeObject->mObject) == NULL)
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
		testObject = *activeObject->mObject;
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
		testObject = *activeObject->mObject;
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
		testObject = *activeObject->mObject;
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
		RECT activeObjectRect = activeObject->mObject->getRect();
		RECT snappedObjectRect = snappedObject->getRect();
		if(activeObjectRect.top <= snappedObjectRect.bottom && activeObjectRect.bottom >= snappedObjectRect.top && activeObjectRect.right >= snappedObjectRect.left &&
			activeObjectRect.left <= snappedObjectRect.right && activeObject->mObject->getID() != snappedObject->getID())
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
			if(activeObject->mObject != NULL)	{
				int x;

				sprintf(temp, "%s", data.c_str());
				x = atoi(temp);
				if(x <= GAME_WIDTH && x >= 0)
					activeObject->mObject->setXY(x, activeObject->mObject->getY());
				else
					activeObject->mObject->setXY(GAME_WIDTH, activeObject->mObject->getY());

				updateDragRects();
			}
			break;
		}
	case INPUT_YPOS:
		{
			if(activeObject->mObject != NULL)
			{
				int y;

				sprintf(temp, "%s", data.c_str());
				y = atoi(temp);
				if(y <= GAME_HEIGHT && y >= 0)
					activeObject->mObject->setXY(activeObject->mObject->getX(), y);
				else
					activeObject->mObject->setXY(activeObject->mObject->getX(), GAME_HEIGHT);

				updateDragRects();
			}
			break;
		}
	case INPUT_WIDTH:
		{
			if(activeObject->mObject != NULL)
			{
				int width;

				sprintf(temp, "%s", data.c_str());
				width = atoi(temp);
				activeObject->mObject->setWidth(width);

				updateDragRects();
			}
			break;
		}
	case INPUT_HEIGHT:
		{	
			if(activeObject->mObject != NULL)
			{
				int height;

				sprintf(temp, "%s", data.c_str());
				height = atoi(temp);
				activeObject->mObject->setHeight(height);

				updateDragRects();
			}
			break;
		}
	case INPUT_ENDX:
		{
			if(activeObject->mObject != NULL)
			{
				POS endPos;
				endPos = activeObject->mMovingPlatform->getEndPos();

				sprintf(temp, "%s", data.c_str());
				endPos.x = atoi(temp);
				activeObject->mMovingPlatform->setEndPos(endPos);

				//updateMovingPath();
			}
			break;
		}
	case INPUT_SPEED:
		{
			if(activeObject->mObject != NULL)
			{
				float speed;

				sprintf(temp, "%s", data.c_str());
				speed = atof(temp);

				activeObject->mMovingPlatform->setSpeed(speed);
			}
			break;
		}
	case INPUT_SPAWNX:
		{
			POS spawn = mLevel->getSpawn();

			sprintf(temp, "%s", data.c_str());
			spawn.x = atoi(temp);

			mLevel->setSpawn(spawn);
			break;
		}
	case INPUT_SPAWNY:
		{
			POS spawn = mLevel->getSpawn();

			sprintf(temp, "%s", data.c_str());
			spawn.y = atoi(temp);

			mLevel->setSpawn(spawn);
			break;
		}
	case DROPBOX_TEXTURE:
		{
			if(activeObject->mObject != NULL)
			{
				strcpy(buffer, data.c_str());
				if(strcmp(buffer, "grass_platform") == 0)
					activeObject->mObject->setTextureSource("misc\\textures\\grass_platform.bmp");
				if(strcmp(buffer, "brick_platform") == 0)
					activeObject->mObject->setTextureSource("misc\\textures\\brick_platform.bmp");
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
						POS start;
						POS end;
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
			if(activeObject->mObject != NULL)	{
				if(activeObject->mObject->getType() == STATIC_PLATFORMA)
					mLevel->deleteStaticObject(activeObject->mObject->getID());	// ska lägga till för dynamic också!
				else if(activeObject->mObject->getType() == MOVING_PLATFORM)
					mLevel->deleteDynamicObject(activeObject->mObject->getID());
				resetInputBoxes();
				activeObject->clear();
			}
			break;
		}
	case BUTTON_SAVE:
		{
			strcpy(buffer, ACTIVE_LEVEL.c_str());
			mLevel->saveLevel(buffer);
			break;
		}
	case BUTTON_TRYLEVEL:
		{
			strcpy(buffer, ACTIVE_LEVEL.c_str());
			mLevel->saveLevel(buffer);
			tryLevel = true;
			break;
		}
	case ACTIVE_OBJECT:
		{
			// updatera inputboxarna 

			// coords
			sprintf(buffer, "%i", (int)activeObject->mObject->getX());
			iPositionX->setValue(buffer); 

			sprintf(buffer, "%i", (int)activeObject->mObject->getY());
			iPositionY->setValue(buffer); 

			sprintf(buffer, "%i", activeObject->mObject->getWidth());
			iWidth->setValue(buffer); 

			sprintf(buffer, "%i", activeObject->mObject->getHeight());
			iHeight->setValue(buffer);

			// update texture source
			sprintf(buffer, "%s", activeObject->mObject->getTextureSource());
			if(strcmp(buffer, "misc\\textures\\grass_platform.bmp") == 0)
				textureDropBox->setValue("grass_platform");
			else if(strcmp(buffer, "misc\\textures\\brick_platform.bmp") == 0)
				textureDropBox->setValue("brick_platform");

			// if moving platform
			// updatera start, end och speed

			if(activeObject->mObject->getType() == MOVING_PLATFORM)
			{			
				POS tmpPoint = activeObject->mMovingPlatform->getStartPos();
				sprintf(buffer, "%i", (int)tmpPoint.x);
				iStartX->setValue(buffer);

				sprintf(buffer, "%i", (int)tmpPoint.y);
				iStartY->setValue(buffer);

				tmpPoint = activeObject->mMovingPlatform->getEndPos();

				sprintf(buffer, "%i", (int)tmpPoint.x);
				iEndX->setValue(buffer);

				sprintf(buffer, "%i", (int)tmpPoint.y);
				iEndY->setValue(buffer);

				sprintf(buffer, "%.2f", activeObject->mMovingPlatform->getSpeed());
				iSpeed->setValue(buffer);

				// tmpPoint = endPos
				//updateMovingPath();
			}

			updateDragRects();
			break;
		}
	case CHECKBOX_SHOWPATH:
		{
			strcpy(buffer, data.c_str());
			if(strcmp(buffer, "True") == 0)
				showPaths = true;
			else if(strcmp(buffer, "False") == 0)
				showPaths = false;
			break;
		}
	case MOVE_SPAWNPOS:
		{
			// spawnPos 

			POS spawnPos = mLevel->getSpawn();
			sprintf(buffer, "%i", (int)mLevel->getSpawn().x);
			iSpawnX->setValue(buffer);
			sprintf(buffer, "%i", (int)mLevel->getSpawn().y);
			iSpawnY->setValue(buffer);
		}
	}
	
}

void Editor::moveEndPos(void)
{
	float dx = gDInput->mouseDX();
	float dy = gDInput->mouseDY();
	POS endPos;

	endPos = activeObject->mMovingPlatform->getEndPos();

	endPos.x += dx;
	activeObject->mMovingPlatform->setEndPos(endPos);
	//updateMovingPath();
	messageHandler(ACTIVE_OBJECT);
}

void Editor::moveSpawnPos(void)
{
	float dx = gDInput->mouseDX();
	float dy = gDInput->mouseDY();

	POS spawnPos = mLevel->getSpawn();

	spawnPos.x += dx;
	spawnPos.y += dy;

	mLevel->setSpawn(spawnPos);
}


