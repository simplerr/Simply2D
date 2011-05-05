#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "d3dUtil.h"
#include <crtdbg.h>
#include <string>
#include "TextureHandler.h"
#include "constants.h"

enum FrameType
{
	GROUNDLEFT,
	GROUNDRIGHT,
	AIRLEFT,
	AIRRIGHT,
	NADA
};

enum direction;

class TextureHandler;
class Shape;

class Graphics
{
public:
	Graphics(std::string s);
	~Graphics();

	IDirect3DTexture9* loadTexture(char *fileName);	
	IDirect3DTexture9* loadObjectTexture(char *fileName);

	void BlitTexture(IDirect3DTexture9 *texture, RECT rDest,		// utan animation
    D3DCOLOR vertexColour = 0xFFFFFFFF, float rotate = 0.0f);

	void BlitTexture(IDirect3DTexture9 *texture, int x, int y, int width, int height,		// utan animation
    D3DCOLOR vertexColour = 0xFFFFFFFF, float rotate = 0.0f);

	void BlitAnimation(IDirect3DTexture9 *texture, RECT rDest,		// med animation
		D3DCOLOR vertexColour, int width, int height, int frame, float rotate, direction frameDir);

	void loadFont(void);
	void drawText(char *text, int x, int y, D3DCOLOR textColor = D3DCOLOR_ARGB(255,0,0,0));

	void BlitFullRectBordered(float x, float y, int width, int height, D3DCOLOR fillColor = D3DCOLOR_ARGB( 155, 155, 000, 000 ) ,
		int borderThickness = 2, D3DCOLOR borderColor = D3DCOLOR_ARGB( 255, 000, 000, 000 ));

	void BlitRect(float x, float y, int width, int height, D3DCOLOR fillColor = D3DCOLOR_ARGB( 155, 155, 000, 000 ));
	void BlitRect(RECT rect, D3DCOLOR fillColor = D3DCOLOR_ARGB( 155, 155, 000, 000 ));	

	bool drawShape(Shape shape, D3DCOLOR fillColor = D3DCOLOR_ARGB( 255, 255, 000, 000 ));
	bool drawShape(Shape shape, IDirect3DTexture9 *texture);

private:
	IDirect3DVertexBuffer9* mVB_texture;//mVB_texture;
	IDirect3DVertexBuffer9* mVB_rect;
	ID3DXFont* mFont;

	TextureHandler* mTextureHandler;
};

// global så att alla kommer åt det

#endif