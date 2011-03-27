#include "CameraManager.h"

CameraManager::CameraManager()
{
	gGameCamera = new Camera(600, 450, 1200, 900);
	gGuiCamera = new Camera(1300, 450, 200, 900);
}

CameraManager::~CameraManager()
{
	delete gGameCamera;
	delete gGuiCamera;
}

void CameraManager::setCamera(Cam camera)
{
	if(camera == GAME_CAMERA)	{
		if(!gGameCamera->getActive())	{
			gGameCamera->activate(true);
			gGuiCamera->activate(false);
		}
	}
	else if(camera == GUI_CAMERA)	{
		if(!gGuiCamera->getActive())	{
			gGuiCamera->activate(true);
			gGameCamera->activate(false);
		}
	}
}
