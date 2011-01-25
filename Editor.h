#ifndef _EDITOR_H_
#define _EDITOR_H_

#include "d3dUtil.h"
#include "constants.h"
#include "Window.h"
#include "InputBox.h"
#include "TextBox.h"
#include "GameWorld.h"
#include "ListBox.h"
#include "DropBox.h"
#include "Button.h"
#include "ActiveObject.h"
#include "CheckBox.h"

static const int ADD = 150;

enum DragRect
{
	DRAGLEFT,
	DRAGRIGHT,
	DRAGUP,
	DRAGDOWN
};

class Editor : public Window	
{
public:
	Editor();
	~Editor();

	void updateAll(float dt);
	int renderAll(void);
	//void keyPressed(WPARAM wParam);

	void addMouse(Mouse *a_mouse){mMouse = a_mouse;};
	void buildGUI(void);

	void movePlatform(void);
	void resizePlatform(DragRect drag);

	void updateInputBoxes(void);
	void resetInputBoxes(void);
	void updateDragRects(void);

	bool objectSnapping(Object *object, float dx, float dy);
	bool stillSnapped(void);

	void messageHandler(WindowID sender, string data = "nothing");

	void loadLevel(char *source) {mLevel->loadLevel(source);};

	void moveEndPos(void);

	void updateMovingPath(void);	// ta arg?
private:
	RECT gameArea;

	Mouse *mMouse;
	POINT mousePos;

	GameWorld *mLevel;
	ActiveObject *activeObject;
	Object *snappedObject;

	string changeTexture;

	RECT dragLeft, dragTop, dragBottom, dragRight;

	int snapCount;
	direction snapDir;

	ObjectType mPrevActiveObjectType;

	int createObjectTextPos;

	bool movingEndPos;

	const int SNAP_SENSE;
	const int SNAP_DIST;
	// window controls
private:
	TextBox *tPositionX; 
	TextBox *tPositionY;
	TextBox *tWidth;
	TextBox *tHeight;

	TextBox *tStartX;
	TextBox *tStartY;
	TextBox *tEndX;
	TextBox *tEndY;
	TextBox *tSpeed;

	InputBox *iPositionX;
	InputBox *iPositionY;
	InputBox *iWidth;
	InputBox *iHeight;

	InputBox *iStartX;
	InputBox *iStartY;
	InputBox *iEndX;
	InputBox *iEndY;
	InputBox *iSpeed;

	ListBox *listBox;

	Button *createButton;
	Button *deleteButton;
	Button *saveButton;

	DropBox *textureDropBox;
	
	CheckBox *pathCheckBox;
};

#endif