#include "Graphics.h"
#include "Vertex.h"
#include <tchar.h>
#include "CameraManager.h"
#include "Object.h"
#include "Shape.h"

// the old way i did this
//const DWORD D3DFVF_TLVERTEX = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
//const DWORD D3DFVF_RECTVERTEX = (D3DFVF_XYZ | D3DFVF_DIFFUSE);

extern CameraManager* gCameraManager;

Graphics::Graphics(std::string s)
{
	char buffer[256];
	InitAllVertexDeclarations();

	// set vertex shader
	//gd3dDevice->SetVertexShader(NULL);

	mVB_texture = NULL;
	mVB_rect = NULL;

	// up to 16 vertices
	if(FAILED(gd3dDevice->CreateVertexBuffer(sizeof(TextureVertex) *16, D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &mVB_texture, NULL)))
		MessageBox(0, "Error creating TextureVertex buffer", 0, 0);

	if(FAILED(gd3dDevice->CreateVertexBuffer(sizeof(RectVertex) *16, D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &mVB_rect, NULL)))
		MessageBox(0, "Error creating RectVertex buffer", 0, 0);

	mTextureHandler = new TextureHandler();

	loadFont();
}

Graphics::~Graphics()
{
	ReleaseCOM(mVB_texture);
	ReleaseCOM(mVB_rect);
	ReleaseCOM(mFont);

	delete mTextureHandler;
	// not much to do
	//delete mVB_texture;
}

IDirect3DTexture9* Graphics::loadObjectTexture(char *fileName)
{
	return mTextureHandler->loadTexture(fileName);
}

IDirect3DTexture9* Graphics::loadTexture(char *fileName)
{
	IDirect3DTexture9 *texture;
	D3DXIMAGE_INFO SrcInfo;      //Optional

	//Use a magenta colourkey
	D3DCOLOR colorkey = 0xFFFF00FF;

	// Load image from file
	if (FAILED(D3DXCreateTextureFromFileEx (gd3dDevice, fileName, 0, 0, 1, 0, 
        D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
        colorkey, &SrcInfo, NULL, &texture)))
	{
		MessageBox(0, "Error loading texture", 0, 0);
		return NULL;
	}
	return texture;
}


bool Graphics::drawShape(Shape shape, D3DCOLOR fillColor)
{
	// can't go outside buffer size
	if(shape.pointList.size() > 16)
		return false;

	gd3dDevice->SetStreamSource(0, mVB_rect, 0, sizeof(RectVertex));
	gd3dDevice->SetVertexDeclaration(RectVertex::Decl);
	  
	RectVertex *vertices = 0;

	HR(mVB_rect->Lock(0, 0, (void**)&vertices, 0));
	int i;
	//for(i = 0; i < shape.pointList.size(); i++)
	for(i = shape.pointList.size(); i > 0; i--)
	{
		vertices[i].color = fillColor;
		vertices[i].pos.x = shape.pointList[i].x + shape.origin.x;
		vertices[i].pos.y = shape.pointList[i].y + shape.origin.y;
		vertices[i].pos.z = 0.0f;	
	}

	// sista ska bindas till första
	vertices[i].color = fillColor;
	vertices[i].pos.x = shape.pointList[0].x + shape.origin.x;
	vertices[i].pos.y = shape.pointList[0].y + shape.origin.y;
	vertices[i].pos.z = 0.0f;

	//Unlock the vertex buffer
	HR(mVB_rect->Unlock());

	//Draw image
	HR(gd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID));
	gd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);	

	return true;
}

