#include "DropBox.h"
#include "Editor.h"
#include "StaticPlatform.h"
#include "MovingPlatform.h"
#include "Enemy.h"
#include "Camera.h"

// Window behöver ingen mus längre
// Editor ska ha den 

extern Camera* gGameCamera;

Editor::Editor() : Window(NULL, EDITOR, 1100, 400, 200, 800), SNAP_SENSE(30), SNAP_DIST(10)
{
	mLevel = new Level(NULL);
	
	gameArea.top = 0;
	gameArea.bottom = GAME_HEIGHT;
	gameArea.left = 0;
	gameArea.right = GAME_WIDTH;

	activeObject = new ActiveObject();

	snapCount = SNAP_SENSE;
	snapDir = ALL;
	snappedObject = NULL;
	movingObject = false;
	movingEndPos = false;
	movingSpawnPos = false;
	showPaths = false;
	tryLevel = false;
	currentAction = IDLE;

	createObjectTextPos = 290;
	mPrevActiveObjectType = NO_OBJECT;

	test = new char[256];
	test = "test";
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

	listBox = new ListBox(this, LISTBOX_OBJECTTYPE, 66, 340, 110, 60);	// shouldn't take height, should expand on addItem

	createButton = new Button(this, BUTTON_CREATE, "Create", 40, 420, 60, 20, D3DCOLOR_ARGB(255, 90, 140, 140));
	deleteButton = new Button(this, BUTTON_DELETE, "Delete", 40, 270, 60, 20, D3DCOLOR_ARGB(255, 90, 140, 140));
	saveButton = new Button(this, BUTTON_SAVE, "Save", 110, 420, 60, 20, D3DCOLOR_ARGB(255, 90, 140, 140));
	bTryLevel = new Button(this, BUTTON_TRYLEVEL, "Test", 40, 630, 60, 20, D3DCOLOR_ARGB(255, 90, 140, 140));

	textureDropBox = new DropBox(this, DROPBOX_TEXTURE, 76, 240, 130, 20, 20);

	pathCheckBox = new CheckBox(this, CHECKBOX_SHOWPATH, "Show paths: ", 110, 605, 16, 16);

	listBox->addItem("Static Platform", 22, D3DCOLOR_ARGB( 255, 230, 230, 230 ));
	listBox->addItem("Moving Platform", 22, D3DCOLOR_ARGB( 255, 200, 200, 200 ));
	listBox->addItem("Enemy", 22, D3DCOLOR_ARGB( 255, 230, 230, 230));

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
	//screenMousePos = mMouse->getPos();
	POINT tmpMousePos = mMouse->getPos();
	mOffset = gGameCamera->getOffset(); // the delta of the camera from it's orginal position ( center of the screen ) 

	//mLevel->updateLevel(dt);
	
	MovingPlatform *tmpPlatform;

	// mousewheel scaling
	if(activeObject->mObject != NULL)
	{
		if(activeObject->mObject->getResizeable())
		{
			if(gDInput->mouseDZ() > 0)	{
				activeObject->mObject->scale(ALL, 8, 8);

				// messageHandler(OBJECT_SCALED)
				messageHandler(ACTIVE_OBJECT);
			}
			else if(gDInput->mouseDZ() < 0)	{
				if(activeObject->mObject->getHeight() > 50 && activeObject->mObject->getWidth() > 50)	{

					activeObject->mObject->scale(ALL, -8, -8);
					messageHandler(ACTIVE_OBJECT);
				}
			}
		}
	}
		
	// select platform
	if(gDInput->mouseButtonPressed(LEFTBUTTON))	{
		//if(mousePos.x > gameArea.left && mousePos.x < gameArea.right && mousePos.y > gameArea.top)	// gameArea == true
		if(mMouse->getScreenPos().x > gameArea.left && mMouse->getScreenPos().x < gameArea.right && mMouse->getScreenPos().y > gameArea.top)	// just 3, works though
		{	
			Object *tmpObject;
			tmpObject = NULL;
			
			if(activeObject->mObject != NULL)
			{
				// if not marked the activeObject
				if(!(tmpMousePos.x > activeObject->mObject->getRect().left && tmpMousePos.x < activeObject->mObject->getRect().right
					&& tmpMousePos.y > activeObject->mObject->getRect().top && tmpMousePos.y < activeObject->mObject->getRect().bottom))
				{
					if(activeObject->mMovingPlatform || activeObject->mEnemy)
					{
						if(activeObject->mMovingPlatform)	{
							if(!(tmpMousePos.x > activeObject->mMovingPlatform->getEndPosRect().left && tmpMousePos.x < activeObject->mMovingPlatform->getEndPosRect().right
							&& tmpMousePos.y > activeObject->mMovingPlatform->getEndPosRect().top && tmpMousePos.y < activeObject->mMovingPlatform->getEndPosRect().bottom))
								tmpObject = mLevel->getObjectAt(tmpMousePos);
						}
						else if(activeObject->mEnemy)	{
							if(!(tmpMousePos.x > activeObject->mEnemy->getEndPosRect().left && tmpMousePos.x < activeObject->mEnemy->getEndPosRect().right
							&& tmpMousePos.y > activeObject->mEnemy->getEndPosRect().top && tmpMousePos.y < activeObject->mEnemy->getEndPosRect().bottom))
								tmpObject = mLevel->getObjectAt(tmpMousePos);
						}
					}
					else	{
						tmpObject = mLevel->getObjectAt(tmpMousePos);
					}
				}
				else	// if the active object is pressed
					tmpObject = activeObject->mObject;
			}
			else if(activeObject->mObject == NULL)
				tmpObject = mLevel->getObjectAt(tmpMousePos);

			if(tmpObject != NULL)
			{
				activeObject->setObject((tmpObject));
				tmpObject = NULL;
			}
			else if(activeObject->mMovingPlatform || activeObject->mEnemy)	
			{
				if(activeObject->mMovingPlatform)	{
					if(!(tmpMousePos.x > activeObject->mMovingPlatform->getEndPosRect().left && tmpMousePos.x < activeObject->mMovingPlatform->getEndPosRect().right
					&& tmpMousePos.y > activeObject->mMovingPlatform->getEndPosRect().top && tmpMousePos.y < activeObject->mMovingPlatform->getEndPosRect().bottom))
						activeObject->clear();
				}
				else if(activeObject->mEnemy)	{
					if(!(tmpMousePos.x > activeObject->mEnemy->getEndPosRect().left && tmpMousePos.x < activeObject->mEnemy->getEndPosRect().right
					&& tmpMousePos.y > activeObject->mEnemy->getEndPosRect().top && tmpMousePos.y < activeObject->mEnemy->getEndPosRect().bottom))
						activeObject->clear();
				}
			}
			else
				activeObject->clear();

			if(activeObject->mObject != NULL)	
			{
				movingSpawnPos = false;
				// nollställ
				snapDir = ALL;
				snapCount = SNAP_SENSE;

				// update inputboxes - med activPlatforms värden ;d
				messageHandler(ACTIVE_OBJECT);

				// static eller dynamic platform?
				if((activeObject->mObject->getType() == MOVING_PLATFORM || activeObject->mObject->getType() == NORMAL_ENEMY) && (mPrevActiveObjectType != MOVING_PLATFORM && mPrevActiveObjectType != NORMAL_ENEMY))
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

					if(activeObject->mObject->getType() == MOVING_PLATFORM)
						mPrevActiveObjectType = MOVING_PLATFORM;			
					else if(activeObject->mObject->getType() == NORMAL_ENEMY)
						mPrevActiveObjectType = NORMAL_ENEMY;
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
				if(tmpMousePos.x > spawnRect.left && tmpMousePos.x < spawnRect.right && tmpMousePos.y > spawnRect.top && tmpMousePos.y < spawnRect.bottom)	{
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
			sendMousePress(mMouse->getScreenPos().x, mMouse->getScreenPos().y);
		}
		// initiera/updatera dragAreas
		if(activeObject->mObject != NULL)
		{
			updateDragRects();
		}
	}
	// move, resize, and change endPos of active object
	// move the camera
	if(gDInput->mouseButtonDown(LEFTBUTTON))			// HAXING ITW
	{
		// if mousedown
		if(activeObject->mObject != NULL)
		{
			RECT activeObjectRect = activeObject->mObject->getRect();
			//if(mousePos.x > gameArea.left && mousePos.x < gameArea.right && mousePos.y > gameArea.top && mousePos.y < gameArea.bottom)
			//{	
				// change end-pos
				if(activeObject->mObject->getType() == MOVING_PLATFORM)
				{
					// moves the end pos, now also works even if the mouse is a bit behind
					if(currentAction == MOVING_ENDPOS)
						moveEndPos();
					else if(currentAction == IDLE && tmpMousePos.x > activeObject->mMovingPlatform->getEndPosRect().left && tmpMousePos.x < activeObject->mMovingPlatform->getEndPosRect().right
						&& tmpMousePos.y > activeObject->mMovingPlatform->getEndPosRect().top && tmpMousePos.y < activeObject->mMovingPlatform->getEndPosRect().bottom)		
					{			
						currentAction = MOVING_ENDPOS;
						moveEndPos();						
					}
				}

				if(activeObject->mObject->getType() == NORMAL_ENEMY)
				{
					if(currentAction == MOVING_ENDPOS)
						moveEndPos();
					else if(currentAction == IDLE && tmpMousePos.x > activeObject->mEnemy->getEndPosRect().left && tmpMousePos.x < activeObject->mEnemy->getEndPosRect().right
						&& tmpMousePos.y > activeObject->mEnemy->getEndPosRect().top && tmpMousePos.y < activeObject->mEnemy->getEndPosRect().bottom)
					{				
						currentAction = MOVING_ENDPOS;
						moveEndPos();							
					}
				}

				// resize
				// the object should even if the mouse is outside the box, that is when currentAction == SCALE_XXXX
				if(activeObject->mObject->getResizeable()) 
				{
					// will do for now, could check for IDLE instead
					if(currentAction != MOVING_OBJECT && currentAction != MOVING_ENDPOS)
					{	
						if(currentAction == SCALE_LEFT)
							resizePlatform(DRAGLEFT);
						else if(currentAction == IDLE && tmpMousePos.x > dragLeft.left && tmpMousePos.x < dragLeft.right && tmpMousePos.y > dragLeft.top && tmpMousePos.y < dragLeft.bottom)	{
								resizePlatform(DRAGLEFT);
								currentAction = SCALE_LEFT;
						}
						else if(currentAction == SCALE_RIGHT)
							resizePlatform(DRAGRIGHT);
						else if(currentAction == IDLE && tmpMousePos.x > dragRight.left && tmpMousePos.x < dragRight.right && tmpMousePos.y > dragRight.top && tmpMousePos.y < dragRight.bottom)	{
								resizePlatform(DRAGRIGHT);
								currentAction = SCALE_RIGHT;
						}
						else if(currentAction == SCALE_UP)
							resizePlatform(DRAGUP);
						else if(currentAction == IDLE && tmpMousePos.x > dragTop.left && tmpMousePos.x < dragTop.right && tmpMousePos.y > dragTop.top && tmpMousePos.y < dragTop.bottom)	{
								resizePlatform(DRAGUP);
								currentAction = SCALE_UP;
						}
						else if(currentAction == SCALE_DOWN)
							resizePlatform(DRAGDOWN);
						else if(currentAction == IDLE && tmpMousePos.x > dragBottom.left && tmpMousePos.x < dragBottom.right && tmpMousePos.y > dragBottom.top && tmpMousePos.y < dragBottom.bottom)	{
								resizePlatform(DRAGDOWN);
								currentAction = SCALE_DOWN;
						}
					}
				}

			// move the object
			if(currentAction == MOVING_OBJECT)
				movePlatform();
			else if(currentAction == IDLE && tmpMousePos.x > activeObjectRect.left && tmpMousePos.x < activeObjectRect.right && tmpMousePos.y > activeObjectRect.top && tmpMousePos.y < activeObjectRect.bottom)
			{
				currentAction = MOVING_OBJECT;
				movePlatform();
			}			
		}

		if(movingSpawnPos)	{
			moveSpawnPos();
			messageHandler(MOVE_SPAWNPOS);			
		}
	}

	// scroll knapp nere -> rör kamera
	if(gDInput->mouseButtonDown(MIDDLEBUTTON))
	{
		// shouldn't be able to move outside to the left
		if(gGameCamera->getX() >= 500 || (gGameCamera->getX() < 500 && gDInput->mouseDX() < 0))	{
			gGameCamera->move(-gDInput->mouseDX(), 0);
			mMouse->setVX(tmpMousePos.x - gDInput->mouseDX());
		}
	}

	// set the currentAction to IDLE
	if(gDInput->mouseButtonReleased(LEFTBUTTON) && currentAction != IDLE)
	{
			currentAction = IDLE;
	}

	// check keyboard inputs
	if(gDInput->keyPressed(DIK_P))
		messageHandler(BUTTON_DELETE);

	return 1;
}
// körs när man tar tag i markerad plattform
void Editor::movePlatform(void)
{		
	RECT activeObjectRect = activeObject->mObject->getRect();
		// updatera koordinater
		float dx = gDInput->mouseDX();
		float dy = gDInput->mouseDY();

		// så musen inte rör sig vid kanten
		/*if(activeObjectRect.left <= 0 || activeObjectRect.right >= GAME_WIDTH)
				mMouse->setMousePos(mousePos.x - dx , mousePos.y);
			if(activeObjectRect.top <= 0 || activeObjectRect.bottom >= GAME_HEIGHT)
				mMouse->setMousePos(mousePos.x , mousePos.y - dy);*/

		// kolla begränsningar - true -> gameArea + ingen krock med annan plattform!
		//if(activeObjectRect.left >= 0 && activeObjectRect.right <= GAME_WIDTH && activeObjectRect.top >= 0 && activeObjectRect.bottom <= GAME_HEIGHT)
		//{
				
			if(!objectSnapping(activeObject->mObject, dx, dy))
			{
				if(snapDir == ALL)	// skulle kännas bättre med NONE
				{
					activeObject->move(dx, dy);						
				}
				else if(snapDir == LEFT || snapDir == RIGHT)
				{
					// dx kan vara båda + att om den inte är nära snapped ska den röra sig fritt
					if(snapCount >= SNAP_SENSE || snapCount <= -SNAP_SENSE )// || stillSnapped() == false)	// no longer snapped
					{		
						test = "stillSnapped() == false";
						mMouse->move(0, -dy);
						activeObject->move(dx, dy);
						snapDir = ALL;
					}
					else	{	// snapped, don't move the object or mouse
						test = "dont move mouse";
						snapCount += dx;
						// musens ska inte röra på sig!
						mMouse->move(-dx, 0);
						// ska kunna röra plattformen lodrätt
						activeObject->move(0, dy);
					}	
				}
				else if(snapDir == UP || snapDir == DOWN)
				{
					if(snapCount >= SNAP_SENSE || snapCount <= -SNAP_SENSE)// || stillSnapped() == false)
					{
						activeObject->move(dx, dy);
						snapDir = ALL;
					}
					else	{
						snapCount += dy;
						//mMouse->setMousePos(mMouse->getPos().x , mMouse->getPos().y - dy);
						mMouse->move(0, -dy);
						activeObject->move(dx, 0);
					}					
				}	
			//}			
			
		}

		// sätt till kanten
		/*else
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
		}*/
		messageHandler(ACTIVE_OBJECT);
}

// just renders the GUI to the left
int Editor::renderGui()
{
	Window::renderAll();

	if(currentAction == MOVING_OBJECT)
		sprintf(buffer, "moving_platfrom");
	else if(currentAction == MOVING_ENDPOS)
		sprintf(buffer, "moving_endpos");
	else if(currentAction == SCALE_LEFT)
		sprintf(buffer, "scaling_left");
	else if(currentAction == IDLE)
		sprintf(buffer, "idling");

	gGraphics->drawText(buffer, 1050,530);

	gGraphics->drawText(test, GAME_WIDTH +10, 650);

	gGraphics->drawText("Spawn:", GAME_WIDTH +10, 7);
	gGraphics->drawText("Active object:", GAME_WIDTH +10, 90);
	gGraphics->drawText("Create object:", GAME_WIDTH +10, createObjectTextPos);

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
		if((activeObject->mObject->getWidth() >= 50 && dx > 0) || dx < 0)	{
			activeObjectRect.left += dx;
			activeObject->mObject->scale(LEFT, dx, 0);
			//activeObject->mObject->setXY(activeObjectRect.left + activeObject->mObject->getWidth()/2, activeObject->mObject->getY());			
		}
		else
			mMouse->setMousePos(mMouse->getPos().x - dx, mMouse->getPos().y);
	}	
	else if(drag == DRAGRIGHT)
	{
		if((activeObject->mObject->getWidth() >= 50 && dx < 0) || dx > 0)	{
			activeObjectRect.right += dx;
			activeObject->mObject->scale(RIGHT, dx, 0);		// drar man musen åt höger ökar bredden
			activeObject->mObject->setXY(activeObjectRect.left + activeObject->mObject->getWidth()/2, activeObject->mObject->getY());
		}
		else
			mMouse->setMousePos(mMouse->getPos().x - dx, mMouse->getPos().y);
	}
	else if(drag == DRAGUP)
	{
		if((activeObject->mObject->getHeight() >= 50 && dy > 0) || dy < 0)	{
			activeObjectRect.top += dy;
			activeObject->mObject->scale(UP, 0, -dy);
			activeObject->mObject->setXY(activeObject->mObject->getX(), activeObjectRect.top + activeObject->mObject->getHeight()/2);
		}
		else
			mMouse->setMousePos(mMouse->getPos().x, mMouse->getPos().y - dy);
	}
	else if(drag == DRAGDOWN)
	{	
		if((activeObject->mObject->getHeight() >= 50 && dy < 0) || dy > 0)	{
			activeObjectRect.bottom += dy;
			activeObject->mObject->scale(DOWN, 0, dy);
			activeObject->mObject->setXY(activeObject->mObject->getX(), activeObjectRect.top + activeObject->mObject->getHeight()/2);
		}
		else
			mMouse->setMousePos(mMouse->getPos().x, mMouse->getPos().y - dy);
	}
	messageHandler(ACTIVE_OBJECT);
}

bool Editor::objectSnapping(Object *object, float dx, float dy)
{
	Object testObject = *activeObject->mObject;
	RECT activeObjectRect = activeObject->mObject->getRect();
	POINT tmpMousePos = mMouse->getPos();
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
				mMouse->move(snapDist - dx, 0);
				activeObject->move(snapDist, 0);

				snapCount = 0;
				snapDir = LEFT;
				test = "snapped-right";
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
				mMouse->move(-snapDist - dx, 0);
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
				mMouse->move(0, snapDist - dy);
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
				mMouse->move(0, -snapDist - dy);
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
			activeObjectRect.left <= snappedObjectRect.right && activeObject->mObject->getID() != snappedObject->getID())	// ville nog egentligen ta reda på om id = last snap id
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
						mLevel->addObject(platform);
					}
					else if(value == "Moving Platform")
					{				
						POS start;
						POS end;
						start.x = 200;
						start.y = 300;
						end.x = 600;
						end.y = 300;
						MovingPlatform *platform = new MovingPlatform(200, 300, 100, 100, "misc\\textures\\grass_platform.bmp", start, end);
						mLevel->addObject(platform);
					}
					else if(value == "Enemy")
					{
						POS start;
						POS end;
						start.x = 200;
						start.y = 500;
						end.x = 600;
						end.y = 500;
						Enemy *enemy = new Enemy(200, 500, 36, 36, "misc\\textures\\bad_mario.bmp", start, end);
						mLevel->addObject(enemy);
					}
					// aktiv plattform = den nya?
			}
			break;
		}
	case BUTTON_DELETE:
		{		
			if(activeObject->mObject != NULL)	{
				if(activeObject->mObject->getType() == STATIC_PLATFORMA)
					mLevel->deleteObject(activeObject->mObject->getID());	// ska lägga till för dynamic också!
				else if(activeObject->mObject->getType() == MOVING_PLATFORM || activeObject->mObject->getType() == NORMAL_ENEMY)
					mLevel->deleteObject(activeObject->mObject->getID());
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
			mMouse->restore();
			gGameCamera->restore();
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

			sprintf(buffer, "%i", (int)activeObject->mObject->getWidth());
			iWidth->setValue(buffer); 

			sprintf(buffer, "%i", (int)activeObject->mObject->getHeight());
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

	if(activeObject->mMovingPlatform)
	{
		endPos = activeObject->mMovingPlatform->getEndPos();

		endPos.x += dx;
		activeObject->mMovingPlatform->setEndPos(endPos);
	}
	else if(activeObject->mEnemy)
	{
		endPos = activeObject->mEnemy->getEndPos();

		endPos.x += dx;
		activeObject->mEnemy->setEndPos(endPos);
	}
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

int Editor::renderLevel(void)
{
	if(showPaths)
		mLevel->drawEditorLevel();
	else if(!showPaths)
		mLevel->drawLevel();

	POS spawnPos = mLevel->getSpawn();
	gGraphics->BlitRect(spawnPos.x, spawnPos.y, USER_WIDTH, USER_HEIGHT, D3DCOLOR_ARGB(220, 220, 40, 0));

	// displays the orange effect
	if(activeObject->mObject != NULL)	{
		gGraphics->BlitRect(activeObject->mObject->getRect(), D3DCOLOR_ARGB(150, 255, 166, 0));

		// diplays the drag areas in different colours
		if(activeObject->mObject->getResizeable())
		{
			sprintf(buffer, "drag: %i", dragLeft.right);
			//MessageBox(0, buffer, 0, 0);
			

			gGraphics->BlitRect(dragLeft, D3DCOLOR_ARGB(220, 130, 166, 255));
			gGraphics->BlitRect(dragRight, D3DCOLOR_ARGB(220, 130, 166, 255));
			gGraphics->BlitRect(dragTop, D3DCOLOR_ARGB(220, 130, 166, 255));
			gGraphics->BlitRect(dragBottom, D3DCOLOR_ARGB(220, 130, 166, 255));
		}

		// displays the path of the moving object
		if((activeObject->mObject->getType() == MOVING_PLATFORM || activeObject->mObject->getType() == NORMAL_ENEMY)&& !showPaths)
		{
			bool drawPath = true;
			RECT activeObjectRect = activeObject->mObject->getRect();
			POS endPos;
			if(activeObject->mMovingPlatform)
				endPos = activeObject->mMovingPlatform->getEndPos();
			else if(activeObject->mEnemy)
				endPos = activeObject->mEnemy->getEndPos();

			RECT pathRect;
			if(endPos.x > activeObject->mObject->getX())
			{
				// inside object -> dont draw path
				if(endPos.x <= activeObject->mObject->getX() + activeObject->mObject->getWidth())
					drawPath = false;
				else	{
					pathRect.left = activeObjectRect.right;
					pathRect.right = endPos.x;
					pathRect.top = activeObject->mObject->getY() + activeObject->mObject->getHeight()/2 - 5;
					pathRect.bottom = activeObject->mObject->getY() + activeObject->mObject->getHeight()/2 + 5;
				}
			}
			else if(endPos.x < activeObject->mObject->getX())
			{
				// inside object -> dont draw path
				if(endPos.x + activeObject->mObject->getWidth() >= activeObject->mObject->getX())
					drawPath = false;
				else	{
					pathRect.left = endPos.x + activeObject->mObject->getWidth();
					pathRect.right = activeObjectRect.left;
					pathRect.top = activeObject->mObject->getY() + activeObject->mObject->getHeight()/2 - 5;
					pathRect.bottom = activeObject->mObject->getY() + activeObject->mObject->getHeight()/2 + 5;
				}
			}
			else
				pathRect = activeObjectRect;

			// dont draw if inside object
			if(drawPath)
				gGraphics->BlitRect(pathRect, D3DCOLOR_ARGB(150, 0, 166, 255));

			// displays the end pos, and the drag rect of the active object
			if(activeObject->mMovingPlatform)
				gGraphics->BlitRect(activeObject->mMovingPlatform->getEndPosRect(), D3DCOLOR_ARGB(150, 255, 166, 0));
			else if(activeObject->mEnemy)
				gGraphics->BlitRect(activeObject->mEnemy->getEndPosRect(), D3DCOLOR_ARGB(150, 255, 166, 0));
			
		}
	}
	return 1;
}


