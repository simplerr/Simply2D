#ifndef TESTSTATE_H
#define TESTSTATE_H

#include "PlayState.h"

class TestState : public PlayState
{
public:
	void handleEvents(Game* game, UINT msg, WPARAM wParam, LPARAM lParam) {
		switch(msg)
		{
			case WM_KEYDOWN:
				if( wParam == VK_ESCAPE )
					game->changeState(EditorStage::Instance());
			break;
		}		
	}

	static TestState* Instance() {
		return &mTestState;
	}

	void drawGui(Game* game) {
		gGraphics->drawText("Press ESC to return", 1020, 20);
	}
protected:
	TestState() {};
private:
	static TestState mTestState;
};

#endif