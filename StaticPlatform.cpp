#include "StaticPlatform.h"
#include <fstream>

using namespace std;

StaticPlatform::StaticPlatform(float x, float y, int width, int height, char *textureSource)
	:Object(x, y, width, height, textureSource, STATIC_PLATFORMA)
{
	// nada
	setResizeable(true);
	setStatic(true);
}

StaticPlatform::~StaticPlatform()
{
	// nada!
}

void StaticPlatform::saveToFile(ofstream *fout)
{
	*fout << getType() << " " << getX() << " " << getY() << " " << getWidth() << " " << getHeight() << " " << getTextureSource() << endl;
}