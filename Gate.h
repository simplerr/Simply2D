#ifndef GATE_H
#define GATE_H

#include "Object.h"
#include "Player.h"

class Gate	:	public Object
{
public:
	Gate(float x, float y, int width, int height, char *textureSource, float buttonx, float buttony, float openTime);
	~Gate();

	void update(float dt);
	void draw(void);

	void saveToFile(std::ofstream *fout);
	void move(float dx, float dy);
	void scale(direction side, int dwidth, int dheight);
	void onPlayerCollision(Player *player, MTV mtv);

	std::vector<Property> getProperties(void);
	void loadProperties(std::vector<Property> propertyList);

	void setPlayer(Player *player);
	void setLevel(Level *level);

	bool insideActivateArea(void);
private:
	Player *mPlayer;
	RECT mActivationRect;
	Object *mOpenButton;
	float mOpenTime;
	float mTimeElapsed;
	bool mOpen;

};
#endif