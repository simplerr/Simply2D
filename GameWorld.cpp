#include "GameWorld.h"
#include "MovingPlatform.h"
#include "StaticPlatform.h"
using namespace std;

/*GameWorld::GameWorld()
{
	nextStaticID = 0;
	nextDynamicID = 0;

	createLevel();

	collisionText = new char[256];

	mPlayer = NULL;
}*/

GameWorld::GameWorld(Player *player)
{
	nextStaticID = 0;
	nextDynamicID = 0;	

	collisionText = new char[256];

	mPlayer = player;

	createLevel();
}

GameWorld::~GameWorld()
{
	// rensa upp listan
	for (int i = 0;i < mStaticObjectList.size();i++)
	{
		delete mStaticObjectList[i];
	}

	// samma för dynamic listan!
	for (int i = 0;i < mDynamicObjectList.size();i++)
	{
		delete mDynamicObjectList[i];
	}
}
void GameWorld::createLevel(void)
{
	// i editor är player NULL
	if(mPlayer != NULL)
	{
		/*POINT start;
		POINT end;
		start.x = 100;
		start.y = 400;
		end.x = 600;
		end.y = 400;
		MovingPlatform *movingPlatform = new MovingPlatform(100, 400, 100, 100, "misc\\textures\\brick_platform.bmp", start, end, mPlayer);
		addDynamicObject(movingPlatform);
		start.x = 100;
		start.y = 200;
		end.x = 600;
		end.y = 200;
		MovingPlatform *movingPlatform1 = new MovingPlatform(100, 200, 100, 100, "misc\\textures\\grass_platform.bmp", start, end, mPlayer, HORIZONTAL, 0.04f);
		addDynamicObject(movingPlatform1);*/
	}
}

void GameWorld::saveLevel(char* levelFile)
{
	ofstream fout;
	char buffer[256];
	int width, height;
	
	fout.open(levelFile);

	//fout << "objects:\n";

	for (int i = 0;i < mStaticObjectList.size();i++)
	{
		mStaticObjectList[i]->saveToFile(&fout);
	}

	for (int i = 0;i < mDynamicObjectList.size();i++)
	{
		
		mDynamicObjectList[i]->saveToFile(&fout);
	}

	fout.close();	
}

void GameWorld::loadLevel(char* levelFile)
{	
	// rensa upp så leveln är tom innan man laddar in nya object!
	if(mStaticObjectList.size() > 0)	
		mStaticObjectList.clear();

	// samma för dynamic listan!
	if(mDynamicObjectList.size() > 0)
		mDynamicObjectList.clear();
	char buffer[256];

	Object *loadedObject;
	ObjectType type;
	char *text = new char[256];
	char *textureSource = new char[256];

	int tmpType, height, width, xpos, ypos;

	ifstream fin;
	fin.open(levelFile);
	int lines = 0;

	while(!fin.eof())	{
		//MessageBox(0, "sDA", 0, 0);
		fin.getline(text, 256);
		lines++;
	}
	fin.close();
	fin.open(levelFile);
	lines -= 1;
	for(int j = 0; j<lines;j++)
	{
		fin >> tmpType;
		
		if(tmpType == STATIC_PLATFORMA)	
		{
			fin >> xpos >> ypos >> width >> height >> textureSource;	
			sprintf(buffer, "xpos: %i, ypos: %i, width: %i, height: %i, source: %s", xpos, ypos, width, height, textureSource);
			//MessageBox(0, buffer, 0, 0);
			loadedObject = new StaticPlatform(xpos, ypos, width, height, textureSource);
			addStaticObject(loadedObject);
		}
		else if(tmpType == MOVING_PLATFORM)
		{
			POINT startPos, endPos;
			float speed;
			fin >> xpos >> ypos >> startPos.x >> startPos.y >> endPos.x >> endPos.y >> width >> height >> speed >> textureSource;
			sprintf(buffer, "xpos: %i, ypos: %i, startPos.x: %i, startPos.y: %i, endPos.x: %i, endPos.y: %i, width: %i, height: %i, speed: %f , source: %s", xpos, ypos, startPos.x,
				startPos.y, endPos.x, endPos.y, width, height, speed, textureSource);
			loadedObject = new MovingPlatform(xpos, ypos, width, height, textureSource, startPos, endPos, mPlayer, HORIZONTAL, speed);
			addDynamicObject(loadedObject);
		}
		// else if(tmpType == ENEMY)
		// ...
		
	}
	
	
	fin.close();
}

