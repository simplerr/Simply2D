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
	level_completed = NULL;
	gray_button_hoover = NULL;
	gray_button_normal = NULL;
	new_level = NULL;
	game_over = NULL;
	bullet = NULL;
	turret = NULL;
	turretFlipped = NULL;
	unpressed_gate_button = NULL;
	pressed_gate_button = NULL;
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
	ReleaseCOM(level_completed);
	ReleaseCOM(gray_button_normal);
	ReleaseCOM(gray_button_hoover);
	ReleaseCOM(new_level);
	ReleaseCOM(game_over);
	ReleaseCOM(bullet);
	ReleaseCOM(turret);
	ReleaseCOM(turretFlipped);
	ReleaseCOM(unpressed_gate_button);
	ReleaseCOM(pressed_gate_button);
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
	else if(fileName == LEVEL_COMPLETED_SOURCE)
	{
		if(level_completed != NULL)
			return level_completed;
		else
		{
			 if (FAILED(D3DXCreateTextureFromFileEx (gd3dDevice, fileName, 0, 0, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
				colorkey, &SrcInfo, NULL, &level_completed)))
			 {
				MessageBox(0, "Error loading texture", 0, 0);
				return NULL;
			 }
		return level_completed;
		}
	}
	else if(fileName == GRAY_BUTTON_NORMAL_SOURCE)
	{
		if(gray_button_normal != NULL)
			return gray_button_normal;
		else
		{
			 if (FAILED(D3DXCreateTextureFromFileEx (gd3dDevice, fileName, 0, 0, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
				colorkey, &SrcInfo, NULL, &gray_button_normal)))
			 {
				MessageBox(0, "Error loading texture", 0, 0);
				return NULL;
			 }
		return gray_button_normal;
		}
	}
	else if(fileName == GRAY_BUTTON_HOOVER_SOURCE)
	{
		if(gray_button_hoover != NULL)
			return gray_button_hoover;
		else
		{
			 if (FAILED(D3DXCreateTextureFromFileEx (gd3dDevice, fileName, 0, 0, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
				colorkey, &SrcInfo, NULL, &gray_button_hoover)))
			 {
				MessageBox(0, "Error loading texture", 0, 0);
				return NULL;
			 }
		return gray_button_hoover;
		}
	}
	else if(fileName == NEW_LEVEL_SOURCE)
	{
		if(new_level != NULL)
			return new_level;
		else
		{
			 if (FAILED(D3DXCreateTextureFromFileEx (gd3dDevice, fileName, 0, 0, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
				colorkey, &SrcInfo, NULL, &new_level)))
			 {
				MessageBox(0, "Error loading texture", 0, 0);
				return NULL;
			 }
		return new_level;
		}
	}
	else if(fileName == GAME_OVER_SOURCE)
	{
		if(game_over != NULL)
			return game_over;
		else
		{
			 if (FAILED(D3DXCreateTextureFromFileEx (gd3dDevice, fileName, 0, 0, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
				colorkey, &SrcInfo, NULL, &game_over)))
			 {
				MessageBox(0, "Error loading texture", 0, 0);
				return NULL;
			 }
		return game_over;
		}
	}
	else if(fileName == BULLET_SOURCE)
	{
		if(bullet != NULL)
			return bullet;
		else
		{
			 if (FAILED(D3DXCreateTextureFromFileEx (gd3dDevice, fileName, 0, 0, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
				colorkey, &SrcInfo, NULL, &bullet)))
			 {
				MessageBox(0, "Error loading texture", 0, 0);
				return NULL;
			 }
		return bullet;
		}
	}
	else if(fileName == TURRET_SOURCE)
	{
		if(turret != NULL)
			return turret;
		else
		{
			 if (FAILED(D3DXCreateTextureFromFileEx (gd3dDevice, fileName, 0, 0, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
				colorkey, &SrcInfo, NULL, &turret)))
			 {
				MessageBox(0, "Error loading texture", 0, 0);
				return NULL;
			 }
		return turret;
		}
	}
	else if(fileName == TURRET_FLIPPED_SOURCE)
	{
		if(turretFlipped != NULL)
			return turretFlipped;
		else
		{
			 if (FAILED(D3DXCreateTextureFromFileEx (gd3dDevice, fileName, 0, 0, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
				colorkey, &SrcInfo, NULL, &turretFlipped)))
			 {
				MessageBox(0, "Error loading texture", 0, 0);
				return NULL;
			 }
		return turretFlipped;
		}
	}
	else if(fileName == ACTIVATEBUTTON_UNPRESSED_SOURCE)
	{
		if(unpressed_gate_button != NULL)
			return unpressed_gate_button;
		else
		{
			 if (FAILED(D3DXCreateTextureFromFileEx (gd3dDevice, fileName, 0, 0, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
				colorkey, &SrcInfo, NULL, &unpressed_gate_button)))
			 {
				MessageBox(0, "Error loading texture", 0, 0);
				return NULL;
			 }
		return unpressed_gate_button;
		}
	}
	else if(fileName ==  ACTIVATEBUTTON_PRESSED_SOURCE)
	{
		if(pressed_gate_button != NULL)
			return pressed_gate_button;
		else
		{
			 if (FAILED(D3DXCreateTextureFromFileEx (gd3dDevice, fileName, 0, 0, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
				colorkey, &SrcInfo, NULL, &pressed_gate_button)))
			 {
				MessageBox(0, "Error loading texture", 0, 0);
				return NULL;
			 }
		return pressed_gate_button;
		}
	}
}