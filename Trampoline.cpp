#include "Trampoline.h"
#include "Player.h"

Trampoline::Trampoline(float x, float y, int width, int height, int boostHeight, char *textureSource)
	:Object(x, y, width, height, textureSource, TRAMPOLINE)
{
	mBoostHeight = boostHeight;
	setResizeable(false);
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
	*fout << getType() << " " << (int)getX() << " " << (int)getY() << " ";
	*fout << getWidth() << " " << getHeight() << " " << mBoostHeight << " ";
	*fout << getTextureSource() <<  std::endl;
}

void Trampoline::loadFromFile(std::ofstream *fout)
{

}

void Trampoline::onPlayerCollision(Player *player, MTV mtv)
{
	// boost if true
	if(mtv.pushY < 0)
	{
		player->jump(mBoostHeight);
	}
		
}

std::vector<Property> Trampoline::getProperties(void)
{
	std::vector<Property> properties = Object::getProperties();

	Property tmp;
	char buffer[16];

	tmp.name = "boost";
	sprintf(buffer, "%i", (int)mBoostHeight);
	tmp.value = buffer;

	properties.push_back(tmp);

	return properties;
}

void Trampoline::loadProperties(std::vector<Property> propertyList)
{
	Object::loadProperties(propertyList);

	int tmp;

	tmp = atoi(propertyList[4].value.c_str());
	mBoostHeight = tmp;
}
