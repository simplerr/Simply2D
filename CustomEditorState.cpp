#include "CustomEditorState.h"
#include "MainMenuState.h"
#include <fstream>
#include <stdio.h>

void CustomEditorState::init(Game* game)
{
	// create the window handler
	mWindowHandler = new WindowHandler(700, 200, 256, 128);
	mWindowHandler->setBackground(NEW_LEVEL_SOURCE);
	mWindowHandler->setVisible(false);

	mLabel = new TextBox(mWindowHandler, LABEL, "Name:", 50, 70, 50, 24);
	mLevelName = new InputBox(mWindowHandler, LEVEL_NAME, 200, 70, 60, 20, 12);
	mCreateButton = new Button(mWindowHandler, BUTTON_CREATE, "Create", 130, 105, 60, 30, true);
	mCreateButton->connect(&CustomEditorState::messageHandler, this);

	CustomLevelState::init(game);
	addItem("New", (char*)NORMAL_BUTTON_SOURCE.c_str(), (char*)HOOVER_BUTTON_SOURCE.c_str());
	addItem("Delete", (char*)NORMAL_BUTTON_SOURCE.c_str(), (char*)HOOVER_BUTTON_SOURCE.c_str());
	mCustomLevelMenu->connect(&CustomEditorState::menuHandler, this);

	mDeletePressed = false;
}

void CustomEditorState::cleanup()
{
	CustomLevelState::cleanup();
	delete mWindowHandler;
	mWindowHandler = NULL;
}

void CustomEditorState::update(double dt)
{
	// call menuHandler() when a item is pressed
	// state changing takes place there
	updateMenu();

	if(mWindowHandler != NULL)
		mWindowHandler->update(dt);
}

void CustomEditorState::drawMain(void)
{
	CustomLevelState::drawMain();

	mWindowHandler->draw();
}

void CustomEditorState::handleEvents(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_KEYDOWN:
			if( wParam == VK_ESCAPE )
				changeState(MainMenuState::Instance());
			else
				mWindowHandler->keyPressed(wParam);
	}
}

bool CustomEditorState::menuHandler(std::string name)	
{

	if(name == "New")
	{
		mWindowHandler->setVisible(true);
	}
	else if(name == "Delete")
	{
		mDeletePressed = true;
	}
	else	
	{
		/*add levels\\ and .txt */
		string tmp = "levels\\";
		tmp.append(name);
		tmp.append(".txt");

		if(!mDeletePressed)
		{
			changeState(EditorState::Instance());
			EditorState::Instance()->setLevel(tmp);
			EditorState::Instance()->setMusic(getMusic());

			return false;
		}
		else
		{
			/* remove the pressed level */
			remove(tmp.c_str());
			removeItem(tmp);
			mDeletePressed = false;
			return false;
		}
	}
	return true;
}

bool CustomEditorState::messageHandler(WindowID id, std::string value)
{
	// create a new level!
	if(id == BUTTON_CREATE)
	{
		/*add levels\\ and .txt */
		string tmp = "levels\\";
		tmp.append(mLevelName->getValue());
		tmp.append(".txt");

		ofstream fout(tmp);
		fout << "300 300\n600 300\n" << tmp << endl;

		fout.close();

		changeState(EditorState::Instance());
		EditorState::Instance()->setLevel(tmp);
		return false;
	}

	return true;
}
