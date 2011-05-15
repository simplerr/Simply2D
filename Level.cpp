#include "Level.h"
#include "StaticPlatform.h"
#include "Object.h"
#include "MovingPlatform.h"
#include "Trampoline.h"
#include "WallJump.h"
#include "Teleport.h"
#include "Enemy.h"
#include "Spike.h"
#include "CameraManager.h"
#include "PlayState.h"
#include "LevelCompletedState.h"
#include <fstream>
#include "EditorState.h"
#include "TestState.h"
#include "GameOverState.h"
#include "Bullet.h"
#include <list>
#include <iterator>
#include "Turret.h"
#include "Gate.h"
#include "ActivateButton.h"
#include "GunPowerup.h"


using namespace std;

extern CameraManager* gCameraManager;

Level::Level(Player *player)
{
	mPlayer = player;
	mLevelWarp = NULL;
	mLevelType = CAMPAIGN;
	mIdOffset = 0;
	mProgress.loadProgress("levels\\campaign\\campaign_progress.txt");
	mLoaded = false;
	mPlayTime = 0;
}

Level::~Level()
{
	// clean up the list
	// rensa upp listan
	for (int i = 0;i < mObjectList.size();i++)
	{
		delete mObjectList[i];
	}
	
	mObjectList.clear();

	mPlayer = NULL;
	delete mLevelWarp;
}

// saves a level to a .txt file
void Level::saveLevel(char* levelFile)
{
	ofstream fout;
	int width, height;

	//string tmp = "levels\\";
	//tmp.append(levelFile);
	
	//fout.open(tmp.c_str());
	fout.open(levelFile);

	// spawn, goal + nextlevel
	fout << (int)spawnPos.x << " " << (int)spawnPos.y << endl;
	fout << (int)mLevelWarp->getX() << " " << (int)mLevelWarp->getY() << endl;
	if(mLevelType == CAMPAIGN)
		fout << mNextLevel << endl;
	else
		fout << "CUSTOM" << endl;

	for (int i = 0;i < mObjectList.size();i++)
	{
		mObjectList[i]->saveToFile(&fout);
	}

	fout.close();	
}

