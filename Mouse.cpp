#include "Mouse.h"
#include "Graphics.h"

Mouse::Mouse(HWND hwnd)
{
	mMainWnd = hwnd;

	// start position for the mouse
	GetCursorPos(&mPos);

	RECT windowRect;	
	GetWindowRect(mMainWnd, &windowRect);

	mPos.x -= windowRect.left + 8;
	mPos.y -= windowRect.top + 30;
}

Mouse::~Mouse()
{
	//dtor
}
void Mouse::updateMouseWIN(void)
{
	static POINT tmpPos;
	static int screenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	static int screenHeight = GetSystemMetrics(SM_CYFULLSCREEN);
	RECT windowRect;	
	
	GetCursorPos(&tmpPos);
	GetWindowRect(mMainWnd, &windowRect);
	
	// så det inte blir negativa värden
	if(tmpPos.x <= windowRect.left)
		mPos.x = windowRect.left;
	else if(tmpPos.x >= windowRect.right) 
		mPos.x = windowRect.right;
	else if(tmpPos.y <= windowRect.top)
		mPos.y = windowRect.top;
	else if(tmpPos.y >= windowRect.bottom)
		mPos.y = windowRect.bottom;
	else	{
		// jazzy - för att få bra värden!
		mPos.x = tmpPos.x - windowRect.left;
		mPos.y = tmpPos.y - windowRect.top;

		mPos.x-=8;
		mPos.y-=30;		
	}	
}

void Mouse::updateMouseDX(void)
{
	POINT tmpPos;
	RECT windowRect;

	// right now the position can be negative, if the players moves outside the screen
	mPos.x += gDInput->mouseDX();
	mPos.y += gDInput->mouseDY();

}

bool Mouse::buttonDown(MouseButton button)
{
	if(gDInput->mouseButtonDown(button))
		return true;
	// lägg till mer vid behov
}

void Mouse::drawMousePos(void)
{
	char buffer[256];
	sprintf(buffer, "Mouse x: %i\nMouse y: %i", (int)mPos.x, (int)mPos.y);
	gGraphics->drawText(buffer, GAME_WIDTH + 10	, 710); 
}

void Mouse::setMousePos(int x, int y)
{
	RECT windowRect;	
	GetWindowRect(mMainWnd, &windowRect);

	SetCursorPos(windowRect.left + 8 + x, windowRect.top + 30 + y);
	mPos.x = x;
	mPos.y = y;
}

void Mouse::setX(int x)
{
	RECT windowRect;	
	GetWindowRect(mMainWnd, &windowRect);

	SetCursorPos(windowRect.left + 8 + x, windowRect.top  + mPos.y);
}

void Mouse::setY(int y)
{

}

/* DIRECT INPUT STYLISHH
static int screenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	static int screenHeight = GetSystemMetrics(SM_CYFULLSCREEN);
	RECT windowRect;
	tx += gDInput->mouseDX();
	ty += gDInput->mouseDY();

	GetCursorPos(&mouseStartPos);

	GetWindowRect(mhMainWnd, &windowRect);

	sprintf(rect_buffer, "RECT\ntop: %i\nleft: %i\nbottom: %i\nright: %i\nWIDTH: %i\nHEIGHT: %i\n", windowRect.top,
	windowRect.left, windowRect.bottom, windowRect.right, screenWidth, screenHeight);	

	// A LA DX SPECIAL, annars ökar de fast man är vid kanten >_>
	if(tx <= 0)
		tx = 0;
	if(tx >= 2959)
		tx = 2959;
	if(ty <= 0)
		ty = 0;
	if(ty >= 1049)
		ty = 1049;
	
	// så det inte blir negativa värden
	if(tx <= windowRect.left)
		mx = windowRect.left;
	else if(tx >= windowRect.right) 
		mx = windowRect.right;
	else if(ty <= windowRect.top)
		my = windowRect.top;
	else if(ty >= windowRect.bottom)
		my = windowRect.bottom;
	else	{
		// jazzy
		mx = tx - windowRect.left;
		my = ty - windowRect.top;

		mx-=8;
		my-=30;		
	}	*/
