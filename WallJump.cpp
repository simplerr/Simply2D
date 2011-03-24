#include "WallJump.h"
#include "Player.h"

WallJump::WallJump(float x, float y, int width, int height, char *textureSource)
	: Object(x, y, width, height, textureSource, WALLJUMP)
{
	// nothing?:S
}

WallJump::~WallJump()
{

}

void WallJump::onPlayerCollision(Player *player, MTV mtv)
{
	if(mtv.pushX != 0)
		player->testWallJump(this->getID());
}