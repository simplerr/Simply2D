#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <string>
#include "d3dUtil.h"

static const std::string GRASS_PLATFORM_SOURCE = "grass_platform.bmp";
static const std::string BRICK_PLATFORM_SOURCE = "brick_platform.bmp";
static const std::string PLAYER_SOURCE = "mario.bmp";
static const std::string BACKGROUND_SOURCE = "cool_background.bmp";

#define MOUSEPRESS 1

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