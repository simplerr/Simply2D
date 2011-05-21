#ifndef CUSTOMEDITORSTATE_H
#define CUSTOMEDITORSTATE_H

#include "GameState.h"
#include "CustomLevelState.h"
#include "EditorState.h"
#include "WindowHandler.h"

class CustomEditorState : public CustomLevelState
{
public:
	void init(Game* game);
	void cleanup(void);
	void update(double dt);
	void drawMain(void);
	bool menuHandler(std::string name);

	bool messageHandler(WindowID id, WindowMessage msg);
	void handleEvents(UINT msg, WPARAM wParam, LPARAM lParam);

	static CustomEditorState* Instance() {
		return &mCustomEditorState;
	}
protected:
	CustomEditorState() {};
private:
	static CustomEditorState mCustomEditorState;

	bool mDeletePressed;

	WindowHandler *mWindowHandler;
	Button* mCreateButton;
	InputBox* mLevelName;
	TextBox* mLabel;
};

#endif