#include "Object.h"

class WallJump : public Object
{
public:
	WallJump(float x, float y, int width, int height, char *textureSource);
	~WallJump();

	void draw(void);

	void onPlayerCollision(Player *player, MTV mtv);
private:
};