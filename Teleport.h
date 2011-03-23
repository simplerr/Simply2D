#ifndef TELEPORT_H
#define TELEPORT_H

#include "Object.h"

class Object;

class Teleport :	public Object
{
public:
	Teleport(float enterX, float enterY, float destX, float destY, int width, int height, char *enterTextureSource, char *destTextureSource);
	~Teleport();

	void onPlayerCollision(Player *player, MTV mtv);
	void teleport(Player *player);
	void draw(void);
	void saveToFile(std::ofstream *fout);
	ObjectArea getAreaAt(double mx, double my);
	void drawEditorFX(void); // draws the end tp
	void moveDest(float dx, float dy);
private:
	float mDestX;
	float mDestY;

	IDirect3DTexture9* mDestTexture;
	char *mDestTextureSource;
};

#endif