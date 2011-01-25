#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <string>
#include "d3dUtil.h"

static const std::string GRASS_PLATFORM_SOURCE = "misc\\textures\\grass_platform.bmp";
static const std::string BRICK_PLATFORM_SOURCE = "misc\\textures\\brick_platform.bmp";
static const std::string PLAYER_SOURCE = "misc\\textures\\mario.bmp";
static const std::string BACKGROUND_SOURCE = "misc\\textures\\cool_background.bmp";
static const std::string ACTIVE_LEVEL = "levels\\level_1.txt";
static const int WINDOW_WIDTH = 1200;
static const int WINDOW_HEIGHT = 800;
static const int GAME_WIDTH = 1000;
static const int GAME_HEIGHT = 800;

#define MOUSEPRESS 1

struct POS
{
	float x;
	float y;
};

enum WindowID
{
	EDITOR,
	INPUT_XPOS,
	INPUT_YPOS,
	INPUT_WIDTH,
	INPUT_HEIGHT,
	INPUT_STARTX,
	INPUT_STARTY,
	INPUT_ENDX,
	INPUT_ENDY,
	INPUT_SPEED,
	TEXT_XPOS,
	TEXT_YPOS,
	TEXT_WIDTH,
	TEXT_HEIGHT,
	TEXT_STARTX,
	TEXT_STARTY,
	TEXT_ENDX,
	TEXT_ENDY,
	TEXT_SPEED,
	LISTBOX_OBJECTTYPE,
	BUTTON_CREATE,
	BUTTON_DELETE,
	BUTTON_SAVE,
	DROPBOX_TEXTURE,
	ACTIVE_OBJECT,
	CHECKBOX_SHOWPATH,
	NOT_SET
};

struct CollisionStruct
{
	bool hori;
	bool vert;
};

enum CollisionDirection
{
	NONE_COLLISION,
	LEFT_COLLISION,
	RIGHT_COLLISION,
	UP_COLLISION,
	DOWN_COLLISION,
	LEFT_DOWN_COLLISION,
	RIGHT_DOWN_COLLISION,
	LEFT_UP_COLLISION,
	RIGHT_UP_COLLISION
};

enum ObjectType
{
	NO_OBJECT,
	STATIC_PLATFORMA,
	MOVING_PLATFORM,
	NORMAL_ENEMY,
	TELEPORTER,
	LADDER,
	TRAMPOLINE,	
};

struct WindowRect
{
	int x;
	int y;
	int width;
	int height;
	int left;
	int right;
	int top;
	int bottom;
};

enum direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	ALL
};

// så att alla kan nå den
struct TLVERTEX
{
    float x;
    float y;
    float z;
    float rhw;
    D3DCOLOR colour;
    float u;
    float v;
};

struct RECTVERTEX
{
    float x;
    float y;
    float z;
    float rhw;
    D3DCOLOR colour;   
};

#endif