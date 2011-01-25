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
	MovingPlatform(float x, float y, int width, int height, char *textureSource, POS startPos, POS endPos, Player *player, movingType moveType = HORIZONTAL, float speed = 0.02f );
	~MovingPlatform();

	void update(float dt);
	void draw(void);

	void setStartPos(POS pos);
	void setEndPos(POS pos);
	void setSpeed(float speed) { mSpeed = speed;};
	POS getStartPos(void) { return mStartPos;};
	POS getEndPos(void)	{return mEndPos;};
	float getSpeed(void) {return mSpeed;};
	RECT getEndPosRect(void);

	bool getPlayerCollision(void);
	void movePlayer(double dx, double dy)	{mPlayer->move(dx, dy);};
	Player *getPlayer(void) {return mPlayer;};

	void editorMove(float dx, float dy);
	void move(float dx, float dy);
	void scale(int dwidth, int dheight);

	void saveToFile(ofstream *fout);

private:
	POS mStartPos;
	POS mEndPos;
	float mSpeed;
	float mDX;

	int mTravelX;
	int mTravelY;

	goal mGoalDir;
	movingType mMoveType;

	Player *mPlayer;
};

#endif