// loads a level from a .txt file
void Level::loadLevel(char* levelFile)
{	
	/* activation button list to be connected */
	std::vector<ActivateButton*> buttonList;

	// set the level source
	mLevelSource = string(levelFile);

	// clean the level before loading it
	if(mObjectList.size() > 0)	
		mObjectList.clear();

	Object *loadedObject;
	ObjectType type;
	char text[256];
	string textureSource;
	string destTextureSource;

	int objectType, height, width, xpos, ypos;
	int objectId = 0;

	ifstream fin;
	fin.open(levelFile);
	int lines = 0;

	// get number of lines
	while(!fin.eof())	{
		fin.getline(text, 256);
		lines++;
	}
	fin.close();
	fin.open(levelFile);
	lines -= 4;		// correctment, since the first 3 lines arent object data (spawn, warp, next level)

	fin >> spawnPos.x >> spawnPos.y;

	// fix the Warp object
	int tmpx, tmpy;
	fin >> tmpx >> tmpy;
	mLevelWarp = new Object(tmpx, tmpy, 100, 100, (char*)WARP_SOURCE.c_str(), LEVEL_WARP);
	mLevelWarp->setResizeable(false);

	fin >> mNextLevel;
	if(mNextLevel == "CUSTOM")
		mLevelType = CUSTOM;
	else
		mLevelType = CAMPAIGN;

	// load each line and add the object it generates to mObjectList
	for(int j = 0; j<lines;j++)
	{
		fin >> objectType;
		fin >> objectId;
		// might add a Object::loadFromFile() or a file manager
		if(objectType == STATIC_PLATFORMA)	
		{
			fin >> xpos >> ypos >> width >> height >> textureSource;				
			loadedObject = new StaticPlatform(xpos, ypos, width, height, (char*)textureSource.c_str());
		}
		else if(objectType == MOVING_PLATFORM)
		{
			POS startPos, endPos;
			float speed;
			fin >> xpos >> ypos >> startPos.x >> startPos.y >> endPos.x >> endPos.y >> width >> height >> speed >> textureSource;
			loadedObject = new MovingPlatform(xpos, ypos, width, height, (char*)textureSource.c_str(), startPos, endPos, HORIZONTAL, speed);
		}
		else if(objectType == NORMAL_ENEMY)
		{
			POS startPos, endPos;
			float speed;
			int health, damage;

			fin >> xpos >> ypos >> startPos.x >> startPos.y >> endPos.x >> endPos.y >> width >> height >> speed >> health >> damage >> textureSource;
			loadedObject = new Enemy(xpos, ypos, width, height, (char*)textureSource.c_str(), startPos, endPos, HORIZONTAL, speed, health, damage);			
		}	
		else if(objectType == TELEPORT)
		{
			int destx, desty;
			fin >> xpos >> ypos >> destx >> desty >> width >> height >> textureSource >> destTextureSource;
			loadedObject = new Teleport(xpos, ypos, destx, desty, width, height, (char*)textureSource.c_str(), (char*)destTextureSource.c_str());
		}
		else if(objectType == TRAMPOLINE)
		{
			double maxSpeed, accel;
			fin >> xpos >> ypos >> width >> height >> accel >> maxSpeed >> textureSource;
			loadedObject = new Trampoline(xpos, ypos, width, height, accel, maxSpeed, (char*)textureSource.c_str());
		}
		else if(objectType == WALLJUMP)
		{
			fin >> xpos >> ypos >> width >> height >> textureSource;				
			loadedObject = new WallJump(xpos, ypos, width, height, (char*)textureSource.c_str());
		}
		else if(objectType == SPIKE)
		{
			int dmg;
			fin >> xpos >> ypos >> width >> height >> dmg >> textureSource;
			loadedObject = new Spike(xpos, ypos, width, height, (char*)textureSource.c_str(), dmg);
		}
		else if(objectType == TURRET)
		{
			float speed, firerate;
			int dir;
			int damage, health, lifelen;

			fin >> xpos >> ypos >> width >> height >> speed >> health >> damage >> lifelen >> firerate >> dir >> textureSource;
			Turret *tmpTurret = new Turret(xpos, ypos, width, height, (char*)textureSource.c_str(), health, (direction)dir, damage, speed, lifelen, firerate);
			tmpTurret->setPlayer(mPlayer);

			loadedObject = tmpTurret;
		}
		else if(objectType == ACTIVATE_BUTTON)
		{
			int gateId;
			fin >> xpos >> ypos >> width >> height >> gateId >> textureSource;				
			ActivateButton *tmpButton = new ActivateButton(xpos, ypos, width, height, (char*)textureSource.c_str());
			tmpButton->setLevel(this);
			tmpButton->setPlayer(mPlayer);
			tmpButton->setGateId(gateId);
			buttonList.push_back(tmpButton);

			loadedObject = tmpButton;
		}
		else if(objectType == GATE)
		{
			float openTime;
			fin >> xpos >> ypos >> width >> height >> openTime >> textureSource;				
			loadedObject = new Gate(xpos, ypos, width, height, (char*)textureSource.c_str(), openTime);
		}
		else if(objectType == GUNPOWERUP)
		{
			int ammo, type;
			fin >> xpos >> ypos >> width >> height >> ammo >> type >> textureSource;				
			loadedObject = new GunPowerup(xpos, ypos, width, height, (char*)textureSource.c_str(), ammo, type);
		}
		//loadedObject->setID(objectId);
		addObject(loadedObject, objectId);
	}

	fin.close();

	if(mPlayer != NULL)
		spawnPlayer();

	mIdOffset = objectId;
	mLoaded = true;

	/* connect buttons to gates */
	for(int i = 0; i < buttonList.size(); i++)
	{
		if(buttonList[i]->getGateId() != 99999)	{
			for(int j = 0; j < mObjectList.size(); j++)
			{
				if(mObjectList[j]->getID() == buttonList[i]->getGateId())
					buttonList[i]->connectGate((Gate*)mObjectList[j]);
			}
		}
	}
}

void Level::addObject(Object *object, int id)
{
	if(object != NULL)	{
		if(mLoaded)	{
			mIdOffset++;
			object->setID(mIdOffset);
		}
		else	{
			object->setID(id);
		}
		mObjectList.push_back(object);
	}
	else
		MessageBox(0, "Null object", 0, 0);
}

