#include "Gate.h"

Gate::Gate(float x, float y, int width, int height, char *textureSource, float buttonx, float buttony,float openTime)
	: Object(x, y, width, height, textureSource, GATE)
{
	mOpenButton = new Object(buttonx, buttony, 20, 40, (char*)GATEBUTTON_UNPRESSED_SOURCE.c_str(), GATEBUTTON); 

	mActivationRect = mOpenButton->getShape()->getRect();
	mActivationRect.left -= 20;
	mActivationRect.right += 20;
	mActivationRect.top -= 20;
	mActivationRect.bottom += 20;

	mOpen = false;
	mOpenTime = openTime;
	mTimeElapsed = 0;
}

Gate::~Gate()
{
	// dtor
}

void Gate::update(float dt)
{
	if(mPlayer->getActivateKey())
	{
		// inside activation rect?
		RECT playerRect = mPlayer->getShape()->getRect();
		if(playerRect.right > mActivationRect.left && playerRect.left < mActivationRect.right && playerRect.bottom > mActivationRect.top
		&& playerRect.top < mActivationRect.bottom)	{

			if(!mOpen)	{
				// open the gate
				move(0, 2000);	// hack
				mPlayer->setActivateKey(false);
				mTimeElapsed = 0;
				mOpen = true;
			}
			else	{
				// close the gate
				move(0, -2000);	// hack
				mPlayer->setActivateKey(false);
				mOpen = false;
			}
		}
	}

	if(mOpen)
	{
		mTimeElapsed += dt;

		if(mTimeElapsed >= mOpenTime)	{
			// close the gate
			mOpen = false;
			move(0, -2000);
		}
	}
}

void Gate::draw(void)
{
	Object::draw();
	mOpenButton->draw();
	gGraphics->BlitRect(mActivationRect);
}

void Gate::saveToFile(std::ofstream *fout)
{

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

	return properties;
}

void Gate::loadProperties(std::vector<Property> propertyList)
{

}

void Gate::setPlayer(Player *player)
{
	mPlayer = player;
}

bool Gate::insideActivateArea(void)
{
	if(mPlayer->getX() > mActivationRect.left && mPlayer->getX() < mActivationRect.right && mPlayer->getY() > mActivationRect.top
		&& mPlayer->getY() < mActivationRect.bottom)
		return true;
	else
		return false;
}