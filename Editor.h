#ifndef _EDITOR_H_
#define _EDITOR_H_

#include "d3dUtil.h"
#include "constants.h"
#include "Window.h"
#include "InputBox.h"
#include "TextBox.h"
#include "GameWorld.h"
#include "ListBox.h"
#include "Button.h"

enum DragRect
{
	DRAGLEFT,
	DRAGRIGHT,
	DRAGUP,
	DRAGDOWN
};

class Editor	
{
public:
	Editor();
	~Editor();

	void updateAll(float dt);
	void renderAll(void);
	void keyPressed(WPARAM wParam);

	void addMouse(Mouse *a_mouse){mMouse = a_mouse;};

	void movePlatform(void);
	void resizePlatform(DragRect drag);

	void buildBaseLevel(void);

	void updateInputBoxes(void);
	void resetInputBoxes(void);
	void updateDragRects(void);

	bool objectSnapping(Object *object, float dx, float dy);
	bool stillSnapped(void);
private:
	Window *sideBar;
	Window *gameArea;
	// alla andra windows?:O

	Mouse *mMouse;
	POINT mousePos;

	GameWorld *mLevel;
	Object *activeObject;
	Object *snappedObject;

	string itemToCreate;
	string create;
	string delet;

	RECT dragLeft, dragTop, dragBottom, dragRight;

	int snapCount;
	direction snapDir;

	const int SNAP_SENSE;
	const int SNAP_DIST;
};

#endif