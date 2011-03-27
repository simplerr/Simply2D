#ifndef TESTSTATE_H
#define TESTSTATE_H

#include "PlayState.h"

class TestState : public PlayState
{
public:
	void handleEvents(Game* game) {
		if(gDInput->keyPressed(DIK_ESC))
			game->changeState(EditorStage::Instance());
	}

	static TestState* Instance() {
		return &mTestState;
	}

	void draw(Game* game) {
		PlayState::draw();
		gGraphics->drawText("Press ESC to return", 1020, 20);
	}
protected:
	TestState() {};
private:
	static TestState mTestState;
};

#endif