void Level::deleteObject(int ID)
{	
	//eh 
	int i = 0;
	vector<Object*>::iterator itr =  mObjectList.begin();
	while(itr != mObjectList.end() && i < mObjectList.size())
	{
		if(mObjectList[i]->getID() == ID)
		{
			delete mObjectList[i];		// viktigt att deleta innan!
			mObjectList[i] = NULL;
			itr = mObjectList.erase(itr);	
			break;
		}
		else	{
			itr++;
			i++;
		}
	}
}

// used to remove a single object, mostly by the editor
void Level::removeObject(int ID)
{
	int i = 0;
	vector<Object*>::iterator itr =  mObjectList.begin();
	while(itr != mObjectList.end() && i < mObjectList.size())
	{
		if(mObjectList[i]->getID() == ID)
		{
			mObjectList[i]->onRemove();
			delete mObjectList[i];		// viktigt att deleta innan!
			mObjectList[i] = NULL;
			itr = mObjectList.erase(itr);	
			break;
		}
		else	{
			itr++;
			i++;
		}
	}
}

bool Level::updateLevel(double dt)	
{
	/* update the time since the level started */
	if(mLevelType != TEST)	{
		mPlayTime += dt;
	}

	// update enemies
	// update entities
	bool playerActivation = false;

	for (int i = 0;i < mObjectList.size();i++)
	{
		mObjectList[i]->update(dt);

		/* remove dead enemies */
		if(mObjectList[i]->getRemove())
			deleteObject(mObjectList[i]->getID());

		/* activate pressed buttons - hack */
		else if(mObjectList[i]->getType() == ACTIVATE_BUTTON)	{
			if(mPlayer->getActivateKey())	{
				ActivateButton *button = dynamic_cast<ActivateButton*>(mObjectList[i]);
				if(button->insideActivateArea())
					playerActivation = true;
			}
		}
	}

	if(!playerActivation)	{
		mPlayer->setActivateKey(false);
	}

	/* update player if he is not dead */

	if(mPlayer->getHealth() > 0 && mPlayer->getY() <= 1500)	{
		// should return false if the user completed a map
		mPlayer->update(dt, this);
		collision(mPlayer, dt);
	}
	else if(mPlayer->getHealth() <= 0 || mPlayer->getY() >= 1500)	{
		
		if(mPlayer->getY() >= 1500)
			gSound->playEffect("misc\\sound\\die_abyss.wav");

		string current = getLevelName();

		if(mLevelType != TEST)	{
			string tmpCurrent = current;
			LevelType type = getType();

			if(mLevelType == CAMPAIGN)
				tmpCurrent.erase(0, 16);
			else if(mLevelType == CUSTOM)
				tmpCurrent.erase(0, 7);

			tmpCurrent.erase(tmpCurrent.end()-4, tmpCurrent.end());

			mProgress.addTry(tmpCurrent);
			PlayState::Instance()->changeState(GameOverState::Instance());
			GameOverState::Instance()->setLevels(current, "next");	// next wont get used
			GameOverState::Instance()->setFailedType(type);
		}
		else if(mLevelType == TEST)	{
			TestState::Instance()->changeState(EditorState::Instance());
			EditorState::Instance()->setLevel(current);
			gSound->playMusic("misc\\sound\\menu_loop.wav", true, true);
		}
	}

	return true;
}

void Level::drawLevel(void)
{
	for (int i = 0;i < mObjectList.size();i++)
	{
		mObjectList[i]->draw();
	}

	//gGraphics->BlitTexture(mLevelWarp.texture, mLevelWarp.getRect(), 0xFFFFFFFF, 0);

	if(mLevelWarp != NULL)
		mLevelWarp->draw();

	if(mPlayer != NULL)
		mPlayer->draw();
}

void Level::drawGui(void)
{
	gGraphics->BlitRect(1300, 135, 120, 80, D3DCOLOR_ARGB(255, 255, 255, 255));

	mPlayer->drawGui();
	
	char buffer[256];
	sprintf(buffer, "Time: %.3f", mPlayTime);
	gGraphics->drawText(buffer, 1250, 150);
}

