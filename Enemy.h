#ifndef ENEMY_H
#define ENEMY_H

#include <fstream>
#include "MovingObject.h"
#include "constants.h"
//#include "Events.h"

class Event;
class MovingObject;

class Enemy : public MovingObject
{
public:
	Enemy(float x, float y, int width, int height, char *textureSource, POS startPos, POS endPos, movingType moveType = HORIZONTAL, float speed = 0.02f,
		int health = 100, int dmg = 50);
	~Enemy();

	void update(float dt);
	void draw(void);

	void OnEvent(Event *e);	

	void saveToFile(std::ofstream *fout);
	void move(float dx, float dy);
	void scale(int dwidth, int dheight);
	void onPlayerCollision(Player *player);

	void damage(int dmg) {mHealth -= dmg;};
	int getHealth(void) {return mHealth;};
	bool getAlive(void)	{return alive;};
private:
	int mHealth;
	int mDamage;
	bool attackReady;
	bool deathAnim;
	bool alive;
	double cooldown;
	double dtsum;
private:
	int frame;	// maybe local static?
	direction faceDir;
};

#endif