#include "Spike.h"
#include "Player.h"

Spike::Spike(float x, float y, int width, int height, char *textureSource, int damage)
	:Object(x, y, width, height, textureSource, SPIKE, TRIANGLE)
{
	mDamage = damage;
	setResizeable(false);
}

Spike::~Spike()
{
	// dtor
}

void Spike::update(float dt)
{

}

void Spike::draw(void)
{
	Object::draw();
}

void Spike::saveToFile(std::ofstream *fout)
{
	*fout << getType() << " " << getX() << " " << getY() << " " << getWidth() << " " << getHeight() << " " << getDamage() << " " << getTextureSource() << std::endl;
}

void Spike::move(float dx, float dy)
{

}

void Spike::scale(direction side, int dwidth, int dheight)
{

}

void Spike::onPlayerCollision(Player *player, MTV mtv)
{
	player->damage(5000);
}

std::vector<Property> Spike::getProperties(void)
{
	std::vector<Property> propertyList;

	propertyList = Object::getProperties();

	Property tmp;
	char buffer[16];

	tmp.name = "damage";
	sprintf(buffer, "%i", mDamage);
	tmp.value = buffer;
	
	propertyList.push_back(tmp);

	return propertyList;
}

void Spike::loadProperties(std::vector<Property> propertyList)
{
	Object::loadProperties(propertyList);

	int tmp;

	tmp = atoi(propertyList[4].value.c_str());	// damage
	if(tmp != mDamage)	{
			mDamage = tmp;
	}
}