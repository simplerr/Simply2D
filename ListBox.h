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
	bool pressed(int mx, int my);
	void hoover(int mx, int my);

	int wm_lbuttondown(int x, int y); 
	//int wm_keydown(WPARAM wParam);

	void move(int dx, int dy);	// useful?? - nOT! ._s

	void addItem(string name, int height, D3DCOLOR color = D3DCOLOR_ARGB( 255, 170, 230, 230 ));		// ska få autokordinater!

	boost::function<bool(WindowID id)> callback;

	template <class T>
	void connect(bool(T::*_callback)(WindowID id), T* _object)	{	// hack?
		callback = boost::bind(_callback, _object, _1);
	}
	
	void setValue(string value);
	string getValue(void);

private:
	string				mValue;
	vector<ListItem>	mItemList;
	int					mItems;
	bool				mMouseOver;
	bool				mMarked;
	RECT				mHooverRect;
	RECT				mMarkedRect;
};

#endif