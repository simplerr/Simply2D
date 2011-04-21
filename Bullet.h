#ifndef BULLET_H
#define BULLET_H

#include "Object.h"

enum BulletOwner
{
	PLAYER,
	ENEMIES
};

class Bullet : public Object
{
public:
	Bullet(float x, float y, int width, int height, direction dir, int damage, float speed, int lifelength, BulletOwner owner, char *textureSource);
	~Bullet();

	void update(float dt);
	void draw(void);
	void onObjectCollision(Object* object);
	void erase(void) {mErase = true;}
	bool getErased(void) {return mErase;}

private:
	float mSpeed;
	int mDamage;
	int mLifeLength;
	float mTravelled;
	direction mDir;
	BulletOwner mOwner;
	bool mErase;
};

#endif