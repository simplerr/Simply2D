#ifndef _TEXTBOX_H_
#define _TEXTBOX_H_

//#include "O_Drawing.h"
#include <string>
#include <vector>
#include "d3dUtil.h"
#include <vector>
#include "Graphics.h"
#include <string>
#include "DirectInput.h"
#include "constants.h"
#include "Mouse.h"
using namespace std;

// Developing a GUI Using C++ and DirectX
// Source Listing 1
// Window Class

static char buffer[256];

class Window
{
public:
  Window(char* name, int x, int y, int width, int height, D3DCOLOR color = D3DCOLOR_ARGB( 155, 155, 200, 000 ));
  virtual ~Window();
  virtual void init(void) {/*ctor?*/};
  void setParent(Window* parent)	{mParent = parent;};
  Window *getParent(void)	{return(mParent);};
  Window *getWindow(void) {return(this);};
  char *getName(void) {return mName;};
  Window *findChildAtCoord(int x, int y);
  int windowCount(void) {return mSubWins.size();};

  WindowRect getInfo(void)	{return mPosition;};

  /////////////
  // section I:  window management controls
  /////////////

  int addWindow(Window *w);
  int removewindow(Window *w){/*Bygg egen ptr class?*/};
  //int addwindow(string type, int x, int y, int width, int height);
  
  /////////////
  // Section II: coordinates
  /////////////  

  void setpos(int x, int y)		{/*icke bråttom*/};
  void setsize(int width, int height)		{/*icke bråttom*/};
  void updateRectToNewXY(void);
  bool isActive(void) { return mActive;};

  virtual void updateWindow(float dt)	{if(mActiveWin != this)mActiveWin->updateWindow(dt);};

  //messaging
  //?int calCall(void); 
  void handleMessages(int msg, int mx = 0, int my = 0);
  virtual int wm_lbuttondown(int x, int y);
  virtual int wm_keydown(WPARAM wParam);//	{return 1;};

  void keyPressed(WPARAM wParam);

  virtual int renderAll(void);// varför detta: ? int x, int y, int drawme = 1); 
 
  void setName(char *name) {mName = name;};
  string getValue(char *name);
  void setValue(char *name, string value);
  void setActive(bool state){mActiveWin->mActive = state;};
  void setVisibility(char *name, bool value);

protected:
   Window *mParent;
   Window *mActiveWin;
   vector<Window*> mSubWins;
   WindowRect mPosition;
   D3DCOLOR mColor;

   char *mName;
   string mValue;
   char *activeWin;

   bool mVisible;
   bool mActive;
   bool inputState;
};


#endif