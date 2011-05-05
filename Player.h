#ifndef PLAYER_H
#define PLAYER_H

#include "d3dUtil.h"
#include "constants.h"
#include "DirectInput.h"
#include "Graphics.h"
#include "Object.h"
#include "Bullet.h"
#include <list>

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
	bool update(double dt, Level *Level);
	void draw(void);
	void drawGui(void);
	//void setFrameType(void);

	void onLostDevice(void);
	void onResetDevice(void);

	void move(double dx, double dy);

	//void setOnGround(bool b) {mOnGround = b;};

	void setFalling(bool b) {mFalling = b;}
	void onGround(bool b)	{mOnGround = b;}
	void jump(int height);

	void setXY(float x, float y);
	RECT getRect(void);
	bool getFalling(void) {return mFalling;}
	double getDY(void) {return mDY;}
	double getDX(void) {return mDX;}
	Shape* getShape(void);
	double getX(void);
	double getY(void);
	int getHealth(void)								{return mHealth;}

	bool getActivateKey(void);
	void setActivateKey(bool b);

	void setPrevWallJumpID(int id)	{prevWallJumpID = id;};
	void testWallJump(int id);
	void damage(int dmg)	{ mHealth -= dmg;};
	void lootGun(int ammo, int bulletType);
public:
	// lazyness
	double mDX;
	double mDY;
	std::list<Bullet> mBulletList;
private:
	Shape mShape;
	double mX;
	double mY;
	int mWidth;
	int mHeight;
	bool mFalling;
	bool mOnGround;
	int mHealth;
	int mDamage;
	bool mActivateKey;

	const int JUMP_HEIGHT;
	int MAX_HEIGHT;

	double mDrawX;
	double mDrawY;

	bool mJumping;
	bool mWallJumpOk;

	int mAmmo;
	bool mFireEffect;
	double mFireTime;
 
	IDirect3DTexture9* playerTexture;
	IDirect3DTexture9* mLeftGunTexture;
	IDirect3DTexture9* mRightGunTexture;
	IDirect3DTexture9* mBulletFire;
private:
	int prevWallJumpID;
	int frame;
	direction faceDir;
	bool inair;
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