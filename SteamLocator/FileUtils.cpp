#include "FileUtils.h"
#include "iostream"

using namespace std;
int openDirectory(LPCWSTR directory) {
    // 调用系统资源管理器

    HINSTANCE result = ShellExecute(
        NULL,
        L"open",
        directory,
        NULL,
        NULL,
        SW_SHOWNORMAL
    );

    return (INT_PTR)result;
}

bool is_folder_exist(const fs::path& folder_path) {
    return fs::exists(folder_path) && fs::is_directory(folder_path);
}



void list_folder_contents(const fs::path& folder_path, std::vector<std::pair<std::string, std::string>>& paths) {

    try {
        for (const auto& entry : fs::directory_iterator(folder_path)) {
            const auto& path = entry.path();
            if (fs::is_directory(path)) {

                paths.push_back(make_pair(path.filename().string(), path.generic_string()));
            }
            else if (fs::is_regular_file(path)) {
                continue;
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cout<< "Error:  %s   "<<e.what()<<endl;
    }
}
