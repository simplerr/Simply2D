#include "player.h"
#include "Object.h"
#include "Game.h"
#include "CameraManager.h"
#include <time.h>
#include "Sound.h"

extern CameraManager* gCameraManager;
extern Sound* gSound;

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
	mFalling = true;

	prevWallJumpID = 1337; 
	mWallJumpOk = false;
	mActivateKey = false;
	mHittedCeiling = false;

	mAmmo = 0;
	mFireEffect = false;
	mFireTime = 0;

	mSpeedY = 0;
	mMaxJumpSpeed = .8;

	mStepTime = .4;	// .5 is the length of the step sound

	mRightGunTexture = gGraphics->loadObjectTexture((char*)RIGHT_GUN_SOURCE.c_str());
	mLeftGunTexture = gGraphics->loadObjectTexture((char*)LEFT_GUN_SOURCE.c_str());
	mBulletFire = gGraphics->loadTexture("misc\\textures\\bullet_fire.bmp");

	srand (time(NULL));
}
Player::~Player()
{
	// dtor
	ReleaseCOM(mBulletFire);
}

void Player::onLostDevice()
{
	// nothing to do?
}

void Player::onResetDevice(void)
{
	// nothing to do?
}

bool Player::update(double dt, Level *Level)
{
	static double dtsum = 0;
	static bool moving = false;
	static double dJump = 0;
	RECT tmpRect;
	double tmpX = mX;
	double tmpY = mY;
	
	if(mFalling)	{
		if(mSpeedY < 5)
			mSpeedY += 100*dt;
		else
			mSpeedY += 20*dt;
	}

	// update frame
	if(dtsum >= .08)	{
		frame++;
		dtsum = 0;
	}
	else
		dtsum += dt;

	if(mFireEffect)	{
		if(mFireTime > .1)	{
			mFireEffect = false;
		}
		else	{
			mFireTime += dt;
		}
	}

	if(frame > 3 || !moving)
		frame = 0;

	moving = false;

	mDX = 0;
	if(gDInput->keyPressed(DIK_W))	{
		if(mOnGround)	{
			jump(.0001, 5);
		}
		else if(mWallJumpOk)	{
			jump(.1, 20);
			mWallJumpOk = false;
		}
	}

	if(mJumping)
	{
		/* jumps untill the speed is 0 */
		if(mSpeedY < 0)	{
			mSpeedY += mAccel*dt;
		}
		
		if(mSpeedY >= 0 || mHittedCeiling)	{
			mFalling = true;
			mJumping = false;
			mSpeedY = 0;
			mHittedCeiling = false;
		}
	}

	/* strafing */
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

	/* activation key */
	if(gDInput->keyPressed(DIK_E))	{
		mActivateKey = true;
	}

	if(gDInput->keyPressed(DIK_SPACE) && mAmmo > 0)	{
		Bullet tmpBullet(mX-64, mY-5, 32, 16, faceDir, 50, 500, 200, PLAYER, (char*)BULLET_SOURCE.c_str());

		if(faceDir == RIGHT)
			tmpBullet.move(95, 0);

		mBulletList.push_back(tmpBullet);
		mAmmo--;
		mFireEffect = true;
		mFireTime = 0;

		/* sound effect */
		int random = rand() % 3;
		
		if(random == 0)
			gSound->playEffect("misc\\sound\\gun_pistol1.wav");	
		else if(random == 1)
			gSound->playEffect("misc\\sound\\gun_pistol2.wav");
		else if(random == 2)
			gSound->playEffect("misc\\sound\\gun_pistol3.wav");
	}

	/* update bullet list */
	std::list<Bullet>::iterator i = mBulletList.begin();
	while( i != mBulletList.end())
	{	
		if(!i->getErased())	{
			i->update(dt);
			++i;
		}
		
		else	{
			/* delete whats needed */
			i = mBulletList.erase(i);
		}
	}
	
	if(mShape.origin.x >= 616)
		gCameraManager->gameCamera()->addMovement(mDX, 0);	
	
	if(!mOnGround)	{
		frame = 4;
	}
	else if(moving)	{
		if(mStepTime >= .4)	{
			int random = rand() % 4;

			if(random == 0)
				gSound->playEffect("misc\\sound\\pl_step1.wav");
			else if(random == 1)
				gSound->playEffect("misc\\sound\\pl_step2.wav");
			else if(random == 2)
				gSound->playEffect("misc\\sound\\pl_step3.wav");
			else if(random == 3)
				gSound->playEffect("misc\\sound\\pl_step4.wav");

			mStepTime = 0;
		}
		else
			mStepTime += dt;
	}

	move(mDX, mSpeedY);

	return true;//Level->collision(this);
}

