#ifndef TESTSTATE_H
#define TESTSTATE_H

#include "PlayState.h"
#include "EditorState.h"

class TestState : public PlayState
{
public:
	void handleEvents(Game* game, UINT msg, WPARAM wParam, LPARAM lParam) {
		switch(msg)
		{
			case WM_KEYDOWN:
				// should acctually pause the editor instead
				if( wParam == VK_ESCAPE )	{
					string levelName = getLevel();
					game->changeState(EditorState::Instance());
					EditorState::Instance()->setLevel(levelName);
				}
			break;
		}		
	}

	static TestState* Instance() {
		return &mTestState;
	}

	void drawGui(Game* game) {
		PlayState::drawGui(game);
		gGraphics->drawText("Press ESC to return", 1220, 20);
	}
protected:
	TestState() {};
private:
	static TestState mTestState;
};

#endif