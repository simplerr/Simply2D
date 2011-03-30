#include "Object.h"
#include "Shape.h"
#include <fstream>
using namespace std;

Object::Object(float x, float y, int width, int height, char *textureSource, ObjectType type, bool updates)
{
	mShape.origin.x = x;
	mShape.origin.y = y;

	// these are defined in local space
	// origin is in the top left of the shape	
	mShape.addPoint(Shape::Point(0, 0));	// top - left
	mShape.addPoint(Shape::Point(0, height));	// bottom - left
	mShape.addPoint(Shape::Point(width, height));   // bottom - right
	mShape.addPoint(Shape::Point(width, 0));	// top - right

	mType = type;
	mUpdates = updates;

	mTexture = gGraphics->loadObjectTexture(textureSource);
	strcpy(mTextureSource, textureSource);
	//mTextureSource = textureSource;
}
Object::~Object()
{
	// dtor
	// ska inte radera texturen, det sköter Level
}
void Object::draw(void)
{
	if(resizeable)
	{
		// scale it in the x direction
		D3DXMATRIX texScaling;
		D3DXMatrixScaling(&texScaling, getWidth()/256, 1.0f, 0.0f);
		HR(gd3dDevice->SetTransform(D3DTS_TEXTURE0, &texScaling));

		gGraphics->drawShape(mShape, mTexture);

		// restore to std scaling
		D3DXMatrixScaling(&texScaling, 1.0f, 1.0f, 0.0f);
		HR(gd3dDevice->SetTransform(D3DTS_TEXTURE0, &texScaling));
	}
	else
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
	mShape.origin.x += dx;
	mShape.origin.y += dy;
}

void Object::scale(direction side, int dwidth, int dheight)
{
	mShape.scale(side, dwidth, dheight);	
}

void Object::saveToFile(ofstream *fout)
{
	*fout << getType() << " " << getX() << " " << getY() << " " << getWidth() << " " << getHeight() << " " << getTextureSource() << endl;
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

std::vector<Property> Object::getProperties(void)
{
	std::vector<Property> properties;

	Property tmp;
	char buffer[16];

	tmp.name = "x";
	sprintf(buffer, "%i", (int)mShape.origin.x);
	tmp.value = buffer;

	properties.push_back(tmp);

	tmp.name = "y";
	sprintf(buffer, "%i", (int)mShape.origin.y);
	tmp.value = buffer;

	properties.push_back(tmp);

	tmp.name = "width";
	sprintf(buffer, "%i", (int)mShape.aabb.right);
	tmp.value = buffer;

	properties.push_back(tmp);

	tmp.name = "height";
	sprintf(buffer, "%i", (int)mShape.aabb.bottom);
	tmp.value = buffer;
	
	properties.push_back(tmp);

	return properties;
}

void Object::loadProperties(std::vector<Property> propertyList)
{
	// change the string to int and load it into the object
	int tmp;

	tmp = atoi(propertyList[0].value.c_str());	// x
	mShape.origin.x = tmp;

	tmp = atoi(propertyList[1].value.c_str());	// y
	mShape.origin.y = tmp;

	tmp = atoi(propertyList[2].value.c_str());	// width
	if(tmp != mShape.aabb.right)	{
			mShape.setWidth(tmp);
	}

	tmp = atoi(propertyList[3].value.c_str());	// height
	if(tmp != mShape.aabb.bottom)	{
			mShape.setHeight(tmp);
	}
}

void Object::drawEditorFX(void)
{
	// orange effect
	draw();
	//gGraphics->BlitRect(getRect(), D3DCOLOR_ARGB(150, 255, 166, 0));	
}

ObjectArea Object::getAreaAt(double mx, double my)
{
	RECT rect = getRect();
	RECT *dragRects = getDragRects();

	// first check the dragRects
	for(int i = 0; i < 4; i++)	
	{
		if(mx > dragRects[i].left && mx < dragRects[i].right && my > dragRects[i].top && my < dragRects[i].bottom)	{
			if(i == L)
				return DRAG_LEFT;
			else if(i == R)
				return DRAG_RIGHT;
			else if(i == T)
				return DRAG_UP;
			else if(i == B)
				return DRAG_DOWN;
		}
	}

	if(mx > rect.left && mx < rect.right && my > rect.top && my < rect.bottom)
		return BODY;

	return OUTSIDE;
}

RECT* Object::getDragRects(void)
{
	RECT rects[4];
	RECT rect = getRect();	

	// left drag rect
	rects[L].left = rect.left;
	rects[L].right = rects[L].left + 20;
	rects[L].top = rect.top + 20;
	rects[L].bottom = rect.bottom - 20;

	// right drag rect
	rects[R].right = rect.right;
	rects[R].left = rects[R].right - 20;
	rects[R].top = rect.top + 20;
	rects[R].bottom = rect.bottom - 20;

	// top drag rect
	rects[T].top = rect.top;
	rects[T].bottom = rects[T].top + 20;
	rects[T].left = rect.left + 20;
	rects[T].right = rect.right - 20;

	// bottom drag rect
	rects[B].bottom = rect.bottom;
	rects[B].top = rects[B].bottom - 20;
	rects[B].left = rect.left + 20;
	rects[B].right = rect.right - 20;

	return rects;
}