bool Level::collision(Player *player, float dt)
{
	bool onGround = false;
	/* loop through each object and test collision against the player */
	for(int i = 0; i < mObjectList.size(); i++)
	{		
		MTV mtv = polyCollision(mObjectList[i]->getShape(), player->getShape());

		if(mtv.collision)	{
			// push out from collision
			player->move(mtv.pushX, mtv.pushY);

			// the camera have to follow the player
			if(player->getX() >= 616)
				gCameraManager->gameCamera()->addMovement(mtv.pushX, 0);

			// what's gonna happen with the player?
			mObjectList[i]->onPlayerCollision(player, mtv, dt);	

			// on ground?
			if(onGround)
				continue;
			
			if(mtv.pushY < 0)	{
				// trampoline?
				if(mObjectList[i]->getType() != TRAMPOLINE)		{
					onGround = true;
					player->setSpeedY(0);
				}

				// could use a if statement, but not neccessary ( if prev != 1337 )
				player->setPrevWallJumpID(1337);
				
			}
			else if(mtv.pushY > 0)	{
				/* hitted a roof */
				player->hittedCeiling(true);
			}
		}	

		
		/* check bullet collisions */
		if(player->mBulletList.size() > 0)	{
			std::list<Bullet>::iterator j = player->mBulletList.begin();
			while( j != player->mBulletList.end())
			{	
				if(polyCollision(mObjectList[i]->getShape(), j->getShape()).collision)	{
					j->onObjectCollision(mObjectList[i]);
					j->erase();
				}

				j++;
			}
		}
	}

	if(onGround)
		player->onGround(true);
	else
		player->onGround(false);

	// check if player have completed the map
	if(polyCollision(mLevelWarp->getShape(), player->getShape()).collision)	{
		/* get the information we need before the level gets destroyed */
		LevelType type = getType();
		string current = this->getLevelName();
		string next = this->getNextLevel();

		/* set the next level to playable */
		if(mLevelType == CAMPAIGN)
			next.erase(0, 16);
		else if(mLevelType == CUSTOM)
			next.erase(0, 7);

		next.erase(next.end()-4, next.end());
		mProgress.setProgress(next, true);

		string tmpCurrent = current;

		if(mLevelType == CAMPAIGN)
			tmpCurrent.erase(0, 16);
		else if(mLevelType == CUSTOM)
			tmpCurrent.erase(0, 7);
		tmpCurrent.erase(tmpCurrent.end()-4, tmpCurrent.end());

		if(mPlayTime < mProgress.getBestTime(tmpCurrent) || mProgress.getBestTime(tmpCurrent) == 0)
			mProgress.setBestTime(tmpCurrent, mPlayTime);

		mProgress.addTry(tmpCurrent);

		/* change state */
		if(mLevelType != TEST)	{
			PlayState::Instance()->changeState(LevelCompletedState::Instance());

			/* send the neccessary data to the new state */
			LevelCompletedState::Instance()->setCompletedType(type);
			LevelCompletedState::Instance()->setLevels(current, next);
		}
		else	{
			TestState::Instance()->changeState(EditorState::Instance());
			EditorState::Instance()->setLevel(current);
			gSound->playMusic("misc\\sound\\menu_loop.wav", true, true);
		}

		/* ADD TEST TYPE!!! */
	}

	return true;
}

