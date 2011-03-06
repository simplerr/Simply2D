#ifndef _MOUSE_H_
#define _MOUSE_H_

#include "d3dUtil.h"
#include "DirectInput.h"

enum MouseButton
{
	LEFTBUTTON,
    RIGHTBUTTON,
	MIDDLEBUTTON
};

class Mouse
{
public:
	Mouse(HWND hwnd);
	~Mouse();
	void updateMouseWIN(void);
	void updateMouseDX(void);

	POINT getPos(void)	{return mPos;};
	POINT getScreenPos(void);
	bool buttonDown(MouseButton button);
	void drawMousePos(void);

	void move(int dx, int dy);

	void setMousePos(int x, int y);
	void setX(int x);
	void setY(int y);
	void setVX(int x)	{mPos.x = x;};
	void setVY(int y)	{mPos.y = y;};

	void restore(void);

private:
	POINT mPos;
	HWND mMainWnd;
};

#endif