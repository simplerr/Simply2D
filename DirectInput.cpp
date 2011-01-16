//=============================================================================
// DirectInput.cpp by Frank Luna (C) 2005 All Rights Reserved.
//=============================================================================

#include "d3dUtil.h"
#include "DirectInput.h"
#include "d3dApp.h"

DirectInput* gDInput = 0;

DirectInput::DirectInput(DWORD keyboardCoopFlags, DWORD mouseCoopFlags)
{
	ZeroMemory(mKeyboardState, sizeof(mKeyboardState));
	ZeroMemory(&mMouseState, sizeof(mMouseState));

	HR(DirectInput8Create(gd3dApp->getAppInst(), DIRECTINPUT_VERSION, 
		IID_IDirectInput8, (void**)&mDInput, 0));

	HR(mDInput->CreateDevice(GUID_SysKeyboard, &mKeyboard, 0));
	HR(mKeyboard->SetDataFormat(&c_dfDIKeyboard));
	HR(mKeyboard->SetCooperativeLevel(gd3dApp->getMainWnd(), keyboardCoopFlags));
	HR(mKeyboard->Acquire());

	HR(mDInput->CreateDevice(GUID_SysMouse, &mMouse, 0));
	HR(mMouse->SetDataFormat(&c_dfDIMouse2));
	HR(mMouse->SetCooperativeLevel(gd3dApp->getMainWnd(), mouseCoopFlags));
	HR(mMouse->Acquire());
}

DirectInput::~DirectInput()
{
	ReleaseCOM(mDInput);
	mKeyboard->Unacquire();
	mMouse->Unacquire();
	ReleaseCOM(mKeyboard);
	ReleaseCOM(mMouse);
}

void DirectInput::poll()
{
	// Poll keyboard.
	memcpy(mLastKeyboardState, mKeyboardState, sizeof(mKeyboardState));
	HRESULT hr = mKeyboard->GetDeviceState(sizeof(mKeyboardState), (void**)&mKeyboardState); 
	if( FAILED(hr) )
	{
		// Keyboard lost, zero out keyboard data structure.
		ZeroMemory(mKeyboardState, sizeof(mKeyboardState));

		 // Try to acquire for next time we poll.
		hr = mKeyboard->Acquire();
	}

	// Poll mouse.
	memcpy(&mLastMouseState, &mMouseState, sizeof(mMouseState));
	hr = mMouse->GetDeviceState(sizeof(DIMOUSESTATE2), (void**)&mMouseState); 
	if( FAILED(hr) )
	{
		// Mouse lost, zero out mouse data structure.
		ZeroMemory(&mMouseState, sizeof(mMouseState));

		// Try to acquire for next time we poll.
		hr = mMouse->Acquire(); 
	}
}

bool DirectInput::keyDown(char key)
{
	return (mKeyboardState[key] & 0x80) != 0;
}
// fixa till ... >_>
bool DirectInput::keyPressed(char key)
{	
	return (!mLastKeyboardState[key] && mKeyboardState[key] & 0x80 ) != 0;	
}

bool DirectInput::mouseButtonDown(int button)
{
	return (mMouseState.rgbButtons[button] & 0x80) != 0;
}

bool DirectInput::mouseButtonPressed(int button)
{
	return (!mLastMouseState.rgbButtons[button] && mMouseState.rgbButtons[button] & 0x80) != 0;
}

float DirectInput::mouseDX()
{
	return (float)mMouseState.lX;
}

float DirectInput::mouseDY()
{
	return (float)mMouseState.lY;
}

float DirectInput::mouseDZ()
{
	return (float)mMouseState.lZ;
}

char DirectInput::ScanToChar(DWORD scanCode) const
{
	//obtain keyboard information
	static HKL layout = GetKeyboardLayout(0);
	static UCHAR keyboardState[256];
	if (GetKeyboardState(keyboardState) == false)
		return 0;

	//translate keyboard press scan code identifier to a char
	UINT vk = MapVirtualKeyEx(scanCode, 1, layout);
	USHORT asciiValue;
	ToAscii(vk, scanCode, keyboardState, &asciiValue, 0);

	return 'a';
}


char DirectInput::getInput(void)
{
	for(int i = 0;i<255;i++){
		if(mKeyboardState[i] != mLastKeyboardState[i])	{
			if(mKeyboardState[i] & 0x80)	{
				char tmp;
				tmp = ScanToChar(DIK_A);
				return tmp;//mKeyboardState[i];
			}
		}
	}		
	return 'n';
}