void GameWorld::addStaticObject(Object *object)
{
	if(object != NULL)	{
		object->setID(nextStaticID);
		mStaticObjectList.push_back(object);
		nextStaticID++;
	}
	else
		MessageBox(0, "Null object", 0, 0);
}
void GameWorld::addDynamicObject(Object *object)
{
	if(object != NULL)	{
		object->setID(nextDynamicID);
		mDynamicObjectList.push_back(object);
		nextDynamicID++;
	}
	else
		MessageBox(0, "Null object", 0, 0);
}

void GameWorld::deleteStaticObject(int ID)
{	
	//eh 
	int i = 0;
	vector<Object*>::iterator itr =  mStaticObjectList.begin();
	while(itr != mStaticObjectList.end() && i < mStaticObjectList.size())
	{
		if(mStaticObjectList[i]->getID() == ID)
		{
			delete mStaticObjectList[i];		// viktigt att deleta innan!
			itr = mStaticObjectList.erase(itr);			
			break;
		}
		else	{
			itr++;
			i++;
		}
	}
	nextStaticID -=1;
}

void GameWorld::deleteDynamicObject(int ID)
{	
	//eh 
	int i = 0;
	vector<Object*>::iterator itr =  mDynamicObjectList.begin();
	while(itr !=  mDynamicObjectList.end() && i <  mDynamicObjectList.size())
	{
		if(mDynamicObjectList[i]->getID() == ID)
		{
			delete  mDynamicObjectList[i];		// viktigt att deleta innan!
			itr =  mDynamicObjectList.erase(itr);			
			break;
		}
		else	{
			itr++;
			i++;
		}
	}
	nextDynamicID -=1;
}


void GameWorld::updateLevel(double dt)	
{
	// update enemies
	// update entities
	for (int i = 0;i < mDynamicObjectList.size();i++)
	{
		mDynamicObjectList[i]->update(dt);
	}
}

void GameWorld::drawLevel(void)
{
	if(!gGraphics)
		MessageBox(0, "NULL PTR", 0, 0);

	// statiska
	for (int i = 0;i < mStaticObjectList.size();i++)
	{
		mStaticObjectList[i]->draw();
	}

	// dynamiska
	for (int i = 0;i < mDynamicObjectList.size();i++)
	{
		mDynamicObjectList[i]->draw();
	}

	// drawEnemies
}

