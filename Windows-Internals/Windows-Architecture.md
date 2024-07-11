# Windows Architecture

The Windows oeprating system operates under two different modes, user and kernel. Typically, applications run in user mode, when an application wants to accomplish a task, such as creating a file, the program cannot  do so on it's own. The only entity that can complete that task is the kernel.

# Function Call Flow

1. User Process - This is typically an application that is executed by the user, such as chrome or Word.

2. Subsystem DLLs - These are DLLs that contain API functions that can be called by user processes. Some example of these are kernel32.dll, advapi32.dll, and user32.dll.

3. Ntdll.dll - This is a system wide DLL that is the lowest layer available in user mode. This DLL is essentially what makes the transistion between user and kernel mode. Sometimes, this is referred to as the NativeAPI or NTAPI.