void Player::draw(void)
{
	/* draw animation */
	RECT playerRect;//= getRect();	
	playerRect.left = mDrawX - mWidth/2;
	playerRect.right = mDrawX + mWidth/2;
	playerRect.top = mDrawY - mHeight/2;
	playerRect.bottom = mDrawY + mHeight/2;
	
	gGraphics->BlitAnimation(playerTexture, playerRect, 0xFFFFFFFF, 0, 0, frame, 0.0f, faceDir);

	/* draw the gun */
	if(mAmmo > 0)
	{
		RECT r;

		r.top = getY() + mShape.getAABB().bottom / 2;
		r.bottom = getY() + mShape.getAABB().bottom;

		if(faceDir == RIGHT)	{	
			r.left = getX() + mShape.getAABB().right / 2;
			r.right = getX() + mShape.getAABB().right + 20;
			gGraphics->BlitTexture(mRightGunTexture, r);	
		}
		else if(faceDir == LEFT)	{
			r.left = getX() - 20;
			r.right = getX() + mShape.getAABB().right / 2;
			gGraphics->BlitTexture(mLeftGunTexture, r);	
		}
	}

	/* draw the bullet explosion */
	if(mFireEffect)	
	{
		if(faceDir == LEFT)	{
			gGraphics->BlitTexture(mBulletFire, getX()-32, getY()+10, 20, 20);
			//mJustFired = false;
		}
		else if(faceDir == RIGHT)	{
			gGraphics->BlitTexture(mBulletFire, getX()+16+mShape.getAABB().right, getY()+10, 20, 20);
		}
	}
	/* draw bullet list */
	std::list<Bullet>::iterator i = mBulletList.begin();
	while( i != mBulletList.end())
	{	
		i->draw();
		i++;
	}
}

void Player::drawGui(void)
{
	/* draw health */
	char buffer[256];
	sprintf(buffer, "Health: %i", mHealth);
	gGraphics->drawText(buffer, 1250, 100);

	/* draw ammo */
	sprintf(buffer, "Ammo: %i", mAmmo);
	gGraphics->drawText(buffer, 1250, 125);
	
	/* draw y speed */
	sprintf(buffer, "YSpeed: %f", mSpeedY);
	gGraphics->drawText(buffer, 1250, 175);
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
	if(x > 600);
		gCameraManager->gameCamera()->addMovement(x - 600, 0);

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

void Player::jump(double accel, double maxSpeed)
{
	mJumping = true;
	mAccel = accel;
	mMaxJumpSpeed = maxSpeed;
	mSpeedY = -mMaxJumpSpeed;
}

void Player::testWallJump(int id)
{
 	if(prevWallJumpID != id)	{
		mWallJumpOk = true;
		prevWallJumpID = id;
	}
}

Shape* Player::getShape(void)							
{
	return &mShape;
}

bool Player::getActivateKey(void)
{
	return mActivateKey;
}

void Player::setActivateKey(bool b)
{
	mActivateKey = b;
}

void Player::lootGun(int ammo, int bulletType)
{
	mAmmo = ammo;
}

void Player::setSpeedY(double speed)
{
	mSpeedY = speed;
}

void Player::setSpeedX(double speed)
{
	// later
}

void Player::onGround(bool b)
{
	mOnGround = b;
}

void Player::setAccel(double accel)
{
	mAccel = accel;
}