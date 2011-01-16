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
	POINT getPos(void)	{return mPos;};
	bool buttonDown(MouseButton button);
	void drawMousePos(void);

	void setMousePos(int x, int y);
	void setX(int x);
	void setY(int y);

private:
	POINT mPos;
	HWND mMainWnd;
};

#endif