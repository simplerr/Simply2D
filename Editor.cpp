#include <stdio.h>
#include "DropBox.h"
#include "Editor.h"
#include "StaticPlatform.h"
#include "MovingPlatform.h"
#include "WallJump.h"
#include "Enemy.h"
#include "Teleport.h"
#include "Trampoline.h"
#include "Spike.h"
#include "CameraManager.h"
#include "WindowHandler.h"


// Window behöver ingen mus längre
// Editor ska ha den 

extern CameraManager* gCameraManager;
extern Mouse* gMouse;

Editor::Editor() : SNAP_SENSE(30), SNAP_DIST(10)
{
	mWindowHandler = new WindowHandler(1300, 450, 200, 900);
	mLevel = new Level(NULL);
	
	gameArea.top = 0;
	gameArea.bottom = GAME_HEIGHT;
	gameArea.left = 0;
	gameArea.right = GAME_WIDTH;

	mActiveObject = NULL;

	snapCount = SNAP_SENSE;
	snapDir = ALL;
	snappedObject = NULL;
	movingObject = false;
	movingEndPos = false;
	movingSpawnPos = false;
	movingWarp = false;
	showPaths = false;
	tryLevel = false;
	currentAction = IDLE;

	createObjectTextPos = 290;
	mPrevActiveObjectType = NO_OBJECT;
}
Editor::~Editor()
{
	//mLevel->saveLevel("level_1.txt")
	delete mLevel;
	delete mWindowHandler;
}


void Editor::addPropertyPair(Property prop)
{
	PropertyPair tmpPair;
	int y = 178 + 30 * mPropertyPairs.size();

	tmpPair.name = new TextBox(mWindowHandler, OBJECT_INFO, prop.name, 40, y, 60, 20);
	tmpPair.value = new InputBox(mWindowHandler, OBJECT_INFO, 110, y, 60, 20, 4);
	tmpPair.value->setValue(prop.value);
	tmpPair.value->connect(&Editor::messageHandler, this);

	mPropertyPairs.push_back(tmpPair);

	mPropertyCount++;
}

