#ifndef ENEMY_H
#define ENEMY_H

#include "MovingObject.h"

class Enemy : public MovingObject
{
public:
	Enemy(float x, float y, int width, int height, char *textureSource, POS startPos, POS endPos, Player *player, movingType moveType = HORIZONTAL, float speed = 0.02f,
		int health = 100, int dmg = 50);
	~Enemy();

	void update(float dt);
	void draw(void);

	void saveToFile(std::ofstream *fout);
	void move(float dx, float dy);
	void scale(int dwidth, int dheight);
	void onPlayerCollision(void);

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