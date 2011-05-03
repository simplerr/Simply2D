#include "StatsState.h"
#include "MainMenuState.h"

StatsState StatsState::mStatsState;

void StatsState::init(Game* game)
{
	// important!
	setGame(game);

	// load the campaign progress and all the stats
	mProgress.loadProgress("levels\\campaign\\campaign_progress.txt");

	mTableX = 550;
	mTableY = 300;
}

void StatsState::cleanup()
{

}

void StatsState::pause()
{

}
	
void StatsState::resume()
{

}

void StatsState::handleEvents(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_KEYDOWN:
			if( wParam == VK_ESCAPE )
				changeState(MainMenuState::Instance());
		break;
	}
}
	
void StatsState::update(double dt)
{

}
	
void StatsState::drawMain(void)
{
	gGraphics->BlitRect(600, 450, 1200, 900, D3DCOLOR_ARGB( 155, 155, 200, 000));

	/* backgrounds */
	gGraphics->BlitRect(650, 380, 260, 250, D3DCOLOR_ARGB(255,0,0,0));
	gGraphics->BlitRect(650, 380, 250, 240, D3DCOLOR_ARGB(255,255,255,255));
	gGraphics->BlitRect(650, 280, 260, 50, D3DCOLOR_ARGB(255,0,0,0));
	gGraphics->BlitRect(650, 280, 250, 40, D3DCOLOR_ARGB(255,180,180,240));

	/* titles */
	gGraphics->drawText("Level:", 550, 275);
	gGraphics->drawText("Record:", 630, 275);
	gGraphics->drawText("Tries:", 725, 275);

	/* get the list */
	std::vector<LevelProgress> progressList = mProgress.getList();
	char tmp[256];

	for(int i = 0; i < progressList.size(); i++)
	{
		if(i % 2 > 0)
			gGraphics->BlitRect(650, mTableY + 10 + 20*i, 250, 20, D3DCOLOR_ARGB(255,200,200,255));
		else
			gGraphics->BlitRect(650, mTableY + 10 + 20*i, 250, 20, D3DCOLOR_ARGB(255,255,255,255));

		gGraphics->drawText((char*)progressList[i].name.c_str(), mTableX, mTableY + 20*i);
		
		sprintf(tmp, "%.3f", progressList[i].bestTime);
		gGraphics->drawText(tmp, mTableX + 100, mTableY + 20*i);

		sprintf(tmp, "%i", progressList[i].tries);
		gGraphics->drawText(tmp, mTableX + 200, mTableY + 20*i);

	}
}
	
void StatsState::drawGui(void)
{
	// the green side
	gGraphics->BlitRect(1300, 450, 200, 900, D3DCOLOR_ARGB( 155, 155, 200, 000));
}
	
void StatsState::drawBkgd(void)
{

}