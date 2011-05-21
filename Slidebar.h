#ifndef SLIDEBAR_H
#define SLIDEBAR_H

#include "Window.h"
#include "boost\function.hpp"

class TextBox;
class InputBox;

class Slidebar	:	public Window
{
public:
	Slidebar(WindowHandler* handler, WindowID id, string display, int x, int y, int width, int height, float value, float minValue, float maxValue);
	~Slidebar();

	boost::function<bool(WindowID id, WindowMessage msg)> callback;

	template <class T>
	void connect(bool(T::*_callback)(WindowID id, WindowMessage msg), T* _object)	{
		callback = boost::bind(_callback, _object, _1, _2);
		mInputBox->connect(callback);
	}

	void update(double dt);
	void draw(void);
	bool pressed(int mx, int my);

private:
	float		mValue;
	float		mMinValue;
	float		mMaxValue;
	float		mSliderPos;
	RECT		mSlider;
	RECT		mSlideBackground;
	bool		mMovingSlider;
	int			mSliderWidth;
	TextBox		*mTextName;
	InputBox	*mInputBox;
	// name
	// data
};

#endif