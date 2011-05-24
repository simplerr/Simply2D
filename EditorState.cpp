#include "EditorState.h"

#include "CameraManager.h"
#include "MainMenuState.h"
#include "TestState.h"

extern CameraManager* gCameraManager;

EditorState EditorState::mEditorState;

void EditorState::init(Game* game)
{
	// important!
	setGame(game);

	mEditor = new Editor();
	mEditor->buildGUI();

	// load the background
	mBkgdTexture = gGraphics->loadTexture("misc\\textures\\cloud_bkgd.bmp"); 
		
	if(!mBkgdTexture)
		MessageBox(0, "Couldn't background texture", 0, 0);
}

void EditorState::cleanup()
{
	ReleaseCOM(mBkgdTexture);
	delete mEditor;
}

void EditorState::pause()
{
	// don't know how to to yet
}

void EditorState::resume()
{
	// don't know how to to yet
}

void EditorState::handleEvents(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_KEYDOWN:			
		if(wParam == VK_ESCAPE)	{
			changeState(MainMenuState::Instance());
		}
		else
			mEditor->keyPressed(wParam);		// fixit
		break;
	}
}

void EditorState::update(double dt)
{
	// test button pressed
	if(mEditor->updateAll(dt) < 0)	{
		string tmp = mEditor->getTestLevel();
		changeState(TestState::Instance());
		TestState::Instance()->setLevel(tmp);
		TestState::Instance()->setLevelType(TEST);
	}
	gCameraManager->gameCamera()->move();		// move the camera accordingly
}

void EditorState::drawMain(void)
{
	drawBkgd();
	mEditor->renderLevel();
}

void EditorState::drawGui(void)
{
	//gGraphics->BlitRect(1300, 450, 200, 900, D3DCOLOR_ARGB( 155, 155, 200, 000));
	mEditor->renderGui();
}

void EditorState::drawBkgd(void)
{	
	RECT r1;
	r1.top = 0;
	r1.left = 0;
	r1.right = 4800;
	r1.bottom = 900;

	// scale 4x in x
	D3DXMATRIX texScaling;
	D3DXMatrixScaling(&texScaling, 4.0f, 1.0f, 0.0f);
	HR(gd3dDevice->SetTransform(D3DTS_TEXTURE0, &texScaling));

	gGraphics->BlitTexture(mBkgdTexture, r1, 0xFFFFFFFF, 0.0f);

	// restore to std scaling
	D3DXMatrixScaling(&texScaling, 1.0f, 1.0f, 0.0f);
	HR(gd3dDevice->SetTransform(D3DTS_TEXTURE0, &texScaling));
}

void EditorState::setLevel(string level)
{
	mEditor->loadLevel((char*)level.c_str()); 
}