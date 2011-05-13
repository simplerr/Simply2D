#include "Teleport.h"
#include "Player.h"
#include "Sound.h"

extern Sound* gSound;

Teleport::Teleport(float enterX, float enterY, float destX, float destY, int width, int height, char *enterTextureSource, char *destTextureSource)
		  :Object(enterX, enterY, width, height, enterTextureSource, TELEPORT)
{
	mDestX = destX;
	mDestY = destY;

	// load the destination texture
	mDestTexture = gGraphics->loadObjectTexture(destTextureSource);
	strcpy(mDestTextureSource, destTextureSource);

	setResizeable(false);
}

Teleport::~Teleport()
{
	// dtor
}

void Teleport::onPlayerCollision(Player *player, MTV mtv, float dt)
{
	teleport(player);
}

void Teleport::teleport(Player *player)
{
	player->setXY(mDestX, mDestY);
	gSound->playEffect("misc\\sound\\teleport.wav");
}

void Teleport::draw(void)
{
	// draw the enter sprite
	Object::draw();
}

void Teleport::drawEditorFX(void)
{
	Object::draw();

	// draw the destination sprite
	RECT r;
	r.left = mDestX - getWidth()/2;
	r.right = mDestX + getWidth()/2;
	r.top = mDestY - getHeight()/2;
	r.bottom = mDestY + getHeight()/2;

	gGraphics->BlitTexture(mDestTexture, r, 0xFFFFFFFF, 0);
}

void Teleport::saveToFile(std::ofstream *fout)
{
	*fout << getType() << " " << getID() << " " << (int)getX() << " " << (int)getY() << " " << (int)mDestX << " " << (int)mDestY << " ";
	*fout << getWidth() << " " << getHeight() << " ";
	*fout << getTextureSource() << " " << mDestTextureSource << endl;
}

ObjectArea Teleport::getAreaAt(double mx, double my)
{
	ObjectArea area = Object::getAreaAt(mx, my);
	RECT destRect;
	destRect.left = mDestX - getWidth()/2;
	destRect.right = mDestX + getWidth()/2;
	destRect.top = mDestY - getHeight()/2;
	destRect.bottom = mDestY + getHeight()/2;

	if(mx > destRect.left && mx < destRect.right && my > destRect.top && my < destRect.bottom)
		area = TP_DESTAREA;

	return area;
}

void Teleport::moveDest(float dx, float dy)
{
	mDestX += dx;
	mDestY += dy;
}

std::vector<Property> Teleport::getProperties(void)
{
	std::vector<Property> properties = Object::getProperties();

	Property tmp;
	char buffer[16];

	tmp.name = "dest x";
	sprintf(buffer, "%i", (int)mDestX);
	tmp.value = buffer;

	properties.push_back(tmp);

	tmp.name = "dest y";
	sprintf(buffer, "%i", (int)mDestY);
	tmp.value = buffer;

	properties.push_back(tmp);

	return properties;
}
void Teleport::loadProperties(std::vector<Property> propertyList)
{
	Object::loadProperties(propertyList);

	int tmp;

	tmp = atoi(propertyList[4].value.c_str());
	mDestX = tmp;

	tmp = atoi(propertyList[5].value.c_str());
	mDestY = tmp;
}

