#ifndef CUSTOMEDITORSTATE_H
#define CUSTOMEDITORSTATE_H

#include "GameState.h"
#include "CustomLevelState.h"
#include "EditorState.h"

class CustomEditorState : public CustomLevelState
{
public:
	void update(double dt)	{
		// consider to combine this with the menuHandler()
		updateMenu();

		// check if the playe pressed a menu element
		string result = menuHandler();

		// menu item was pressed -> set level
		if(result != "none")	{
			result.append(".txt");

			changeState(EditorState::Instance());
			EditorState::Instance()->setLevel(result);
		}
	}

	static CustomEditorState* Instance() {
		return &mCustomEditorState;
	}
protected:
	CustomEditorState() {};
private:
	static CustomEditorState mCustomEditorState;
};

#endif