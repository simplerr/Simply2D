#include <list>
#include <string>
#include "d3dUtil.h"
#include "DirectInput.h"
#include "Mouse.h"

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
	IDirect3DTexture9* standardTexture;	// kanske en normal och en "down" textur sen?
	IDirect3DTexture9* onSelectTexture;
	IDirect3DTexture9* onPressTexture;
	buttonState state;
	int ID;
	// soundfile etc..
};

class Menu
{
public:
	Menu(HWND hMainWnd, std::string menuName, navigationType a_navigation, int itemAmount = 2, int a_spacing = 30);
	~Menu();

	void drawMenu(void);
	void updateMenu(POINT mousePos);

	void addMenuItem(MenuItem newItem);
	void addMenuItem(std::string itemName, char *textureSource);
	void addMenuItem(std::string itemName, char *standardTextureSource, char *onSelectTextureSource, char *onPressTextureSource);
	void buildMenu(int itemWidth = 100, int itemHeight = 50);

	void displayCoordinates(void);

	void setMenuBackground(char *textureSource, int x, int y, int width, int height);

	bool buttonPressed(POINT mousePos, std::string pressedButton);
	void updateSelectedItem(POINT mousePos);

	//MenuItem createMenuItem(...);
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
};