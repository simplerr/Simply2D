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

static const int ADD = 150;

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

	void moveObject(void);
	void resizePlatform(DragRect drag);
	void moveSpawnPos(void);

	void updateInputBoxes(void);
	void resetInputBoxes(void);

	bool objectSnapping(Object *object, float dx, float dy);
	bool stillSnapped(void);

	void messageHandler(WindowID sender, string data = "nothing");
	void resetActive(void) {mActiveObject = NULL;};

	void loadLevel(char *source) {mLevel->loadLevel(source);};

	void moveEndPos(void);

	void updateMovingPath(void);	// ta arg?

	void setTest(bool arg) { tryLevel = arg;};

	void addPropertyPair(Property prop);

	void updatePropertyWidgets(void);
private:

	int propertyCount;
	std::vector<PropertyPair> propertyPairs;
	Object *mActiveObject;

	RECT gameArea;
	Mouse *mMouse;
	//POINT mousePos;
	Level *mLevel;
	
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

	char *test;

	int mOffset;
	CurrentAction currentAction;
private:
	TextBox *tSpawnX;
	TextBox *tSpawnY;
	InputBox *iSpawnX;
	InputBox *iSpawnY;

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