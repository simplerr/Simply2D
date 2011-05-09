#include "Bullet.h"
#include "Enemy.h"
#include "Player.h"
#include "Sound.h"

extern Sound *gSound;

Bullet::Bullet(float x, float y, int width, int height, direction dir, int damage, float speed, int lifelength, BulletOwner owner, char *textureSource)
	: Object(x, y, width, height, textureSource, BULLET)
{
	mDamage = damage;
	mLifeLength = lifelength;
	mOwner = owner;
	mDir = dir;
	mSpeed = speed;
	mErase = false;
	mTravelled = 0;
	setResizeable(false);

	if(mOwner == ENEMIES)
		mRightBullet = gGraphics->loadTexture("misc\\textures\\right_turret_laser.bmp");
	else
		mRightBullet = gGraphics->loadTexture("misc\\textures\\right_bullet.bmp");
}

Bullet::~Bullet()
{
	// dtor
}

void Bullet::update(float dt)
{
	/* update position */
	if(mDir == RIGHT)
		move(mSpeed, 0);
	else if(mDir == LEFT)
		move(-mSpeed, 0);

	mTravelled += mSpeed;

	if(mTravelled > mLifeLength)
		mErase = true;
}
	
void Bullet::draw(void)
{
	Object::draw();


	if(mDir == LEFT)
		Object::draw();
	else
		gGraphics->drawShape(*getShape(), mRightBullet);
}

void Bullet::onObjectCollision(Object *object)
{
	// hurt the object!
	if(object->getType() == NORMAL_ENEMY)
	{
		Enemy *enemy = dynamic_cast<Enemy*>(object);
		enemy->damage(mDamage);
		if(enemy->getHealth() <= 0)	{
			gSound->playEffect("misc\\sound\\head_jump.wav");
			
			int random = rand() % 10;

			if(random == 0)
				gSound->playEffect("misc\\sound\\nukem1.wav");
			else if(random == 1)
				gSound->playEffect("misc\\sound\\nukem2.wav");

		}
	}
}

void Bullet::onPlayerCollision(Player *player, MTV mtv)	
{
	player->damage(mDamage);
	int random = rand() % 3;

	if(random == 0)
		gSound->playEffect("misc\\sound\\hit1.wav");	
	else if(random == 1)
		gSound->playEffect("misc\\sound\\hit2.wav");
	else if(random == 2)
		gSound->playEffect("misc\\sound\\hit3.wav");
}