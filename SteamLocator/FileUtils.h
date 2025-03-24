#pragma once
#include <windows.h>
#include <wtypes.h>
#include <filesystem>

namespace fs = std::filesystem;

//  目标目录路径
int openDirectory(LPCWSTR directory);

bool is_folder_exist(const fs::path& folder_path);

void list_folder_contents(const fs::path& folder_path, std::vector<std::pair<std::string, std::string>>& paths);