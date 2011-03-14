#include "Object.h"
#include "Shape.h"
#include <fstream>
using namespace std;

Object::Object(float x, float y, int width, int height, char *textureSource, ObjectType type, bool updates)
{
	mShape.origin.x = x;
	mShape.origin.y = y;

	// these are defined in local space
	// origin is in the top left of the shape	
	mShape.addPoint(Shape::Point(0, 0));	// top - left
	mShape.addPoint(Shape::Point(0, height));	// bottom - left
	mShape.addPoint(Shape::Point(width, height));   // bottom - right
	mShape.addPoint(Shape::Point(width, 0));	// top - right

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
	//gGraphics->BlitTexture(mTexture, getRect(), 0xFFFFFFFF, 0);
	gGraphics->drawShape(mShape, mTexture);
}

RECT Object::getRect(void)
{
	/*rect.left = mX - mWidth/2;
	rect.right = mX + mWidth/2;
	rect.top = mY - mHeight/2;
	rect.bottom = mY + mHeight/2;*/

	return mShape.getRect();
}

void Object::move(float dx, float dy)
{
	mShape.origin.x += dx;
	mShape.origin.y += dy;
}

void Object::scale(direction side, int dwidth, int dheight)
{
	mShape.scale(side, dwidth, dheight);	
}

void Object::saveToFile(ofstream *fout)
{
	*fout << getType() << " " << getX() << " " << getY() << " " << getWidth() << " " << getHeight() << " " << getTextureSource() << endl;
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

void Object::editorMove(float dx, float dy)		
{
	mX += dx;
	mY += dy;

	mShape.origin.x += dx;
	mShape.origin.y += dy;
}

double Object::getWidth(void)											
{
	return mShape.aabb.right - mShape.aabb.left;
}

double Object::getHeight(void)										
{
	return mShape.aabb.bottom - mShape.aabb.top;
}

// returns the center position
double Object::getX(void)										
{
	return mShape.origin.x ;//+ getWidth()/2;
}
double Object::getY(void)
{
	return mShape.origin.y ;//+ getHeight()/2;
}

std::vector<Property> Object::getProperties(void)
{
	std::vector<Property> properties;

	Property tmp;
	char buffer[16];

	tmp.name = "x";
	sprintf(buffer, "%i", (int)mShape.origin.x);
	tmp.value = buffer;

	properties.push_back(tmp);

	tmp.name = "y";
	sprintf(buffer, "%i", (int)mShape.origin.y);
	tmp.value = buffer;

	properties.push_back(tmp);

	tmp.name = "width";
	sprintf(buffer, "%i", (int)mShape.aabb.right);
	tmp.value = buffer;

	properties.push_back(tmp);

	tmp.name = "height";
	sprintf(buffer, "%i", (int)mShape.aabb.bottom);
	tmp.value = buffer;
	
	properties.push_back(tmp);

	return properties;
}

void Object::loadProperties(std::vector<Property> propertyList)
{
	// change the string to int and load it into the object
	int tmp;

	tmp = atoi(propertyList[0].value.c_str());	// x
	mShape.origin.x = tmp;

	tmp = atoi(propertyList[1].value.c_str());	// y
	mShape.origin.y = tmp;

	tmp = atoi(propertyList[2].value.c_str());	// width
	if(tmp != mShape.aabb.right)	{
			mShape.setWidth(tmp);
	}

	tmp = atoi(propertyList[3].value.c_str());	// height
	if(tmp != mShape.aabb.bottom)	{
			mShape.setHeight(tmp);
	}
}