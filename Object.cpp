#include "Object.h"
#include "Shape.h"
#include <fstream>
using namespace std;

Object::Object(float x, float y, int width, int height, char *textureSource, ObjectType type, bool updates)
{
	mX = x;
	mY = y;
	mWidth = width;
	mHeight = height;

	// since all objects only are rects right now
	mShape.origin.x = mX - mWidth/2;
	mShape.origin.y = mY - mHeight/2;

	// these are defined in local space
	// origin is in the top left of the shape	
	mShape.addPoint(Shape::Point(0, 0));	// top - left
	mShape.addPoint(Shape::Point(0, mHeight));	// bottom - left
	mShape.addPoint(Shape::Point(mWidth, mHeight));   // bottom - right
	mShape.addPoint(Shape::Point(mWidth, 0));	// top - right

	/*mShape.addPoint(Shape::Point(0, mHeight/3));	// bottom - left
	mShape.addPoint(Shape::Point(0, mHeight*(2/3)));	// top - left
	mShape.addPoint(Shape::Point(mWidth/3, mHeight));   // bottom - right
	mShape.addPoint(Shape::Point(mWidth*(2/3), mHeight));	// top - right
	mShape.addPoint(Shape::Point(mWidth, mHeight*(2/3)));	// top - right
	mShape.addPoint(Shape::Point(mWidth, mHeight/3));	// top - right
	mShape.addPoint(Shape::Point(mWidth*(2/3), 0));	// top - right
	mShape.addPoint(Shape::Point(mWidth/3, 0));	// top - right*/

	mType = type;
	mUpdates = updates;

	mTexture = gGraphics->loadObjectTexture(textureSource);
	mTextureSource = new char[256];
	memcpy(mTextureSource,textureSource, 255);
}
Object::~Object()
{
	// dtor
	// ska inte radera texturen, det sköter Level
}
void Object::draw(void)
{
	//gGraphics->BlitTexture(mTexture, getRect(), 0xFFFFFFFF, 0);
	gGraphics->drawShape(mShape, mTexture);
}

RECT Object::getRect(void)
{
	/*rect.left = mX - mWidth/2;
	rect.right = mX + mWidth/2;
	rect.top = mY - mHeight/2;
	rect.bottom = mY + mHeight/2;*/

	return mShape.getRect();
}

void Object::move(float dx, float dy)
{
	mX += dx;
	mY += dy;
}

void Object::scale(direction side, int dwidth, int dheight)
{
	if(side == LEFT)
	{			
		for(int i = 0; i < mShape.pointList.size(); i++)	{
			if(mShape.pointList[i].x != mShape.aabb.left)	{
				mShape.pointList[i].x -= (double)dwidth;
				mShape.aabb.right -= (double)dwidth/2;									
			}
		}
		mShape.origin.x += (double)dwidth;
	}
	else if(side == RIGHT)
	{			
		for(int i = 0; i < mShape.pointList.size(); i++)	{
			if(mShape.pointList[i].x != mShape.aabb.left)	{
				mShape.pointList[i].x += dwidth;
				mShape.aabb.right += (double)dwidth/2;
			}
		}
	}
	else if(side == UP)
	{			
		for(int i = 0; i < mShape.pointList.size(); i++)	{
			if(mShape.pointList[i].y != mShape.aabb.top)	{
				mShape.pointList[i].y += dheight;
				mShape.aabb.bottom += (double)dheight/2;
			}
				
		}
		mShape.origin.y -= (double)dheight;
	}
	else if(side == DOWN)
	{			
		for(int i = 0; i < mShape.pointList.size(); i++)	{
			if(mShape.pointList[i].y != mShape.aabb.top)	{
				mShape.pointList[i].y += dheight;
				mShape.aabb.bottom += (double)dheight/2;
			}
		}
	}
	else if(side == ALL)
	{
		for(int i = 0; i < mShape.pointList.size(); i++)	
		{
			// down && up
			if(mShape.pointList[i].y != mShape.aabb.top)	{
				mShape.pointList[i].y += 2*dheight;
				mShape.aabb.bottom += (double)dheight;
			}
			if(mShape.pointList[i].x != mShape.aabb.left)	{
				mShape.pointList[i].x += 2*dwidth;
				mShape.aabb.right += (double)dwidth;
			}
		}
		mShape.origin.y -= (double)dheight;
		mShape.origin.x -= (double)dwidth;
	}
		
	//mWidth += dwidth;
	//mHeight += dheight;
}

void Object::saveToFile(ofstream *fout)
{
	*fout << getType() << " " << mX << " " << mY << " " << mWidth << " " << mHeight << " " << getTextureSource() << endl;
}
void Object::loadFromFile(std::ofstream *fout)
{

}

void Object::setTextureSource(char *source)
{
	memcpy(mTextureSource, source, 255);
	char buffer[256];
	mTexture = gGraphics->loadTexture(mTextureSource);
}

void Object::editorMove(float dx, float dy)		
{
	mX += dx;
	mY += dy;

	mShape.origin.x += dx;
	mShape.origin.y += dy;
}

double Object::getWidth(void)											
{
	return mShape.aabb.right - mShape.aabb.left;
}

double Object::getHeight(void)										
{
	return mShape.aabb.bottom - mShape.aabb.top;
}

// returns the center position
double Object::getX(void)										
{
	return mShape.origin.x ;//+ getWidth()/2;
}
double Object::getY(void)
{
	return mShape.origin.y ;//+ getHeight()/2;
}