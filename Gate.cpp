#include "Gate.h"
#include "ActivateButton.h"

Gate::Gate(float x, float y, int width, int height, char *textureSource, float openTime)
	: Object(x, y, width, height, textureSource, GATE)
{
	mOpen = false;
	mOpenTime = openTime;
	mTimeElapsed = 0;
	setResizeable(false);
	mActivateButton = NULL;
}

Gate::~Gate()
{
	// dtor
}

void Gate::update(float dt)
{
	if(mOpen)
	{
		mTimeElapsed += dt;

		if(mTimeElapsed >= mOpenTime)	{
			// close the ActivateButton
			mOpen = false;
			move(0, -2000);
		}
	}
}

void Gate::draw(void)
{
	Object::draw();
}

void Gate::saveToFile(std::ofstream *fout)
{
	*fout << getType() << " " << getID() << " " << getX() << " " << getY() << " " << getWidth() << " " << getHeight() << " ";
	*fout << mOpenTime << " " << getTextureSource() << std::endl;
}
	
void Gate::move(float dx, float dy)
{
	Object::move(dx, dy);
}
	
void Gate::scale(direction side, int dwidth, int dheight)
{

}
	
void Gate::onPlayerCollision(Player *player, MTV mtv)
{

}

std::vector<Property> Gate::getProperties(void)
{
	std::vector<Property> properties = Object::getProperties();

	Property tmp;
	char buffer[16];

	tmp.name = "open";
	sprintf(buffer, "%.2f", mOpenTime);
	tmp.value = buffer;

	properties.push_back(tmp);

	tmp.name = "id";
	sprintf(buffer, "%i", getID());
	tmp.value = buffer;
	
	properties.push_back(tmp);

	return properties;
}

void Gate::loadProperties(std::vector<Property> propertyList)
{
	Object::loadProperties(propertyList);

	float tmp;

	tmp = atof(propertyList[2].value.c_str());	// open time
	if(tmp != mOpenTime)	{
			mOpenTime = tmp;
	}
}

void Gate::activate(void)
{
	if(!mOpen)	{
		// open the ActivateButton
		move(0, 2000);	// hack
		mTimeElapsed = 0;
		mOpen = true;
	}
	else	{
		// close the ActivateButton
		move(0, -2000);	// hack
		mOpen = false;
	}
}

bool Gate::getOpen(void)
{
	return mOpen;
}

void Gate::setActivateButton(ActivateButton *button)
{
	mActivateButton = button;
}

void Gate::onRemove(void) 
{
	if(mActivateButton != NULL)	{
		mActivateButton->setGateId(9999);
		mActivateButton->connectGate(NULL);
	}
}