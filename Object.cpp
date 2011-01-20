#include "Object.h"

Object::Object(float x, float y, int width, int height, char *textureSource, ObjectType type)
{
	mX = x;
	mY = y;
	mWidth = width;
	mHeight = height;
	mType = type;

	mTexture = gGraphics->loadObjectTexture(textureSource);
	mTextureSource = new char[256];
	memcpy(mTextureSource,textureSource, 255);
	//mTextureSource = textureSource;

	// ska egentligen inte ladda en ny textur ifall det redan finns ett objekt med denna textur!
	// skapa en TextureHandler class eller liknande!
	// mTexture = gGraphics->loadTexture(textureSource);
	// fixas nu i Level::addXXXObject(...)

	// texturen laddas vid addObject(...)
}
Object::~Object()
{
	// dtor
	// ska inte radera texturen, det sköter GameWorld
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
	//fout->open(filename);

	*fout << getType() << " " << mX << " " << mY << " " << mWidth << " " << mHeight << " " << getTextureSource() << endl;

	//fout.close();
}