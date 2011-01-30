#include "Graphics.h"
#include <tchar.h>

const DWORD D3DFVF_TLVERTEX = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
const DWORD D3DFVF_RECTVERTEX = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

Graphics::Graphics(std::string s)
{
	// set vertex shader
	gd3dDevice->SetVertexShader(NULL);
	//gd3dDevice->SetFVF(D3DFVF_TLVERTEX);

	// create vertex buffer
	// spelar ingen roll med = NULL, men bra vana 
	if(FAILED(gd3dDevice->CreateVertexBuffer(sizeof(TLVERTEX) *4, D3DUSAGE_DYNAMIC, D3DFVF_TLVERTEX, D3DPOOL_DEFAULT, &vertexBuffer, NULL)))
		MessageBox(0, "Kunde inte skapa Vertex Buffer", 0, 0);


	//gd3dDevice->SetFVF(D3DFVF_RECTVERTEX);
	if(FAILED(gd3dDevice->CreateVertexBuffer(sizeof(RECTVERTEX) *4, D3DUSAGE_DYNAMIC, D3DFVF_RECTVERTEX, D3DPOOL_DEFAULT, &vertexBuffer2, NULL)))
		MessageBox(0, "Kunde inte skapa Vertex Buffer", 0, 0);

	mTextureHandler = new TextureHandler();

	loadFont();
}

Graphics::~Graphics()
{
	ReleaseCOM(vertexBuffer);
	ReleaseCOM(vertexBuffer2);
	ReleaseCOM(mFont);

	delete mTextureHandler;
	// not much to do
	//delete vertexBuffer;
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

void Graphics::BlitTexture(IDirect3DTexture9 *texture, RECT rDest,
    D3DCOLOR vertexColour, float rotate)
{
  gd3dDevice->SetFVF(D3DFVF_TLVERTEX);
  gd3dDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(TLVERTEX));
  //char buffer[256];
  //sprintf(buffer, "top: %i left: %i right %i bottom %i", rDest.top, rDest.left, rDest.right, rDest.bottom);
  //MessageBox(0, buffer, 0, 0);
  TLVERTEX *vertices;
  vertexBuffer->Lock(0, 0, (void**)&vertices, 0);

  vertices[0].colour = vertexColour;
  vertices[0].x = (float) rDest.left - 0.5f;
  vertices[0].y = (float) rDest.top - 0.5f;
  vertices[0].z = 0.0f;
  vertices[0].rhw = 1.0f;
  vertices[0].u = 0.0f;
  vertices[0].v = 0.0f;

  vertices[1].colour = vertexColour;
  vertices[1].x = (float) rDest.right - 0.5f;
  vertices[1].y = (float) rDest.top - 0.5f;
  vertices[1].z = 0.0f;
  vertices[1].rhw = 1.0f;
  vertices[1].u = 1.0f;
  vertices[1].v = 0.0f;

  vertices[2].colour = vertexColour;
  vertices[2].x = (float) rDest.right - 0.5f;
  vertices[2].y = (float) rDest.bottom - 0.5f;
  vertices[2].z = 0.0f;
  vertices[2].rhw = 1.0f;
  vertices[2].u = 1.0f;
  vertices[2].v = 1.0f;

  vertices[3].colour = vertexColour;
  vertices[3].x = (float) rDest.left - 0.5f;
  vertices[3].y = (float) rDest.bottom - 0.5f;
  vertices[3].z = 0.0f;
  vertices[3].rhw = 1.0f;
  vertices[3].u = 0.0f;
  vertices[3].v = 1.0f;

  if (rotate != 0)
   {
    RECT rOrigin;
    float centerX, centerY;

    //Find center of destination rectangle
    centerX = (float)(rDest.left + rDest.right) / 2;
    centerY = (float)(rDest.top + rDest.bottom) / 2;

    //Translate destination rect to be centered on the origin
    rOrigin.top = rDest.top - (int)(centerY);
    rOrigin.bottom = rDest.bottom - (int)(centerY);
    rOrigin.left = rDest.left - (int)(centerX);
    rOrigin.right = rDest.right - (int)(centerX);

    //Rotate vertices about the origin
    vertices[0].x = rOrigin.left * cosf(rotate) -
                              rOrigin.top * sinf(rotate);
    vertices[0].y = rOrigin.left * sinf(rotate) +
                              rOrigin.top * cosf(rotate);

    vertices[1].x = rOrigin.right * cosf(rotate) -
                                  rOrigin.top * sinf(rotate);
    vertices[1].y = rOrigin.right * sinf(rotate) +
                                  rOrigin.top * cosf(rotate);

    vertices[2].x = rOrigin.right * cosf(rotate) -
                                  rOrigin.bottom * sinf(rotate);
    vertices[2].y = rOrigin.right * sinf(rotate) +
                                  rOrigin.bottom * cosf(rotate);

    vertices[3].x = rOrigin.left * cosf(rotate) -
                                  rOrigin.bottom * sinf(rotate);
    vertices[3].y = rOrigin.left * sinf(rotate) +
                                  rOrigin.bottom * cosf(rotate);

    //Translate vertices to proper position
    vertices[0].x += centerX;
    vertices[0].y += centerY;
    vertices[1].x += centerX;
    vertices[1].y += centerY;
    vertices[2].x += centerX;
    vertices[2].y += centerY;
    vertices[3].x += centerX;
    vertices[3].y += centerY;
  }

  //Unlock the vertex buffer
  vertexBuffer->Unlock();

  //Set texture
  gd3dDevice->SetTexture (0, texture);

  //Draw image
  gd3dDevice->DrawPrimitive (D3DPT_TRIANGLEFAN, 0, 2);
}

