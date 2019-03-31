#ifndef _MFLLAPI_H
#define _MFLLAPI_H

#include <stddef.h>
#include <windows.h>

#ifdef _UNICODE
#	define ffll_get_msg_text		ffll_get_msg_textW
#else
#	define ffll_get_msg_text		ffll_get_msg_textA
#endif // unicode

#ifdef _WIN32
#	define	WIN_FFLL_API	__stdcall
#else
#	define	WIN_FFLL_API
#endif

extern "C"
{
	// MFLL APIs
	double WIN_FFLL_API MFLLFuzzyInference(LPSTR fcl_str, double* crisp_inputs, long input_size);
	double WIN_FFLL_API MFLLFuzzyInferenceByFile(LPSTR fcl_file, double* crisp_inputs, long input_size);
} 


#endif // _MFLLAPI_H