#include "Bullet.h"
#include "Enemy.h"
#include "Player.h"

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
	}
}

void Bullet::onPlayerCollision(Player *player, MTV mtv)	
{
	player->damage(mDamage);
}