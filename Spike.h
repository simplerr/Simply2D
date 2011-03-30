#include "Object.h"

class Spike : public Object
{
public:
	Spike(float x, float y, int width, int height, char *textureSource, int damage);
	~Spike();

	void update(float dt);
	void draw(void);

	void saveToFile(std::ofstream *fout);
	void move(float dx, float dy);
	void scale(direction side, int dwidth, int dheight);
	void onPlayerCollision(Player *player, MTV mtv);

	std::vector<Property> getProperties(void);
	void loadProperties(std::vector<Property> propertyList);

	int getDamage(void)	{return mDamage;};

private:
	int mDamage;
};