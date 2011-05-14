#include "Trampoline.h"
#include "Player.h"
#include "Sound.h"

extern Sound *gSound;

Trampoline::Trampoline(float x, float y, int width, int height, double accel, double maxSpeed, char *textureSource)
	:Object(x, y, width, height, textureSource, TRAMPOLINE)
{
	mMaxSpeed = maxSpeed;
	mBoostAccel = accel;
	setResizeable(false);
	mBoostHeight = 100;
}

Trampoline::~Trampoline()
{
	// dtor
}


void Trampoline::draw(void)
{
	Object::draw();
}

void Trampoline::saveToFile(std::ofstream *fout)
{
	*fout << getType() << " " << getID() << " " << (int)getX() << " " << (int)getY() << " ";
	*fout << getWidth() << " " << getHeight() << " " << mBoostAccel << " " << mMaxSpeed << " ";
	*fout << getTextureSource() <<  std::endl;
}

void Trampoline::loadFromFile(std::ofstream *fout)
{

}

void Trampoline::onPlayerCollision(Player *player, MTV mtv, float dt)
{
	// boost if true
	if(mtv.pushY < 0)
	{
		player->jump(mBoostAccel, mMaxSpeed, mBoostHeight);
		gSound->playEffect("misc\\sound\\boing.wav");
	}
		
}

std::vector<Property> Trampoline::getProperties(void)
{
	std::vector<Property> properties = Object::getProperties();

	Property tmp;
	char buffer[16];

	tmp.name = "accel";
	sprintf(buffer, "%.1f", mBoostAccel);
	tmp.value = buffer;

	properties.push_back(tmp);

	tmp.name = "cap";
	sprintf(buffer, "%.2f", mMaxSpeed);
	tmp.value = buffer;

	properties.push_back(tmp);

	return properties;
}

void Trampoline::loadProperties(std::vector<Property> propertyList)
{
	Object::loadProperties(propertyList);

	double tmp;

	tmp = atof(propertyList[2].value.c_str());
	mBoostAccel = tmp;

	tmp = atof(propertyList[3].value.c_str());
	mMaxSpeed = tmp;
}
