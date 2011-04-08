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
#include "Mouse.h"
#include "Functors.h"
using namespace std;

// Developing a GUI Using C++ and DirectX
// Source Listing 1
// Window Class

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
	
	virtual void update(double dt)	{}
	virtual void draw(void)			{}
	virtual bool pressed(int mx, int my)		{return true;}
	virtual void hoover(int mx, int my)		{}

	WindowID getID(void)		{return mID;}
	int getPrimaryID(void)			{return mPrimaryID;}
	void setPrimaryID(int id)	{mPrimaryID = id;}
	
	// also does stuff neccassary on deactivation
	virtual void setActive(bool b)	{mActive = b;}
	void overlaped(bool b)			{mOverlaped = b;}
	bool getOverlap(void)			{return mOverlaped;}

	// ??
	virtual void onDeactive(void);
	virtual int wm_keydown(WPARAM wParam)	{return 1;}

	bool isActive(void) { return mActive;}
	void setValue(string value) {mValue = value;}
	void setVisible(bool value) {mVisible = value;}
	virtual void move(int dx, int dy) {mX += dx; mY += dy;}
	virtual void setPos(int x, int y) {mX = x; mY = y;}

	/*
	void setActive(bool state){if(mActiveWin != NULL) mActiveWin->mActive = state;};
	virtual void messageHandler(WindowID sender, string data = "nothing") {};
	void keyPressed(WPARAM wParam);
	void sendMousePress(int mx, int my);
	virtual int wm_lbuttondown(int x, int y);
	
	Window *findChildAtCoord(int x, int y);
	*/

	int getX(void) { return mX;}
	int getY(void) { return mY;}
	int getWidth(void) { return mWidth;}
	int getHeight(void) { return mHeight;}
	string getValue(void)	{return mValue;}
	bool getVisible(void) {return mVisible;}
	RECT getRect(void);

protected:
	//MessageFunctor* mFunctor; 

	WindowID mID;
	string mValue;
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