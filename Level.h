#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include "constants.h"
/*#include "Object.h"
#include "MovingObject.h
#include "Player.h"
*/
#include <list>
#include <fstream>
#include <string>
#include <vector>

// might be error
#include "Object.h"

class Object;
class MovingObject;
class Player;

struct MTV
{
	MTV(){};
	MTV(double d, CollisionPolygon::Point axis)	{pushX = d*axis.x; pushY = d*axis.y;};

	bool collision;
	double pushX;
	double pushY;
};

class Level
{
public:
	Level(Player *player);		// take width, height, spawn pos? nah - loads needed stuff from file
	~Level();

	void saveLevel(char* levelFile);
	void loadLevel(char* levelFile);

	void addObject(Object *object);
	void deleteObject(int ID);
	Object *getObjectAt(POINT mpos);

	void updateLevel(double dt);	
	void drawLevel(void);
	void drawEditorLevel(void);

	void collision(Player *player);
	MTV polyCollision(CollisionPolygon *polygonA, CollisionPolygon *polygonB);
	Object* objectIntersection(Object *activeObject);
	int getSnapDist(RECT rect, direction checkDir);
	POS getSpawn(void)  {return spawnPos;};
	RECT getSpawnRect(void);
	void setSpawn(POS pos)	{spawnPos = pos;};
	void spawnPlayer(void);

private:
	std::vector<Object*> mObjectList;		// platforms, ladders, trampolines, teleporters, walljumps etc.. ALL
	std::vector<Object*> mCollisionList;

	char *collisionText;

	POS spawnPos;
	int nextObjectID;
	Player *mPlayer;
};

#endif