bool Graphics::drawShape(Shape shape, IDirect3DTexture9 *texture)
{
	// can't go outside buffer size
	if(shape.pointList.size() > 16)
		return false;

	gd3dDevice->SetVertexDeclaration(TextureVertex::Decl);
	gd3dDevice->SetStreamSource(0, mVB_texture, 0, sizeof(TextureVertex));	

	TextureVertex *vertices = 0;
	mVB_texture->Lock(0, 0, (void**)&vertices, 0);
	
	int i;
	for(i = 0; i < shape.pointList.size(); i++)
	{
		vertices[i].pos.x = shape.pointList[i].x + shape.origin.x;
		vertices[i].pos.y = shape.pointList[i].y + shape.origin.y;
		vertices[i].pos.z = 0.0f;
		vertices[i].tex0.x = (shape.pointList[i].x) / (shape.getAABB().right);
		vertices[i].tex0.y = (shape.pointList[i].y) / (shape.getAABB().bottom);
	}

	// sista ska bindas till första
	vertices[i].pos.x = shape.pointList[0].x + shape.origin.x;
	vertices[i].pos.y = shape.pointList[0].y + shape.origin.y;
	vertices[i].pos.z = 0.0f;
	vertices[i].tex0.x = (shape.pointList[0].x - shape.origin.x) / (shape.getAABB().right - shape.origin.x);
	vertices[i].tex0.y = (shape.pointList[0].y - shape.origin.y) / (shape.getAABB().bottom- shape.origin.y);

	//Unlock the vertex buffer
	mVB_texture->Unlock();

	//Set texture
	gd3dDevice->SetTexture (0, texture);

	//Draw image
	gd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
	gd3dDevice->SetTexture (0, NULL);
}


void Graphics::BlitTexture(IDirect3DTexture9 *texture, RECT rDest,
    D3DCOLOR vertexColour, float rotate)
{
  //gd3dDevice->SetFVF(D3DFVF_TLVERTEX);
  gd3dDevice->SetVertexDeclaration(TextureVertex::Decl);
  gd3dDevice->SetStreamSource(0, mVB_texture, 0, sizeof(TextureVertex));	

  TextureVertex *vertices = 0;
  mVB_texture->Lock(0, 0, (void**)&vertices, 0);

  vertices[0].pos.x = (float) rDest.left;
  vertices[0].pos.y = (float) rDest.top;
  vertices[0].pos.z = 0.0f;
  vertices[0].tex0.x = 0.0f;
  vertices[0].tex0.y = 0.0f;

  vertices[1].pos.x = (float) rDest.right;
  vertices[1].pos.y = (float) rDest.top;
  vertices[1].pos.z = 0.0f;
  vertices[1].tex0.x = 1.0f;
  vertices[1].tex0.y = 0.0f;

  vertices[2].pos.x = (float) rDest.right;
  vertices[2].pos.y = (float) rDest.bottom;
  vertices[2].pos.z = 0.0f;
  vertices[2].tex0.x = 1.0f;
  vertices[2].tex0.y = 1.0f;

  vertices[3].pos.x = (float) rDest.left;
  vertices[3].pos.y = (float) rDest.bottom;
  vertices[3].pos.z = 0.0f;
  vertices[3].tex0.x = 0.0f;
  vertices[3].tex0.y = 1.0f;

  //Unlock the vertex buffer
  mVB_texture->Unlock();

  //Set texture
  gd3dDevice->SetTexture (0, texture);

  //Draw image
  gd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
  gd3dDevice->SetTexture (0, NULL);
}

