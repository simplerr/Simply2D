#include "player.h"
#include "Object.h"
//#include <fstream>

Player::Player(string filename, int x, int y, int width, int height)
	:JUMP_HEIGHT(80)
{
	playerTexture = gGraphics->loadTexture("misc\\textures\\mario.bmp");
	if(!playerTexture)
		MessageBox(0, "Error loading player texture", 0, 0);		

	mWidth = width;
	mHeight = height;
	mX = x;
	mY = y;

	frame = 1;
	frameType = NADA;
	inair = true;
	mFalling = true;

	errorText = new char[256];
	errorText = "...";
}

Player::~Player()
{
	// dtor
}

void Player::onLostDevice()
{
	// inget att göra?
}

void Player::onResetDevice(void)
{
	// inget att göra?
}


void Player::update(double dt, GameWorld *Level)
{
	errorText = "....";

	CollisionStruct collisions;
	static double dtsum = 0;
	static bool moving = false;
	static double dJump = 0;
	static bool jumped = false;
	RECT tmpRect;
	double tmpX = mX;
	double tmpY = mY;

	// update frame
	if(dtsum >= .08)	{
		frame++;
		dtsum = 0;
	}
	else
		dtsum += dt;

	if(frame > 4 || !moving)
		frame = 1;

	moving = false;

	mDX = 0;
	mDY = 0;

	// Y kordinat updateras och beräknas!

	if(mFalling)	{
		mDY = dt*FALLSPEED;
	}
	else if(gDInput->keyPressed(DIK_SPACE))
		jumped = true;

	if(jumped)
	{
		// faller när han nått maxhöjd
		if(dJump <= -JUMP_HEIGHT)	{
			mFalling = true;
			jumped = false;
			dJump = 0;
		}
		else {
			mDY = -(double)JUMPSPEED*dt;
			dJump += mDY;
		}		
	}
	else
		dJump = 0;

	if(gDInput->keyDown(DIK_A))	{
		mDX = -dt*MOVESPEED;
		moving = true;
		faceDir = LEFT;
	}
	else if(gDInput->keyDown(DIK_D))	{
		mDX = dt*MOVESPEED;
		moving = true;
		faceDir = RIGHT;
	}

	collisions = Level->collision(this);	

	if(collisions.vert == false && collisions.hori == false)	{
		errorText = "no collision!";
		move(mDX, mDY);	
	}
	else if(collisions.vert == false || (collisions.vert && jumped)){
		move(0, mDY);
		errorText = "right or left!";
	}
	else if(collisions.hori == false)	{
		move(mDX, 0);	
	}

	// updatera mFalling
	if(mFalling == true && collisions.vert)	{
		mFalling = false;
	}
	else if(!collisions.vert)
		mFalling = true;

	if(jumped == true  && collisions.vert && dJump < -1)	{
		jumped = false;
		mFalling = true;
	}

	setFrameType();
}

void Player::draw(void)
{
	// updatera kordinaterna 
	RECT playerRect = getRect();
	
	gGraphics->BlitAnimation(playerTexture, playerRect, 0xFFFFFFFF, 0, 0, frame, 0.0f, frameType);

	gGraphics->drawText(errorText, 850, 300);
}

void Player::setFrameType(void)
{
	if(!mFalling)
	{
		if(faceDir == RIGHT)
			frameType = GROUNDRIGHT;
		else if(faceDir == LEFT)
			frameType = GROUNDLEFT;
	}
	else if(mFalling)
	{
		if(faceDir == RIGHT)
			frameType = AIRRIGHT;
		else if(faceDir == LEFT)
			frameType = AIRLEFT;
	}
}

RECT Player::getRect(void)
{	
	RECT rect;

	rect.left = mX - mWidth/2;
	rect.right = mX + mWidth/2;
	rect.top = mY - mHeight/2;
	rect.bottom = mY + mHeight/2;

	return rect;
}

void Player::move(double dx, double dy)
{
	mX += dx;
	mY += dy;
}