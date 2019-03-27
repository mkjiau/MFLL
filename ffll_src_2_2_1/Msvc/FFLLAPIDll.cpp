// FFLLAPIDLL.cpp : Defines the entry point for the DLL application.
//

#include "../debug.h"

// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>
 
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
	
        int tmpDbgFlag;

        /*
         * Set the debug-heap flag to keep freed blocks in the
         * heap's linked list - This will allow us to catch any
         * inadvertent use of freed memory
         */
        tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
         tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
	//	tmpDbgFlag |= _CRTDBG_CHECK_ALWAYS_DF;
    //   tmpDbgFlag |= _CRTDBG_DELAY_FREE_MEM_DF;
         _CrtSetDbgFlag(tmpDbgFlag);
		 break;

		case DLL_THREAD_ATTACH:
			break;
 		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
		// If you call "dump" here you get a bunch of false memory leaks. Best
		// to let it be called automatically at program termination (by turning 
		// on the _CRTDBG_LEAK_CHECK_DF flag above)

  		//	 _CrtDumpMemoryLeaks();
			break;
		
		}
    return TRUE;
}
