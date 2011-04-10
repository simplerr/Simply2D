#ifndef MENU_H
#define MENU_H

#include <list>
#include <string>
#include "d3dUtil.h"
#include "DirectInput.h"
#include "Mouse.h"
#include <boost\function.hpp>
#include <boost\bind.hpp>

enum buttonState
{
	STANDARD,
	PRESSED,
	SELECTED
};

enum navigationType
{
	MOUSE,
	ARROWKEYS
};

struct MenuItem
{
	RECT itemRect;
	std::string itemName;	
	IDirect3DTexture9* standardTexture;
	IDirect3DTexture9* onSelectTexture;
	buttonState state;
	int ID;
	// soundfile etc..
};

class Menu
{
public:
	Menu(std::string menuName, navigationType a_navigation, bool useFonts = false, int itemAmount = 2, int a_spacing = 30);
	~Menu();

	void draw(void);
	void update(POINT mousePos);

	void addMenuItem(MenuItem newItem);
	void addMenuItem(std::string itemName, char *textureSource);
	void addMenuItem(std::string itemName, char *standardTextureSource, char *onSelectTextureSource);
	void buildMenu(int itemWidth = 100, int itemHeight = 50);

	void displayCoordinates(void);

	void setMenuBackground(char *textureSource, int x, int y, int width, int height);

	boost::function<bool(std::string name)> callback;

	template <class T>
	void connect(bool(T::*_callback)(std::string name), T* _object)	{
		callback = boost::bind(_callback, _object, _1);
	}

private:
	std::list<MenuItem> mMenuItemList;
	std::string mName;
	
	HWND mhMainWnd;

	RECT backgroundRect;
	IDirect3DTexture9* backgroundTexture;

	int spacing;
	int bkgdX, bkgdY;

	navigationType navigation;
	int numbersOfItems;
	int idCounter;

	bool mUseFonts;
};

#endif