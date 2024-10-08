#include <Windows.h>
#include <iostream>

int main()
{
    const DWORD attributeCount = 1;

    LPSTARTUPINFOEXW si = new STARTUPINFOEXW();
    si->StartupInfo.cb = sizeof(STARTUPINFOEXW);

    SIZE_T lpSize = 0;

    // call once to get lpSize
    InitializeProcThreadAttributeList(
        NULL,
        attributeCount,
        0,
        &lpSize);

    // allocate the memory
    si->lpAttributeList = (LPPROC_THREAD_ATTRIBUTE_LIST)malloc(lpSize);

    // call again to initialise the list
    InitializeProcThreadAttributeList(
        si->lpAttributeList,
        attributeCount,
        0,
        &lpSize);

    // open a handle to the desired parent
    HANDLE hParent = OpenProcess(
        PROCESS_CREATE_PROCESS,
        FALSE,
        5584); // hardcoded pid of explorer, can be anything you choose

    // update the list
    UpdateProcThreadAttribute(
        si->lpAttributeList,
        NULL,
        PROC_THREAD_ATTRIBUTE_PARENT_PROCESS,
        &hParent,
        sizeof(HANDLE),
        NULL,
        NULL);

    // create process
    PPROCESS_INFORMATION pi = new PROCESS_INFORMATION();
    wchar_t cmd[] = L"notepad.exe\0";

    CreateProcess(
        NULL,
        cmd,
        NULL,
        NULL,
        FALSE,
        EXTENDED_STARTUPINFO_PRESENT,
        NULL,
        NULL,
        &si->StartupInfo, 
        pi);

    // print the pid
    printf("PID: %d\n", pi->dwProcessId);

    // cleanup list and memory
    DeleteProcThreadAttributeList(si->lpAttributeList);
    free(si->lpAttributeList);

    // close handle to parent
    CloseHandle(hParent);
}
