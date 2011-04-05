#include "TextureHandler.h"

TextureHandler::TextureHandler()
{
	grassPlatform = NULL;
	brickPlatform = NULL;
	playerTexture = NULL;
	backgroundTexture = NULL;
	enemyTexture = NULL;
	enemyTexture2 = NULL;
	tpEnter = NULL;
	tpDest = NULL;
	trampoline = NULL;
	walljump = NULL;
	spike = NULL;
	warp = NULL;
	normal_button = NULL;
	hoover_button = NULL;
}
TextureHandler::~TextureHandler()
{
	ReleaseCOM(grassPlatform);
	ReleaseCOM(brickPlatform);
	ReleaseCOM(playerTexture);
	ReleaseCOM(backgroundTexture);
	ReleaseCOM(enemyTexture);
	ReleaseCOM(enemyTexture2);
	ReleaseCOM(tpEnter);
	ReleaseCOM(tpDest);
	ReleaseCOM(trampoline);
	ReleaseCOM(walljump);
	ReleaseCOM(spike);
	ReleaseCOM(warp);
	ReleaseCOM(normal_button);
	ReleaseCOM(hoover_button);
}

IDirect3DTexture9* TextureHandler::loadTexture(char *fileName)
{
	IDirect3DTexture9 *texture;
	D3DXIMAGE_INFO SrcInfo;      //Optional

	//Use a magenta colourkey
	D3DCOLOR colorkey = 0xFFFF00FF;

	if(fileName == GRASS_PLATFORM_SOURCE)	
	{
		if(grassPlatform != NULL)
			return grassPlatform;
		else
		{
			 if (FAILED(D3DXCreateTextureFromFileEx (gd3dDevice, fileName, 0, 0, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
				colorkey, &SrcInfo, NULL, &grassPlatform)))
			 {
				MessageBox(0, "Error loading texture", 0, 0);
				return NULL;
			 }
		return grassPlatform;
		}
	}

	else if(fileName == BRICK_PLATFORM_SOURCE)	
	{
		if(brickPlatform != NULL)
			return brickPlatform;
		else
		{
			 if (FAILED(D3DXCreateTextureFromFileEx (gd3dDevice, fileName, 0, 0, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
				colorkey, &SrcInfo, NULL, &brickPlatform)))
			 {
				MessageBox(0, "Error loading texture", 0, 0);
				return NULL;
			 }
		return brickPlatform;
		}
	}

	else if(fileName == PLAYER_SOURCE)	
	{
		if(playerTexture != NULL)
			return playerTexture;
		else
		{
			 if (FAILED(D3DXCreateTextureFromFileEx (gd3dDevice, fileName, 0, 0, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
				colorkey, &SrcInfo, NULL, &playerTexture)))
			 {
				MessageBox(0, "Error loading texture", 0, 0);
				return NULL;
			 }
		return playerTexture;
		}
	}

	else if(fileName == BACKGROUND_SOURCE)	
	{
		if(backgroundTexture != NULL)
			return backgroundTexture;
		else
		{
			 if (FAILED(D3DXCreateTextureFromFileEx (gd3dDevice, fileName, 0, 0, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
				colorkey, &SrcInfo, NULL, &backgroundTexture)))
			 {
				MessageBox(0, "Error loading texture", 0, 0);
				return NULL;
			 }
		return backgroundTexture;
		}
	}
	else if(fileName == ENEMY_SOURCE)
	{
		if(enemyTexture != NULL)
			return enemyTexture;
		else
		{
			 if (FAILED(D3DXCreateTextureFromFileEx (gd3dDevice, fileName, 0, 0, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
				colorkey, &SrcInfo, NULL, &enemyTexture)))
			 {
				MessageBox(0, "Error loading texture", 0, 0);
				return NULL;
			 }
		return enemyTexture;
		}
	}
	else if(fileName == "misc\\textures\\bad_mario_n.bmp")
	{
		if(enemyTexture2 != NULL)
			return enemyTexture2;
		else
		{
			 if (FAILED(D3DXCreateTextureFromFileEx (gd3dDevice, fileName, 0, 0, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
				colorkey, &SrcInfo, NULL, &enemyTexture2)))
			 {
				MessageBox(0, "Error loading texture", 0, 0);
				return NULL;
			 }
		return enemyTexture2;
		}
	}
	else if(fileName == TP_ENTER)
	{
		if(tpEnter != NULL)
			return tpEnter;
		else
		{
			 if (FAILED(D3DXCreateTextureFromFileEx (gd3dDevice, fileName, 0, 0, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
				colorkey, &SrcInfo, NULL, &tpEnter)))
			 {
				MessageBox(0, "Error loading texture", 0, 0);
				return NULL;
			 }
		return tpEnter;
		}
	}
	else if(fileName == TP_DEST)
	{
		if(tpDest != NULL)
			return tpDest;
		else
		{
			 if (FAILED(D3DXCreateTextureFromFileEx (gd3dDevice, fileName, 0, 0, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
				colorkey, &SrcInfo, NULL, &tpDest)))
			 {
				MessageBox(0, "Error loading texture", 0, 0);
				return NULL;
			 }
		return tpDest;
		}
	}
	else if(fileName == TRAMPOLINE_SOURCE)
	{
		if(trampoline != NULL)
			return trampoline;
		else
		{
			 if (FAILED(D3DXCreateTextureFromFileEx (gd3dDevice, fileName, 0, 0, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
				colorkey, &SrcInfo, NULL, &trampoline)))
			 {
				MessageBox(0, "Error loading texture", 0, 0);
				return NULL;
			 }
		return trampoline;
		}
	}
	else if(fileName == WALLJUMP_SOURCE)
	{
		if(walljump != NULL)
			return walljump;
		else
		{
			 if (FAILED(D3DXCreateTextureFromFileEx (gd3dDevice, fileName, 0, 0, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
				colorkey, &SrcInfo, NULL, &walljump)))
			 {
				MessageBox(0, "Error loading texture", 0, 0);
				return NULL;
			 }
		return walljump;
		}
	}
	else if(fileName == SPIKE_SOURCE)
	{
		if(spike != NULL)
			return spike;
		else
		{
			 if (FAILED(D3DXCreateTextureFromFileEx (gd3dDevice, fileName, 0, 0, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
				colorkey, &SrcInfo, NULL, &spike)))
			 {
				MessageBox(0, "Error loading texture", 0, 0);
				return NULL;
			 }
		return spike;
		}
	}
	else if(fileName == WARP_SOURCE)
	{
		if(warp != NULL)
			return warp;
		else
		{
			 if (FAILED(D3DXCreateTextureFromFileEx (gd3dDevice, fileName, 0, 0, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
				colorkey, &SrcInfo, NULL, &warp)))
			 {
				MessageBox(0, "Error loading texture", 0, 0);
				return NULL;
			 }
		return warp;
		}
	}
	else if(fileName == NORMAL_BUTTON_SOURCE)
	{
		if(normal_button != NULL)
			return normal_button;
		else
		{
			 if (FAILED(D3DXCreateTextureFromFileEx (gd3dDevice, fileName, 0, 0, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
				colorkey, &SrcInfo, NULL, &normal_button)))
			 {
				MessageBox(0, "Error loading texture", 0, 0);
				return NULL;
			 }
		return normal_button;
		}
	}
	else if(fileName == HOOVER_BUTTON_SOURCE)
	{
		if(hoover_button != NULL)
			return hoover_button;
		else
		{
			 if (FAILED(D3DXCreateTextureFromFileEx (gd3dDevice, fileName, 0, 0, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
				colorkey, &SrcInfo, NULL, &hoover_button)))
			 {
				MessageBox(0, "Error loading texture", 0, 0);
				return NULL;
			 }
		return hoover_button;
		}
	}
}