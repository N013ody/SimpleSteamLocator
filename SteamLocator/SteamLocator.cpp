

#include <iostream>
#include <RegHelper.h>
#include<FileUtils.h>
#include <Steam.h>
#include <SteamGame.h>
using namespace std;


int main()
{

    wstring errorMsg;
    wstring steamPath = readRegValue(errorMsg,
        HKEY_CURRENT_USER,
        L"Software\\Valve\\Steam",
        L"SteamPath");

    wstring steam = steamPath + L"/steam.exe";

    if (!steamPath.empty()) {
        wcout << L"Steam install path: " << steamPath << std::endl;
        wcout << L"Steam.exe path: " << steam << std::endl;
    }

    Steam steamClient = Steam();

    /**
    SteamGame game = SteamGame("Aim Lab");
    
    cout<<"GameName: " << game.GetGameName() << std::endl;
    cout << "GamePath: " << game.GetGamePath() << std::endl;
    openDirectory(StringToLPCWSTR(game.GetGamePath()));

   **/


    steamClient.upDataGames();
    vector<SteamGame> games = steamClient.getGames();
    cout<<"find game: "<< games.size() << std::endl;
    for (SteamGame game : games) {
        cout<<"GameName: " << game.GetGameName()  << std::endl;
        cout<<"GamePath: " << game.GetGamePath() << std::endl;
    }

    system("pause");
    return 0;

}