// TEXTUREN MÅSTE VARA HA ^2 I WIDTH OCH HEIGHT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void Graphics::BlitAnimation(IDirect3DTexture9 *texture, RECT rDest,
    D3DCOLOR vertexColour, int width, int height, int frame, float rotate, direction frameDir) // height & width onödigt?
{
  gd3dDevice->SetFVF(D3DFVF_TLVERTEX);
  gd3dDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(TLVERTEX));

  int i = frame / 4;
  int j = frame % 4;

  char buffer[256];
  
  //RECT R = {j*0.25, i*0.25, (j+1)*0.25, (i+1)*0.25};
  float left = j*0.25;
  float top = i*0.5;
  float right = (j+1)*0.25;
  float bottom = (i+1)*0.5;
  
  /*RECT R;
  left = 0;
  top = 0;
  right = 1;
  bottom = 1;*/

  // facing dir is left
  if(frameDir == LEFT)	{
	float tmp = left;
	left = right;
	right = tmp;
  }
  /*float constWidth = 0.25f;
  float left, right, top, bottom;
	
  right = (frame*constWidth);
  left = (right - constWidth);
  top = 0.0f;
  bottom = 0.5f;
  if(frameType == GROUNDLEFT){	  
	  right = -right;
	  left = -left;
	 
  }
  else if(frameType == AIRRIGHT)
  {	  
	  left = 0;
	  right = 0.25f;
	  top = 0.5f;
	  bottom = 1.0f;
  }
  else if(frameType == AIRLEFT)
  {
	  left = 0.25;
	  right = 0.0f;
	  top = 0.5f;
	  bottom = 1.0f;
  }*/

  TLVERTEX *vertices;
  vertexBuffer->Lock(0, 0, (void**)&vertices, 0);

  vertices[0].colour = vertexColour;
  vertices[0].x = (float) rDest.left;
  vertices[0].y = (float) rDest.top;
  vertices[0].z = 0.0f;
  vertices[0].rhw = 1.0f;
  vertices[0].u = left;
  vertices[0].v = top;

  vertices[1].colour = vertexColour;
  vertices[1].x = (float) rDest.right;
  vertices[1].y = (float) rDest.top;
  vertices[1].z = 0.0f;
  vertices[1].rhw = 1.0f;
  vertices[1].u =  right;
  vertices[1].v = top;

  vertices[2].colour = vertexColour;
  vertices[2].x = (float) rDest.right;
  vertices[2].y = (float) rDest.bottom ;
  vertices[2].z = 0.0f;
  vertices[2].rhw = 1.0f;
  vertices[2].u =  right;
  vertices[2].v = bottom;

  vertices[3].colour = vertexColour;
  vertices[3].x = (float) rDest.left;
  vertices[3].y = (float) rDest.bottom;
  vertices[3].z = 0.0f;
  vertices[3].rhw = 1.0f;
  vertices[3].u = left;
  vertices[3].v = bottom;

    RECT rOrigin;
    float centerX, centerY;

    //Find center of destination rectangle
    centerX = (float)(rDest.left + rDest.right) / 2;
    centerY = (float)(rDest.top + rDest.bottom) / 2;

	  //Translate destination rect to be centered on the origin
    rOrigin.top = rDest.top - (int)(centerY);
    rOrigin.bottom = rDest.bottom - (int)(centerY);
    rOrigin.left = rDest.left - (int)(centerX);
    rOrigin.right = rDest.right - (int)(centerX);

    //Rotate vertices about the origin
    vertices[0].x = rOrigin.left * cosf(rotate) -
                              rOrigin.top * sinf(rotate);
    vertices[0].y = rOrigin.left * sinf(rotate) +
                              rOrigin.top * cosf(rotate);

    vertices[1].x = rOrigin.right * cosf(rotate) -
                                  rOrigin.top * sinf(rotate);
    vertices[1].y = rOrigin.right * sinf(rotate) +
                                  rOrigin.top * cosf(rotate);

    vertices[2].x = rOrigin.right * cosf(rotate) -
                                  rOrigin.bottom * sinf(rotate);
    vertices[2].y = rOrigin.right * sinf(rotate) +
                                  rOrigin.bottom * cosf(rotate);

    vertices[3].x = rOrigin.left * cosf(rotate) -
                                  rOrigin.bottom * sinf(rotate);
    vertices[3].y = rOrigin.left * sinf(rotate) +
                                  rOrigin.bottom * cosf(rotate);
  
    //Translate vertices to proper position
    vertices[0].x += centerX;
    vertices[0].y += centerY;
    vertices[1].x += centerX;
    vertices[1].y += centerY;
    vertices[2].x += centerX;
    vertices[2].y += centerY;
    vertices[3].x += centerX;
    vertices[3].y += centerY;

  //Unlock the vertex buffer
  vertexBuffer->Unlock();

  //Set texture
  gd3dDevice->SetTexture (0, texture);

  //Draw image
  gd3dDevice->DrawPrimitive (D3DPT_TRIANGLEFAN, 0, 2);
}


