#include "GunPowerup.h"
#include "Player.h"
#include "Sound.h"

extern Sound *gSound;

GunPowerup::GunPowerup(float x, float y, int width, int height, char *textureSource, int ammo, int bulletType)
	:Object(x, y, width, height, textureSource, GUNPOWERUP)
{
	mAmmo = ammo;
	mBulletType = bulletType;
	setResizeable(false);
}
	
GunPowerup::~GunPowerup()
{

}

void GunPowerup::saveToFile(std::ofstream *fout)
{
	*fout << getType() << " " << getID() << " " << getX() << " " << getY() << " ";
	*fout << getWidth() << " " << getHeight() << " " << mAmmo << " " << mBulletType << " " << getTextureSource() << std::endl;
}
	
void GunPowerup::onPlayerCollision(Player *player, MTV mtv)
{
	player->lootGun(mAmmo, mBulletType);
	gSound->mEngine->play2D("misc\\sound\\gun_pickup.wav");
	setRemove(true);
}

std::vector<Property> GunPowerup::getProperties(void)
{
	std::vector<Property> properties = Object::getProperties();

	Property tmp;
	char buffer[16];

	tmp.name = "ammo";
	sprintf(buffer, "%i", mAmmo);
	tmp.value = buffer;
	
	properties.push_back(tmp);

	tmp.name = "type";
	sprintf(buffer, "%i", mBulletType);
	tmp.value = buffer;
	
	properties.push_back(tmp);

	return properties;
}

void GunPowerup::loadProperties(std::vector<Property> propertyList)
{
	Object::loadProperties(propertyList);

	int tmp;

	tmp = atoi(propertyList[2].value.c_str());	// ammo
	if(tmp != mAmmo)	{
			mAmmo = tmp;
	}

	tmp = atoi(propertyList[3].value.c_str());	// bullet type
	if(tmp != mBulletType)	{
			mBulletType = tmp;
	}
}