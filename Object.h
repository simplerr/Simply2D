#ifndef OBJECT_H
#define OBJECT_H

#include "constants.h"
#include "d3dUtil.h"
#include "Shape.h"
#include "Level.h"
#include "MTV.h"
#include <string.h>
//#include "Player.h"
//#include "Events.h"

class Player;
class Event;

struct Property
{
	std::string name;
	std::string value;
};

enum ShapeType
{
	QUAD = 301,
	TRIANGLE
};

class Object
{
public:
	Object(float x, float y, int width, int height, char *textureSource, ObjectType type, ShapeType shape = QUAD, bool updates = false);
	virtual ~Object();

	virtual void draw(void);
	virtual void drawEditorFX(void);
	virtual void drawPath(void)	{};
	virtual void OnEvent(Event *e)	{};

	virtual ObjectArea getAreaAt(double mx, double my);

	Shape* getShape(void);
	double getX(void);
	double getY(void);
	double getHeight(void);
	double getWidth(void);
	RECT* getDragRects(void);
	int getID(void)												{return mID;};
	int getType(void)											{return mType;};
	RECT getRect(void);	
	char* getTextureSource(void)								{return mTextureSource;};

	void setID(int ID) {mID = ID;};
	void setTexture(IDirect3DTexture9* texture)					{mTexture = texture;};
	void setTextureSource(char *source);
	IDirect3DTexture9 *getTexture(void)							{return mTexture;};

	virtual void editorMove(float dx, float dy);
	virtual void move(float dx, float dy);
	virtual void scale(direction side, int dwidth, int dheight);
	virtual void setXY(float x, float y)						{mX = x; mY = y;};

	void setWidth(int width)									{mWidth = width;};
	void setHeight(int height)									{mHeight = height;};

	virtual void update(float dt)								{};

	virtual void saveToFile(std::ofstream *fout);
	virtual void loadFromFile(std::ofstream *fout);
	bool getResizeable(void)									{return resizeable;};
	void setResizeable(bool b)									{resizeable = b;};

	// new 
	virtual void onPlayerCollision(Player *player, MTV mtv)				{};

	bool doUpdate(void) 										{return mUpdates;};
	void setStatic(bool b)										{mStatic = b;};
	bool isStatic(void)											{return mStatic;};

	void flipHorizontal(void);

	virtual std::vector<Property> getProperties(void);
	virtual void loadProperties(std::vector<Property> propertyList);

private:
	IDirect3DTexture9* mTexture;
	char mTextureSource[256];

	Shape mShape;

	float mX;
	float mY;
	int mWidth;
	int mHeight;
	int mID;
	ObjectType mType;
	bool resizeable;
	bool mUpdates;
	bool mStatic;
};

#endif