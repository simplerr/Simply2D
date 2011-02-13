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

enum CurrentAction
{
	SCALE_LEFT,
	SCALE_RIGHT,
	SCALE_UP,
	SCALE_DOWN,
	MOVING_OBJECT,
	MOVING_ENDPOS,
	MOVING_SPAWN,
	IDLE
};

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

	int updateAll(float dt);
	int renderAll(void)	{ return 1;};
	int renderLevel(void);
	int renderGui(void);

	void addMouse(Mouse *a_mouse){mMouse = a_mouse;};
	void buildGUI(void);

	void movePlatform(void);
	void resizePlatform(DragRect drag);
	void moveSpawnPos(void);

	void updateInputBoxes(void);
	void resetInputBoxes(void);
	void updateDragRects(void);

	bool objectSnapping(Object *object, float dx, float dy);
	bool stillSnapped(void);

	void messageHandler(WindowID sender, string data = "nothing");
	void resetActive(void) { activeObject->clear();};

	void loadLevel(char *source) {mLevel->loadLevel(source);};

	void moveEndPos(void);

	void updateMovingPath(void);	// ta arg?

	void setTest(bool arg) { tryLevel = arg;};
private:
	RECT gameArea;
	Mouse *mMouse;
	//POINT mousePos;
	GameWorld *mLevel;
	ActiveObject *activeObject;
	Object *snappedObject;
	string changeTexture;
	RECT dragLeft, dragTop, dragBottom, dragRight;
	int snapCount;
	direction snapDir;
	ObjectType mPrevActiveObjectType;
	int createObjectTextPos;
	bool movingObject;
	bool movingEndPos;
	bool movingSpawnPos;
	bool showPaths;
	bool tryLevel;
	const int SNAP_SENSE;
	const int SNAP_DIST;

	int mOffset;
	CurrentAction currentAction;
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
	TextBox *tSpawnX;
	TextBox *tSpawnY;

	InputBox *iPositionX;
	InputBox *iPositionY;
	InputBox *iWidth;
	InputBox *iHeight;
	InputBox *iSpawnX;
	InputBox *iSpawnY;

	InputBox *iStartX;
	InputBox *iStartY;
	InputBox *iEndX;
	InputBox *iEndY;
	InputBox *iSpeed;

	ListBox *listBox;

	Button *createButton;
	Button *deleteButton;
	Button *saveButton;
	Button *bTryLevel;
	Button *bQuitTrying;

	DropBox *textureDropBox;
	
	CheckBox *pathCheckBox;
};

#endif