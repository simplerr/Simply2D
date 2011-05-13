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

void WallJump::onPlayerCollision(Player *player, MTV mtv, float dt)
{
	if(mtv.pushX != 0)
		player->testWallJump(this->getID());
}

void WallJump::draw(void)
{
	// scale it in the x direction
	D3DXMATRIX texScaling;
	D3DXMatrixScaling(&texScaling, getWidth()/256, getHeight()/128, 0.0f);
	HR(gd3dDevice->SetTransform(D3DTS_TEXTURE0, &texScaling));

	gGraphics->drawShape(*getShape(), getTexture());

	// restore to standard scaling
	D3DXMatrixScaling(&texScaling, 1.0f, 1.0f, 0.0f);
	HR(gd3dDevice->SetTransform(D3DTS_TEXTURE0, &texScaling));
}
