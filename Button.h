#ifndef _BUTTON_H
#define _BUTTON_H

#include "Window.h"

class Button : public Window
{
public:
	Button(WindowID id, string value, int x, int y, int width, int height, D3DCOLOR color = D3DCOLOR_ARGB( 255, 230, 230, 230));
	~Button();

	int wm_lbuttondown(int x, int y); 
    int renderAll(void);
private:
	// normal = mColor
	//D3DCOLOR pressedColor;

	bool pressed;
};

#endif