/*#ifndef DYNAMIC_OBJECT
#define DYNAMIC_OBJECT

#include "object.h"

class Object;
class Player;

class DynamicObject : public Object
{
public:
	DynamicObject(float x, float y, int width, int height, char *textureSource, ObjectType type, Player *player);
	virtual ~DynamicObject();

	virtual void update(float dt);

	bool getPlayerCollision(void);
	void movePlayer(double dx, double dy)	{mPlayer->move(dx, dy);};
	Player *getPlayer(void) {return mPlayer;};
private:
	Player *mPlayer;
};

#endif*/