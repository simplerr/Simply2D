#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
#include <vector>
#include "d3dUtil.h"
//#include "Object.h"

static const std::string GRASS_PLATFORM_SOURCE = "misc\\textures\\grass_platform.bmp";
static const std::string BRICK_PLATFORM_SOURCE = "misc\\textures\\dirt_grass.bmp";
static const std::string PLAYER_SOURCE = "misc\\textures\\mario.bmp";
static const std::string BACKGROUND_SOURCE = "misc\\textures\\cool_background.bmp";
static const std::string ENEMY_SOURCE = "misc\\textures\\bad_mario.bmp";
static const std::string ACTIVE_LEVEL = "levels\\level_1.txt";
static const std::string TP_ENTER =	"misc\\textures\\tpenter.bmp";
static const std::string TP_DEST =	"misc\\textures\\tpdest.bmp";
static const std::string TRAMPOLINE_SOURCE = "misc\\textures\\trampoline.bmp";
static const std::string WALLJUMP_SOURCE = "misc\\textures\\walljump.bmp";
static const std::string SPIKE_SOURCE = "misc\\textures\\spike.bmp";
static const std::string WARP_SOURCE = "misc\\textures\\warp.bmp";
static const std::string NORMAL_BUTTON_SOURCE = "misc\\textures\\button_normal.bmp";
static const std::string HOOVER_BUTTON_SOURCE = "misc\\textures\\button_hoover.bmp";
static const std::string LEVEL_COMPLETED_SOURCE = "misc\\textures\\level_completed.bmp";
static const std::string GAME_OVER_SOURCE = "misc\\textures\\game_over.bmp";
static const std::string GRAY_BUTTON_NORMAL_SOURCE = "misc\\textures\\gray_button_normal.bmp";
static const std::string GRAY_BUTTON_HOOVER_SOURCE = "misc\\textures\\gray_button_hoover.bmp";
static const std::string NEW_LEVEL_SOURCE = "misc\\textures\\new_level.bmp";
static const std::string BULLET_SOURCE = "misc\\textures\\bullet.bmp";
static const std::string TURRET_SOURCE = "misc\\textures\\turret.bmp";
static const std::string TURRET_FLIPPED_SOURCE = "misc\\textures\\turret2.bmp";
static const std::string ACTIVATEBUTTON_UNPRESSED_SOURCE = "misc\\textures\\unpressed_gate_button.bmp";
static const std::string ACTIVATEBUTTON_PRESSED_SOURCE = "misc\\textures\\pressed_gate_button.bmp";
static const std::string LEFT_GUN_SOURCE = "misc\\textures\\left_gun.bmp";
static const std::string RIGHT_GUN_SOURCE = "misc\\textures\\right_gun.bmp";
static const std::string MAP_GUN_SOURCE = "misc\\textures\\map_gun.bmp";
static const int WINDOW_WIDTH = 1400;
static const int WINDOW_HEIGHT = 900;
static const int GAME_WIDTH = 1200;
static const int GAME_HEIGHT = 900;
static const int USER_WIDTH = 36;
static const int USER_HEIGHT = 36;

#define MOUSEPRESS 1

class Object;

enum DragRects
{
	L,	// left
	R,	// right	
	TO,	// top
	B	// bottom
};

enum ObjectArea
{
	BODY,
	END_RECT,
	DRAG_LEFT,
	DRAG_RIGHT,
	DRAG_UP,
	DRAG_DOWN,
	TP_DESTAREA,
	OUTSIDE
};

enum goal
{
	START,
	END
};

enum movingType
{
	HORIZONTAL,
	VERTICAL
};

struct POS
{
	float x;
	float y;
};

enum WindowID
{
	BUTTON_RETRY,
	LABEL,
	LEVEL_NAME, 
	BUTTON_MENU,
	BUTTON_AGAIN,
	BUTTON_NEXTLEVEL,
	LEVEL_COMPLETEDWINDOW,
	OBJECT_INFO,
	OBJECT_UPDATED,
	OBJECT_DESELECTED,
	OBJECT_SELECTED,
	LOL_TEXTSUBMIT,
	EDITOR,
	INPUT_LEVEL,
	INPUT_NEXT,
	INPUT_XPOS,
	INPUT_YPOS,
	INPUT_WIDTH,
	INPUT_HEIGHT,
	INPUT_STARTX,
	INPUT_STARTY,
	INPUT_ENDX,
	INPUT_ENDY,
	INPUT_SPAWNX,
	INPUT_SPAWNY,
	INPUT_SPEED,
	TEXT_LEVEL,
	TEXT_NEXT,
	TEXT_XPOS,
	TEXT_YPOS,
	TEXT_WIDTH,
	TEXT_HEIGHT,
	TEXT_STARTX,
	TEXT_STARTY,
	TEXT_ENDX,
	TEXT_ENDY,
	TEXT_SPEED,
	TEXT_SPAWNX,
	TEXT_SPAWNY,
	LISTBOX_OBJECTTYPE,
	BUTTON_CREATE,
	BUTTON_DELETE,
	BUTTON_SAVE,
	BUTTON_TRYLEVEL,
	BUTTON_QUITTRYING,
	DROPBOX_TEXTURE,
	CHECKBOX_SHOWPATH,
	MOVE_SPAWNPOS,
	NOT_SET
};

/*struct CollisionStruct
{
	bool hori;
	bool vert;
	Object object1;
	Object object2;
};*/

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
	TELEPORT,
	LADDER,
	TRAMPOLINE,
	WALLJUMP,
	SPIKE,
	LEVEL_WARP,
	BULLET,
	TURRET,
	GATE,
	ACTIVATE_BUTTON,
	GUNPOWERUP
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