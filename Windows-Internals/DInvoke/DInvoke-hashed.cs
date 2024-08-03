using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using DInvoke.DynamicInvoke;

namespace DInvoke
{
    internal class Program
    {
        static void Main(string[] args)
        {
            // Creating startup info, when using CreateProcessW you need two structs, startupinfo and processinfo.
            var startupInfo = new Win32.STARTUPINFO();
            startupInfo.cb = Marshal.SizeOf(startupInfo);

            // Creating parameers for D/Invoke
            object[] parameters =
                {
                null, "notepad.exe", IntPtr.Zero, IntPtr.Zero, false, (uint)0, IntPtr.Zero, null, startupInfo, new Win32.PROCESS_INFORMATION()

            };

            // Get kernel32.dll
            var hKernel = Generic.GetLoadedModuleAddress("E1D0E254473E5DD0FEB49D3C365CEB31", 0xaedfaefffaaaa);

            // get CreateProcessW
            var hCreateProcess = Generic.GetExportAddress(hKernel, "14F6C4631525A5A6D9A303DF24EEF539", 0xaedfaefffaaaa);

            // D/Invoke CreateProcessW
            var success = (bool)Generic.DynamicFunctionInvoke<bool>(hCreateProcess, typeof(Win32.CreateProcessWDelegate), ref parameters);

            // Getting Process Information
            var processInfo = (Win32.PROCESS_INFORMATION)parameters[9];


            // bail if it failed
            if (!success)
            {
                Console.WriteLine("[x] CreateProcessW failed");
                return;
            }

            // print process info
            Console.WriteLine("dwProcessId : {0}", processInfo.dwProcessId);
            Console.WriteLine("dwThreadId  : {0}", processInfo.dwThreadId);
            Console.WriteLine("hProcess    : 0x{0:X}", processInfo.hProcess);
            Console.WriteLine("hThread     : 0x{0:X}", processInfo.hThread);

            // Create an instance of the CloseHandle delegate
            var closeHandleDelegate = (Win32.CloseHandleDelegate)Marshal.GetDelegateForFunctionPointer(
             Generic.GetLibraryAddress("kernel32.dll", "CloseHandle"),
             typeof(Win32.CloseHandleDelegate)
         ); ;

            // close handles
            closeHandleDelegate(processInfo.hThread);
            closeHandleDelegate(processInfo.hProcess);
        }
    }
}
