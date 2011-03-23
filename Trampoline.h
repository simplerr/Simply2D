#include "Object.h"

class Trampoline : public Object
{
public:
	Trampoline(float x, float y, int width, int height, int boostHeight, char *textureSource);
	~Trampoline();

	
	void draw(void);

	void onPlayerCollision(Player *player, MTV mtv);
	std::vector<Property> getProperties(void);
	void loadProperties(std::vector<Property> propertyList);
	void saveToFile(std::ofstream *fout);
	void loadFromFile(std::ofstream *fout);

private:
	int mBoostHeight;
};