using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using DInvoke.DynamicInvoke;
using System.Diagnostics.SymbolStore;

namespace Dinvoke
{
    internal class Win32
    {

     
        // Use D/Invoke to define a delegate for CreateProcessW
        [UnmanagedFunctionPointer(CallingConvention.StdCall, CharSet = CharSet.Unicode, SetLastError = true)]
        public delegate bool CreateProcessWDelegate(
            string applicationName,
            string commandLine,
            IntPtr processAttributes,
            IntPtr threadAttributes,
            bool inheritHandles,
            CREATION_FLAGS creationFlags,
            IntPtr environment,
            string currentDirectory,
            ref STARTUPINFO startupInfo,
            out PROCESS_INFORMATION processInfo);


        // Use D/Invoke to define a delegate for CloseHandle
        [UnmanagedFunctionPointer(CallingConvention.StdCall, CharSet = CharSet.Unicode, SetLastError = true)]
        public delegate bool CloseHandleDelegate(IntPtr hObject);

        public static bool CloseHandleDInvoke(IntPtr hObject)
        {
            IntPtr functionPtr = Generic.GetLibraryAddress("kernel32.dll", "CloseHandle");

            CloseHandleDelegate closeHandle = (CloseHandleDelegate)Marshal.GetDelegateForFunctionPointer(functionPtr, typeof(CloseHandleDelegate));

            return closeHandle(hObject);


        }

        // Struct for CreateProcessW flags
        [Flags]
        public enum CREATION_FLAGS : uint
        {
            DEBUG_PROCESS = 0x00000001,
            DEBUG_ONLY_THIS_PROCESS = 0x00000002,
            CREATE_SUSPENDED = 0x00000004,
            DETACHED_PROCESS = 0x00000008,
            CREATE_NEW_CONSOLE = 0x00000010,
            NORMAL_PRIORITY_CLASS = 0x00000020,
            IDLE_PRIORITY_CLASS = 0x00000040,
            HIGH_PRIORITY_CLASS = 0x00000080,
            REALTIME_PRIORITY_CLASS = 0x00000100,
            CREATE_NEW_PROCESS_GROUP = 0x00000200,
            CREATE_UNICODE_ENVIRONMENT = 0x00000400,
            CREATE_SEPARATE_WOW_VDM = 0x00000800,
            CREATE_SHARED_WOW_VDM = 0x00001000,
            CREATE_FORCEDOS = 0x00002000,
            BELOW_NORMAL_PRIORITY_CLASS = 0x00004000,
            ABOVE_NORMAL_PRIORITY_CLASS = 0x00008000,
            INHERIT_PARENT_AFFINITY = 0x00010000,
            INHERIT_CALLER_PRIORITY = 0x00020000,
            CREATE_PROTECTED_PROCESS = 0x00040000,
            EXTENDED_STARTUPINFO_PRESENT = 0x00080000,
            PROCESS_MODE_BACKGROUND_BEGIN = 0x00100000,
            PROCESS_MODE_BACKGROUND_END = 0x00200000,
            CREATE_SECURE_PROCESS = 0x00400000,
            CREATE_BREAKAWAY_FROM_JOB = 0x01000000,
            CREATE_PRESERVE_CODE_AUTHZ_LEVEL = 0x02000000,
            CREATE_DEFAULT_ERROR_MODE = 0x04000000,
            CREATE_NO_WINDOW = 0x08000000,
            PROFILE_USER = 0x10000000,
            PROFILE_KERNEL = 0x20000000,
            PROFILE_SERVER = 0x40000000,
            CREATE_IGNORE_SYSTEM_DEFAULT = 0x80000000,
        }

        // Initialize struct for STARTUPINFO
        [StructLayout(LayoutKind.Sequential)]
        public struct STARTUPINFO
        {
            public int cb;
            public IntPtr lpReserved;
            public IntPtr lpDesktop;
            public IntPtr lpTitle;
            public int dwX;
            public int dwY;
            public int dwXSize;
            public int dwYSize;
            public int dwXCountChars;
            public int dwYCountChars;
            public int dwFillAttribute;
            public int dwFlags;
            public short wShowWindow;
            public short cbReserved2;
            public IntPtr lpReserved2;
            public IntPtr hStdInput;
            public IntPtr hStdOutput;
            public IntPtr hStdError;
        }

        // Initialize Struct for PROCESS_INFORMATION
        [StructLayout(LayoutKind.Sequential)]
        public struct PROCESS_INFORMATION
        {
            public IntPtr hProcess;
            public IntPtr hThread;
            public int dwProcessId;
            public int dwThreadId;
        }



    }
}
