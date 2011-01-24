#ifndef OBJECT_H
#define OBJECT_H

#include "constants.h"
#include "d3dUtil.h"
#include "Player.h"

class Player;

class Object
{
public:
	Object(float x, float y, int width, int height, char *textureSource, ObjectType type);
	virtual ~Object();

	virtual void draw(void);

	float getX(void) {return mX;};
	float getY(void) {return mY;};
	int getHeight(void) {return mHeight;};
	int getWidth(void) {return mWidth;};
	int getID(void) {return mID;};
	int getType(void) {return mType;};
	RECT getRect(void);		// dynamicly return the rect!
	char* getTextureSource(void) {return mTextureSource;};

	void setID(int ID) {mID = ID;};
	void setTexture(IDirect3DTexture9* texture)	{mTexture = texture;};
	void setTextureSource(char *source);

	virtual void move(float dx, float dy);
	void scale(int dwidth, int dheight);

	void setXY(float x, float y) {mX = x; mY = y;};
	void setWidth(int width) {mWidth = width;};
	void setHeight(int height) {mHeight = height;};

	virtual void update(float dt)	{;};

	virtual void saveToFile(std::ofstream *fout);
	virtual void loadFromFile(std::ofstream *fout);

private:
	IDirect3DTexture9* mTexture;
	char *mTextureSource;
	//RECT mRect;
	float mX;
	float mY;
	int mWidth;
	int mHeight;
	int mID;
	ObjectType mType;
};

#endif