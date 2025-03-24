#pragma once
#include <string>
#include <RegHelper.h>
#include <FileUtils.h>
#include <SteamGame.h>

class SteamGame;

std::wstring GetSteamPath();


class Steam {
public:
	
	Steam();
	Steam(std::wstring path);
	void SetPath(std::wstring path);
	std::wstring GetPath();
	std::wstring GetGameIconPath();
	std::wstring GetGameExePath();
	bool runSteam();
	bool upDataGames();
	string getGamePath(int index);
	vector<SteamGame>getGames();

protected:
    	std::wstring path;
		vector<SteamGame> games;
private:

		
};