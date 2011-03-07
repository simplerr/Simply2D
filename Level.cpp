#include "Level.h"
#include "StaticPlatform.h"
#include "Object.h"
#include "MovingPlatform.h"
#include "Enemy.h"
using namespace std;

Level::Level(Player *player)
{
	collisionText = new char[256];

	mPlayer = player;

	char buffer[256];
	strcpy(buffer, ACTIVE_LEVEL.c_str());
	loadLevel(buffer);

	if(mPlayer != NULL)
		spawnPlayer();

	nextObjectID = 0;
}

Level::~Level()
{
	// clean up the list
	// rensa upp listan
	for (int i = 0;i < mObjectList.size();i++)
	{
		delete mObjectList[i];
	}
}

// saves a level to a .txt file
void Level::saveLevel(char* levelFile)
{
	ofstream fout;
	char buffer[256];
	int width, height;
	
	fout.open(levelFile);
	
	fout << (int)spawnPos.x << " " << (int)spawnPos.y << endl;

	for (int i = 0;i < mObjectList.size();i++)
	{
		mObjectList[i]->saveToFile(&fout);
	}

	fout.close();	
}

// loads a level from a .txt file
void Level::loadLevel(char* levelFile)
{	
	// clean the level before loading it
	if(mObjectList.size() > 0)	
		mObjectList.clear();

	char buffer[256];

	Object *loadedObject;
	ObjectType type;
	char *text = new char[256];
	char *textureSource = new char[256];

	int objectType, height, width, xpos, ypos;

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
	lines -= 2;		// correctment, hax

	fin >> spawnPos.x >> spawnPos.y;

	// load each line and add the object it generates to mObjectList
	for(int j = 0; j<lines;j++)
	{
		fin >> objectType;
		// might add a Object::loadFromFile() or a file manager
		if(objectType == STATIC_PLATFORMA)	
		{
			fin >> xpos >> ypos >> width >> height >> textureSource;				
			loadedObject = new StaticPlatform(xpos, ypos, width, height, textureSource);
		}
		else if(objectType == MOVING_PLATFORM)
		{
			POS startPos, endPos;
			float speed;
			fin >> xpos >> ypos >> startPos.x >> startPos.y >> endPos.x >> endPos.y >> width >> height >> speed >> textureSource;
			loadedObject = new MovingPlatform(xpos, ypos, width, height, textureSource, startPos, endPos, HORIZONTAL, speed);
		}
		else if(objectType == NORMAL_ENEMY)
		{
			POS startPos, endPos;
			float speed;
			int health, damage;

			fin >> xpos >> ypos >> startPos.x >> startPos.y >> endPos.x >> endPos.y >> width >> height >> speed >> health >> damage >> textureSource;
			loadedObject = new Enemy(xpos, ypos, width, height, textureSource, startPos, endPos, HORIZONTAL, speed, health, damage);			
		}	
		addObject(loadedObject);
	}

	fin.close();
}

void Level::addObject(Object *object)
{
	if(object != NULL)	{
		object->setID(nextObjectID);
		mObjectList.push_back(object);
		nextObjectID++;
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
			itr = mObjectList.erase(itr);			
			break;
		}
		else	{
			itr++;
			i++;
		}
	}
	nextObjectID -=1;
}

void Level::updateLevel(double dt)	
{
	// update player
	mPlayer->update(dt, this);
	
	// update enemies
	// update entities
	for (int i = 0;i < mObjectList.size();i++)
	{
		mObjectList[i]->update(dt);

		// remove killed enemies from the list - HAAAAAAAAAAAAAAAACKKKK
		if(mObjectList[i]->getType() == NORMAL_ENEMY)	{
			Enemy *tmpEnemy = dynamic_cast<Enemy*>(mObjectList[i]);
			if(!tmpEnemy->getAlive())
				deleteObject(tmpEnemy->getID());
		}
	}
}

void Level::drawLevel(void)
{
	gGraphics->drawText(collisionText, 700, 20);

	if(mPlayer != NULL)
		mPlayer->draw();

	for (int i = 0;i < mObjectList.size();i++)
	{
		mObjectList[i]->draw();
	}
}

