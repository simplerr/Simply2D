#ifndef LISTBOX_H
#define LISTBOX_H

#include "Window.h"
#include "constants.h"
#include "d3dUtil.h"
#include <string>
#include <boost\function.hpp>
#include <boost\bind.hpp>

class ListBox : public Window
{
public:
	ListBox(WindowHandler* handler, WindowID id, int x, int y, int width, int height, D3DCOLOR color = D3DCOLOR_ARGB( 255, 230, 230, 230 ));
	~ListBox();

	void draw(void);
	void pressed(void);
	void hoover(void);

	int wm_lbuttondown(int x, int y); 
	//int wm_keydown(WPARAM wParam);

	void move(int dx, int dy);	// useful?? - nOT! ._s

	void addItem(string name, int height, D3DCOLOR color = D3DCOLOR_ARGB( 255, 170, 230, 230 ));		// ska få autokordinater!

	boost::function<void(WindowID id, std::string value)> callback;

	template <class T>
	void connect(void(T::*_callback)(WindowID id, std::string value), T* _object)	{
		callback = boost::bind(_callback, _object, _1, _2);
	}

private:
	vector<ListItem> mItemList;
	int mItems;
	bool mMouseOver;
	bool mMarked;
	RECT mHooverRect;
	RECT mMarkedRect;
};

#endif