#include "EditorState.h"

#include "Camera.h"
#include "MainMenuState.h"

extern Camera* gGameCamera;
extern Camera* gGuiCamera;

EditorState EditorState::mEditorState;

void EditorState::init()
{
	mEditor = new Editor();
	mEditor->buildGUI();

	// load the background
	mBkgdTexture = gGraphics->loadTexture("misc\\textures\\city_bkgd_yellow.bmp"); 
		
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

void EditorState::handleEvents(Game* game)
{
	if(gDInput->keyPressed(DIK_ESCAPE))
		game->changeState(MainMenuState::Instance());
}

void EditorState::update(Game* game, double dt)
{
	mEditor->updateAll(dt);
	gGameCamera->move();		// move the camera accordingly
}

void EditorState::draw(Game* game)
{
	// draw the game area
	if(!gGameCamera->getActive())	{
		gGameCamera->activate(true);
		gGuiCamera->activate(false);
	}

	drawBkgd();
	mEditor->renderLevel();

	// draw the gui area
	if(!gGuiCamera->getActive())	{
			gGuiCamera->activate(true);
			gGameCamera->activate(false);
		}
	//gGraphics->BlitRect(1300, 450, 200, 900, D3DCOLOR_ARGB( 155, 155, 200, 000));
	mEditor->renderGui();
}

void EditorState::drawBkgd()
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