// TEXTUREN MÅSTE VARA HA ^2 I WIDTH OCH HEIGHT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void Graphics::BlitAnimation(IDirect3DTexture9 *texture, RECT rDest,
    D3DCOLOR vertexColour, int width, int height, int frame, float rotate, direction frameDir) // height & width onödigt?
{
  gd3dDevice->SetVertexDeclaration(TextureVertex::Decl);
  gd3dDevice->SetStreamSource(0, mVB_texture, 0, sizeof(TextureVertex));

  int i = frame / 4;
  int j = frame % 4;

  char buffer[256];
  
  //RECT R = {j*0.25, i*0.25, (j+1)*0.25, (i+1)*0.25};
  float left = j*0.25;
  float top = i*0.5;
  float right = (j+1)*0.25;
  float bottom = (i+1)*0.5;

  // facing dir is left
  if(frameDir == LEFT)	{
	float tmp = left;
	left = right;
	right = tmp;
  }

  TextureVertex *vertices;
  mVB_texture->Lock(0, 0, (void**)&vertices, 0);

  vertices[0].pos.x = (float) rDest.left;
  vertices[0].pos.y = (float) rDest.top;
  vertices[0].pos.z = 0.0f;
  vertices[0].tex0.x = left;
  vertices[0].tex0.y = top;

  vertices[1].pos.x = (float) rDest.right;
  vertices[1].pos.y = (float) rDest.top;
  vertices[1].pos.z = 0.0f;
  vertices[1].tex0.x =  right;
  vertices[1].tex0.y = top;

  vertices[2].pos.x = (float) rDest.right;
  vertices[2].pos.y = (float) rDest.bottom ;
  vertices[2].pos.z = 0.0f;
  vertices[2].tex0.x =  right;
  vertices[2].tex0.y = bottom;

  vertices[3].pos.x = (float) rDest.left;
  vertices[3].pos.y = (float) rDest.bottom;
  vertices[3].pos.z = 0.0f;
  vertices[3].tex0.x = left;
  vertices[3].tex0.y = bottom;

  //Unlock the vertex buffer
  mVB_texture->Unlock();

  //Set texture
  gd3dDevice->SetTexture (0, texture);

  //Draw image
  gd3dDevice->DrawPrimitive (D3DPT_TRIANGLEFAN, 0, 2);
  gd3dDevice->SetTexture (0, NULL);
}


void Graphics::loadFont(void)
{
	D3DXFONT_DESC fontDesc;
	fontDesc.Height          = 18;
    fontDesc.Width           = 8;
    fontDesc.Weight          = 0;
    fontDesc.MipLevels       = 1;
    fontDesc.Italic          = false;
    fontDesc.CharSet         = DEFAULT_CHARSET;
    fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
    fontDesc.Quality         = 2;
    fontDesc.PitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;
	 _tcscpy(fontDesc.FaceName, _T("Bitstream Vera Sans Mono"));
    //_tcscpy(fontDesc.FaceName, _T("Arial")); Bitstream Vera Sans Mono

	HR(D3DXCreateFontIndirect(gd3dDevice, &fontDesc, &mFont));
}
void Graphics::drawText(char *text, int x, int y, D3DCOLOR textColor)
{
	RECT pos = {x, y, 0, 0};
	HR(mFont->DrawText(0, text, -1, &pos, DT_NOCLIP, textColor));
}

void Graphics::BlitRect(float x, float y, int width, int height, D3DCOLOR fillColor)
{
	  //gd3dDevice->SetFVF(D3DFVF_RECTVERTEX); 
	  gd3dDevice->SetStreamSource(0, mVB_rect, 0, sizeof(RectVertex));
	  gd3dDevice->SetVertexDeclaration(RectVertex::Decl);	  

	  RECT rDest;
	  rDest.left = x-(width/2);
	  rDest.right = x+(width/2);
	  rDest.top = y-(height/2);
	  rDest.bottom = y+(height/2);

	  RectVertex *vertices = 0;

	  HR(mVB_rect->Lock(0, 0, (void**)&vertices, 0));

	  vertices[0].color = fillColor;
	  vertices[0].pos.x = (float) rDest.left;
	  vertices[0].pos.y = (float) rDest.top;
	  vertices[0].pos.z = 0.0f;

	  vertices[1].color = fillColor;
	  vertices[1].pos.x = (float) rDest.right;
	  vertices[1].pos.y = (float) rDest.top;
	  vertices[1].pos.z = 0.0f; 

	  vertices[2].color = fillColor;
	  vertices[2].pos.x = (float) rDest.right;
	  vertices[2].pos.y = (float) rDest.bottom;
	  vertices[2].pos.z = 0.0f; 

	  vertices[3].color = fillColor;
	  vertices[3].pos.x = (float) rDest.left;
	  vertices[3].pos.y = (float) rDest.bottom;
	  vertices[3].pos.z = 0.0f;

	  //Unlock the vertex buffer
	  HR(mVB_rect->Unlock());

	  //Draw image
	  HR(gd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID));
	  gd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);				// ANDRA GÅNGEN FUCKAR DET UR!!
}

