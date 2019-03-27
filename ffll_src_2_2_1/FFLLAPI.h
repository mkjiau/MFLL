//
// File:		FFLLAPI.h	
//
// Purpose:		This file contains the API declarations for FFLL
//
// Copyright © 2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//

#ifndef _FFLLAPI_H
#define _FFLLAPI_H

#include <stddef.h>

// Official API
// NOTE: we don't use __declspec(dllexport) to export, we use a .def file as that is
// the most generic way and avoids any name mangling (or decoration) via aliases
// (MSVC mangles names EVEN with 'extern "C"' if calling convention is __stdcall) and allows
// us to explicitly state the ordinal to avoid version conflicts in the future.

// define which version's we call if _UNICODE

#ifdef _UNICODE
#	define ffll_get_msg_text		ffll_get_msg_textW
#else
#	define ffll_get_msg_text		ffll_get_msg_textA
#endif // unicode

// define how we export the functions
#ifdef _WIN32
#	define	WIN_FFLL_API	__stdcall
#else
#	define	WIN_FFLL_API
#endif

extern "C"
{
// API for creating a FFLL model

int WIN_FFLL_API ffll_new_model() ;
int WIN_FFLL_API ffll_close_model(int model_idx) ;
int WIN_FFLL_API ffll_new_child(int model_idx) ;
int WIN_FFLL_API ffll_load_fcl_file(int model_idx, const char* file); 

 
const wchar_t* WIN_FFLL_API ffll_get_msg_textW(int model_idx);
const char* WIN_FFLL_API ffll_get_msg_textA(int model_idx);

// thread specific functions...

int WIN_FFLL_API ffll_set_value(int model_idx, int child_idx, int var_idx, double value);
double WIN_FFLL_API ffll_get_output_value(int model_idx, int child_idx);

} // end extern "C" for FFLL api
  
#endif // _FFLLAPI_H