void Level::collision(Player *player)
{
	char buffer[256];
	//MTV mtv;
	RECT tmpRect;
	bool collision = false;
	RECT objectRect;
	RECT playerRect = player->getRect();

	// loop through each object and test collision against the player
	for(int i = 0; i < mObjectList.size(); i++)
	{		
		MTV mtv = polyCollision(mObjectList[i]->getPolygon(), player->getPolygon());

		if(mtv.collision)	{
			sprintf(collisionText, "pushX: %f, pushY: %f", mtv.pushX, mtv.pushY);
			//MessageBox(0, buffer, 0, 0);
			player->move(-mtv.pushX, -mtv.pushY);
		}
	}

	/*
	//	AABB TEST
	//
	// add all the collision the list to processed
	// simply a AABB broadphase test
	for (int i = 0;i < mObjectList.size();i++)
	{
		// get the rect
		objectRect = mObjectList[i]->getRect();

		// collision?
		if(!(playerRect.top >= objectRect.bottom ||  playerRect.bottom <= objectRect.top ||  playerRect.right <= objectRect.left ||  playerRect.left >= objectRect.right))
		{
			// a collision occured -> add to list
			mCollisionList.push_back(mObjectList[i]);
		}	
	}

	// process collisions
	if(mCollisionList.size() > 0)
	{
		for (int i = 0;i < mCollisionList.size();i++)
		{
			// get the rect
			objectRect = mCollisionList[i]->getRect();

			// find out direction - normal
			// move player out from object
			// call onEvent()
			MessageBox(0, "hejja!", 0, 0);
		}
	*/

	
}

