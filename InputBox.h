#ifndef _INPUTBOX_H_
#define _INPUTBOX_H_

#include "d3dUtil.h"
#include "Window.h"
#include "Graphics.h"
#include "DirectInput.h"
#include <string>
#include <boost\function.hpp>
#include <boost\bind.hpp>

class InputBox : public Window
{
public:
	InputBox(WindowHandler* handler, WindowID id, DataType dataType, int x, int y, int width, int height, int maxlen, D3DCOLOR color = D3DCOLOR_ARGB( 255, 230, 230, 230 ));
	~InputBox();

	void update(double dt);
	void draw(void);
	bool pressed(int mx, int my);

	void setActive(bool b);
	int wm_lbuttondown(int x, int y); 
	int wm_keydown(WPARAM wParam);
  
	boost::function<bool(WindowID id, WindowMessage msg)> callback;

	template <class T>
	void connect(bool(T::*_callback)(WindowID id, WindowMessage msg), T* _object)	{
		callback = boost::bind(_callback, _object, _1, _2);
	}


	void setValue(string value);
	void setValue(int value);
	void setValue(float value);

	int getInt(void);
	float getFloat(void);
	string getString(void);

	/*template<class T>
	T getValue(void);*/

private:
	int			mMaxLength;
	int			mCaretPos;
	int			mCaretTick;
	bool		mShowCaret;
	bool		mHandled;
	string		mValue;
	DataType	mDataType;
};

#endif

