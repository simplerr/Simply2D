#ifndef MOVINGOBJECT_H
#define MOVINGOBJECT_H

#include "Object.h"
#include <fstream>
class Object;

class MovingObject : public Object
{
public:
	MovingObject(ObjectType type, float x, float y, int width, int height, char *textureSource, POS startPos, POS endPos, movingType moveType, float speed);
	virtual ~MovingObject();

	std::vector<Property> getProperties(void);
	void loadProperties(std::vector<Property> propertyList);

	virtual void update(float dt);
	virtual void draw(void);
	void drawEditorFX(void);
	virtual void saveToFile(std::ofstream *fout);
	virtual void move(float dx, float dy);
	virtual void scale(direction side, int dwidth, int dheight);
	ObjectArea getAreaAt(double mx, double my);

	void drawPath(void);
	void setStartPos(POS pos);
	void setEndPos(POS pos);
	void setSpeed(float speed) { mSpeed = speed;};
	POS getStartPos(void) { return mStartPos;};
	POS getEndPos(void)	{return mEndPos;};
	float getSpeed(void) {return mSpeed;};
	RECT getEndPosRect(void);
	bool getPlayerCollision(Player *player);
	//void movePlayer(double dx, double dy)	{mPlayer->move(dx, dy);};
	//Player *getPlayer(void) {return mPlayer;};
	void editorMove(float dx, float dy);	
	void setXY(float x, float y);

	virtual void onPlayerCollision(Player *player, MTV mtv, float dt);

	direction getMoveDir()	{return mMovingDir;};
	
private:
	POS mStartPos;
	POS mEndPos;
	float mSpeed;
	float mDX;

	int mTravelX;
	int mTravelY;

	direction mMovingDir;
	movingType mMoveType;
};

#endif