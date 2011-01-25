#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include "constants.h"
#include "Object.h"
#include "DynamicObject.h"
#include "Player.h"
#include <list>
#include <fstream>
#include <string>
#include <vector>

class Object;
class DynamicObject;
class Player;

class GameWorld
{
public:
	GameWorld(Player *player);
	//GameWorld();
	~GameWorld();

	void saveLevel(char* levelFile);
	void loadLevel(char* levelFile);

	void deleteStaticObject(int ID);
	void deleteDynamicObject(int ID);
	Object *getObjectAt(POINT mpos);

	void updateLevel(double dt);	
	void drawLevel(void);
	void drawEditorLevel(void);

	CollisionStruct collision(Player *player);
	Object* objectIntersection(Object *activeObject);
	int getSnapDist(RECT rect, direction checkDir);
	POS getSpawn(void)  {return spawnPos;};
	RECT getSpawnRect(void);
	void setSpawn(POS pos)	{spawnPos = pos;};
	void spawnPlayer(void);

	//void addStaticObject(int x, int y, int width, int height);
	//void addDynamicObject(int x, int y, int width, int height);

	void addStaticObject(Object *object);
	void addDynamicObject(Object *object);

private:
	std::vector<Object*> mStaticObjectList;		// platforms, ladders, trampolines, teleporters, walljumps etc..
	std::vector<Object*> mDynamicObjectList;	// enemies, power-ups, 

	char *collisionText;

	int nextStaticID;
	int nextDynamicID;

	POS spawnPos;

	Player *mPlayer;
};

#endif