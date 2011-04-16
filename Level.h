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
#include "MTV.h"

class Object;
class MovingObject;
class Player;

enum LevelType
{
	CAMPAIGN = 301,
	CUSTOM
};


struct Warp
{
	int x,y;
	int width, height;
	IDirect3DTexture9* texture;
	RECT getRect(void)	{
		RECT r;
		r.left = x - width/2;
		r.right = x + width/2;
		r.top = y - height/2;
		r.bottom = y + height/2;

		return r;
	}
};

class Level
{
public:
	Level(Player *player);		// take width, height, spawn pos? nah - loads needed stuff from file
	~Level();

	void saveLevel(char* levelFile);
	void loadLevel(char* levelFile);
	std::string getLevelName(void);
	void setLevelName(std::string levelName) { mLevelSource = levelName;}
	std::string getNextLevel(void) {return mNextLevel;};
	void setNextLevel(std::string nextLevel) { mNextLevel = nextLevel;}

	void addObject(Object *object);
	void deleteObject(int ID);
	Object *getObjectAt(POINT mpos);

	void updateLevel(double dt);	
	void drawLevel(void);
	void drawEditorLevel(void);

	void collision(Player *player);
	MTV polyCollision(Shape *ShapeA, Shape *ShapeB);
	Object* objectIntersection(Object *activeObject);
	int getSnapDist(RECT rect, direction checkDir);
	POS getSpawn(void)  {return spawnPos;};
	RECT getSpawnRect(void);
	Object* getWarp(void) {return mLevelWarp;};
	void setSpawn(POS pos)	{spawnPos = pos;};
	void spawnPlayer(void);

	LevelType getType(void)	{return mLevelType;}

private:
	std::vector<Object*> mObjectList;		// platforms, ladders, trampolines, teleporters, walljumps etc.. ALL
	std::vector<Object*> mCollisionList;

	std::string mLevelSource;
	std::string mNextLevel;

	POS spawnPos;
	Object* mLevelWarp;
	int nextObjectID;
	Player *mPlayer;
	LevelType mLevelType;
};

#endif