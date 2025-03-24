#define _CRT_SECURE_NO_WARNINGS
#include "RegHelper.h"
#include "CharUtils.h"
#include <windows.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

void printRegistryValue(wstring name, DWORD type, const BYTE* data, DWORD dataSize) {
    switch (type) {
    case REG_SZ: {
        size_t length = dataSize / sizeof(wchar_t);
        if (length % sizeof(wchar_t) != 0)length--;
        std::wstring value(reinterpret_cast<const wchar_t*>(data), length);
        std::wcout << L"  " << name << L" (REG_SZ): " << value << std::endl;
        break;

    }
    case REG_EXPAND_SZ: {
        size_t length = dataSize / sizeof(wchar_t);
        if (length % sizeof(wchar_t) != 0)length--;
        std::wstring value(reinterpret_cast<const wchar_t*>(data), length);
        std::wcout << L"  " << name << L" (REG_EXPAND_SZ): " << value << std::endl;
        break;
    }
    case REG_DWORD: {
        DWORD dwValue = *reinterpret_cast<const DWORD*>(data);
        std::wcout << L"  " << name << L" (REG_DWORD): 0x"
            << std::hex << dwValue << std::dec << std::endl;
        break;
    }
    case REG_QWORD: {
        ULONGLONG qwValue = *reinterpret_cast<const ULONGLONG*>(data);
        std::wcout << L"  " << name << L" (REG_QWORD): 0x"
            << std::hex << qwValue << std::dec << std::endl;
        break;
    }
    case REG_BINARY: {
        std::wcout << L"  " << name << L" (REG_BINARY): ";
        for (DWORD i = 0; i < dataSize && i < 16; ++i) {
            std::wcout << std::hex << std::setw(2) << std::setfill(L'0')
                << static_cast<int>(data[i]) << L" ";
        }
        if (dataSize > 16) std::wcout << L"...";
        std::wcout << std::dec << std::endl;
        break;
    }
    case REG_MULTI_SZ: {
        std::wcout << L"  " << name << L" (REG_MULTI_SZ):" << std::endl;
        const wchar_t* p = reinterpret_cast<const wchar_t*>(data);
        while (*p) {
            std::wcout << L"    " << p << std::endl;
            p += wcslen(p) + 1;
        }
        break;
    }
    default:
        std::wcout << L"  " << name << L" (Type: 0x"
            << std::hex << type << std::dec << L") Data: ";
        for (DWORD i = 0; i < dataSize && i < 16; ++i) {
            std::wcout << std::hex << std::setw(2) << std::setfill(L'0')
                << static_cast<int>(data[i]) << L" ";
        }
        if (dataSize > 16) std::wcout << L"...";
        std::wcout << std::dec << std::endl;
    }
}

wstring readRegValue(wstring& msg, HKEY hKey, LPCWSTR valuePath, LPCWSTR valueName) {
    HKEY hSubKey;
    DWORD dwType, dwSize = 0;
    wchar_t szBuffer[256] = { 0 };

    // 打开注册表项
    if (RegOpenKeyEx(hKey, valuePath, 0, KEY_READ, &hSubKey) != ERROR_SUCCESS) {
        msg = L"Failed to open registry key. Error: " + std::to_wstring(GetLastError());
        return L"";
    }

    // 获取值大小
    if (RegQueryValueEx(hSubKey, valueName, NULL, &dwType, NULL, &dwSize) != ERROR_SUCCESS) {
        msg = L"Failed to get value size. Error: " + std::to_wstring(GetLastError());
        RegCloseKey(hSubKey);
        return L"";
    }

    if (dwType == REG_SZ) {
        if (dwSize > sizeof(szBuffer)) {
            msg = L"Buffer too small. Required size: " + std::to_wstring(dwSize);
            RegCloseKey(hSubKey);
            return L"";
        }

        // 读取值
        if (RegQueryValueEx(hSubKey, valueName, NULL, NULL, (LPBYTE)szBuffer, &dwSize) == ERROR_SUCCESS) {
            // 终止符
            szBuffer[dwSize / sizeof(wchar_t) - 1] = L'\0';

         
         //   OutputDebugStringW(L"\nRead value: ");
       //     OutputDebugStringW(szBuffer);
         //   OutputDebugStringW(L"\n");

            RegCloseKey(hSubKey);
            return szBuffer;
        }
    }

    msg = L"Unsupported type or read error. Error: " + std::to_wstring(GetLastError());
    RegCloseKey(hSubKey);
    return L"";
}

bool enumRegistryValues(HKEY hKeyRoot,const std::wstring& subKeyPath,std::vector<registryValue>& outputValues,std::string& errorMsg) {
    HKEY hKey = nullptr;
    outputValues.clear();

    // 打开注册表键
    LONG openResult = RegOpenKeyExW(
        hKeyRoot,
        subKeyPath.c_str(),
        0,
        KEY_READ,
        &hKey
    );

    if (openResult != ERROR_SUCCESS) {
        errorMsg = "failed to open key path: " + getLastErrorMessage(openResult);
        return false;
    }

    DWORD index = 0;
    const DWORD MAX_NAME_LEN = 256; 
    wchar_t nameBuffer[MAX_NAME_LEN] = { 0 };
    DWORD nameSize = MAX_NAME_LEN;

    while (true) {
        DWORD type = 0;
        DWORD dataSize = 0;
        BYTE* pData = nullptr;

        
        LONG enumResult = RegEnumValueW(
            hKey,
            index,
            nameBuffer,
            &nameSize,
            nullptr,    
            &type,
            nullptr,     
            &dataSize
        );


        if (enumResult == ERROR_NO_MORE_ITEMS) {
            break; // 完成
        }
        else if (enumResult != ERROR_SUCCESS &&
            enumResult != ERROR_MORE_DATA) {
            errorMsg = "failed loop: " + getLastErrorMessage(enumResult);
            RegCloseKey(hKey);
            return false;
        }

        // 名称缓冲区不足
        if (enumResult == ERROR_MORE_DATA) {
         
            nameSize = MAX_NAME_LEN - 1; // 终止符
        }

        // 分配数据缓冲区
        pData = new (std::nothrow) BYTE[dataSize];
        if (!pData) {
            errorMsg = "Failed";
            RegCloseKey(hKey);
            return false;
        }

       
        nameSize = MAX_NAME_LEN; // 重置缓冲区
        enumResult = RegEnumValueW(
            hKey,
            index,
            nameBuffer,
            &nameSize,
            nullptr,
            &type,
            pData,
            &dataSize
        );

        if (enumResult != ERROR_SUCCESS) {
            delete[] pData;
            errorMsg = "failed get data: " + getLastErrorMessage(enumResult);
            RegCloseKey(hKey);
            return false;
        }

        registryValue newVal;
        newVal.name = nameBuffer;
        newVal.type = type;
        newVal.data = pData;
        newVal.dataSize = dataSize;
        outputValues.push_back(std::move(newVal));

 
        index++;
        nameSize = MAX_NAME_LEN;
    }


    RegCloseKey(hKey);
    return true;
}

bool enumRegistryValues(HKEY hKey, LPCWSTR valuePath) {
    vector<registryValue> values;
    string errorMsg;

    if (!enumRegistryValues(hKey, valuePath, values, errorMsg)) return false;
   std::wcout << L"find " << values.size() << L"  value:\n";
   int index = 0;
   for (const auto& val : values) {
       index++;
       printf("%d.", index);
      // std::wcout << L"name: " << val.name << L"  type: " << val.type << L"  data: " << val.data << L"\n";
    //  printRegistryValue(val.name, val.type, val.data, val.dataSize);
   }

    return true;
}