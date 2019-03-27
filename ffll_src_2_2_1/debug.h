//
// File:		debug.h	
//
// Purpose:		This file has macros to report mem leaks and other debugging stuff
//				that's needed in all files
//
// Copyright © 2000-2001 Louder Than A Bomb! Software
//

// ??? TODO: un-comment this so we get CRT mem leak reporting in debug mode
 
/*
 
#ifdef _DEBUG

// fix for debug NEW macros as defined in "PRB: _CRTDBG_MAP_ALLOC Does Not Work as Documented"
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define MYDEBUG_NEW   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new MYDEBUG_NEW
 
#endif // _DEBUG

 */
