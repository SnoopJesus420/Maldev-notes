using System;
using System.Runtime.InteropServices;
using DInvoke.DynamicInvoke;

namespace DInvoke
{
    internal class Program
    {
        static void Main(string[] args)
        {
            // Initialize STARTUPINFO
            var startupInfo = new Win32.STARTUPINFO();
            startupInfo.cb = Marshal.SizeOf(startupInfo);

            // Initialize PROCESS_INFORMATION
            var processInfo = new Win32.PROCESS_INFORMATION();

            // Creating parameters for D/Invoke
            object[] parameters =
            {
                null, "notepad.exe", IntPtr.Zero, IntPtr.Zero, false, (uint)0, IntPtr.Zero, null, startupInfo, processInfo
            };

            try
            {
                // Invoking CreateProcessW using the ordinal 233
                var hLibrary = Generic.GetLibraryAddress("kernel32.dll", 247);

                // Debug: Ensure we got a valid function pointer
                if (hLibrary == IntPtr.Zero)
                {
                    Console.WriteLine("[x] Failed to get function pointer for CreateProcessW");
                    return;
                }

                var success = (bool)Generic.DynamicFunctionInvoke<bool>(hLibrary, typeof(Win32.CreateProcessWDelegate), ref parameters);

                // Retrieve PROCESS_INFORMATION structure
                processInfo = (Win32.PROCESS_INFORMATION)parameters[9];

                // Check if CreateProcessW succeeded
                if (!success)
                {
                    Console.WriteLine("[x] CreateProcessW failed with error code: " + Marshal.GetLastWin32Error());
                    return;
                }

                // Print process info
                Console.WriteLine("dwProcessId : {0}", processInfo.dwProcessId);
                Console.WriteLine("dwThreadId  : {0}", processInfo.dwThreadId);
                Console.WriteLine("hProcess    : 0x{0:X}", processInfo.hProcess);
                Console.WriteLine("hThread     : 0x{0:X}", processInfo.hThread);

                // Create an instance of the CloseHandle delegate
                var closeHandlePtr = Generic.GetLibraryAddress("kernel32.dll", "CloseHandle");
                var closeHandleDelegate = (Win32.CloseHandleDelegate)Marshal.GetDelegateForFunctionPointer(
                    closeHandlePtr,
                    typeof(Win32.CloseHandleDelegate)
                );

                // Close handles
                closeHandleDelegate(processInfo.hThread);
                closeHandleDelegate(processInfo.hProcess);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Exception: {ex.Message}");
                Console.WriteLine($"Stack Trace: {ex.StackTrace}");
            }
        }
    }
}
