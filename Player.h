#ifndef PLAYER_H
#define PLAYER_H

#include "d3dUtil.h"
#include "constants.h"
#include "GameWorld.h"
#include "DirectInput.h"
#include "Graphics.h"

#include <crtdbg.h>
#include <string>

using namespace std;

#define MOVESPEED 280
#define FALLSPEED 300
#define JUMPSPEED 350

class GameWorld;

class Player
{
public:
	Player(string filename, int x, int y, int width, int height);
	~Player();
	void update(double dt, GameWorld *Level);
	void draw(void);
	void setFrameType(void);

	void onLostDevice(void);
	void onResetDevice(void);

	void move(double dx, double dy);

	//void setOnGround(bool b) {mOnGround = b;};

	void setFalling(bool b) {mFalling = b;};

	RECT getRect(void);
	bool getFalling(void) {return mFalling;};
	double getDY(void) {return mDY;};
	double getDX(void) {return mDX;};

private:
	double mX;
	double mY;
	int mWidth;
	int mHeight;
	bool mFalling;

	const int JUMP_HEIGHT;

	double mDX;
	double mDY;
 
	IDirect3DTexture9* playerTexture;
private:
	int frame;
	playerFrameType frameType;
	direction faceDir;
	bool inair;

	char *errorText;
};
 
#endif