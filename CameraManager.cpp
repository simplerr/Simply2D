#include "CameraManager.h"

CameraManager *gCameraManager	= 0;

CameraManager::CameraManager()
{
	mGameCamera = new Camera(600, 450, 1200, 900);
	mGuiCamera = new Camera(1300, 450, 200, 900);
	mScreenCamera = new Camera(700, 450, 1400, 900);
}

CameraManager::~CameraManager()
{
	delete mGameCamera;
	delete mGuiCamera;
	delete mScreenCamera;
}

void CameraManager::setCamera(Cam camera)
{
	if(camera == GAME_CAMERA)	{
		if(!mGameCamera->getActive())	{
			mGameCamera->activate(true);
			mGuiCamera->activate(false);
		}
	}
	else if(camera == GUI_CAMERA)	{
		if(!mGuiCamera->getActive())	{
			mGuiCamera->activate(true);
			mGameCamera->activate(false);
		}
	}
	else if(camera == SCREEN_CAMERA)	{
		if(!mScreenCamera->getActive())	{
			mScreenCamera->activate(true);
			mScreenCamera->activate(false);
		}
	}
}

Camera* CameraManager::gameCamera(void)
{
	return mGameCamera;
}

Camera* CameraManager::guiCamera(void)
{
	return mGuiCamera;
}

Camera* CameraManager::screenCamera(void)
{
	return mScreenCamera;
}