void Editor::buildGUI(void)
{
	int OFFSET = 55;

	// should only allow input of 11 characters
	tLevel = new TextBox(mWindowHandler, TEXT_LEVEL, "Level:", 40, 22, 60, 20);
	iLevel = new InputBox(mWindowHandler, INPUT_LEVEL, 125, 22, 90, 20, 15);
	iLevel->connect(&Editor::messageHandler, this);
	iLevel->setValue("");

	tNextLevel = new TextBox(mWindowHandler, TEXT_NEXT, "Next:", 40, 50, 60, 20);
	iNextLevel = new InputBox(mWindowHandler, INPUT_NEXT, 125, 50, 90, 20, 15);
	iNextLevel->connect(&Editor::messageHandler, this);
	iNextLevel->setValue("");

	tSpawnX = new TextBox(mWindowHandler, TEXT_SPAWNX, "X:", 40, 95, 60, 20);
	tSpawnY = new TextBox(mWindowHandler, TEXT_SPAWNY, "Y:", 40, 125, 60, 20);

	iSpawnX = new InputBox(mWindowHandler, INPUT_SPAWNX, 110, 95, 60, 20, 4);
	iSpawnX->connect(&Editor::messageHandler, this);

	iSpawnY = new InputBox(mWindowHandler, INPUT_SPAWNY, 110, 125, 60, 20, 4);
	iSpawnY->connect(&Editor::messageHandler, this);

	// sets the values to the spawnPos!
	POS spawnPos = mLevel->getSpawn();
	sprintf(buffer, "%i", (int)mLevel->getSpawn().x);
	iSpawnX->setValue("");
	sprintf(buffer, "%i", (int)mLevel->getSpawn().y);
	iSpawnY->setValue("");

	listBox = new ListBox(mWindowHandler, LISTBOX_OBJECTTYPE, 76, 490 + OFFSET, 130, 155);	// shouldn't take height, should expand on addItem
	listBox->connect(&Editor::messageHandler, this);

	createButton = new Button(mWindowHandler, BUTTON_CREATE, "Create", 40, 642 + OFFSET, 60, 20, D3DCOLOR_ARGB(255, 90, 140, 140));
	createButton->connect(&Editor::messageHandler, this);

	deleteButton = new Button(mWindowHandler, BUTTON_DELETE, "Delete", 110, 642 + OFFSET, 60, 20, D3DCOLOR_ARGB(255, 90, 140, 140));
	deleteButton->connect(&Editor::messageHandler, this);

	saveButton = new Button(mWindowHandler, BUTTON_SAVE, "Save", 110, 672 + OFFSET, 60, 20, D3DCOLOR_ARGB(255, 90, 140, 140));
	saveButton->connect(&Editor::messageHandler, this);

	bTryLevel = new Button(mWindowHandler, BUTTON_TRYLEVEL, "Test", 40, 672 + OFFSET, 60, 20, D3DCOLOR_ARGB(255, 90, 140, 140));
	bTryLevel->connect(&Editor::messageHandler, this);

	textureDropBox = new DropBox(mWindowHandler, DROPBOX_TEXTURE, 76, 612 + OFFSET, 130, 20, 20);
	textureDropBox->connect(&Editor::messageHandler, this);

	pathCheckBox = new CheckBox(mWindowHandler, CHECKBOX_SHOWPATH, "Show paths: ", 110, 697 + OFFSET, 16, 16);
	pathCheckBox->connect(&Editor::messageHandler, this);

	listBox->addItem("Static Platform", 22, D3DCOLOR_ARGB( 255, 230, 230, 230 ));
	listBox->addItem("Moving Platform", 22, D3DCOLOR_ARGB( 255, 200, 200, 200 ));
	listBox->addItem("Enemy", 22, D3DCOLOR_ARGB( 255, 230, 230, 230));
	listBox->addItem("Teleport", 22, D3DCOLOR_ARGB( 255, 200, 200, 200));
	listBox->addItem("Trampoline", 22, D3DCOLOR_ARGB( 255, 230, 230, 230));
	listBox->addItem("Walljump", 22, D3DCOLOR_ARGB( 255, 200, 200, 200));
	listBox->addItem("Spike", 22, D3DCOLOR_ARGB( 255, 230, 230, 230));

	textureDropBox->addItem("grass_platform", D3DCOLOR_ARGB( 255, 200, 200, 200 ));
	textureDropBox->addItem("brick_platform", D3DCOLOR_ARGB( 255, 230, 230, 230 ));
}

