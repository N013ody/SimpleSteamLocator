#include "CharUtils.h"

LPCWSTR StringToLPCWSTR(const std::string& str) {
    // 获取需要的宽字符缓冲区大小
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);

    // 分配宽字符缓冲区
    wchar_t* wstr = new wchar_t[size_needed];

    // 执行转换
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wstr, size_needed);

    return wstr;
}

std::string getLastErrorMessage(DWORD errorCode) {
    LPSTR messageBuffer = nullptr;
    const DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS;

    if (FormatMessageA(flags, nullptr, errorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&messageBuffer, 0, nullptr)) {
        std::string message(messageBuffer);
        LocalFree(messageBuffer);
        return message;
    }
    return "unknown error code: " + std::to_string(errorCode);
}

std::string WideStringToUtf8(LPCWSTR wideStr) {
    if (!wideStr) return "";

    // 获取转换后的长度
    int utf8Len = WideCharToMultiByte(
        CP_UTF8,          // 目标编码
        0,                // 标志
        wideStr,          // 源字符串
        -1,               // 自动计算长度
        nullptr,          // 不需要目标缓冲区
        0,                // 不需要目标缓冲区
        nullptr, nullptr  // 默认替换字符
    );

    // 分配目标缓冲区
    std::string utf8Str(utf8Len, 0);

    // 执行实际转换
    WideCharToMultiByte(
        CP_UTF8,
        0,
        wideStr,
        -1,
        &utf8Str[0],      // 目标缓冲区
        utf8Len,
        nullptr, nullptr
    );

    return utf8Str;
}

std::string DWORDToString(DWORD dwValue) {
    char buffer[11]; // 最大32位无符号整数为4294967295 10字符+1终止符
    int result = sprintf_s(buffer, sizeof(buffer), "%u", dwValue);

    if (result == -1) {
        return nullptr; 
    }

    return std::string(buffer);
}