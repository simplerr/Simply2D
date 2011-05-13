#ifndef ACTIVATEBUTTON_H
#define ACTIVATEBUTTON_H

#include "Object.h"
#include "Gate.h"

class ActivateButton : public Object
{
public:
	ActivateButton(float x, float y, int width, int height, char *textureSource);
	~ActivateButton();

	void update(float dt);
	void draw(void);

	void saveToFile(std::ofstream *fout);
	void move(float dx, float dy);
	void editorMove(float dx, float dy);
	void scale(direction side, int dwidth, int dheight);
	void onPlayerCollision(Player *player, MTV mtv, float dt);

	std::vector<Property> getProperties(void);
	void loadProperties(std::vector<Property> propertyList);

	void setPlayer(Player *player);
	void setLevel(Level *level);
	void setGateId(int id);

	int getGateId(void)	{return mGateId;}

	bool insideActivateArea(void);
	void connectGate(Gate *gate);
	// set level

private:
	Player *mPlayer;
	Level *mLevel;
	RECT mActivationRect;
	Gate *mGate;
	int mGateId;
	IDirect3DTexture9* mPressedTexture;
};

#endif