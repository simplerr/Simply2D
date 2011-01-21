#ifndef LISTBOX_H
#define LISTBOX_H

#include "Window.h"
#include "constants.h"
#include "d3dUtil.h"

class ListBox : public Window
{
public:
	ListBox(char* name, int x, int y, int width, int height, D3DCOLOR color = D3DCOLOR_ARGB( 255, 230, 230, 230 ));
	~ListBox();

	void updateWindow(float dt);
	int wm_lbuttondown(int x, int y); 
	int wm_keydown(WPARAM wParam);
    int renderAll(void);

	void addItem(string name, int height, D3DCOLOR color = D3DCOLOR_ARGB( 255, 170, 230, 230 ));		// ska få autokordinater!, OBS måste ha samma på alla items!
	//void addItem(char *name, int x, int y, int width, int height);  den ovan ska användas istället!
	// setActiveItem()
	// getActiveItemName()
private:
	//RECT listArea; -- Window::mPostion
	vector<ListItem> itemList;
	//char *activeItemName; = mValue
	int items;
};

#endif