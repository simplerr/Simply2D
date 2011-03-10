#ifndef STATIC_PLATFORM
#define STATIC_PLATFORM

#include "Object.h"
#include "d3dUtil.h"
#include "Player.h"

class StaticPlatform : public Object
{
public:
	StaticPlatform(float x, float y, int width, int height, char *textureSource);
	~StaticPlatform();

	void saveToFile(ofstream *fout);
private:
};

#endif

