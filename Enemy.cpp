#include "Enemy.h"
#include "Player.h"
#include "Events.h"
#include "Level.h"
#include "Sound.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

extern Sound *gSound;

Enemy::Enemy(float x, float y, int width, int height, char *textureSource, POS startPos, POS endPos, movingType moveType, float speed, int health, int dmg)
	: MovingObject(NORMAL_ENEMY, x, y, width, height, textureSource,  startPos, endPos, moveType, speed)
{
	mHealth = health;
	mDamage = dmg;
	attackReady = true;
	frame = 0;
	alive = true;
	deathAnim = false;
	faceDir = RIGHT;
	dtsum = 0;
	cooldown = 0;
	setResizeable(false);
	playsDeathSound = false;
	srand (time(NULL));
}

Enemy::~Enemy()
{
	// dtor
}

void Enemy::update(float dt)
{
	// do normal update for a moving object
	MovingObject::update(dt);

	// update frame
	if(dtsum >= .15)	{
		frame++;
		dtsum = 0;
	}
	else
		dtsum += dt;

	if(!deathAnim && frame > 3)
		frame = 0;
	else if(deathAnim && frame > 4)
		setRemove(true);	// will remove it from the list

	// update attack cooldown
	if(!attackReady)
	{
		if(cooldown >= 1)	{
			attackReady = true;
			cooldown = 0;
		}
		else 
			cooldown += dt;
	}

	// checks health, sets deathanim
	if(mHealth <= 0 && !deathAnim)	{
		deathAnim = true;
		frame = 4;
	}
}
void Enemy::draw(void)
{
	// draw animation corresponding to active status
	faceDir = getMoveDir();

	gGraphics->BlitAnimation(getTexture(), getRect(), 0xFFFFFFFF, 0, 0, frame, 0.0f, faceDir);
}

void Enemy::saveToFile(std::ofstream *fout)
{
	*fout << getType() << " " << getID() << " " << (int)getX() << " " << (int)getY() << " " << (int)getStartPos().x << " " << (int)getStartPos().y << " " << (int)getEndPos().x << " " << (int)getEndPos().y << " ";
	*fout << getWidth() << " " << getHeight() << " " << getSpeed() << " " << mHealth << " " << mDamage << " ";
	*fout << getTextureSource() << endl;
}
void Enemy::move(float dx, float dy)
{
	// maybe waste
	MovingObject::move(dx, dy);
}
void Enemy::scale(direction side, int dwidth, int dheight)
{
	// maybe waste
	MovingObject::scale(side, dwidth, dheight);
}

void Enemy::onPlayerCollision(Player *player, MTV mtv, float dt)
{
	MovingObject::onPlayerCollision(player, mtv, dt);

	/* player jumped on enemy */
	if(mtv.pushY < 0)
	{
		this->damage(100);
		if(!playsDeathSound)	{
			gSound->playEffect("misc\\sound\\head_jump.wav");
			playsDeathSound = true;
		}
	}
	else
	{
		player->damage(mDamage);
		attackReady = false;
		int random = rand() % 3;

		if(random == 0)
			gSound->playEffect("misc\\sound\\hit1.wav");	
		else if(random == 1)
			gSound->playEffect("misc\\sound\\hit2.wav");
		else if(random == 2)
			gSound->playEffect("misc\\sound\\hit3.wav");
	}
}

void Enemy::OnEvent(Event *e)
{
	PlayerAttackEvent *atkEvent = NULL;
	// could be a switch statement, but prolly not needed here
	if(e->getType() == PLAYER_ATTACK)	{
		atkEvent = dynamic_cast<PlayerAttackEvent*>(e);
	}

	damage(atkEvent->getDamage()); 

	// could be bullet hit
	// spike wall etc..
}

std::vector<Property> Enemy::getProperties(void)
{
	std::vector<Property> properties = MovingObject::getProperties();

	Property tmp;
	char buffer[16];

	tmp.name = "damage";
	sprintf(buffer, "%i", mDamage);
	tmp.value = buffer;
	
	properties.push_back(tmp);

	tmp.name = "health";
	sprintf(buffer, "%i", mHealth);
	tmp.value = buffer;

	properties.push_back(tmp);

	return properties;
}
void Enemy::loadProperties(std::vector<Property> propertyList)
{
	// x,y,widht,height,startx,starty,endx,endy,speed
	MovingObject::loadProperties(propertyList);

	int tmp;

	tmp = atoi(propertyList[5].value.c_str());	// damage
	if(tmp != mDamage)	{
			mDamage = tmp;
	}

	float tmp2;

	tmp2 = atof(propertyList[6].value.c_str());	// health
	if(tmp2 != mHealth)	{
			mHealth = tmp2;
	}
}

void Enemy::damage(int dmg)
{
	mHealth -= dmg;
}