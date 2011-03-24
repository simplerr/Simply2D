#include "player.h"
#include "Object.h"
#include "Game.h"
#include "Camera.h"

extern Camera* gGameCamera;

Player::Player(string filename, int width, int height)
	:JUMP_HEIGHT(80)
{
	playerTexture = gGraphics->loadTexture("misc\\textures\\mario.bmp");
	if(!playerTexture)
		MessageBox(0, "Error loading player texture", 0, 0);		

	mWidth = width;
	mHeight = height;

	mHealth = HEALTH;
	mDamage = DAMAGE;

	faceDir = RIGHT;
	frame = 0;
	inair = true;
	mJumping = false;
	mOnGround = false;

	prevWallJumpID = 1337; 
	mWallJumpOk = false;
}

Player::~Player()
{
	// dtor
}

void Player::onLostDevice()
{
	// nothing to do?
}

void Player::onResetDevice(void)
{
	// nothing to do?
}


void Player::update(double dt, Level *Level)
{
	static double dtsum = 0;
	static bool moving = false;
	static double dJump = 0;
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

	if(frame > 3 || !moving)
		frame = 0;

	moving = false;

	mDX = 0;
	mDY = 0;

	// fall
	mDY = dt*FALLSPEED;
	
	if(gDInput->keyPressed(DIK_SPACE))	{
		if(mOnGround)
			jump(JUMP_HEIGHT);//jumped = true;
		else if(mWallJumpOk)	{
			jump(JUMP_HEIGHT);
			mWallJumpOk = false;
		}
	}

	if(mJumping)
	{
		// faller n�r han n�tt maxh�jd
		if(dJump <= -MAX_HEIGHT)	{
			mFalling = true;
			mJumping = false;
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

	
	if(mShape.origin.x >= 516)
		gGameCamera->addMovement(mDX, 0);

	move(mDX, mDY);
	Level->collision(this);

	if(!mOnGround)
		frame = 4;
}

void Player::draw(void)
{
	// draw animation
	RECT playerRect;//= getRect();	
	playerRect.left = mDrawX - mWidth/2;
	playerRect.right = mDrawX + mWidth/2;
	playerRect.top = mDrawY - mHeight/2;
	playerRect.bottom = mDrawY + mHeight/2;
	
	gGraphics->BlitAnimation(playerTexture, playerRect, 0xFFFFFFFF, 0, 0, frame, 0.0f, faceDir);

	// draw health
	char buffer[256];
	sprintf(buffer, "Health: %i", mHealth);
	gGraphics->drawText(buffer, 1050, 200);
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

	// points are defined in local space, only need to move origin
	mShape.origin.x += dx;
	mShape.origin.y += dy;

	mDrawX = mX;
	mDrawY = mY;
}

double Player::getX(void)
{
	return mShape.origin.x;
}

double Player::getY(void) 
{
	return mShape.origin.y;
}

void Player::setXY(float x, float y)
{
	// find out how much to move the camera
	gGameCamera->addMovement(x - mX, 0);

	mX = x;
	mY = y;

	mShape.origin.x = mX - mWidth/2;
	mShape.origin.y = mY - mHeight/2;
	
	mShape.addPoint(Shape::Point(0, 0));
	mShape.addPoint(Shape::Point(0, mHeight));
	mShape.addPoint(Shape::Point(mWidth, mHeight));
	mShape.addPoint(Shape::Point(mWidth, 0));
	
	mDrawX = mX;
	mDrawY = mY;
}

void Player::jump(int height)
{
	mJumping = true;
	MAX_HEIGHT = height;
}

void Player::testWallJump(int id)
{
 	if(prevWallJumpID != id)	{
		mWallJumpOk = true;
		prevWallJumpID = id;
	}
}