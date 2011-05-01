#ifndef GATE_H
#define GATE_H

#include "Object.h"
#include "Gate.h"

class Gate	:	public Object
{
public:
	Gate(float x, float y, int width, int height, char *textureSource, float openTime);
	~Gate();

	void update(float dt);
	void draw(void);

	void saveToFile(std::ofstream *fout);
	void move(float dx, float dy);
	void scale(direction side, int dwidth, int dheight);
	void onPlayerCollision(Player *player, MTV mtv);

	std::vector<Property> getProperties(void);
	void loadProperties(std::vector<Property> propertyList);

	void activate(void);
private:
	float mOpenTime;
	float mTimeElapsed;
	bool mOpen;
};
#endif