/* SAT - separating axis theorem

	I have tried to comment the code as much as I could.
	This is the steps that we have to do:

	1.) Loop through the sides of Shape A
	2.) Calculate the axis (the normal pointing OUT from the Shape) for each side
	3.) Normalize the axis so it becomes a pure direction vector
	4.) Project each point from Shape A on the axis
	5.) Find the maximum and minimum points
	6.) Project each point from Shape B on the axis
	7.) Find the maximum and minimum points
	8.) Check if they overlap
	9.) Calculate the smallest intersection depth and it's direction (MTV)
	10.) Return the MTV at the end of the function

	These steps have to be repeated for Shape B as well to ensure there's no false collisions

*/
MTV Level::polyCollision(Shape *ShapeA, Shape *ShapeB)
{	
	MTV mtv;
	mtv.collision = false;

	/* broadphase AABB test */
	RECT rectA = ShapeA->getRect();
	RECT rectB = ShapeB->getRect();

	if(rectA.left > rectB.right)
		return mtv;
	else if(rectA.right < rectB.left)
		return mtv;
	else if(rectA.top > rectB.bottom)
		return mtv;
	else if(rectA.bottom < rectB.top)
		return mtv;

	bool b = true;
	char buffer[256];
	Shape::Point axis;															// axis we will project onto
	Shape::Point projection;														// the direction of the projection
	int side;																				// current side 
	double minA, maxA, minB, maxB, axisLen, tmp, minLen = 999999, tmpDepth = 1;;			// doubles needed
																					// minimum intersection

	// test Shape A's sides
	for(side = 0; side < ShapeA->sides; side++)
	{
		/* get the axis we will project on */
		/*
			axis = the normal
			n.x = -(pointA.y - pointB.y)
			n.y = pointA.x - pointB.x

			the normal should be pointing OUT from the object
			in order for the object to move by the proper MTV
		*/

		if(side == 0)	// the first point must get connected to the last
		{	
			axis.x = -(ShapeA->pointList[ShapeA->sides-1].y - ShapeA->pointList[0].y);
			axis.y = ShapeA->pointList[ShapeA->sides-1].x - ShapeA->pointList[0].x;
		}
		else
		{
			axis.x = -(ShapeA->pointList[side-1].y - ShapeA->pointList[side].y);
			axis.y = ShapeA->pointList[side-1].x - ShapeA->pointList[side].x;	// should be reversed?:O

			// axis.y gets negative on the bottom horizontal line
			// this is because we want it to point out from the object
			// not sure if it will work with other shapes though
		}

		/* normalize the axis */
		/*
			n.len = sqrt(n.x^2 + n.y^2)
			n.x = n.x / n.len
			n.y = n.y / n.len
		*/

		axisLen = sqrt(axis.x * axis.x + axis.y * axis.y);
		axis.x /= axisLen;
		axis.y /= axisLen;

		/* project Shape A's points on the axis to find MIN and MAX points */
		/*
			pointA.x = (pointA.x * axis.x + pointA.y * axis.y) * axis.x = dp * axis.x
			pointA.y = (pointA.x * axis.x + pointA.y * axis.y) * axis.y = dp * axis.y
			
			the dot-product is enough, graphicaly it need adjustment but mathematicly it works
			that gives us:

			pointA.x = pointA.x * axis.x + pointA.y * axis.y = dp
			pointA.y = pointA.x * axis.x + pointA.y * axis.y = dp 		
		*/

		minA = maxA = (ShapeA->pointList[0].x * axis.x + ShapeA->pointList[0].y * axis.y);

		for(int i = 1; i < ShapeA->sides; i++)
		{
			tmp = (ShapeA->pointList[i].x * axis.x + ShapeA->pointList[i].y * axis.y);

			if(tmp < minA)
				minA = tmp;
			else if (tmp > maxA)
				maxA = tmp;
		}

		/* correct with offset */
		tmp = ShapeA->origin.x * axis.x + ShapeA->origin.y * axis.y;
		minA += tmp;
		maxA += tmp;

		/* project Shape B's points on the axis to find MIN and MAX points */
		minB = maxB = (ShapeB->pointList[0].x * axis.x + ShapeB->pointList[0].y * axis.y);

		for(int i = 1; i < ShapeB->sides; i++)
		{		
			tmp = (ShapeB->pointList[i].x * axis.x + ShapeB->pointList[i].y * axis.y);		

			if(tmp < minB)
				minB = tmp;
			else if (tmp > maxB)
				maxB = tmp;
		}		

		/* correct with offset */
		tmp = ShapeB->origin.x * axis.x + ShapeB->origin.y * axis.y;
		minB += tmp;
		maxB += tmp;

		/* test if they intersect, if not then store the penetration depth and axis */
		if(minA > maxB || maxA < minB)	{
			mtv.collision = false;
			return mtv;
		}
		else
		{	
			// the projection axis is to be used by the player (objectB)
			if(maxA < maxB)	
			{
				tmpDepth = maxA - minB;
				if(tmpDepth < minLen)	{
					minLen = tmpDepth;
					projection = axis;
				}	
			}
			else if(maxA > maxB)
			{
				tmpDepth = maxB - minA;			
				if(tmpDepth < minLen)	{
					minLen = tmpDepth;
					projection.x = axis.x*-1;
					projection.y = axis.y*-1;					
				}
			}
		}
	}

	/* the same thing again but now for object B, sometimes a collision from Shape A isn't a collision for Shape B */
	/*
		- see above for detailed comments
	*/

	// test Shape B's sides
	for(side = 0; side < ShapeB->sides; side++)
	{
		/* get the axis we will project on */
		if(side == 0)	// the first point must get connected to the last
		{			
			axis.x = -(ShapeB->pointList[ShapeB->sides-1].y - ShapeB->pointList[0].y);
			axis.y = ShapeB->pointList[ShapeB->sides-1].x - ShapeB->pointList[0].x;
		}
		else
		{
			axis.x = -(ShapeB->pointList[side].y - ShapeB->pointList[side-1].y);
			axis.y = ShapeB->pointList[side-1].x - ShapeB->pointList[side].x;
		}

		/* normalize the axis */	
		axisLen = sqrt(axis.x * axis.x + axis.y * axis.y);
		axis.x /= axisLen;
		axis.y /= axisLen;

		/* project Shape B's points on the axis to find MIN and MAX points */		
		minB = maxB = (ShapeB->pointList[0].x * axis.x + ShapeB->pointList[0].y * axis.y);

		for(int i = 1; i < ShapeB->sides; i++)
		{
			tmp = (ShapeB->pointList[i].x * axis.x + ShapeB->pointList[i].y * axis.y);			

			if(tmp < minB)
				minB = tmp;
			else if (tmp > maxB)
				maxB = tmp;
		}

		/* correct with offset */
		tmp = ShapeB->origin.x * axis.x + ShapeB->origin.y * axis.y;
		minB += tmp;
		maxB += tmp;

		/* project Shape A's points on the axis to find MIN and MAX points */	
		minA = maxA = (ShapeA->pointList[0].x * axis.x + ShapeA->pointList[0].y * axis.y);

		for(int i = 1; i < ShapeA->sides; i++)
		{	
			tmp = (ShapeA->pointList[i].x * axis.x + ShapeA->pointList[i].y * axis.y);

			if(tmp < minA)
				minA = tmp;
			else if (tmp > maxA)
				maxA = tmp;
		}

		/* correct with offset */
		tmp = ShapeA->origin.x * axis.x + ShapeA->origin.y * axis.y;
		minA += tmp;
		maxA += tmp;

		/* test if they intersect, if not then return false */
		if(minA > maxB || maxA < minB)	{
			mtv.collision = false;
			return mtv;
		}
		else
		{	
			// get the minimal depth of intersection and the correct axis
			if(maxA < maxB)	
				tmpDepth = maxA - minB;
			else if(maxA > maxB)
				tmpDepth = maxB - minA;
			
			if(tmpDepth < minLen)	{
				
				minLen = tmpDepth;
				projection = axis;			
			}
		}
	}
	

	/* return minimum intersection depth */
	mtv = MTV(minLen, projection);
	mtv.collision = true;
	return mtv;
}

