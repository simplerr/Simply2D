#include "Object.h"
#include "MovingPlatform.h"
#include "StaticPlatform.h"

class ActiveObject
{
public:
	ActiveObject();
	~ActiveObject();

	void setObject(Object *object);
	void clear(void);

	/*RECT getRect(void);
	void move(float dx, float dy);
	void setXY(float x, float y);

	float getX(void);
	float getY(void);
	int getHeight(void);
	int getWidth(void);*/

public:
	Object *mObject;
	MovingPlatform *mMovingPlatform;
	StaticPlatform *mStaticPlatform;
};