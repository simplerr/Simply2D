#include "Object.h"
#include <fstream>
using namespace std;

Object::Object(float x, float y, int width, int height, char *textureSource, ObjectType type, bool updates)
{
	mX = x;
	mY = y;
	mWidth = width;
	mHeight = height;

	// since all objects only are rects right now
	mPolygon.center.x = mX;
	mPolygon.center.y = mY;

	mPolygon.pointList.push_back(CollisionPolygon::Point(mX - mWidth/2, mY - mHeight/2));	// top - left
	mPolygon.pointList.push_back(CollisionPolygon::Point(mX - mWidth/2, mY + mHeight/2));	// bottom - left
	mPolygon.pointList.push_back(CollisionPolygon::Point(mX + mWidth/2, mY + mHeight/2));   // bottom - right
	mPolygon.pointList.push_back(CollisionPolygon::Point(mX + mWidth/2, mY - mHeight/2));	// top - right

	mPolygon.sides = 4;

	mType = type;
	mUpdates = updates;

	mTexture = gGraphics->loadObjectTexture(textureSource);
	mTextureSource = new char[256];
	memcpy(mTextureSource,textureSource, 255);
}
Object::~Object()
{
	// dtor
	// ska inte radera texturen, det sköter Level
}
void Object::draw(void)
{
	gGraphics->BlitTexture(mTexture, getRect(), 0xFFFFFFFF, 0);
}

RECT Object::getRect(void)
{
	RECT rect;
	rect.left = mX - mWidth/2;
	rect.right = mX + mWidth/2;
	rect.top = mY - mHeight/2;
	rect.bottom = mY + mHeight/2;

	return rect;
}

void Object::move(float dx, float dy)
{
	mX += dx;
	mY += dy;
}

void Object::scale(int dwidth, int dheight)
{
	mWidth += dwidth;
	mHeight += dheight;
}

void Object::saveToFile(ofstream *fout)
{
	*fout << getType() << " " << mX << " " << mY << " " << mWidth << " " << mHeight << " " << getTextureSource() << endl;
}
void Object::loadFromFile(std::ofstream *fout)
{

}

void Object::setTextureSource(char *source)
{
	memcpy(mTextureSource, source, 255);
	char buffer[256];
	mTexture = gGraphics->loadTexture(mTextureSource);
}