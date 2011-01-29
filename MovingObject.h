#ifndef MOVINGOBJECT_H
#define MOVINGOBJECT_H

#include "Object.h"

class MovingObject : public Object
{
public:
	MovingObject(float x, float y, int width, int height, char *textureSource, POS startPos, POS endPos, Player *player,  movingType moveType, float speed);
	virtual ~MovingObject();

	virtual void update(float dt);
	virtual void draw(void);
	virtual void saveToFile(ofstream *fout);
	virtual void move(float dx, float dy);
	virtual void scale(int dwidth, int dheight);

	void drawPath(void);
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
	void setXY(float x, float y);
	
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