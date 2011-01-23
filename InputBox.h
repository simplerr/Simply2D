#ifndef _INPUTBOX_H_
#define _INPUTBOX_H_

#include "d3dUtil.h"
#include "Window.h"
#include "Graphics.h"
#include "DirectInput.h"

class InputBox : public Window
{
public:
	InputBox(Window *parent, WindowID id, int x, int y, int width, int height, D3DCOLOR color = D3DCOLOR_ARGB( 255, 230, 230, 230 ));
	~InputBox();

	void updateWindow(float dt);
	int wm_lbuttondown(int x, int y); 
	int wm_keydown(WPARAM wParam);
    int renderAll(void);

	//void takeInput(WPARAM wParam);
private:
	int caretPos;
	int caretTick;
	bool showCaret, handled;
};

#endif

