#pragma once
#include <string>
#include <CharUtils.h>
#include <Steam.h>
using namespace std;
class SteamGame{

public:
	SteamGame() {};
	SteamGame(string name);
	SteamGame(string name, string path);

	std::string GetGamePath();
	std::string GetGameName();
	~SteamGame() {}

protected:
	string gameName;
	string gamePath;
};