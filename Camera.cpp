#include "Camera.h"
#include <iostream>


Camera::Camera(int x, int y, int width, int height)
{
	// set up the game viewport
	mViewport.X = x - width/2;
	mViewport.Y = y - height/2;
	mViewport.Width = width;
	mViewport.Height = height;
	mViewport.MinZ = 0.0f;		// cheeesy!
	mViewport.MaxZ = 1.0f;

	// middle of game area
	mPos.x = x;
	mPos.y = y;
	mViewportPos.x = x;
	mViewportPos.y = y;
	mWidth = width;
	mHeight = height;

	mActive = false;
	// activate(); ??
}

Camera::~Camera()
{
	// dtor
	// release viewport
}

void Camera::activate(bool b)
{
	if(b)	{
		buildViewMatrix();
		buildProjectionMatrix();
		buildViewport();

		mActive = true;
	}
	else if(!b)
		mActive = false;
}

void Camera::buildViewMatrix(void)
{
	D3DXVECTOR3 pos(mPos.x, mPos.y, 1);
	D3DXVECTOR3 target(mPos.x, mPos.y, 0.0f);
	D3DXVECTOR3 up(0.0f, -1.0f, 0.0f);
	D3DXMatrixLookAtLH(&mView, &pos, &target, &up);
	gd3dDevice->SetTransform(D3DTS_VIEW, &mView);
}

void Camera::buildProjectionMatrix(void)
{
	// width & height
	D3DXMatrixOrthoLH(&mProj, mWidth, mHeight, 0, 1);
	gd3dDevice->SetTransform(D3DTS_PROJECTION, &mProj);
}

void Camera::buildViewport(void)
{
	gd3dDevice->SetViewport(&mViewport);
}

void Camera::move(float dx, float dy)
{
	// move the camera position
	mPos.x += dx;
	mPos.y += dy;

	// update the view transform matrix
	buildViewMatrix();
}