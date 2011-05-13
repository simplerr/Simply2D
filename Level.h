#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include "constants.h"
#include <list>
#include <fstream>
#include <string>
#include <vector>
#include "CampaignProgress.h"

// might be error
#include "Object.h"
#include "MTV.h"
//#include "ActivateButton.h"

class ActivateButton;
class Object;
class MovingObject;
class Player;

enum LevelType
{
	CAMPAIGN = 301,
	CUSTOM,
	TEST
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

	void addObject(Object *object, int id = 0);
	void deleteObject(int ID);
	void removeObject(int ID);
	Object *getObjectAt(POINT mpos);

	bool updateLevel(double dt);	
	void drawLevel(void);
	void drawEditorLevel(void);
	void drawGui(void);

	bool collision(Player *player, float dt);
	MTV polyCollision(Shape *ShapeA, Shape *ShapeB);
	Object* objectIntersection(Object *activeObject);
	int getSnapDist(RECT rect, direction checkDir);
	POS getSpawn(void)  {return spawnPos;}
	RECT getSpawnRect(void);
	Object* getWarp(void) {return mLevelWarp;}
	void setSpawn(POS pos)	{spawnPos = pos;}
	void spawnPlayer(void);

	LevelType getType(void)	{return mLevelType;}
	void setType(LevelType type) {mLevelType = type;}

	void connectGate(ActivateButton* button, int id);

private:
	std::vector<Object*> mObjectList;		// platforms, ladders, trampolines, teleporters, walljumps etc.. ALL

	CampaignProgress mProgress;

	std::string mLevelSource;
	std::string mNextLevel;

	POS spawnPos;
	Object* mLevelWarp;
	int mIdOffset;
	Player *mPlayer;
	LevelType mLevelType;
	bool mLoaded;
	double mPlayTime;
};

#endif