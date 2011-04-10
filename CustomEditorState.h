#ifndef CUSTOMEDITORSTATE_H
#define CUSTOMEDITORSTATE_H

#include "GameState.h"
#include "CustomLevelState.h"
#include "EditorState.h"

class CustomEditorState : public CustomLevelState
{
public:
	void init(Game* game)	{
		CustomLevelState::init(game);
		addItem("New", (char*)NORMAL_BUTTON_SOURCE.c_str(), (char*)HOOVER_BUTTON_SOURCE.c_str());
		mCustomLevelMenu->connect(&CustomEditorState::menuHandler, this);
	}

	void update(double dt)	{
		// call menuHandler() when a item is pressed
		// state changing takes place there
		updateMenu();
	}

	bool menuHandler(std::string name)	{
		/*add levels\\ and .txt */
		string tmp = "levels\\";
		tmp.append(name);
		tmp.append(".txt");

		changeState(EditorState::Instance());
		EditorState::Instance()->setLevel(tmp);

		return false;
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