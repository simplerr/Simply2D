#include "Object.h"
#include "MovingPlatform.h"
#include "StaticPlatform.h"
#include "Enemy.h"

class ActiveObject
{
public:
	ActiveObject();
	~ActiveObject();

	void setObject(Object *object);
	void clear(void);

	void move(float dx, float dy)	{mObject->editorMove(dx, dy);};

	/*RECT getRect(void);	
	void setXY(float x, float y);
	float getX(void);
	float getY(void);
	int getHeight(void);
	int getWidth(void);*/

public:
	Object *mObject;
	MovingPlatform *mMovingPlatform;
	StaticPlatform *mStaticPlatform;
	Enemy *mEnemy;

	// ?? //
	//POS mSpawnPos;
	//bool movingSpawnPos;
};