#include "GameState.h"
#include "CampaignProgress.h"

class CampaignLevelState : public GameState
{
public:
	virtual void init(Game* game);
	virtual void cleanup();

	void pause();
	void resume();

	void handleEvents(UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void update(double dt);
	void updateMenu(void);
	virtual void drawMain(void);
	void drawGui(void);
	void drawBkgd(void);
	std::vector<string> getLevels(void);

	bool menuHandler(std::string name);

	static CampaignLevelState* Instance() {
		return &mCampaignLevelState;
	}

	string menuHandler(void);
protected:
	CampaignLevelState() {};
private:
	static CampaignLevelState mCampaignLevelState;
	Menu *mCampaignLevelMenu;
	CampaignProgress mProgress;
};