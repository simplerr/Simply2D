#ifndef STATSSTATE_H
#define STATSSTATE_H

#include "GameState.h"
#include "CampaignProgress.h"

class StatsState : public GameState
{
public:
	void init(Game* game);
	void cleanup();

	void pause();
	void resume();

	void handleEvents(UINT msg, WPARAM wParam, LPARAM lParam);
	void update(double dt);
	void drawMain(void);
	void drawGui(void);
	void drawBkgd(void);

	static StatsState* Instance() {
		return &mStatsState;
	}

protected:
	StatsState() {};
private:
	static StatsState mStatsState;
	CampaignProgress mProgress;

	int mTableX;
	int mTableY;
};

#endif