#include "Object.h"

class GunPowerup : public Object
{
public:
	GunPowerup(float x, float y, int width, int height, char *textureSource, int ammo, int bulletType = 0);
	~GunPowerup();

	void saveToFile(std::ofstream *fout);
	void onPlayerCollision(Player *player, MTV mtv, float dt);

	std::vector<Property> getProperties(void);
	void loadProperties(std::vector<Property> propertyList);
private:
	int mAmmo;
	int mBulletType;
};