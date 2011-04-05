#include "Window.h"

class WindowHandler;

class CheckBox : public Window
{
public:
	CheckBox(WindowHandler* handler, WindowID id, string display, int x, int y, int width = 15, int height = 15, D3DCOLOR checkColor = D3DCOLOR_ARGB( 255, 255, 0, 0),
		 D3DCOLOR color = D3DCOLOR_ARGB( 255, 230, 230, 230));
	~CheckBox();

	void draw(void);
	void pressed(void);
	//void hoover(void);
  
private:
	bool mChecked;
	string mDisplayText;
	D3DCOLOR mCheckedColor;
};