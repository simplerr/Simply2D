#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "Window.h"
#include <string>
#include <boost\function.hpp>
#include <boost\bind.hpp>

class WindowHandler;

class CheckBox : public Window
{
public:
	CheckBox(WindowHandler* handler, WindowID id, string display, int x, int y, int width = 15, int height = 15, D3DCOLOR checkColor = D3DCOLOR_ARGB( 255, 255, 0, 0),
		 D3DCOLOR color = D3DCOLOR_ARGB( 255, 230, 230, 230));
	~CheckBox();

	void draw(void);
	bool pressed(int mx, int my);
  
	boost::function<bool(WindowID id, WindowMessage msg)> callback;

	template <class T>
	void connect(bool(T::*_callback)(WindowID id, WindowMessage msg), T* _object)	{
		callback = boost::bind(_callback, _object, _1, _2);
	}

	bool getChecked(void);
	void setChecked(bool checked);

private:
	bool		mChecked;
	string		mDisplayText;
	D3DCOLOR	mCheckedColor;
};

#endif