#include "MovingPlatform.h"

MovingPlatform::MovingPlatform(float x, float y, int width, int height, char *textureSource, POINT startPos, POINT endPos, Player *player,  movingType moveType, float speed)
	:Object(x, y, width, height, textureSource, MOVING_PLATFORM)
{

	// if horizontal when vertical gets added
	if(startPos.x > endPos.x)	{
		mStartPos = endPos;
		mEndPos = startPos;
	}
	else if(startPos.x < endPos.x)	{
		mStartPos = startPos;
		mEndPos = endPos;
	}
	
	mSpeed = speed;

	mGoalDir = END;
	mMoveType = moveType;

	mPlayer = player;
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

	if(!(playerRect.top -1 >= objectRect.bottom ||  playerRect.bottom +1<= objectRect.top ||  playerRect.right +1<= objectRect.left ||  playerRect.left -1 >= objectRect.right))
		return true;
	else
		return false;
}


void MovingPlatform::draw(void)
{
	// Behöver inte mer avancerad draw
	Object::draw();
}

void MovingPlatform::saveToFile(ofstream *fout)
{

	*fout << getType() << " " << (int)getX() << " " << getY() << " " << mStartPos.x << " " << mStartPos.y << " " << mEndPos.x << " " << mEndPos.y << " ";
	*fout << getWidth() << " " << getHeight() << " " << getSpeed() << " ";
	*fout << getTextureSource() << endl;
}

void MovingPlatform::move(float dx, float dy)
{
	Object::move(dx, dy);

	mStartPos.x += dx;
	mStartPos.y += dy;
	mEndPos.x += dx;
	mEndPos.y += dy;


	// updatera start och end rects!
}

void MovingPlatform::setStartPos(POINT pos)
{
	mStartPos.x = pos.x;
	mStartPos.y = pos.y;
}
void MovingPlatform::setEndPos(POINT pos)
{
	mEndPos.x = pos.x;
	mEndPos.y = pos.y;
}