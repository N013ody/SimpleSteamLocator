#pragma once
#include <string>
#include <memory>
#include <wtypes.h>
#include <windows.h>
#include <stdio.h>

LPCWSTR StringToLPCWSTR(const std::string& str);

std::string WideStringToUtf8(LPCWSTR wideStr);

std::string DWORDToString(DWORD dwValue);

std::string getLastErrorMessage(DWORD errorCode);