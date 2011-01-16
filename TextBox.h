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
	TextBox(char* name, string value, int x, int y, int width, int height, D3DCOLOR color = D3DCOLOR_ARGB( 255, 230, 230, 230 ));	
	~TextBox();

	int wm_lbuttondown(int x, int y); 
    int renderAll(void);
private:
	//string mValue;
	// font color
	// background color
	// border size
	// border color
};

#endif