void Graphics::BlitFullRectBordered(float x, float y, int width, int height, D3DCOLOR fillColor, int borderThickness, D3DCOLOR borderColor)
{ 
	  // så den inte påverkas av andra texture..
	  gd3dDevice->SetTexture (0, NULL);

	  //gd3dDevice->SetFVF(D3DFVF_RECTVERTEX);
	  gd3dDevice->SetVertexDeclaration(RectVertex::Decl);
	  gd3dDevice->SetStreamSource(0, mVB_rect, 0, sizeof(RectVertex));
	  RECT rDest;
	  rDest.left = x-(width/2);
	  rDest.right = x+(width/2);
	  rDest.top = y-(height/2);
	  rDest.bottom = y+(height/2);

	  RectVertex *vertices;
	  RectVertex *borderVertices;

	  /////////////////////////BOOOORDER
	  mVB_rect->Lock(0, 0, (void**)&borderVertices, 0);

	  borderVertices[0].color = borderColor;
	  borderVertices[0].pos.x = (float) rDest.left-borderThickness;
	  borderVertices[0].pos.y = (float) rDest.top-borderThickness;
	  borderVertices[0].pos.z = 0.0f;
	  //borderVertices[0].rhw = 1.0f;	 

	  borderVertices[1].color = borderColor;
	  borderVertices[1].pos.x = (float) rDest.right+borderThickness;
	  borderVertices[1].pos.y = (float) rDest.top-borderThickness;
	  borderVertices[1].pos.z = 0.0f;
	  //borderVertices[1].rhw = 1.0f;	 

	  borderVertices[2].color = borderColor;
	  borderVertices[2].pos.x = (float) rDest.right+borderThickness;
	  borderVertices[2].pos.y = (float) rDest.bottom+borderThickness;
	  borderVertices[2].pos.z = 0.0f;
	  //borderVertices[2].rhw = 1.0f;	 

	  borderVertices[3].color = borderColor;
	  borderVertices[3].pos.x = (float) rDest.left-borderThickness;
	  borderVertices[3].pos.y = (float) rDest.bottom+borderThickness;
	  borderVertices[3].pos.z = 0.0f;
	  //borderVertices[3].rhw = 1.0f;	  

	  //Unlock the vertex buffer
	  mVB_rect->Unlock();

	  gd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

	  mVB_rect->Lock(0, 0, (void**)&vertices, 0);

	  vertices[0].color = fillColor;
	  vertices[0].pos.x = (float) rDest.left;
	  vertices[0].pos.y = (float) rDest.top;
	  vertices[0].pos.z = 0.0f;
	  //vertices[0].rhw = 1.0f;	 

	  vertices[1].color = fillColor;
	  vertices[1].pos.x = (float) rDest.right;
	  vertices[1].pos.y = (float) rDest.top;
	  vertices[1].pos.z = 0.0f;
	  //vertices[1].rhw = 1.0f;	 

	  vertices[2].color = fillColor;
	  vertices[2].pos.x = (float) rDest.right;
	  vertices[2].pos.y = (float) rDest.bottom ;
	  vertices[2].pos.z = 0.0f;
	  //vertices[2].rhw = 1.0f;	 

	  vertices[3].color = fillColor;
	  vertices[3].pos.x = (float) rDest.left;
	  vertices[3].pos.y = (float) rDest.bottom;
	  vertices[3].pos.z = 0.0f;
	  //vertices[3].rhw = 1.0f;
	  //vertices[3].u = left;
	  //vertices[3].v = bottom;	

	  //Unlock the vertex buffer
	  mVB_rect->Unlock();		  

	  //Set texture	  

	  //Draw image
	  gd3dDevice->DrawPrimitive (D3DPT_TRIANGLEFAN, 0, 2);
}

