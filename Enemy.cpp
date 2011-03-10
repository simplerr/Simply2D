#include "Enemy.h"
#include "Player.h"
#include "Events.h"
using namespace std;

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
	if(dtsum >= .08)	{
		frame++;
		dtsum = 0;
	}
	else
		dtsum += dt;

	if(!deathAnim && frame > 3)
		frame = 0;
	else if(deathAnim && frame > 6)
		alive = false; // animation done  -> dead

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
		frame = 5;
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
	*fout << getType() << " " << (int)getX() << " " << (int)getY() << " " << (int)getStartPos().x << " " << (int)getStartPos().y << " " << (int)getEndPos().x << " " << (int)getEndPos().y << " ";
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

void Enemy::onPlayerCollision(Player *player)
{
	MovingObject::onPlayerCollision(player);
	if(player->getDY() > 0)
	{
		// player hit enemy on the head -> enemy = dead
		if(player->getRect().bottom == getRect().top)	{	// smth weird here
			this->damage(100);	// kills the enemy
		}
		else	{
			// 1 attack/second
			if(attackReady)	{
				player->damage(mDamage);
				attackReady = false;
			}
		}
	}
	else
	{	
		if(!(player->getRect().bottom == getRect().top))	{		
			// 1 attack/second
			if(attackReady)	{
				player->damage(mDamage);
				attackReady = false;
			}
		}
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