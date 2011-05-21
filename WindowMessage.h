#ifndef WINDOWMESSAGE_H
#define WINDOWMESSAGE_H

#include <string>
#include "constants.h"
using namespace std;

class WindowMessage
{
public:
	WindowMessage(string data);
	WindowMessage(int data);
	WindowMessage(float data);
	WindowMessage(bool data);
	~WindowMessage();

	int getInt(void);
	float getFloat(void);
	string getString(void);
	bool getBool(void);

private:
	string			mData;
};

#endif

/* int getInt(void);
	bool getBool(void);
	float getFloat(void);
	string getString(void);
	*/