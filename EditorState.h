#ifndef EDITORSTATE_H
#define EDITORSTATE_H

#include "Game.h"
#include "Editor.h"
#include "GameState.h"

class EditorState : public GameState
{
public:
	void init(Game* game);
	void cleanup();

	void pause();
	void resume();

	void handleEvents(UINT msg, WPARAM wParam, LPARAM lParam);
	void update(double dt);
	void drawMain(void);
	void drawGui(void);
	void drawBkgd(void);
	void setLevel(string level);

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