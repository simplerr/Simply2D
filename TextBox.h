#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "d3dUtil.h"
#include "Window.h"

/***********
**** Lägg till
**** -font style
**** -font color
************/

class TextBox : public Window
{
public:
	TextBox(WindowHandler* handler, WindowID id, string display, int x, int y, int width, int height, D3DCOLOR color = D3DCOLOR_ARGB( 255, 230, 230, 230 ));	
	~TextBox();

    void draw(void);
private:
	string mDisplayText;
	//string mValue;
	// font color
	// background color
	// border size
	// border color
};

#endif

