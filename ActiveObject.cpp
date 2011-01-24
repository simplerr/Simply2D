#include "ActiveObject.h"

ActiveObject::ActiveObject()
{
	//mObject = new Object(;
	//mStaticPlatform = new StaticPlatform;
	//mMovingPlatform = new MovingPlatform;

	clear();
}
ActiveObject::~ActiveObject()
{
	// gameWorld deletes what's needed
}

void ActiveObject::setObject(Object *object)
{
	clear();
	mObject = object;

	if(mObject->getType() == MOVING_PLATFORM)
		mMovingPlatform = dynamic_cast<MovingPlatform*>(object);
	else if(mObject->getType() == STATIC_PLATFORMA)
		mStaticPlatform = dynamic_cast<StaticPlatform*>(object);
}

void ActiveObject::clear()
{
	mObject = NULL;
	mStaticPlatform = NULL;
	mMovingPlatform = NULL;
}