CollisionStruct GameWorld::collision(Player *player)
{
	//CollisionDirection collisionDirection = NONE_COLLISION;
	CollisionStruct collisions;
	collisions.hori = false;
	collisions.vert = false;

	RECT tmpRect;
	bool collision = false;
	RECT objectRect;
	RECT playerRect = player->getRect();

	for (int i = 0;i < mStaticObjectList.size();i++)
	{
		// hitta kolliderande object
		objectRect = mStaticObjectList[i]->getRect();

		// inte på marken -> fall
		if(!(playerRect.top >= objectRect.bottom ||  playerRect.bottom + 1<= objectRect.top ||  playerRect.right <= objectRect.left ||  playerRect.left >= objectRect.right))
		{
			collisions.vert = true;
		}

		// spelaren har rört på sig
		if(player->getDX() < 0)
		{
			if(!(playerRect.top >= objectRect.bottom ||  playerRect.bottom <= objectRect.top ||  playerRect.right <= objectRect.left ||  playerRect.left + player->getDX() >= objectRect.right))
			{
				collisions.hori = true;
			}
		}
		else if(player->getDX() > 0)
		{
			if(!(playerRect.top >= objectRect.bottom ||  playerRect.bottom <= objectRect.top ||  playerRect.right + player->getDX() <= objectRect.left ||  playerRect.left >= objectRect.right))
			{
				collisions.hori = true;
			}
		}
		
		if(player->getDY() < 0)
		{
			if(!(playerRect.top + player->getDY() >= objectRect.bottom ||  playerRect.bottom <= objectRect.top ||  playerRect.right <= objectRect.left ||  playerRect.left >= objectRect.right))
			{
				collisions.vert = true;
			}
		}
		else if(player->getDY() > 0)
		{
			if(!(playerRect.top >= objectRect.bottom ||  playerRect.bottom  + player->getDY() <= objectRect.top ||  playerRect.right <= objectRect.left ||  playerRect.left >= objectRect.right))
			{
				collisions.vert = true;
			}
		}

	}

	for (int i = 0;i < mDynamicObjectList.size();i++)
	{
		// hitta kolliderande object
		objectRect = mDynamicObjectList[i]->getRect();

		// inte på marken -> fall
		if(!(playerRect.top >= objectRect.bottom ||  playerRect.bottom + 1<= objectRect.top ||  playerRect.right <= objectRect.left ||  playerRect.left >= objectRect.right))
		{
			collisions.vert = true;
		}

		// spelaren har rört på sig
		if(player->getDX() < 0)
		{
			if(!(playerRect.top >= objectRect.bottom ||  playerRect.bottom <= objectRect.top ||  playerRect.right <= objectRect.left ||  playerRect.left + player->getDX() >= objectRect.right))
			{
				collisions.hori = true;
			}
		}
		else if(player->getDX() > 0)
		{
			if(!(playerRect.top >= objectRect.bottom ||  playerRect.bottom <= objectRect.top ||  playerRect.right + player->getDX() <= objectRect.left ||  playerRect.left >= objectRect.right))
			{
				collisions.hori = true;
			}
		}
		
		if(player->getDY() < 0)
		{
			if(!(playerRect.top + player->getDY() >= objectRect.bottom ||  playerRect.bottom <= objectRect.top ||  playerRect.right <= objectRect.left ||  playerRect.left >= objectRect.right))
			{
				collisions.vert = true;
			}
		}
		else if(player->getDY() > 0)
		{
			if(!(playerRect.top >= objectRect.bottom ||  playerRect.bottom  + player->getDY() <= objectRect.top ||  playerRect.right <= objectRect.left ||  playerRect.left >= objectRect.right))
			{
				collisions.vert = true;
			}
		}

	}

	return collisions;
}

Object* GameWorld::getObjectAt(POINT mpos)
{
	Object *tmp = NULL;
	RECT objectRect;

	for (int i = 0;i < mStaticObjectList.size();i++)
	{
		objectRect = mStaticObjectList[i]->getRect();

		if(mpos.x > objectRect.left && mpos.x < objectRect.right && 
			mpos.y > objectRect.top && mpos.y < objectRect.bottom)
			tmp = mStaticObjectList[i];
	}
	// tröck inte på nått statiskt, prova dynamiskt!
	if(tmp == NULL)
	{
		for (int i = 0;i < mDynamicObjectList.size();i++)
		{
			objectRect = mDynamicObjectList[i]->getRect();

			if(mpos.x > objectRect.left && mpos.x < objectRect.right && 
				mpos.y > objectRect.top && mpos.y < objectRect.bottom)
				tmp = mDynamicObjectList[i];
		}
	}

	// samma sak för dynamic listan!
	return tmp;
}

Object* GameWorld::objectIntersection(Object *activeObject)
{
	RECT objectRect;
	RECT activeObjectRect = activeObject->getRect();
		// returnerar true om det blir en intersect
		for (int i = 0;i < mStaticObjectList.size();i++)
		{		
			objectRect = mStaticObjectList[i]->getRect();
			if(activeObjectRect.top <= objectRect.bottom && activeObjectRect.bottom >= objectRect.top && activeObjectRect.right >=objectRect.left &&
				activeObjectRect.left <= objectRect.right && activeObject->getID() != mStaticObjectList[i]->getID())
			{
				return mStaticObjectList[i];
			}
		}
	
	return NULL;
}