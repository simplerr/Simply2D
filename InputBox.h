#ifndef _INPUTBOX_H_
#define _INPUTBOX_H_

#include "d3dUtil.h"
#include "Window.h"
#include "Graphics.h"
#include "DirectInput.h"
#include <string>
#include <boost\function.hpp>
#include <boost\bind.hpp>

class InputBox : public Window
{
public:
	InputBox(WindowHandler* handler, WindowID id, int x, int y, int width, int height, int maxlen, D3DCOLOR color = D3DCOLOR_ARGB( 255, 230, 230, 230 ));
	~InputBox();

	void update(double dt);
	void draw(void);
	void pressed(void);
	//void hoover(void);

	void setActive(bool b);
	int wm_lbuttondown(int x, int y); 
	int wm_keydown(WPARAM wParam);
  
	boost::function<void(WindowID id, std::string value)> callback;

	template <class T>
	void connect(void(T::*_callback)(WindowID id, std::string value), T* _object)	{
		callback = boost::bind(_callback, _object, _1, _2);
	}

private:
	int maxLength;
	int caretPos;
	int caretTick;
	bool showCaret, handled;
};

#endif

