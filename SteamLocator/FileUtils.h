#pragma once
#include <windows.h>
#include <wtypes.h>
#include <filesystem>

namespace fs = std::filesystem;

//  Ŀ��Ŀ¼·��
int openDirectory(LPCWSTR directory);

bool is_folder_exist(const fs::path& folder_path);

void list_folder_contents(const fs::path& folder_path, std::vector<std::pair<std::string, std::string>>& paths);