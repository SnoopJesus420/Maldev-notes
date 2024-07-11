# Windows Architecture

The Windows oeprating system operates under two different modes, user and kernel. Typically, applications run in user mode, when an application wants to accomplish a task, such as creating a file, the program cannot  do so on it's own. The only entity that can complete that task is the kernel.

# Function Call Flow Basics

1. User Process - This is typically an application that is executed by the user, such as chrome or Word.

2. Subsystem DLLs - These are DLLs that contain API functions that can be called by user processes. Some example of these are kernel32.dll, advapi32.dll, and user32.dll.

3. Ntdll.dll - This is a system wide DLL that is the lowest layer available in user mode. This DLL is essentially what makes the transistion between user and kernel mode. Sometimes, this is referred to as the NativeAPI or NTAPI.

4. Executive Kernel - This is what is known as the Windows kernel. This is what calls other drivers and modules to complete tasks. The Windows kernel is partially stored in C:\Windows\system32\ntoskrnl.exe


# Function Call Flow Example

Let's say for example a user has a program open and wants to create a file. The function call flow would look something like this:

1. User Application - Program makes a call to CreateFile, which is avaible in kernel32.dll. This DLL is critical because it exposes applications to the WinAPI and therefore can be loaded by most applications.

2. kern32.dll - kernel32 will then make the equivalent call to the NativeAPI function, NtCreateFile, which is found in ntdll.dll.

3. ntdll.dll - Ntdll.dll will then execute a assembly sysenter (x86) or syscall (x64) instruction, which will transfer the execution to kernel mode.

4. Kernel - The kernel NtCreateFile function is then used to call kernel leve drivers and modules to perform the requested task.


# Directly Invoking the Native API
It's important to remember that applications can invoke syscalls (i.e. NTDLL functions) directly without having to go through the Windows API (subsystem dlls). Essentially the Windows API is a wrapper for the Native API. The reason why applications don't do this more often is because the Native API it's not officially documented, unlike the Windows API. Also, Microsoft tends to advise againts directly invoking syscalls because they can be changed at any time without warning.
