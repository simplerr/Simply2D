#ifndef DROPBOX_H
#define DROPBOX_H

#include "Window.h"
#include "constants.h"
#include <string>
#include <boost\function.hpp>
#include <boost\bind.hpp>

class DropBox : public Window
{
public:
	DropBox(WindowHandler* handler, WindowID id, int x, int y, int width, int height, int itemHeight, D3DCOLOR color = D3DCOLOR_ARGB( 255, 230, 230, 230));
	~DropBox();

	void update(float dt);
	void draw(void);
	bool pressed(int mx, int my);
	void hoover(int mx, int my);
	void setActive(bool b);

	void addItem(string name, D3DCOLOR color = D3DCOLOR_ARGB( 255, 170, 230, 230 ));
	void move(int dx, int dy);
	void setPos(int x, int y);
	void fixSize(void);

	boost::function<bool(WindowID id, std::string value)> callback;

	template <class T>
	void connect(bool(T::*_callback)(WindowID id, std::string value), T* _object)	{
		callback = boost::bind(_callback, _object, _1, _2);
	}

private:
	vector<ListItem> mItemList;
	RECT mActivationRect;
	RECT mRightSign;
	int mItems;
	bool mExpanded;
	int mItemHeight;
	int mSignSide;
};

#endif