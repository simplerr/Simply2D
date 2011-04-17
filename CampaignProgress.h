#ifndef CAMPAIGNPROGRESS_H
#define CAMPAIGNPROGRESS_H

#include <string>
#include <vector>

struct LevelProgress
{
	std::string name;
	bool playable;
	// time, points, deaths, date can be added!
};


class CampaignProgress
{
public:
	CampaignProgress();
	~CampaignProgress();

	int loadProgress(std::string source);
	int saveProgress(std::string source);

	LevelProgress getProgress(std::string levelName);
	int setProgress(std::string levelName, bool b);
private:
	std::vector<LevelProgress> mProgressList;
};

#endif