void Graphics::loadFont(void)
{
	D3DXFONT_DESC fontDesc;
	fontDesc.Height          = 18;
    fontDesc.Width           = 7;
    fontDesc.Weight          = 0;
    fontDesc.MipLevels       = 1;
    fontDesc.Italic          = false;
    fontDesc.CharSet         = DEFAULT_CHARSET;
    fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
    fontDesc.Quality         = DEFAULT_QUALITY;
    fontDesc.PitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;
    _tcscpy(fontDesc.FaceName, _T("Arial"));

	HR(D3DXCreateFontIndirect(gd3dDevice, &fontDesc, &mFont));
}
void Graphics::drawText(char *text, int x, int y, D3DCOLOR textColor)
{
	RECT pos = {x, y, 0, 0};
	HR(mFont->DrawText(0, text, -1, &pos, DT_NOCLIP, textColor));
}

void Graphics::BlitRect(float x, float y, int width, int height, D3DCOLOR fillColor)
{
	  // så den inte påverkas av andra texture..
	  gd3dDevice->SetTexture (0, NULL);

	  gd3dDevice->SetFVF(D3DFVF_RECTVERTEX);
	  gd3dDevice->SetStreamSource(0, vertexBuffer2, 0, sizeof(RECTVERTEX));
	  RECT rDest;
	  rDest.left = x-(width/2);
	  rDest.right = x+(width/2);
	  rDest.top = y-(height/2);
	  rDest.bottom = y+(height/2);

	  RECTVERTEX *vertices;

	  vertexBuffer2->Lock(0, 0, (void**)&vertices, 0);

	  vertices[0].colour = fillColor;
	  vertices[0].x = (float) rDest.left;
	  vertices[0].y = (float) rDest.top;
	  vertices[0].z = 0.0f;
	  vertices[0].rhw = 1.0f;	 

	  vertices[1].colour = fillColor;
	  vertices[1].x = (float) rDest.right;
	  vertices[1].y = (float) rDest.top;
	  vertices[1].z = 0.0f;
	  vertices[1].rhw = 1.0f;	 

	  vertices[2].colour = fillColor;
	  vertices[2].x = (float) rDest.right;
	  vertices[2].y = (float) rDest.bottom ;
	  vertices[2].z = 0.0f;
	  vertices[2].rhw = 1.0f;	 

	  vertices[3].colour = fillColor;
	  vertices[3].x = (float) rDest.left;
	  vertices[3].y = (float) rDest.bottom;
	  vertices[3].z = 0.0f;
	  vertices[3].rhw = 1.0f;

	  //Unlock the vertex buffer
	  vertexBuffer2->Unlock();		  

	  //Set texture	  

	  //Draw image
	  gd3dDevice->DrawPrimitive (D3DPT_TRIANGLEFAN, 0, 2);
}

