#pragma once
#include <windows.h>
#include <string>
#include <iostream>
using namespace std;
struct registryValue {
    wstring name;
    DWORD type;
    BYTE* data; // ��Ϊ�洢�ֽ��������ָ��
    DWORD dataSize;
};

wstring readRegValue(wstring& msg, HKEY hKey, LPCWSTR valuePath, LPCWSTR valueName);
bool enumRegistryValues(HKEY hKey, LPCWSTR valuePath);
void printRegistryValue(wstring name, DWORD type, const BYTE* data, DWORD dataSize);