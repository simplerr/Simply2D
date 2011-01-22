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
using namespace std;

// Developing a GUI Using C++ and DirectX
// Source Listing 1
// Window Class

struct ListItem
{
	std::string itemName;
	RECT rect;
	int x, y, width, height;
	D3DCOLOR color;
	// background color
	// selected color
};


static char buffer[256];

class Window
{
public:
	Window(WindowID id, int x, int y, int width, int height, D3DCOLOR color = D3DCOLOR_ARGB( 155, 155, 200, 000 ));
	virtual ~Window();
	virtual void init(void) {};
	void setParent(Window* parent)	{mParent = parent;};
	Window *getParent(void)	{return(mParent);};
	Window *getWindow(void) {return(this);};
	WindowID getID(void) {return mID;};
	Window *findChildAtCoord(int x, int y);
	int windowCount(void) {return mSubWins.size();};

	WindowRect getInfo(void)	{return mPosition;};

	int addWindow(Window *w);
	int removewindow(Window *w){};

	void setpos(int x, int y)		{};
	void setsize(int width, int height)		{};
	virtual void updateRectToNewXY(void);
	bool isActive(void) { return mActive;};

	virtual void updateWindow(float dt)	{if(mActiveWin != this)mActiveWin->updateWindow(dt);};

 
	virtual int renderAll(void);
  
	//void setName(char *name) {mName = name;};
	string getValue(WindowID id);
	void setValue(WindowID id, string value);
	void setActive(bool state){mActiveWin->mActive = state;};
	void setVisibility(WindowID id, bool value);
	string getOwnValue(void)	{return mValue;};
	RECT getRect(void);
	void updateRect(void);

	virtual void messageHandler(WindowID sender, string data = "nothing") {};
	void keyPressed(WPARAM wParam);
	void sendMousePress(int mx, int my);
	virtual int wm_lbuttondown(int x, int y);
	virtual int wm_keydown(WPARAM wParam);

protected:
	Window *mParent;
	Window *mActiveWin;
	vector<Window*> mSubWins;
	WindowRect mPosition;
	D3DCOLOR mColor;

	WindowID mID;
	string mValue;
	WindowID mActiveWinID;

	bool mVisible;
	bool mActive;
	bool inputState;
};


#endif