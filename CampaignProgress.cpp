#include "CampaignProgress.h"
#include <fstream>

using namespace std;

CampaignProgress::CampaignProgress()
{
	// nothing todo?
}

CampaignProgress::~CampaignProgress()
{
	// nothing todo?
	mProgressList.clear();
}

int CampaignProgress::loadProgress(std::string source)
{
	ifstream fin(source);

	/* find out number of levels (lines) */
	int levels = 0;
	static char tmpChar[256];

	fin.tellg();

	while(fin.getline(tmpChar, 255))
		levels++;
	
	/* reset position */
	fin.close();
	fin.open(source);

	static LevelProgress progress;
	mProgressList.clear();
	for(int i = 0; i < levels; i++)
	{
		fin >> progress.name >> progress.playable;

		mProgressList.push_back(progress);
	}

	fin.close();

	return 1;
}

int CampaignProgress::saveProgress(std::string source)
{
	ofstream fout(source);

	for(int i = 0; i < mProgressList.size(); i++)
	{
		fout << mProgressList[i].name << " " << mProgressList[i].playable << endl;
	}

	fout.close();

	return 1;
}

LevelProgress CampaignProgress::getProgress(std::string levelName)
{
	for(int i = 0; i < mProgressList.size(); i++)
	{
		if(mProgressList[i].name == levelName)
			return mProgressList[i];
	}

	LevelProgress tmp;
	tmp.name = "ERROR";
	return tmp;
}

int CampaignProgress::setProgress(std::string levelName, bool b)
{
	for(int i = 0; i < mProgressList.size(); i++)
	{
		if(mProgressList[i].name == levelName)	{
			mProgressList[i].playable = b;
			saveProgress("levels\\campaign\\campaign_progress.txt");
			return 1;
		}
	}

	return 0;
}