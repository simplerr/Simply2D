#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include "Camera.h"

enum Cam
{
	GAME_CAMERA = 200,
	GUI_CAMERA
};

class CameraManager
{
public:
	CameraManager();
	~CameraManager();

	void setCamera(Cam camera);

	Camera *gGameCamera;
	Camera *gGuiCamera;
};

#endif