void Graphics::BlitFullRectBordered(float x, float y, int width, int height, D3DCOLOR fillColor, int borderThickness, D3DCOLOR borderColor)
{ 
	  // så den inte påverkas av andra texture..
	  gd3dDevice->SetTexture (0, NULL);

	  gd3dDevice->SetFVF(D3DFVF_RECTVERTEX);
	  gd3dDevice->SetStreamSource(0, vertexBuffer2, 0, sizeof(RECTVERTEX));
	  RECT rDest;
	  rDest.left = x-(width/2);
	  rDest.right = x+(width/2);
	  rDest.top = y-(height/2);
	  rDest.bottom = y+(height/2);

	  RECTVERTEX *vertices;
	  RECTVERTEX *borderVertices;

	  /////////////////////////BOOOORDER
	  vertexBuffer2->Lock(0, 0, (void**)&borderVertices, 0);

	  borderVertices[0].colour = borderColor;
	  borderVertices[0].x = (float) rDest.left-borderThickness;
	  borderVertices[0].y = (float) rDest.top-borderThickness;
	  borderVertices[0].z = 0.0f;
	  borderVertices[0].rhw = 1.0f;	 

	  borderVertices[1].colour = borderColor;
	  borderVertices[1].x = (float) rDest.right+borderThickness;
	  borderVertices[1].y = (float) rDest.top-borderThickness;
	  borderVertices[1].z = 0.0f;
	  borderVertices[1].rhw = 1.0f;	 

	  borderVertices[2].colour = borderColor;
	  borderVertices[2].x = (float) rDest.right+borderThickness;
	  borderVertices[2].y = (float) rDest.bottom+borderThickness;
	  borderVertices[2].z = 0.0f;
	  borderVertices[2].rhw = 1.0f;	 

	  borderVertices[3].colour = borderColor;
	  borderVertices[3].x = (float) rDest.left-borderThickness;
	  borderVertices[3].y = (float) rDest.bottom+borderThickness;
	  borderVertices[3].z = 0.0f;
	  borderVertices[3].rhw = 1.0f;	  

	  //Unlock the vertex buffer
	  vertexBuffer2->Unlock();

	  gd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

	  vertexBuffer2->Lock(0, 0, (void**)&vertices, 0);

	  vertices[0].colour = fillColor;
	  vertices[0].x = (float) rDest.left;
	  vertices[0].y = (float) rDest.top;
	  vertices[0].z = 0.0f;
	  vertices[0].rhw = 1.0f;	 

	  vertices[1].colour = fillColor;
	  vertices[1].x = (float) rDest.right;
	  vertices[1].y = (float) rDest.top;
	  vertices[1].z = 0.0f;
	  vertices[1].rhw = 1.0f;	 

	  vertices[2].colour = fillColor;
	  vertices[2].x = (float) rDest.right;
	  vertices[2].y = (float) rDest.bottom ;
	  vertices[2].z = 0.0f;
	  vertices[2].rhw = 1.0f;	 

	  vertices[3].colour = fillColor;
	  vertices[3].x = (float) rDest.left;
	  vertices[3].y = (float) rDest.bottom;
	  vertices[3].z = 0.0f;
	  vertices[3].rhw = 1.0f;
	  //vertices[3].u = left;
	  //vertices[3].v = bottom;	

	  //Unlock the vertex buffer
	  vertexBuffer2->Unlock();		  

	  //Set texture	  

	  //Draw image
	  gd3dDevice->DrawPrimitive (D3DPT_TRIANGLEFAN, 0, 2);
}

void Graphics::BlitRect(RECT rect, D3DCOLOR fillColor)
{
	// så den inte påverkas av andra texture..
	  gd3dDevice->SetTexture (0, NULL);
	  gd3dDevice->SetFVF(D3DFVF_RECTVERTEX);
	  gd3dDevice->SetStreamSource(0, vertexBuffer2, 0, sizeof(RECTVERTEX));
	  RECTVERTEX *vertices;

	  vertexBuffer2->Lock(0, 0, (void**)&vertices, 0);

	  vertices[0].colour = fillColor;
	  vertices[0].x = (float) rect.left;
	  vertices[0].y = (float) rect.top;
	  vertices[0].z = 0.0f;
	  vertices[0].rhw = 1.0f;	 

	  vertices[1].colour = fillColor;
	  vertices[1].x = (float) rect.right;
	  vertices[1].y = (float) rect.top;
	  vertices[1].z = 0.0f;
	  vertices[1].rhw = 1.0f;	 

	  vertices[2].colour = fillColor;
	  vertices[2].x = (float) rect.right;
	  vertices[2].y = (float) rect.bottom ;
	  vertices[2].z = 0.0f;
	  vertices[2].rhw = 1.0f;	 

	  vertices[3].colour = fillColor;
	  vertices[3].x = (float) rect.left;
	  vertices[3].y = (float) rect.bottom;
	  vertices[3].z = 0.0f;
	  vertices[3].rhw = 1.0f;

	  //Unlock the vertex buffer
	  vertexBuffer2->Unlock();		  

	  //Set texture	  

	  //Draw image
	  gd3dDevice->DrawPrimitive (D3DPT_TRIANGLEFAN, 0, 2);
}