int Editor::updateAll(float dt)
{
	if(tryLevel)
		return -1;

	mWindowHandler->update(dt);
	POINT tmpMousePos = gMouse->getPos();
	mOffset = gCameraManager->gameCamera()->getOffset(); // the delta of the camera from it's orginal position ( center of the screen ) 
	
	MovingPlatform *tmpPlatform;

	// mousewheel scaling
	if(gDInput->mouseDZ() > 0)
	{
		if(mActiveObject != NULL)	{
			if(mActiveObject->getResizeable())	{
				mActiveObject->scale(ALL, 8, 8);

				messageHandler(OBJECT_UPDATED);
			}
		}
	}
	else if(gDInput->mouseDZ() < 0)	
	{
		if(mActiveObject != NULL)	{
			if(mActiveObject->getResizeable())	{
				if(mActiveObject->getHeight() > 50 && mActiveObject->getWidth() > 50)	{
					mActiveObject->scale(ALL, -8, -8);

					messageHandler(OBJECT_UPDATED);
				}
			}
		}
	}
		
	// select platform
	if(gDInput->mouseButtonPressed(LEFTBUTTON))	{
		// inside game area?
		if(gMouse->getScreenPos().x > gameArea.left && gMouse->getScreenPos().x < gameArea.right && gMouse->getScreenPos().y > gameArea.top)
		{	
			Object *tmpObject;
			tmpObject = NULL;
			bool sameObjectSelected = false;

			if(mActiveObject != NULL)
			{
				// if not selected the activeObject
				if(!(tmpMousePos.x > mActiveObject->getRect().left && tmpMousePos.x < mActiveObject->getRect().right
					&& tmpMousePos.y > mActiveObject->getRect().top && tmpMousePos.y < mActiveObject->getRect().bottom))
				{
					// if not selected the end rect or dest tp of the active object
					if(mActiveObject->getAreaAt(tmpMousePos.x, tmpMousePos.y) == OUTSIDE)
						tmpObject = mLevel->getObjectAt(tmpMousePos);
					else
						sameObjectSelected = true;
				}
				else	// if the active object is pressed
					sameObjectSelected = true;
			}
			else if(mActiveObject == NULL)
				tmpObject = mLevel->getObjectAt(tmpMousePos);

			// new object selected
			if(tmpObject != NULL && !sameObjectSelected)
			{				
				mActiveObject = tmpObject;
				tmpObject = NULL;
				messageHandler(OBJECT_SELECTED);
			}
			// clicked in space
			else if(tmpObject == NULL && !sameObjectSelected)	{

				if(mActiveObject != NULL)	{
					if(mActiveObject->getAreaAt(tmpMousePos.x, tmpMousePos.y) != END_RECT)	{
						mActiveObject = NULL;
					}
				}						
			}

			if(mActiveObject != NULL)	
			{
				movingSpawnPos = false;
				snapDir = ALL;
				snapCount = SNAP_SENSE;
				messageHandler(OBJECT_SELECTED);	
			}		
			else
			{	
				// moving the spawn pos?
				RECT spawnRect = mLevel->getSpawnRect();
				if(tmpMousePos.x > spawnRect.left && tmpMousePos.x < spawnRect.right && tmpMousePos.y > spawnRect.top && tmpMousePos.y < spawnRect.bottom)	{
					movingSpawnPos = true;
				}
				else
					movingSpawnPos = false;

				RECT warpRect = mLevel->getWarp()->getRect();
				if(tmpMousePos.x > warpRect.left && tmpMousePos.x < warpRect.right && tmpMousePos.y > warpRect.top && tmpMousePos.y < warpRect.bottom)
					mActiveObject = mLevel->getWarp();
				
				// reset inputboxes
				resetInputBoxes();
			}
		}
		else
		{
			movingSpawnPos = false;
		}
	}
	// move, camera, resize, endPos of mActiveObject
	if(gDInput->mouseButtonDown(LEFTBUTTON))		
	{
		// move the spawn pos
		if(movingSpawnPos)	{
			moveSpawnPos();
			messageHandler(MOVE_SPAWNPOS);			
		}

		if(movingWarp)	{
			//mLevel->getWarp().x += gDInput->mouseDX();
			//mLevel->getWarp().y += gDInput->mouseDY();
		}

		if(mActiveObject != NULL)
		{
			RECT activeObjectRect = mActiveObject->getRect();
			ObjectArea areaType = mActiveObject->getAreaAt(tmpMousePos.x, tmpMousePos.y);			

			if(areaType != OUTSIDE || currentAction != IDLE)
			{
				// move dest tp
				// make a move(ObjectArea, dx, dy) function? will save some dynamic_casts!
				if((areaType == TP_DESTAREA && currentAction == IDLE) || currentAction == MOVING_TPDEST)
				{
					Teleport *tmpTp = dynamic_cast<Teleport*>(mActiveObject);
					tmpTp->moveDest(gDInput->mouseDX(), gDInput->mouseDY());
					currentAction = MOVING_TPDEST;
					messageHandler(OBJECT_UPDATED);
				}

				// resize it
				if(mActiveObject->getResizeable())
				{
					if((currentAction == SCALE_LEFT || currentAction == SCALE_RIGHT || currentAction == SCALE_UP || currentAction == SCALE_DOWN || currentAction == IDLE)
						|| (areaType == DRAG_LEFT || areaType == DRAG_RIGHT || areaType == DRAG_UP || areaType == DRAG_DOWN))
					{
							if(currentAction == SCALE_LEFT)
								resizePlatform(DRAGLEFT);
							else if(currentAction == IDLE && areaType == DRAG_LEFT)	{
									resizePlatform(DRAGLEFT);
									currentAction = SCALE_LEFT;
							}
							else if(currentAction == SCALE_RIGHT)
								resizePlatform(DRAGRIGHT);
							else if(currentAction == IDLE && areaType == DRAG_RIGHT)	{
									resizePlatform(DRAGRIGHT);
									currentAction = SCALE_RIGHT;
							}
							else if(currentAction == SCALE_UP)
								resizePlatform(DRAGUP);
							else if(currentAction == IDLE && areaType == DRAG_UP)	{
									resizePlatform(DRAGUP);
									currentAction = SCALE_UP;
							}
							else if(currentAction == SCALE_DOWN)
								resizePlatform(DRAGDOWN);
							else if(currentAction == IDLE && areaType == DRAG_DOWN)	{
									resizePlatform(DRAGDOWN);
									currentAction = SCALE_DOWN;
							}
					}
				}
				// move the object
				if(currentAction == MOVING_OBJECT)
				{
					moveObject();					
					currentAction = MOVING_OBJECT;
				}
				else if(areaType == BODY && currentAction != SCALE_LEFT && currentAction != SCALE_RIGHT && currentAction != SCALE_UP && currentAction != SCALE_DOWN)
				{
					moveObject();					
					currentAction = MOVING_OBJECT;	
				}	

				// move the endpos
				if(currentAction == MOVING_ENDPOS)
					moveEndPos();
				else if(currentAction == IDLE && areaType == END_RECT)	{
					currentAction = MOVING_ENDPOS;
					moveEndPos();
				}
			}
		}
	}

	// scroll knapp nere -> rör kamera
	if(gDInput->mouseButtonDown(MIDDLEBUTTON))
	{
		// shouldn't be able to move outside to the left
		if(gCameraManager->gameCamera()->getX() > 600 || (gCameraManager->gameCamera()->getX() == 600 && gDInput->mouseDX() < 0))	{
			gCameraManager->gameCamera()->addMovement(-gDInput->mouseDX(), 0);
			gMouse->setVX(tmpMousePos.x - gDInput->mouseDX());
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
	else if(gDInput->keyPressed(DIK_C))
		messageHandler(BUTTON_CREATE);

	return 1;
}
// körs när man tar tag i markerad plattform
void Editor::moveObject(void)
{		
	RECT activeObjectRect = mActiveObject->getRect();
		// updatera koordinater
		float dx = gDInput->mouseDX();
		float dy = gDInput->mouseDY();
				
			if(!objectSnapping(mActiveObject, dx, dy))
			{
				if(snapDir == ALL)
				{
					mActiveObject->editorMove(dx, dy);						
				}
				else if(snapDir == LEFT || snapDir == RIGHT)
				{
					// dx kan vara båda + att om den inte är nära snapped ska den röra sig fritt
					if(snapCount >= SNAP_SENSE || snapCount <= -SNAP_SENSE )
					{		
						gMouse->move(0, -dy);
						mActiveObject->editorMove(dx, dy);
						snapDir = ALL;
					}
					else	{	// snapped, don't move the object or mouse						
						snapCount += dx;
						gMouse->move(-dx, 0);	// dont move the mouse
						mActiveObject->editorMove(0, dy);	// allow movement in the oppisite direction (up/down)
					}	
				}
				else if(snapDir == UP || snapDir == DOWN)
				{
					if(snapCount >= SNAP_SENSE || snapCount <= -SNAP_SENSE)
					{
						mActiveObject->editorMove(dx, dy);
						snapDir = ALL;
					}
					else	{
						snapCount += dy;
						gMouse->move(0, -dy);
						mActiveObject->editorMove(dx, 0);
					}					
				}						
		}

		messageHandler(OBJECT_UPDATED);
}

// just renders the GUI to the right
int Editor::renderGui()
{
	// the green side
	gGraphics->BlitRect(1300, 450, 200, 900, D3DCOLOR_ARGB( 155, 155, 200, 000));

	mWindowHandler->draw();

	gGraphics->drawText("Spawn:", GAME_WIDTH +10, 65);
	gGraphics->drawText("Active object:", GAME_WIDTH +10, 140);
	gGraphics->drawText("Create object:", GAME_WIDTH +10, 440);

	return 1;
}

void Editor::resetInputBoxes(void)
{
	for(int i = 0; i < mPropertyPairs.size(); i++)
	{
		mPropertyPairs[i].value->setValue("");
	}
}

void Editor::resizePlatform(DragRect drag)
{
	// updatera movements
	float dx = gDInput->mouseDX();
	float dy = gDInput->mouseDY();

	RECT activeObjectRect = mActiveObject->getRect();

	if(drag == DRAGLEFT)
	{
		if((mActiveObject->getWidth() >= 50 && dx > 0) || dx < 0)	{
			activeObjectRect.left += dx;
			mActiveObject->scale(LEFT, dx, 0);		
		}
		else
			gMouse->move(-dx, 0);
	}	
	else if(drag == DRAGRIGHT)
	{
		if((mActiveObject->getWidth() >= 50 && dx < 0) || dx > 0)	{
			activeObjectRect.right += dx;
			mActiveObject->scale(RIGHT, dx, 0);		// drar man musen åt höger ökar bredden
			mActiveObject->setXY(activeObjectRect.left + mActiveObject->getWidth()/2, mActiveObject->getY());
		}
		else
			gMouse->move(-dx, 0);
	}
	else if(drag == DRAGUP)
	{
		if((mActiveObject->getHeight() >= 50 && dy > 0) || dy < 0)	{
			activeObjectRect.top += dy;
			mActiveObject->scale(UP, 0, -dy);
			mActiveObject->setXY(mActiveObject->getX(), activeObjectRect.top + mActiveObject->getHeight()/2);
		}
		else
			gMouse->move(0, -dy);
	}
	else if(drag == DRAGDOWN)
	{	
		if((mActiveObject->getHeight() >= 50 && dy < 0) || dy > 0)	{
			activeObjectRect.bottom += dy;
			mActiveObject->scale(DOWN, 0, dy);
			mActiveObject->setXY(mActiveObject->getX(), activeObjectRect.top + mActiveObject->getHeight()/2);
		}
		else
			gMouse->move(0, -dy);
	}

	messageHandler(OBJECT_UPDATED);
}

bool Editor::objectSnapping(Object *object, float dx, float dy)
{
	Object testObject = *mActiveObject;
	RECT activeObjectRect = mActiveObject->getRect();
	POINT tmpMousePos = gMouse->getPos();
	RECT snapObjectRect;
	bool toReturn = false;
	int snapDist;

	// om aktiv plattform inte är i någon annan plattform
	if(mLevel->objectIntersection(mActiveObject) == NULL)
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
				gMouse->move(snapDist - dx, 0);
				mActiveObject->editorMove(snapDist, 0);

				snapCount = 0;
				snapDir = LEFT;
				return true;
			}
		}

		// till vänster
		testObject = *mActiveObject;
		testObject.move(-SNAP_DIST, 0);
		snappedObject = mLevel->objectIntersection(&testObject);
		if(snappedObject != NULL)
		{
			// snap
			if(dx < 0)
			{
				snapObjectRect = snappedObject->getRect();
				snapDist = activeObjectRect.left - snapObjectRect.right;
				gMouse->move(-snapDist - dx, 0);
				mActiveObject->editorMove(-snapDist, 0);

				snapCount = 0;
				snapDir = RIGHT;
				return true;
			}
		}

		// under
		testObject = *mActiveObject;
		testObject.move(0, SNAP_DIST);
		snappedObject = mLevel->objectIntersection(&testObject);
		if(snappedObject != NULL)
		{
			// snap
			if(dy > 0)
			{
				snapObjectRect = snappedObject->getRect();
				snapDist = snapObjectRect.top - activeObjectRect.bottom;
				gMouse->move(0, snapDist - dy);
				mActiveObject->editorMove(0, snapDist);

				snapCount = 0;
				snapDir = DOWN;
				return true;
			}
		}

		// över
		testObject = *mActiveObject;
		testObject.move(0, -SNAP_DIST);
		snappedObject = mLevel->objectIntersection(&testObject);
		if(mLevel->objectIntersection(&testObject))
		{
			// snap
			if(dy < 0)
			{
				snapObjectRect = snappedObject->getRect();
				snapDist = activeObjectRect.top - snapObjectRect.bottom;
				gMouse->move(0, -snapDist - dy);
				mActiveObject->editorMove(0, -snapDist);

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
		RECT activeObjectRect = mActiveObject->getRect();
		RECT snappedObjectRect = snappedObject->getRect();
		if(activeObjectRect.top <= snappedObjectRect.bottom && activeObjectRect.bottom >= snappedObjectRect.top && activeObjectRect.right >= snappedObjectRect.left &&
			activeObjectRect.left <= snappedObjectRect.right && mActiveObject->getID() != snappedObject->getID())	// ville nog egentligen ta reda på om id = last snap id
			return true;
	}

	return false;
}

bool Editor::messageHandler(WindowID sender, string data)
{
	char temp[256];

	switch(sender)
	{
	case OBJECT_INFO:
		{
			if(mActiveObject != NULL)
			{
				// load properties into the active object
				// a vector of properties have to be sent to the object
				std::vector<Property> propertyList;

				// build property list from the information of the widgets
				Property tmpProperty;
				for(int i = 0; i < mPropertyPairs.size(); i++)	{
					tmpProperty.name = mPropertyPairs[i].name->getValue();
					tmpProperty.value = mPropertyPairs[i].value->getValue();
					propertyList.push_back(tmpProperty);
				}
				mActiveObject->loadProperties(propertyList);

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
			if(mActiveObject != NULL)
			{
				strcpy(buffer, data.c_str());
				if(strcmp(buffer, "grass_platform") == 0)
					mActiveObject->setTextureSource("misc\\textures\\grass_platform.bmp");
				if(strcmp(buffer, "brick_platform") == 0)
					mActiveObject->setTextureSource("misc\\textures\\brick_platform.bmp");
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
					else if(value == "Teleport")
					{
						Teleport *teleport = new Teleport(200, 500, 600, 300, 50, 100, "misc\\textures\\tpenter.bmp", "misc\\textures\\tpdest.bmp");
						mLevel->addObject(teleport);
					}
					else if(value == "Trampoline")
					{
						Trampoline *trampoline = new Trampoline(300, 300, 32, 32, 100, "misc\\textures\\trampoline.bmp");
						mLevel->addObject(trampoline);
					}
					else if(value == "Walljump")
					{
						WallJump *walljump = new WallJump(300, 300, 50, 50, "misc\\textures\\walljump.bmp");
						mLevel->addObject(walljump);
					}
					else if(value == "Spike")
					{
						Spike *spike = new Spike(300, 300, 500, 500, "misc\\textures\\spike.bmp", 100);
						mLevel->addObject(spike);
					}
					// aktiv plattform = den nya?
			}
			break;
		}
	case BUTTON_DELETE:
		{		
			if(mActiveObject != NULL && mActiveObject->getType() != LEVEL_WARP)	{
					mLevel->deleteObject(mActiveObject->getID());	// ska lägga till för dynamic också!

				resetInputBoxes();
				mActiveObject = NULL;
			}
			break;
		}
	case BUTTON_SAVE:
		{
			// levels save function does the work
			strcpy(buffer, iLevel->getValue().c_str());

			// set the levels next level
			string tmp = "levels\\";
			tmp.append(iNextLevel->getValue());
			tmp.append(".txt");
			mLevel->setNextLevel(tmp);

			// rename the filename and the level name if the name has changed
			string compareString = "levels\\";
			compareString.append(iLevel->getValue());
			compareString.append(".txt");
			if(mOldLevelName != compareString)	{
				mLevel->setLevelName(compareString);

				string newName = "levels\\";
				newName.append(iLevel->getValue());
				newName.append(".txt");

				if(rename(mOldLevelName.c_str(), newName.c_str()) != 0)
					MessageBox(0, "heer", 0, 0); 			
			}

			// add  .txt
			//tmp = "levels\\";
			//tmp.append(string(buffer));
			//tmp.append(".txt");
			mLevel->saveLevel((char*)compareString.c_str());
			break;
		}
	case BUTTON_TRYLEVEL:
		{
			string tmp = "levels\\";
			tmp.append(iLevel->getValue().c_str());
			tmp.append(".txt");

			gMouse->restore();
			gCameraManager->gameCamera()->restore();
			mLevel->saveLevel((char*)tmp.c_str());
			tryLevel = true;
			break;
		}
	case OBJECT_SELECTED:
		{
			if(mActiveObject != NULL)
			{
				// delete old property widgets
				// remove old propertyPair list
				for(int i = 0; i < mPropertyPairs.size(); i++)	{
					mWindowHandler->removeWindow(mPropertyPairs[i].name);
					mWindowHandler->removeWindow(mPropertyPairs[i].value);

					mPropertyPairs[i].name = NULL;
					mPropertyPairs[i].value = NULL;
				}

				mPropertyPairs.clear();

				std::vector<Property> properties = mActiveObject->getProperties();

				for(int i = 0; i < properties.size(); i++)
				{
					addPropertyPair(properties[i]);			
				}
			}
			
			break;
		}
	case OBJECT_DESELECTED:
		{
			// empty widgets on information about the object
			mActiveObject = NULL;
			resetInputBoxes();
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
			break;
		}

	case OBJECT_UPDATED:
		{
			// update the input boxes with the new information!
			updatePropertyWidgets();
		}
	}
	
	return true;
}

void Editor::moveEndPos(void)
{
	float dx = gDInput->mouseDX();
	float dy = gDInput->mouseDY();
	POS endPos;

	// when this function gets called we know that the active object is a moving one
	// therefor we type cast it in order to reach getEndPos() and setEndPos()

	MovingObject *tmpObject = dynamic_cast<MovingObject*>(mActiveObject);

	endPos = tmpObject->getEndPos();
	endPos.x += dx;
	tmpObject->setEndPos(endPos);

	messageHandler(OBJECT_UPDATED);
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
		mLevel->drawEditorLevel();		// hax ._-
	else if(!showPaths)
		mLevel->drawLevel();

	// draw the spawn pos
	POS spawnPos = mLevel->getSpawn();
	gGraphics->BlitRect(spawnPos.x, spawnPos.y, USER_WIDTH, USER_HEIGHT, D3DCOLOR_ARGB(220, 220, 40, 0));

	// draw the activeObject orange effect and end pos + dest tp etc..
	if(mActiveObject != NULL)	{
		mActiveObject->drawEditorFX();
		gGraphics->BlitRect(mActiveObject->getRect(), D3DCOLOR_ARGB(150, 255, 166, 0));	
	}

	return 1;
}

void Editor::updatePropertyWidgets(void)
{
	std::vector<Property> activeObjectProperties = mActiveObject->getProperties();
	for(int i = 0; i < mPropertyPairs.size(); i++)
	{
		mPropertyPairs[i].value->setValue(activeObjectProperties[i].value);
	}
}

void Editor::loadLevel(char *source) 
{
	mLevel->loadLevel(source);

	// remove level\ and .txt
	string tmp = string(source);
	mOldLevelName = tmp;
	tmp.erase(tmp.size()-4, tmp.size());
	tmp.erase(0, 7);
	iLevel->setValue(tmp);

	mLevel->getSpawn().x;

	// hax warning

	itoa(mLevel->getSpawn().x, buffer, 10);
	tmp = string(buffer);
	iSpawnX->setValue(tmp);

	itoa(mLevel->getSpawn().y, buffer, 10);
	tmp = string(buffer);
	iSpawnY->setValue(tmp);

	// load the nextmap and display it
	// remove level\ and .txt
	tmp = mLevel->getNextLevel();
	tmp.erase(0, 7);
	tmp.erase(tmp.size()-4, tmp.size());

	iNextLevel->setValue(tmp);
}

string Editor::getTestLevel(void)
{
	string tmp = mLevel->getLevelName();
	return tmp;
}

void Editor::keyPressed(WPARAM wParam)
{
	mWindowHandler->keyPressed(wParam);
}

