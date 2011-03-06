#ifndef MOVINGPLATFORM_H
#define MOVINGPLATFORM_H

#include "MovingObject.h"
//#include "Player.h"

class MovingObject;
class Player;

class MovingPlatform : public MovingObject
{
public:
	MovingPlatform(float x, float y, int width, int height, char *textureSource, POS startPos, POS endPos, movingType moveType = HORIZONTAL, float speed = 0.02f);
	~MovingPlatform();

	void update(float dt);
	void draw(void);

	void saveToFile(std::ofstream *fout);
	void move(float dx, float dy);
	void scale(int dwidth, int dheight);

private:
	// wut needed? >_>
};

#endif