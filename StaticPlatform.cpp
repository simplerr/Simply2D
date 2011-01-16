#include "StaticPlatform.h"

StaticPlatform::StaticPlatform(float x, float y, int width, int height, char *textureSource)
	:Object(x, y, width, height, textureSource, STATIC_PLATFORMA)
{
	// nada
}

StaticPlatform::~StaticPlatform()
{
	// nada!
}