/* SAT - separating axis theorem

	I have tried to comment the code as much as I could.
	This is the steps that we have to do:

	1.) Loop through the sides of polygon A
	2.) Calculate the axis (the normal pointing OUT from the polygon) for each side
	3.) Normalize the axis so it becomes a pure direction vector
	4.) Project each point from polygon A on the axis
	5.) Find the maximum and minimum points
	6.) Project each point from polygon B on the axis
	7.) Find the maximum and minimum points
	8.) Check if they overlap
	9.) Calculate the smallest intersection depth and it's direction (MTV)
	10.) Return the MTV at the end of the function

	These steps have to be repeated for polygon B as well to ensure there's no false collisions

*/
MTV Level::polyCollision(CollisionPolygon *polygonA, CollisionPolygon *polygonB)
{	
	CollisionPolygon::Point axis;															// axis we will project onto
	CollisionPolygon::Point projection;														// the direction of the projection
	int side;																				// current side 
	double minA, maxA, minB, maxB, axisLen, tmp, minLen = 999999, tmpDepth = 1;;			// doubles needed
	MTV mtv;																				// minimum intersection

	// test polygon A's sides
	for(side = 0; side < polygonA->sides; side++)
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
			axis.x = -(polygonA->pointList[polygonA->sides-1].y - polygonA->pointList[0].y);
			axis.y = polygonA->pointList[polygonA->sides-1].x - polygonA->pointList[0].x;	
		}
		else
		{
			axis.x = -(polygonA->pointList[side].y - polygonA->pointList[side-1].y);
			axis.y = polygonA->pointList[side-1].x - polygonA->pointList[side].x;	// should be reversed?:O
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

		/* project polygon A's points on the axis to find MIN and MAX points */
		/*
			pointA.x = (pointA.x * axis.x + pointA.y * axis.y) * axis.x = dp * axis.x
			pointA.y = (pointA.x * axis.x + pointA.y * axis.y) * axis.y = dp * axis.y
			
			the dot-product is enough, graphicaly it need adjustment but mathematicly it works
			that gives us:

			pointA.x = pointA.x * axis.x + pointA.y * axis.y = dp
			pointA.y = pointA.x * axis.x + pointA.y * axis.y = dp 		
		*/

		minA = maxA = (polygonA->pointList[0].x * axis.x + polygonA->pointList[0].y * axis.y);

		for(int i = 1; i < polygonA->sides; i++)
		{
			tmp = (polygonA->pointList[i].x * axis.x + polygonA->pointList[i].y * axis.y);

			if(tmp < minA)
				minA = tmp;
			else if (tmp > maxA)
				maxA = tmp;
		}

		/* project polygon B's points on the axis to find MIN and MAX points */
		minB = maxB = (polygonB->pointList[0].x * axis.x + polygonB->pointList[0].y * axis.y);

		for(int i = 1; i < polygonB->sides; i++)
		{		
			tmp = (polygonB->pointList[i].x * axis.x + polygonB->pointList[i].y * axis.y);		

			if(tmp < minB)
				minB = tmp;
			else if (tmp > maxB)
				maxB = tmp;
		}		

		/* test if they intersect, if not then store the penetration depth and axis */
		if(minA > maxB || maxA < minB)	{
			mtv.collision = false;
			return mtv;
		}
		else
		{			
			if(minA <= maxB)	{
				tmpDepth = maxB - minA;
			}
			else if(maxA >= minB)	{
				tmpDepth = maxA - minB;
			}
			
			if(tmpDepth < minLen)	{
				minLen = tmpDepth;
				projection = axis;
			}		
		}
	}

	/* the same thing again but now for object B, sometimes a collision from polygon A isn't a collision for polygon B */
	/*
		- see above for detailed comments
	*/

	// test polygon B's sides
	for(side = 0; side < polygonB->sides; side++)
	{
		/* get the axis we will project on */
		if(side == 0)	// the first point must get connected to the last
		{			
			axis.x = -(polygonB->pointList[polygonB->sides-1].y - polygonB->pointList[0].y);
			axis.y = polygonB->pointList[polygonB->sides-1].x - polygonB->pointList[0].x;
		}
		else
		{
			axis.x = -(polygonB->pointList[side].y - polygonB->pointList[side-1].y);
			axis.y = polygonB->pointList[side-1].x - polygonB->pointList[side].x;
		}

		/* normalize the axis */	
		axisLen = sqrt(axis.x * axis.x + axis.y * axis.y);
		axis.x /= axisLen;
		axis.y /= axisLen;

		/* project polygon B's points on the axis to find MIN and MAX points */		
		minB = maxB = (polygonB->pointList[0].x * axis.x + polygonB->pointList[0].y * axis.y);

		for(int i = 1; i < polygonB->sides; i++)
		{
			tmp = (polygonB->pointList[i].x * axis.x + polygonB->pointList[i].y * axis.y);			

			if(tmp < minB)
				minB = tmp;
			else if (tmp > maxB)
				maxB = tmp;
		}

		/* project polygon A's points on the axis to find MIN and MAX points */	
		minA = maxA = (polygonA->pointList[0].x * axis.x + polygonA->pointList[0].y * axis.y);

		for(int i = 1; i < polygonA->sides; i++)
		{	
			tmp = (polygonA->pointList[i].x * axis.x + polygonA->pointList[i].y * axis.y);

			if(tmp < minA)
				minA = tmp;
			else if (tmp > maxA)
				maxA = tmp;
		}

		/* test if they intersect, if not then return false */
		if(minA > maxB || maxA < minB)	{
			mtv.collision = false;
			return mtv;
		}
		else
		{	
			double tmpDepth = 1;
			
			if(minA <= maxB)	{
				tmpDepth = maxB - minA;
			}
			else if(maxA >= minB)	{
				tmpDepth = maxA - minB;
			}
			
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
	if(!gGraphics)
		MessageBox(0, "NULL PTR", 0, 0);

	// statiska
	for (int i = 0;i < mObjectList.size();i++)
	{
		mObjectList[i]->draw();
		if(mObjectList[i]->getType() == MOVING_PLATFORM || mObjectList[i]->getType() == NORMAL_ENEMY)
			mObjectList[i]->drawPath();		// base class shouldn't have a drawPath() function
	}
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

