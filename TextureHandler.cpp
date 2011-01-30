#include "TextureHandler.h"

TextureHandler::TextureHandler()
{
	grassPlatform = NULL;
	brickPlatform = NULL;
	playerTexture = NULL;
	backgroundTexture = NULL;
	enemyTexture = NULL;
}
TextureHandler::~TextureHandler()
{
	ReleaseCOM(grassPlatform);
	ReleaseCOM(brickPlatform);
	ReleaseCOM(playerTexture);
	ReleaseCOM(backgroundTexture);
	ReleaseCOM(enemyTexture);
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
}