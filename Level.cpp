#include "Level.h"
#include "StaticPlatform.h"
#include "Object.h"
#include "MovingPlatform.h"
#include "Trampoline.h"
#include "WallJump.h"
#include "Teleport.h"
#include "Enemy.h"
#include "Camera.h"
#include <fstream>

using namespace std;

extern Camera* gGameCamera;

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
	char *destTextureSource = new char[256];		// ulgy hax, dont use NEW HERE FFS!

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
		else if(objectType == TELEPORT)
		{
			int destx, desty;
			fin >> xpos >> ypos >> destx >> desty >> width >> height >> textureSource >> destTextureSource;
			loadedObject = new Teleport(xpos, ypos, destx, desty, width, height, textureSource, destTextureSource);
		}
		else if(objectType == TRAMPOLINE)
		{
			int boostHeight;

			fin >> xpos >> ypos >> width >> height >> boostHeight >> textureSource;
			loadedObject = new Trampoline(xpos, ypos, width, height, boostHeight, textureSource);
		}
		else if(objectType == WALLJUMP)
		{
			fin >> xpos >> ypos >> width >> height >> textureSource;				
			loadedObject = new WallJump(xpos, ypos, width, height, textureSource);
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
	if(mPlayer != NULL)
		mPlayer->draw();

	for (int i = 0;i < mObjectList.size();i++)
	{
		mObjectList[i]->draw();
	}
}

void Level::collision(Player *player)
{
	bool onGround = false;
	// loop through each object and test collision against the player
	for(int i = 0; i < mObjectList.size(); i++)
	{		
		MTV mtv = polyCollision(mObjectList[i]->getShape(), player->getShape());

		if(mtv.collision)	{
			// push out from collision
			player->move(mtv.pushX, mtv.pushY);

			// the camera have to follow the player
			if(player->getX() >= 516)
				gGameCamera->addMovement(mtv.pushX, 0);

			// what's gonna happen with the player?
			mObjectList[i]->onPlayerCollision(player, mtv);	

			// on ground?
			if(onGround)
				continue;
			
			if(mtv.pushY < 0)	{
				// trampoline?
				if(mObjectList[i]->getType() != TRAMPOLINE)	
					onGround = true;

				// could use a if statement, but not neccessary ( if prev != 1337 )
				player->setPrevWallJumpID(1337);
			}
		}	
	}

	if(onGround)
		player->onGround(true);
	else
		player->onGround(false);
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
			axis.x = -(ShapeA->pointList[side].y - ShapeA->pointList[side-1].y);
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
		//mObjectList[i]->draw();
		//if(mObjectList[i]->getType() == MOVING_PLATFORM || mObjectList[i]->getType() == NORMAL_ENEMY)
		//	mObjectList[i]->drawPath();		// base class shouldn't have a drawPath() function
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

