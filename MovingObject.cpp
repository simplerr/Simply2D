#include "MovingObject.h"
#include "Player.h"
#include "Level.h"

MovingObject::MovingObject(ObjectType type, float x, float y, int width, int height, char *textureSource, POS startPos, POS endPos, movingType moveType, float speed)
	:Object(x, y, width, height, textureSource, type)
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

	setStatic(false);

	mStartPos = startPos;
	mEndPos = endPos;

	if(mStartPos.x < mEndPos.x)
		mMovingDir = RIGHT;
	else if(mStartPos.x > mEndPos.x)
		mMovingDir = LEFT;
	else
		mMovingDir = LEFT; // if ==

	mSpeed = speed;
	mMoveType = moveType;
	mTravelX = mEndPos.x - mStartPos.x;
	mTravelY = mEndPos.y - mStartPos.y;
}

MovingObject::~MovingObject()
{
	// dtor
}

void MovingObject::update(float dt)
{
		// flytta plattform
		if(mMoveType == HORIZONTAL)
		{
			if(mMovingDir == LEFT)
			{
				if(mStartPos.x < mEndPos.x)
				{
					if(getX() > mStartPos.x)
						move(-mSpeed, 0);
					else
						mMovingDir = RIGHT;
				}
				else if(mStartPos.x > mEndPos.x)
				{
					if(getX() > mEndPos.x)	{
						move(-mSpeed, 0);					
					}
					else mMovingDir = RIGHT;
				}

				// move player
				/*if(getPlayer() != NULL)	{
					if(getPlayerCollision())
						onPlayerCollision();
				}*/
			}
			else if(mMovingDir == RIGHT)
			{
				if(mStartPos.x < mEndPos.x)	{
					if(getX() < mEndPos.x)	
						move(mSpeed, 0);
					else
						mMovingDir = LEFT;
				}
				else if(mStartPos.x > mEndPos.x)	{
					if(getX() < mStartPos.x)
						move(mSpeed, 0);
					else mMovingDir = LEFT;
				}

				/*if(getPlayer() != NULL)	{
					if(getPlayerCollision())
						onPlayerCollision();
				}*/
			}
		}
}

bool MovingObject::getPlayerCollision(Player *player)
{
	RECT objectRect = getRect();
	RECT playerRect = player->getRect();	

	if(!(playerRect.top -1 >= objectRect.bottom ||  playerRect.bottom +1<= objectRect.top ||  playerRect.right +1 <= objectRect.left ||  playerRect.left  -1 >= objectRect.right))
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
			pathRect.right = endPos.x;
			pathRect.top = getY() + getHeight()/2 - 5;
			pathRect.bottom = getY() + getHeight()/2 +5;
		}
	else if(endPos.x < getX())
		{
			pathRect.left = endPos.x + getWidth();
			pathRect.right = activeObjectRect.left;
			pathRect.top = getY() + getHeight()/2 - 5;
			pathRect.bottom = getY() + getHeight()/2 + 5;
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

void MovingObject::scale(direction side, int dwidth, int dheight)
{
	//activeObject->setXY(activeObjectRect.left + activeObject->getWidth()/2, activeObject->getY());

	Object::scale(side, dwidth, dheight);

	mStartPos.x = getX();
	mStartPos.y = getY();

	mEndPos.y = mStartPos.y + mTravelY;
	mEndPos.x = mStartPos.x + mTravelX;
	
}

RECT MovingObject::getEndPosRect(void)
{
	RECT tmpRect;
	tmpRect.left = mEndPos.x;
	tmpRect.right = mEndPos.x + getWidth();
	tmpRect.top = mEndPos.y;
	tmpRect.bottom = mEndPos.y + getHeight();

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

void MovingObject::onPlayerCollision(Player *player, MTV mtv)
{
	// if the player stands on platform
	if(mtv.pushY < 0)
	{
		if(getMoveDir() == LEFT)
			player->move(-mSpeed, 0);//player->mDX-=mSpeed;//
		else if(getMoveDir() == RIGHT)
			player->move(mSpeed, 0);//player->mDX+=mSpeed;//player->move(mSpeed, 0);
	}
}

std::vector<Property> MovingObject::getProperties(void)
{
	std::vector<Property> properties = Object::getProperties();

	mStartPos.x = getX();
	mStartPos.y = getY();

	Property tmp;
	char buffer[16];

	tmp.name = "endx";
	sprintf(buffer, "%i", (int)mEndPos.x);
	tmp.value = buffer;
	
	properties.push_back(tmp);

	tmp.name = "endy";
	sprintf(buffer, "%i", (int)mEndPos.y);
	tmp.value = buffer;
	
	properties.push_back(tmp);

	tmp.name = "speed";
	sprintf(buffer, "%.2f", mSpeed);
	tmp.value = buffer;
	
	properties.push_back(tmp);

	return properties;
}

void MovingObject::loadProperties(std::vector<Property> propertyList)
{
	// x,y,widht,height
	Object::loadProperties(propertyList);

	int tmp;

	// these are only neccessary for now!
	tmp = atoi(propertyList[4].value.c_str());	// endx
	if(tmp != mEndPos.x)	{
			mEndPos.x = tmp;
	}

	float tmp2;

	tmp2 = atof(propertyList[6].value.c_str());	// speed
	if(tmp2 != mSpeed)	{
			mSpeed = tmp2;
	}
}

void MovingObject::drawEditorFX(void)
{
	Object::drawEditorFX();

	bool drawPath = true;
	RECT rect = getRect();
	POS endPos= getEndPos();

	RECT pathRect;
	if(endPos.x > getX())
	{
		// inside object -> dont draw path
		if(endPos.x <= getX() + getWidth())
			drawPath = false;
		else	{
			pathRect.left = rect.right;
			pathRect.right = endPos.x;
			pathRect.top = getY() + getHeight()/2 - 5;
			pathRect.bottom = getY() + getHeight()/2 + 5;
		}
	}
	else if(endPos.x < getX())
	{
		// inside object -> dont draw path
		if(endPos.x + getWidth() >= getX())
			drawPath = false;
		else	{
			pathRect.left = endPos.x + getWidth();
			pathRect.right = rect.left;
			pathRect.top = getY() + getHeight()/2 - 5;
			pathRect.bottom = getY() + getHeight()/2 + 5;
		}
	}
	else
		pathRect = rect;

	// dont draw if inside object
	if(drawPath)
		gGraphics->BlitRect(pathRect, D3DCOLOR_ARGB(150, 0, 166, 255));


	gGraphics->BlitRect(getEndPosRect(), D3DCOLOR_ARGB(150, 255, 166, 0));
}

ObjectArea MovingObject::getAreaAt(double mx, double my)
{
	ObjectArea tmpArea = Object::getAreaAt(mx, my);
	RECT endPosRect = getEndPosRect();


	// now also check the end rect
	if(mx > endPosRect.left && mx < endPosRect.right && my > endPosRect.top && my < endPosRect.bottom)
		tmpArea = END_RECT;

	return tmpArea;
}