void Graphics::BlitRect(RECT rect, D3DCOLOR fillColor)
{
	// så den inte påverkas av andra texture..
	  gd3dDevice->SetTexture (0, NULL);
	  //gd3dDevice->SetFVF(D3DFVF_RECTVERTEX);
	  gd3dDevice->SetStreamSource(0, mVB_rect, 0, sizeof(RectVertex));
	  gd3dDevice->SetVertexDeclaration(RectVertex::Decl);

	  RectVertex *vertices;

	  mVB_rect->Lock(0, 0, (void**)&vertices, 0);

	  vertices[0].color = fillColor;
	  vertices[0].pos.x = (float) rect.left;
	  vertices[0].pos.y = (float) rect.top;
	  vertices[0].pos.z = 0.0f;
	  //vertices[0].rhw = 1.0f;	 

	  vertices[1].color = fillColor;
	  vertices[1].pos.x = (float) rect.right;
	  vertices[1].pos.y = (float) rect.top;
	  vertices[1].pos.z = 0.0f;
	  //vertices[1].rhw = 1.0f;	 

	  vertices[2].color = fillColor;
	  vertices[2].pos.x = (float) rect.right;
	  vertices[2].pos.y = (float) rect.bottom ;
	  vertices[2].pos.z = 0.0f;
	  //vertices[2].rhw = 1.0f;	 

	  vertices[3].color = fillColor;
	  vertices[3].pos.x = (float) rect.left;
	  vertices[3].pos.y = (float) rect.bottom;
	  vertices[3].pos.z = 0.0f;
	  //vertices[3].rhw = 1.0f;

	  //Unlock the vertex buffer
	  mVB_rect->Unlock();		  

	  //Set texture	  

	  //Draw image
	  gd3dDevice->DrawPrimitive (D3DPT_TRIANGLEFAN, 0, 2);
}

void Graphics::BlitTexture(IDirect3DTexture9 *texture, int x, int y, int width, int height,		// utan animation
    D3DCOLOR vertexColour, float rotate)
{
  RECT rDest;
  rDest.left = x;
  rDest.right = x + width;
  rDest.top = y;
  rDest.bottom = y + height;
	
  gd3dDevice->SetVertexDeclaration(TextureVertex::Decl);
  gd3dDevice->SetStreamSource(0, mVB_texture, 0, sizeof(TextureVertex));	

  TextureVertex *vertices = 0;
  mVB_texture->Lock(0, 0, (void**)&vertices, 0);

  vertices[0].pos.x = (float) rDest.left;
  vertices[0].pos.y = (float) rDest.top;
  vertices[0].pos.z = 0.0f;
  vertices[0].tex0.x = 0.0f;
  vertices[0].tex0.y = 0.0f;

  vertices[1].pos.x = (float) rDest.right;
  vertices[1].pos.y = (float) rDest.top;
  vertices[1].pos.z = 0.0f;
  vertices[1].tex0.x = 1.0f;
  vertices[1].tex0.y = 0.0f;

  vertices[2].pos.x = (float) rDest.right;
  vertices[2].pos.y = (float) rDest.bottom;
  vertices[2].pos.z = 0.0f;
  vertices[2].tex0.x = 1.0f;
  vertices[2].tex0.y = 1.0f;

  vertices[3].pos.x = (float) rDest.left;
  vertices[3].pos.y = (float) rDest.bottom;
  vertices[3].pos.z = 0.0f;
  vertices[3].tex0.x = 0.0f;
  vertices[3].tex0.y = 1.0f;

  //Unlock the vertex buffer
  mVB_texture->Unlock();

  //Set texture
  gd3dDevice->SetTexture (0, texture);

  //Draw image
  gd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
  gd3dDevice->SetTexture (0, NULL);
}