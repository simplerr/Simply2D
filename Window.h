#ifndef _TEXTBOX_H_
#define _TEXTBOX_H_

//#include "O_Drawing.h"
#include <string>
#include <vector>
#include "d3dUtil.h"
#include <vector>
#include "Graphics.h"
#include <string>
#include "DirectInput.h"
#include "constants.h"
#include "WindowMessage.h"

#include "Functors.h"
using namespace std;

class WindowHandler;
class Editor;

struct ListItem
{
	std::string itemName;
	//RECT rect;
	int x, y, width, height;
	D3DCOLOR color;

	RECT getRect()
	{
		RECT tmpRect;
		tmpRect.left = x - width/2;
		tmpRect.right = x + width/2;
		tmpRect.top = y - height/2;
		tmpRect.bottom = y + height/2;

		return tmpRect;
	}
	// background color
	// selected color
};

static char buffer[256];

class Window
{
public:
	Window(WindowHandler *handler, WindowID id, int x, int y, int width, int height, D3DCOLOR color = D3DCOLOR_ARGB( 155, 155, 200, 000));
	virtual ~Window();
	
	virtual void update(double dt)			{ }
	virtual void draw(void)					{ }
	virtual bool pressed(int mx, int my)	{ return true;}
	virtual void hoover(int mx, int my)		{ }
	
	// hack primary and normal id? why?
	WindowID getID(void)					{ return mID;} 
	int getPrimaryID(void)					{ return mPrimaryID;}
	void setPrimaryID(int id)				{ mPrimaryID = id;}
	
	// also does stuff neccassary on deactivation
	virtual void setActive(bool b)			{ mActive = b;}
	void overlaped(bool b)					{ mOverlaped = b;}
	bool getOverlap(void)					{ return mOverlaped;}
	RECT getRect(void);

	// hack
	virtual void onDeactive(void);
	virtual int wm_keydown(WPARAM wParam)	{ return 1;}

	bool isActive(void)						{ return mActive;}
	void setVisible(bool value)				{ mVisible = value;}
	virtual void move(int dx, int dy)		{ mX += dx; mY += dy;}
	virtual void setPos(int x, int y)		{ mX = x; mY = y;}

	int getX(void)							{ return mX;}
	int getY(void)							{ return mY;}
	int getWidth(void)						{ return mWidth;}
	int getHeight(void)						{ return mHeight;}
	bool getVisible(void)					{ return mVisible;}

	//virtual void setValue(T value)			{ mValue = value;}
	//T getValue(void)						{ return mValue;}
protected:
	WindowID mID;
	D3DCOLOR mColor;

	int mPrimaryID;
	int mX;
	int mY;
	int mWidth;
	int mHeight;

	bool mVisible;
	bool mActive;
	bool mInputState;
	bool mOverlaped;
};

#endif