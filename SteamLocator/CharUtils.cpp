#include "CharUtils.h"

LPCWSTR StringToLPCWSTR(const std::string& str) {
    // ��ȡ��Ҫ�Ŀ��ַ���������С
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);

    // ������ַ�������
    wchar_t* wstr = new wchar_t[size_needed];

    // ִ��ת��
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

    // ��ȡת����ĳ���
    int utf8Len = WideCharToMultiByte(
        CP_UTF8,          // Ŀ�����
        0,                // ��־
        wideStr,          // Դ�ַ���
        -1,               // �Զ����㳤��
        nullptr,          // ����ҪĿ�껺����
        0,                // ����ҪĿ�껺����
        nullptr, nullptr  // Ĭ���滻�ַ�
    );

    // ����Ŀ�껺����
    std::string utf8Str(utf8Len, 0);

    // ִ��ʵ��ת��
    WideCharToMultiByte(
        CP_UTF8,
        0,
        wideStr,
        -1,
        &utf8Str[0],      // Ŀ�껺����
        utf8Len,
        nullptr, nullptr
    );

    return utf8Str;
}

std::string DWORDToString(DWORD dwValue) {
    char buffer[11]; // ���32λ�޷�������Ϊ4294967295 10�ַ�+1��ֹ��
    int result = sprintf_s(buffer, sizeof(buffer), "%u", dwValue);

    if (result == -1) {
        return nullptr; 
    }

    return std::string(buffer);
}