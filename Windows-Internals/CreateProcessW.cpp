#include <Windows.h>
#include<iostream>
using namespace std;

int main(void) {

	// Initialize STARTUPINFOW struct
	STARTUPINFOW si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);


	// Initialize PROCESS_INFORMATION struct
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));

	// Variable to store command
	wchar_t cmd[] = L"notepad.exe\0";


	// Creating the process
	if (CreateProcessW(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {

		// Wait until child process exits
		WaitForSingleObject(pi.hProcess, INFINITE);

		// Close the process and threads
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

	}
	else {

		// Display an error message via GetLastError() API call
		cerr << "CreateProcessW failed (" << GetLastError() << ").\n";
	}

	return 0;

}
