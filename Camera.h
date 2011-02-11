#ifndef CAMERA_H
#define CAMERA_H

#include "d3dApp.h"
#include "d3dUtil.h"

class Camera
{
public:
	Camera(int x, int y, int width, int height);
	~Camera();

	D3DVIEWPORT9 getViewport(void)	{return mViewport;};
	D3DXMATRIX getView(void)		{return mView;};
	D3DXMATRIX getProj(void)		{return mProj;};
	int getX(void)					{return mPos.x;};
	int getY(void)					{return mPos.y;};
	int getWidth(void)				{return mWidth;};
	int getHeight(void)				{return mHeight;};

	void activate(bool b);
	bool getActive(void)	{ return mActive;};

	void buildViewMatrix(void);
	void buildProjectionMatrix(void);
	void buildViewport(void);

	void move(float dx, float dy);
private:
	D3DVIEWPORT9 mViewport;
	D3DXMATRIX mView;
	D3DXMATRIX mProj;
	
	D3DXVECTOR2 mPos;
	D3DXVECTOR2 mViewportPos;
	int mWidth;
	int mHeight;

	bool mActive;

	/*D3DVIEWPORT9 mGameViewPort;
	D3DVIEWPORT9 mGuiViewPort;

	D3DXMATRIX mGameView;
	D3DXMATRIX mGuiView;
	D3DXMATRIX mGameProj;*/
};

#endif