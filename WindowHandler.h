#ifndef WINDOWHANDLER_H
#define WIDNOWHANDLER_H

#include "Window.h"
#include <vector>

using namespace std;
class Window;

class WindowHandler
{
public:
	WindowHandler();											// for a window that covers the whole screen
	WindowHandler(int x, int y, int width, int height);			// for a window that is local
	~WindowHandler();

	bool update(double dt);										// updates the active window, checks for mouse presses/hoovers
	void draw(void);											// draw all windows in the list

	void addWindow(Window* window);								// adds window to list
	void removeWindow(Window* window);							// deletes window with same ID

	RECT getRect(void);											// returns the rect, used to set windows positions
	void keyPressed(WPARAM wParam);								// keyboard input

	void setBackground(string source);							// sets a background, default is none

private:
	vector<Window*> mWindowList;
	Window* mActiveWindow;

	IDirect3DTexture9* mBackground;
	int mNextPrimaryID;
	int mX;
	int mY;
	int mWidth;
	int mHeight;
};

#endif