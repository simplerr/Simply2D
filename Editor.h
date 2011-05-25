#ifndef _EDITOR_H_
#define _EDITOR_H_

#include "d3dUtil.h"
#include "constants.h"
#include "Window.h"
#include "InputBox.h"
#include "TextBox.h"
#include "Level.h"
#include "ListBox.h"
#include "DropBox.h"
#include "Button.h"
#include "ActiveObject.h"
#include "CheckBox.h"

using namespace std;

static const int ADD = 150;

class WindowHandler;

enum CreateMethod
{
	METHOD_MOUSE,
	METHOD_BUTTON
};

struct PropertyPair
{
	//Property prop;
	TextBox *name;

	InputBox *value;
};

enum CurrentAction
{
	SCALE_LEFT,
	SCALE_RIGHT,
	SCALE_UP,
	SCALE_DOWN,
	MOVING_OBJECT,
	MOVING_ENDPOS,
	MOVING_TPDEST,
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

class Editor
{
public:
	Editor();
	~Editor();

	int updateAll(float dt);
	int renderLevel(void);
	int renderGui(void);

	void buildGUI(void);

	void moveObject(void);
	void resizePlatform(DragRect drag);
	void moveSpawnPos(void);

	void updateInputBoxes(void);
	void resetInputBoxes(void);

	bool objectSnapping(Object *object, float dx, float dy);
	bool stillSnapped(void);

	bool messageHandler(WindowID id, WindowMessage msg = WindowMessage(false));
	void resetActive(void) {mActiveObject = NULL;};

	void loadLevel(char *source);
	string getTestLevel(void);

	void moveEndPos(void);

	void updateMovingPath(void);	// ta arg?

	void setTest(bool arg) { tryLevel = arg;};

	void addPropertyPair(Property prop);

	void updatePropertyWidgets(void);

	void keyPressed(WPARAM wParam);

	void createObject(CreateMethod method);

	void ctrlDown(bool down);

	
private:

	WindowHandler*			mWindowHandler;
	Level*					mLevel;
	Object*					mActiveObject;
	int						mPropertyCount;		// remove?!
	
	vector<PropertyPair>	mPropertyPairs;

	RECT		gameArea;
	
	Object*		snappedObject;
	string		changeTexture;
	RECT		dragLeft, dragTop, dragBottom, dragRight;
	int			snapCount;
	bool		mObjectSnapped;
	direction	snapDir;
	ObjectType	mPrevActiveObjectType;
	int			createObjectTextPos;
	bool		movingObject;
	bool		movingEndPos;
	bool		movingSpawnPos;
	bool		movingWarp;
	bool		showPaths;
	bool		tryLevel;
	const int	SNAP_SENSE;
	const int	SNAP_DIST;

	char*		test;

	int			mOffset;
	CurrentAction currentAction;
	string mOldLevelName;

	bool		mCtrlDown;
private:
	TextBox *tLevel;
	TextBox *tSpawnX;
	TextBox *tSpawnY;
	TextBox *tNextLevel;

	InputBox *iLevel;
	InputBox *iSpawnX;
	InputBox *iSpawnY;
	InputBox *iNextLevel;

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