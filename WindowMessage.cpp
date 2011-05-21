#include "WindowMessage.h"
#include "boost\lexical_cast.hpp"

WindowMessage::WindowMessage(string data)	
{
	mData = boost::lexical_cast<string>(data);
}

WindowMessage::WindowMessage(int data)	
{
	mData = boost::lexical_cast<string>(data);
}

WindowMessage::WindowMessage(float data)	
{
	mData = boost::lexical_cast<string>(data);
}

WindowMessage::WindowMessage(bool data)	
{
	if(data)
		mData = "true";
	else
		mData = "false";
}

WindowMessage::~WindowMessage() 
{
	// dtor
}

std::string WindowMessage::getString(void)
{
	return mData;
}

int WindowMessage::getInt(void)
{
	return boost::lexical_cast<int>(mData);
}
	
float WindowMessage::getFloat(void)
{
	return boost::lexical_cast<float>(mData);
}

bool WindowMessage::getBool(void)
{
	if(mData == "true")
		return true;
	else
		return false;
}
