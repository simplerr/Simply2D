#ifndef EDITORSTATE_H
#define EDITORSTATE_H

#include "Game.h"
#include "Editor.h"
#include "GameState.h"

class EditorState : public GameState
{
public:
	void init();
	void cleanup();

	void pause();
	void resume();

	void handleEvents(Game* game);
	void update(Game* game, double dt);
	void drawMain(Game* game);
	void drawGui(Game* game);
	void drawBkgd(void);

	static EditorState* Instance() {
		return &mEditorState;
	}
protected:
	EditorState() {};
private:
	static EditorState mEditorState;

	Editor *mEditor;
	IDirect3DTexture9* mBkgdTexture;

};

#endif