#ifndef DROPBOX_H
#define DROPBOX_H

#include "Window.h"
#include "constants.h"

class DropBox : public Window
{
public:
	DropBox(WindowHandler* handler, WindowID id, int x, int y, int width, int height, int itemHeight, D3DCOLOR color = D3DCOLOR_ARGB( 255, 230, 230, 230));
	~DropBox();

	void update(float dt);
	void draw(void);
	void pressed(void);
	void hoover(void);

	void addItem(string name, D3DCOLOR color = D3DCOLOR_ARGB( 255, 170, 230, 230 ));
	void move(int dx, int dy);
	void setPos(int x, int y);
	void fixSize(void);
	void onDeactive(void);
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