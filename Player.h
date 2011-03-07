#ifndef PLAYER_H
#define PLAYER_H

#include "d3dUtil.h"
#include "constants.h"
//#include "Level.h"
#include "DirectInput.h"
#include "Graphics.h"
#include "Object.h"

#include <crtdbg.h>
#include <string>

using namespace std;

#define MOVESPEED 280
#define FALLSPEED 300
#define JUMPSPEED 350
#define HEALTH 100
#define DAMAGE 50

class Level;

class Player
{
public:
	Player(string filename, int width, int height);
	~Player();
	void update(double dt, Level *Level);
	void draw(void);
	//void setFrameType(void);

	void onLostDevice(void);
	void onResetDevice(void);

	void move(double dx, double dy);

	//void setOnGround(bool b) {mOnGround = b;};

	void setFalling(bool b) {mFalling = b;};

	void setXY(float x, float y);
	RECT getRect(void);
	bool getFalling(void) {return mFalling;};
	double getDY(void) {return mDY;};
	double getDX(void) {return mDX;};
	Shape* getShape(void)							{return &mShape;};
	POS getPos(void);

	void damage(int dmg)	{ mHealth -= dmg;};
public:
	// lazyness
	double mDX;
	double mDY;
private:
	// shouldn't be filled in setXY()!
	Shape mShape;
	double mX;
	double mY;
	int mWidth;
	int mHeight;
	bool mFalling;

	int mHealth;
	int mDamage;

	const int JUMP_HEIGHT;

	double mDrawX;
	double mDrawY;
 
	IDirect3DTexture9* playerTexture;
private:
	int frame;
	direction faceDir;
	bool inair;

	char *errorText;
};
 
#endif

/*#ifndef PLAYER_H
#define PLAYER_H

#include "d3dUtil.h"
#include "constants.h"
#include "Level.h"
#include "DirectInput.h"
#include "Graphics.h"

#include <crtdbg.h>
#include <string>

using namespace std;

#define MAX_AIRSPEED .5
#define MAX_MOVESPEED .5
#define HOR_ACCEL 100
#define VER_ACCEL 500
#define DRAG_FORCE .8

#define MOVESPEED 280
#define FALLSPEED 30
#define JUMPSPEED 350
#define HEALTH 100
#define DAMAGE 50

class Level;

class Player
{
public:
	Player(string filename, int width, int height);
	~Player();
	void update(double dt, Level *Level);
	void draw(void);
	//void setFrameType(void);

	void onLostDevice(void);
	void onResetDevice(void);

	void move(double dx, double dy);

	//void setOnGround(bool b) {mOnGround = b;};

	void setFalling(bool b) {mFalling = b;};

	void setXY(float x, float y) { mX = x; mY = y;};
	RECT getRect(void);
	bool getFalling(void) {return mFalling;};
	double getDY(void) {return mDY;};
	double getDX(void) {return mDX;};

	void damage(int dmg)	{ mHealth-= dmg;};
private:
	double mX;
	double mY;
	int mWidth;
	int mHeight;
	bool mFalling;

	int mHealth;
	int mDamage;

	const int JUMP_HEIGHT;

	double mDX;
	double mDY;
	double mHorSpeed;
	double mVerSpeed;

	IDirect3DTexture9* playerTexture;
private:
	int frame;
	direction faceDir;
	bool inair;

	char *errorText;
};
 
#endif*/