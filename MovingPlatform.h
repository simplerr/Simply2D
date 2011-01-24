#ifndef MOVINGPLATFORM_H
#define MOVINGPLATFORM_H

#include "Object.h"
#include "DynamicObject.h"
#include "Player.h"

/*struct FPOINT
{
	float x;
	float y;
};*/

enum goal
{
	START,
	END
};

enum movingType
{
	HORIZONTAL,
	VERTICAL
};

class MovingPlatform : public Object
{
public:
	MovingPlatform(float x, float y, int width, int height, char *textureSource, POINT startPos, POINT endPos, Player *player, movingType moveType = HORIZONTAL, float speed = 0.02f );
	~MovingPlatform();

	void update(float dt);
	void draw(void);

	void setStartPos(POINT pos);
	void setEndPos(POINT pos);
	void setSpeed(float speed) { mSpeed = speed;};
	POINT getStartPos(void) { return mStartPos;};
	POINT getEndPos(void)	{return mEndPos;};
	float getSpeed(void) {return mSpeed;};

	bool getPlayerCollision(void);
	void movePlayer(double dx, double dy)	{mPlayer->move(dx, dy);};
	Player *getPlayer(void) {return mPlayer;};

	void move(float dx, float dy);

	void saveToFile(ofstream *fout);

private:
	POINT mStartPos;
	POINT mEndPos;
	float mSpeed;

	float mDX;

	goal mGoalDir;
	movingType mMoveType;

	Player *mPlayer;
};

#endif