// return the object that mpos is inside - several = the last one added
Object* Level::getObjectAt(POINT mpos)
{
	Object *tmp = NULL;
	RECT objectRect;

	for (int i = 0;i < mObjectList.size();i++)
	{
		objectRect = mObjectList[i]->getRect();

		if(mpos.x > objectRect.left && mpos.x < objectRect.right && 
			mpos.y > objectRect.top && mpos.y < objectRect.bottom)
			tmp = mObjectList[i];
	}

	RECT warpRect = mLevelWarp->getRect();
	if(mpos.x > warpRect.left && mpos.x < warpRect.right && mpos.y > warpRect.top && mpos.y < warpRect.bottom)
			tmp = mLevelWarp;

	return tmp;
}

// returns true if intersection
Object* Level::objectIntersection(Object *activeObject)
{
	RECT objectRect;
	RECT activeObjectRect = activeObject->getRect();
		// returnerar true om det blir en intersect
		for (int i = 0;i < mObjectList.size();i++)
		{		
			objectRect = mObjectList[i]->getRect();
			if(activeObjectRect.top <= objectRect.bottom && activeObjectRect.bottom >= objectRect.top && activeObjectRect.right >=objectRect.left &&
				activeObjectRect.left <= objectRect.right && activeObject->getID() != mObjectList[i]->getID())
			{
				return mObjectList[i];
			}
		}

	return NULL;
}

