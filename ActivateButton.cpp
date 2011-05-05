#include "ActivateButton.h"
#include "Player.h"

ActivateButton::ActivateButton(float x, float y, int width, int height, char *textureSource)
	: Object(x, y, width, height, textureSource, ACTIVATE_BUTTON)
{
	mActivationRect = getShape()->getRect();
	mActivationRect.left -= 20;
	mActivationRect.right += 20;
	mActivationRect.top -= 20;
	mActivationRect.bottom += 20;

	setResizeable(false);

	mGate = NULL;
	mLevel = NULL;
	mGateId = 99999;

	mPressedTexture = gGraphics->loadObjectTexture((char*)ACTIVATEBUTTON_PRESSED_SOURCE.c_str());
}

ActivateButton::~ActivateButton()
{
	// dtor
}

void ActivateButton::update(float dt)
{
	static bool deleted;
	if(mGate != NULL)
	{
		deleted = false;
		if(mPlayer->getActivateKey())
		{
			// inside activation rect?
			RECT playerRect = mPlayer->getShape()->getRect();
			if(playerRect.right > mActivationRect.left && playerRect.left < mActivationRect.right && playerRect.bottom > mActivationRect.top
			&& playerRect.top < mActivationRect.bottom)	{

				mGate->activate();
				mPlayer->setActivateKey(false);
			}
		}
	}
	else	{
		if(!deleted)
			mGateId = 9999;
	}
}

void ActivateButton::draw(void)
{
	if(mGate != NULL)	{
		if(!mGate->getOpen())
			Object::draw();
		else
			gGraphics->drawShape(*getShape(), mPressedTexture);
	}
	else
		Object::draw();


	//gGraphics->BlitRect(mActivationRect);
}

void ActivateButton::saveToFile(std::ofstream *fout)
{
	*fout << getType() << " " << getID() << " " << getX() << " " << getY() << " " << getWidth() << " " << getHeight() << " ";
	if(mGate != NULL)
		*fout << mGate->getID() << " ";
	else	
		*fout << 99999 << " ";

	*fout << getTextureSource() << endl;
}
	
void ActivateButton::move(float dx, float dy)
{
	Object::move(dx, dy);

	mActivationRect.left += dx;
	mActivationRect.right += dx;
	mActivationRect.top += dy;
	mActivationRect.bottom += dy;
}
	
void ActivateButton::editorMove(float dx, float dy)
{
	Object::editorMove(dx, dy);

	mActivationRect.left += dx;
	mActivationRect.right += dx;
	mActivationRect.top += dy;
	mActivationRect.bottom += dy;
}

void ActivateButton::scale(direction side, int dwidth, int dheight)
{

}
	
void ActivateButton::onPlayerCollision(Player *player, MTV mtv)
{

}

std::vector<Property> ActivateButton::getProperties(void)
{
	std::vector<Property> properties = Object::getProperties();

	Property tmp;
	char buffer[16];

	tmp.name = "gate id";
	if(mGate != NULL)
		sprintf(buffer, "%i", mGate->getID());
	else
		sprintf(buffer, "%i", 99999);

	tmp.value = buffer;
	
	properties.push_back(tmp);

	return properties;
}

void ActivateButton::loadProperties(std::vector<Property> propertyList)
{
	Object::loadProperties(propertyList);

	int tmp;

	tmp = atoi(propertyList[2].value.c_str());	// gate id
	if(mGate == NULL)
		mLevel->connectGate(this, tmp);
	else if(tmp != mGate->getID())	{
		mLevel->connectGate(this, tmp);
	}
}

void ActivateButton::setPlayer(Player *player)
{
	mPlayer = player;
}

void ActivateButton::setLevel(Level *level)
{
	mLevel = level;
}

bool ActivateButton::insideActivateArea(void)
{
	if(mPlayer->getX() > mActivationRect.left && mPlayer->getX() < mActivationRect.right && mPlayer->getY() > mActivationRect.top
		&& mPlayer->getY() < mActivationRect.bottom)
		return true;
	else
		return false;
}

void ActivateButton::connectGate(Gate *gate)
{
	mGate = gate;
	if(mGate != NULL)
		mGate->setActivateButton(this);
}

void ActivateButton::setGateId(int id)
{
	mGateId = id;
}