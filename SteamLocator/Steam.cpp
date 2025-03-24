#include "Steam.h"

namespace fs = std::filesystem;


std::wstring GetSteamPath() {
    std::wstring errorMsg;
    return readRegValue(errorMsg,
        HKEY_CURRENT_USER,
        L"Software\\Valve\\Steam",
        L"SteamPath");
}

Steam::Steam()
{
    std::wstring errorMsg;
	this->path= GetSteamPath();
}

Steam::Steam(std::wstring path)
{
    this->path = path;
}


void Steam::SetPath(std::wstring path)
{
    this->path = path;
}

std::wstring Steam::GetPath()
{
    return this->path;
}

std::wstring Steam::GetGameIconPath()
{
	return this->path+L"\\steam\\games";
}

std::wstring Steam::GetGameExePath()
{
    return this->path + L"\\steam.exe";
}

bool Steam::runSteam()
{
    if (!fs::exists(this->GetGameExePath())) {
        return false;
    }
    return openDirectory(this->GetGameExePath().c_str());
}

bool Steam::upDataGames()
{
    char buffer[255];
    std::string steamPath = WideStringToUtf8(GetSteamPath().c_str());
    sprintf_s(buffer, sizeof(buffer), "%s/steamapps/common",
        steamPath.c_str());
//    cout<<"buffer:  " << buffer << endl;
    if (!is_folder_exist(buffer)) return false; 
    vector<pair<string, string>> pathInput;
    list_folder_contents(buffer, pathInput);
    //first = filename  second = filepath
    for (pair<string,string> info:pathInput)
    {
    //    cout<<"info.second  "<< info.second << endl;
        games.push_back(SteamGame(info.first, info.second));
    }

    return true;
}

string Steam::getGamePath(int index)
{
    return string();
}

vector<SteamGame> Steam::getGames()
{
    return this->games;
}