void Level::drawEditorLevel(void)
{
	// draw the objects in the list with editor effects - HACK

	// statiska
	for (int i = 0;i < mObjectList.size();i++)
	{
		mObjectList[i]->drawEditorFX();
	}

	if(mLevelWarp != NULL)
		mLevelWarp->draw();
}

void Level::spawnPlayer(void)
{
	mPlayer->setXY(spawnPos.x, spawnPos.y);
}

RECT Level::getSpawnRect(void)
{
	RECT tmpRect;

	tmpRect.left = spawnPos.x - USER_WIDTH/2;
	tmpRect.right = spawnPos.x + USER_WIDTH/2;
	tmpRect.top = spawnPos.y - USER_HEIGHT/2;
	tmpRect.bottom = spawnPos.y + USER_HEIGHT/2;

	return tmpRect;
}

std::string Level::getLevelName(void)
{
	return mLevelSource;
}

void Level::connectGate(ActivateButton* button, int id)
{
	for (int i = 0;i < mObjectList.size();i++)
	{
		if(mObjectList[i]->getID() == id)	 {
			button->connectGate((Gate*)mObjectList[i]);
			break;
		}
	}
}

direction Level::snapObject(Object *object, int snapDist)
{
	MTV mtv;
	Object tmpObject = *object;

	if(tmpObject.getShapeType() != QUAD)	{
		Object tmpObject2(object->getX(), object->getY(), object->getWidth(), object->getHeight(), object->getTextureSource(), STATIC_PLATFORMA);
		tmpObject = tmpObject2;
	}
	//else
	//	tmpObject = *object;

	tmpObject.scale(ALL, 2*snapDist, 2*snapDist);

	float dx = gDInput->mouseDX();
	float dy = gDInput->mouseDY();

	// ta reda på hur långt den ska snappa
	RECT snapObjectRect, objectRect; 
	int realSnapDist;

	/*for (int i = 0;i < mObjectList.size();i++)
	{
		if(objectIntersection(object)
	}*/

	for (int i = 0;i < mObjectList.size();i++)
	{
		if(tmpObject.getID() != mObjectList[i]->getID())	{
			/* make sure the object isn't inside anything */
			if(polyCollision(object->getShape(), mObjectList[i]->getShape()).collision)
				return ALL;	// hack - should be NONE

			/* check which side to snap to */
			mtv = polyCollision(tmpObject.getShape(), mObjectList[i]->getShape());
			if(mtv.collision)
			{
				snapObjectRect = mObjectList[i]->getRect();
				objectRect = object->getRect();

				if(mtv.pushX < 0 && dx < 0)	{
					realSnapDist = objectRect.left - snapObjectRect.right;
					object->editorMove(-realSnapDist, dy);
					gDInput->setCursorX(gDInput->getCursorX() - realSnapDist - dx);
					return LEFT;
				}
				else if(mtv.pushX > 0 && dx > 0)	{
					realSnapDist = snapObjectRect.left - objectRect.right;
					object->editorMove(realSnapDist, dy);
					gDInput->setCursorX(gDInput->getCursorX() + realSnapDist - dx);
					return RIGHT;
				}

				if(mtv.pushY < 0 && dy < 0)	{
					realSnapDist = objectRect.top - snapObjectRect.bottom;
					object->editorMove(dx, -realSnapDist);
					gDInput->setCursorY(gDInput->getCursorY() - realSnapDist - dy);
					return UP;
				}
				else if(mtv.pushY > 0 && dy > 0)	{
					realSnapDist = snapObjectRect.top - objectRect.bottom;
					object->editorMove(dx, realSnapDist);
					gDInput->setCursorY(gDInput->getCursorY() + realSnapDist - dy);
					return DOWN;
				}
			}
		}
	}

	// shouldn't be ALL, but NONE is taken :/
	return ALL;
}
