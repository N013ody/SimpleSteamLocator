#include "SteamGame.h"



SteamGame::SteamGame(string name)
{
	char buffer[255];
	this->gameName = name;
	//std::wcout << GetSteamPath()<<endl;
	std::string steamPath = WideStringToUtf8(GetSteamPath().c_str());
	sprintf_s(buffer, sizeof(buffer), "%s/steamapps/common/%s",
		steamPath.c_str(),
		name.c_str());
	this->gamePath = buffer;

	
}

SteamGame::SteamGame(string name, string path) {
	this->gameName = name;
	this->gamePath = path;
}


std::string SteamGame::GetGamePath()
{
	return this->gamePath;
}

std::string SteamGame::GetGameName()
{
	return this->gameName;
}




