#ifndef TEXTURE_HANDLER
#define TEXTURE_HANDLER

#include "d3dUtil.h"
//#include "constants.h"

// bara gameObject texturer!
class TextureHandler
{
public:
	TextureHandler();
	~TextureHandler();

	IDirect3DTexture9* loadTexture(char *fileName);	
private:
	IDirect3DTexture9* grassPlatform;
	IDirect3DTexture9* brickPlatform;
	IDirect3DTexture9* playerTexture;
	IDirect3DTexture9* enemyTexture;
	IDirect3DTexture9* enemyTexture2;
	IDirect3DTexture9* backgroundTexture;
	IDirect3DTexture9* tpEnter;
	IDirect3DTexture9* tpDest;
	IDirect3DTexture9* trampoline;
	IDirect3DTexture9* walljump;
	IDirect3DTexture9* spike;
	IDirect3DTexture9* warp;
	IDirect3DTexture9* normal_button;
	IDirect3DTexture9* hoover_button;

	// angryEnemyTexture
	// normalEnemyTexture
	// ladderTexture
	// teleportTexture
	// trampolineTexture

};

#endif