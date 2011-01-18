#include "MovingPlatform.h"

MovingPlatform::MovingPlatform(float x, float y, int width, int height, char *textureSource, POINT startPos, POINT endPos, Player *player,  movingType moveType, float speed)
	:Object(x, y, width, height, textureSource, MOVING_PLATFORM)
{
	mStartPos = startPos;
	mEndPos = endPos;
	mSpeed = speed;

	mGoalDir = END;
	mMoveType = moveType;

	mPlayer = player;

	/*RECT r = player->getRect();


	char buffer[256];
	sprintf(buffer, "aaaaamX2: %i, mY2: %i, mWidth2: %i, mHeight2: %i", r.left, r.top, r.bottom, r.right);
	MessageBox(0, buffer, 0, 0);*/

	// m�ste ta reda p� om endPos �r st�rre �n startPos
}

MovingPlatform::~MovingPlatform()
{
	// dtor
}

void MovingPlatform::update(float dt)
{
	if(mPlayer != NULL)
	{
		// flytta plattform
		if(mMoveType == HORIZONTAL)
		{
			if(mGoalDir == END)
			{
				if(getX() < mEndPos.x)	{
					move(mSpeed, 0);
				
					if(getPlayer() != NULL)	{
						if(getPlayerCollision())
							movePlayer(mSpeed, 0);
					}
				}
				else
					mGoalDir = START;
			}
			else if(mGoalDir == START)
			{
				if(getX() > mStartPos.x)	{
					move(-mSpeed, 0);

					if(getPlayer() != NULL)	{
						if(getPlayerCollision())
							movePlayer(-mSpeed, 0);
					}
				}
				else 
					mGoalDir = END;
			}
		}
	}

	/*if(getPlayerCollision())	{
		mPlayer->move(
	}*/
}

bool MovingPlatform::getPlayerCollision(void)
{
	RECT objectRect = getRect();
	RECT playerRect = mPlayer->getRect();	

	if(!(playerRect.top >= objectRect.bottom ||  playerRect.bottom <= objectRect.top ||  playerRect.right <= objectRect.left ||  playerRect.left >= objectRect.right))
		return true;
	else
		return false;
}


void MovingPlatform::draw(void)
{
	// Beh�ver inte mer avancerad draw
	Object::draw();
}