#include "MovingObject.h"

MovingObject::MovingObject(float x, float y, int width, int height, char *textureSource, POS startPos, POS endPos, Player *player,  movingType moveType, float speed)
	:Object(x, y, width, height, textureSource, MOVING_PLATFORM)
{

	// if horizontal when vertical gets added
	/*if(startPos.x > endPos.x)	{
		mStartPos = endPos;
		mEndPos = startPos;
	}
	else if(startPos.x < endPos.x)	{
		mStartPos = startPos;
		mEndPos = endPos;
	}*/

	mStartPos = startPos;
	mEndPos = endPos;

	mSpeed = speed;
	mGoalDir = END;
	mMoveType = moveType;
	mPlayer = player;
	mTravelX = mEndPos.x - mStartPos.x;
	mTravelY = mEndPos.y - mStartPos.y;
}

MovingObject::~MovingObject()
{
	// dtor
}

void MovingObject::update(float dt)
{
	
	if(mPlayer != NULL)
	{
		// flytta plattform
		if(mMoveType == HORIZONTAL)
		{
			if(mGoalDir == END)
			{				
				if(mStartPos.x < mEndPos.x)	{					
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
				else if(mStartPos.x > mEndPos.x)	{
					if(getX() > mEndPos.x)	{
						move(-mSpeed, 0);
				
						if(getPlayer() != NULL)	{
							if(getPlayerCollision())
								movePlayer(-mSpeed, 0);
						}
					}
					else
						mGoalDir = START;
				}
			}
			else if(mGoalDir == START)
			{
				if(mStartPos.x < mEndPos.x)	{
					if(getX() > mStartPos.x)	{
						move(-mSpeed, 0);

						if(getPlayer() != NULL)	{
							if(getPlayerCollision())
								movePlayer(-mSpeed, 0);
						}
					}
					else {
						mGoalDir = END;
					}
				}
				else if(mStartPos.x > mEndPos.x)	{
					if(getX() < mStartPos.x)	{
						move(mSpeed, 0);

						if(getPlayer() != NULL)	{
							if(getPlayerCollision())
								movePlayer(mSpeed, 0);
						}
					}
					else {
						mGoalDir = END;
					}
				}
			}
		}
	}

	/*if(getPlayerCollision())	{
		mPlayer->move(
	}*/
}

bool MovingObject::getPlayerCollision(void)
{
	RECT objectRect = getRect();
	RECT playerRect = mPlayer->getRect();	

	if(!(playerRect.top -1 >= objectRect.bottom ||  playerRect.bottom +1<= objectRect.top ||  playerRect.right +1<= objectRect.left ||  playerRect.left -1 >= objectRect.right))
		return true;
	else
		return false;
}


void MovingObject::draw(void)
{
	// Behöver inte mer avancerad draw
	Object::draw();
}

void MovingObject::drawPath(void)
{
	RECT activeObjectRect = getRect();
	POS endPos = getEndPos();

	RECT pathRect;
	if(endPos.x > getX())
		{
			pathRect.left = activeObjectRect.right;
			pathRect.right = endPos.x - getWidth()/2;
			pathRect.top = getY() - 5;
			pathRect.bottom = getY() + 5;
		}
	else if(endPos.x < getX())
		{
			pathRect.left = endPos.x + getWidth()/2;
			pathRect.right = activeObjectRect.left;
			pathRect.top = getY() - 5;
			pathRect.bottom = getY() + 5;
		}
		else
			pathRect = activeObjectRect;

	gGraphics->BlitRect(pathRect, D3DCOLOR_ARGB(150, 0, 166, 255));

	// displays the end pos, and the drag rect of the active object
	gGraphics->BlitRect(getEndPosRect(), D3DCOLOR_ARGB(150, 255, 166, 0));
}

void MovingObject::saveToFile(ofstream *fout)
{

	*fout << getType() << " " << (int)getX() << " " << (int)getY() << " " << (int)mStartPos.x << " " << (int)mStartPos.y << " " << (int)mEndPos.x << " " << (int)mEndPos.y << " ";
	*fout << getWidth() << " " << getHeight() << " " << getSpeed() << " ";
	*fout << getTextureSource() << endl;
}

void MovingObject::move(float dx, float dy)
{
	Object::move(dx, dy);

	// start pos ska inte följa med ..
	/*mStartPos.x += dx;
	mStartPos.y += dy;
	mEndPos.x += dx;
	mEndPos.y += dy;*/


	// updatera start och end rects!
}

void MovingObject::setStartPos(POS pos)
{
	mStartPos.x = pos.x;
	mStartPos.y = pos.y;

	mTravelX = mEndPos.x - mStartPos.x;
	mTravelY = mEndPos.y - mStartPos.y;
}
void MovingObject::setEndPos(POS pos)
{
	mEndPos.x = pos.x;
	mEndPos.y = pos.y;

	mTravelX = mEndPos.x - mStartPos.x;
	mTravelY = mEndPos.y - mStartPos.y;
}

void MovingObject::scale(int dwidth, int dheight)
{
	//activeObject->setXY(activeObjectRect.left + activeObject->getWidth()/2, activeObject->getY());

	Object::scale(dwidth, dheight);

	mStartPos.x = getX();
	mStartPos.y = getY();

	mEndPos.y = mStartPos.y + mTravelY;
	mEndPos.x = mStartPos.x + mTravelX;
	
}

RECT MovingObject::getEndPosRect(void)
{
	RECT tmpRect;
	tmpRect.left = mEndPos.x - getWidth()/2;
	tmpRect.right = mEndPos.x + getWidth()/2;
	tmpRect.top = mEndPos.y - getHeight()/2;
	tmpRect.bottom = mEndPos.y + getHeight()/2;

	return tmpRect;
}

void MovingObject::editorMove(float dx, float dy)
{
	Object::move(dx, dy);

	// start pos ska inte följa med ..
	mStartPos.x += dx;
	mStartPos.y += dy;
	mEndPos.x += dx;
	mEndPos.y += dy;
}

void MovingObject::setXY(float x, float y)
{
	Object::setXY(x, y);

	mStartPos.x = getX();
	mStartPos.y = getY();
	mEndPos.x = mStartPos.x + mTravelX;
	mEndPos.y = mStartPos.y + mTravelY;
}
