#ifndef _BUTTON_H
#define _BUTTON_H

#include <string>
#include "Window.h"
#include <boost\function.hpp>
#include <boost\bind.hpp>

class Button : public Window
{
public:
	Button(WindowHandler *handler, WindowID id, string display, int x, int y, int width, int height, D3DCOLOR color = D3DCOLOR_ARGB( 255, 230, 230, 230));
	Button(WindowHandler *handler, WindowID id, string display, int x, int y, int width, int height, bool b, 
		char* normalTexture = (char*)NORMAL_BUTTON_SOURCE.c_str(), char* hooverTexture = (char*)HOOVER_BUTTON_SOURCE.c_str(),
		bool font = false, D3DCOLOR color = D3DCOLOR_ARGB( 255, 230, 230, 230));
	~Button();

	void update(float dt);
    void draw(void);
	bool pressed(int mx, int my);
	void hoover(int mx, int my);

	boost::function<bool(WindowID id, std::string value)> callback;

	template <class T>
	void connect(bool(T::*_callback)(WindowID id, std::string value), T* _object)	{
		callback = boost::bind(_callback, _object, _1, _2);
	}

private:
	bool mFont;
	bool mMouseOver;
	string mDisplayText;

	IDirect3DTexture9* mNormalTexture;
	IDirect3DTexture9* mHooverTexture;
};

#endif