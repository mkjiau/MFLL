//
// File:		FFLLBase.h	
//
// Purpose:		This file contains the common classes/structs/etc for FFLL
//
// Copyright © 2000-2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//


// The debugger can't handle symbols more than 255 characters long.
// STL often creates symbols longer than that.
// When symbols are longer than 255 characters, the warning is disabled.
#pragma warning(disable:4786)
// disable "did not inline" warnings 
#pragma warning(disable:4710) 


#ifndef _FFLLBASE_H
#define _FFLLBASE_H



// define the numbers that identify warning and error messages 
// each class of message has a base offsets.  We subtract
// this value from the value to get the index into the approprite array
 
// array of text
#define LITERAL_BASE	0
#define STR_NEW			LITERAL_BASE + 0
#define STR_VARIABLE	LITERAL_BASE + 1
#define STR_SET			LITERAL_BASE + 2
#define STR_OUTPUT		LITERAL_BASE + 3
#define STR_COPY_OF		LITERAL_BASE + 4


#define ERROR_BASE					2000
#define ERR_VAR_NON_UNIQUE_ID		ERROR_BASE + 0
#define ERR_SET_NON_UNIQUE_ID		ERROR_BASE + 1
#define ERR_SAME_LEFT_RIGHT_VALS	ERROR_BASE + 2
#define ERR_INVALID_FILE_FORMAT		ERROR_BASE + 3
#define ERR_CANT_DEL_OUTPUT_VAR		ERROR_BASE + 4
#define ERR_EOF_READING_VARS		ERROR_BASE + 5
#define ERR_EOF_READING_RULES		ERROR_BASE + 6
#define ERR_EOF_READING_SETS		ERROR_BASE + 7
#define ERR_OUT_VAR_EXISTS			ERROR_BASE + 8
#define ERR_ALLOC_MEM				ERROR_BASE + 9
#define ERR_INVALID_DEFUZZ_MTHD		ERROR_BASE + 10
#define ERR_INVALID_COMP_MTHD		ERROR_BASE + 11
#define ERR_INVALID_INFERENCE_MTHD	ERROR_BASE + 12
#define ERR_OPENING_FILE			ERROR_BASE + 13
#define ERR_VAR_MIN_VALUE			ERROR_BASE + 14
#define ERR_VAR_MAX_VALUE			ERROR_BASE + 15


#define WARNING_BASE				4000

 
 
// if FFLL is in a DLL we do not want to have inline functions.  They would
// require the program using the library to be re linked.  Without inlines
// you can just drop in the new dll and everything will work (assuming you
// only changed the function's implementation and not it's declaration)
// Note that we avoid inlining virtual functions cuz "it’s best not to use 
// inline virtual functions, since they’re almost never expanded anyway." 
// according to http://msdn.microsoft.com/msdnmag/issues/0600/c/c0600.asp
//
// NOTE: That inlining for FFLL doesn't work when using Microsoft MSVC
// because we define the functions in the .cpp file and MSVC does not like that
#ifdef USE_INLINE
#	define FFLL_INLINE inline
#else
#	define FFLL_INLINE 
#endif 

#include <wchar.h>
#include <string>
#include <assert.h>
#include <float.h> // needed for FLT_MIN
#include <fstream>

// added #define for more compatiblity (Michael Z 4/03)...
#if defined (__MSVCRT__) || defined(_WINDOWS)
#define   swprintf   _snwprintf
#endif

// THIS version must be used by any caller outside this DLL otherwize we get
// mem deallocation errors (in debug mode)
// FFLL_API int convert_to_ascii(const wchar_t* wstr, char* astr, char replace_space = -1 );

// these 2 are NOT exported!
char* convert_to_ascii(const wchar_t* wstr, char replace_space = -1 );
wchar_t* convert_to_wide_char(const char* astr);

namespace FFLLDatatypes 
{
const int OUTPUT_IDX = -1; // constant for output variable

// We're using unsigned char for the rules array cuz it's the smallest we can (easily) implement right now
// we could shrink the memory footprint in 1/2 if we used bytes to store the info.  This would limit
// us to 16 output sets... probably a reasonable amount but may be to limited.
//typedef int RuleArrayType;
typedef unsigned char  RuleArrayType;

// using float for the variable's left/right values can cause a loss of precision so we'll typedef
// a datatype...
typedef double RealType;
 
// ??? changing cuz of change from unsiggned short to int.

const RuleArrayType NO_RULE = 255;	// 255 is the MAX that an unsigned char can hold - so this means we're
//									// limited to 255 set in the output variable... a limitation I believe
									// we can live with
//const RuleArrayType NO_RULE = INT_MAX;	// 255 is the MAX that an unsigned char can hold - so this means we're
									// limited to 255 set in the output variable... a limitation I believe
									// we can live with

// Typedef for the number of elements in the values[] array
typedef int ValuesArrCountType;
///typedef unsigned short ValuesArrCountType;

// The DOM (Degree of Membership) is a value between 0 and MAX_DOM.  we could
// have used a float and represened (as most FL systems do) the value from 0 to 1
// but that would just be a waste of memory.  Using an unsigned char gives us
// up to 256 values in only 8 bits.  Not too bad!  A Float would cost us (at least)
// 4 bytes!
//typedef unsigned char DOMType;
typedef int DOMType;
 	
// create classes for the 'x' and 'y' node points,
// this allows us to overload the '=' operator and EASILY
// perform sanity checks.

// NOTE: The assignment operator has some additional restrictions. 
// It can be overloaded only as a nonstatic member function, not 
// as a friend function. It is the only operator that cannot be 
// inherited; a derived class cannot use a base class's assignment operator. 

class  NodeValue
{
	public:
		operator int();
 		int& operator=(const int& _value);
		int& operator+=(const int& _value);
		int& operator-=(const int& _value);
		void operator<<(const int& _value);

	private:
 		virtual void validate() = 0;

	protected:
	 		int value;

}; // end class NodeValue

class  XNodeValue : public NodeValue
{
	public:
 		int& operator=(const int& _value); // can't be inherited

	private:
		void validate();

}; // end class XNodeValue

class  YNodeValue : public NodeValue
{
	public:
 		int& operator=(const int& _value); // can't be inherited

	private:
		void validate();

}; // end class YNodeValue

class  NodePoint 
{
	public:
		XNodeValue x;
		YNodeValue y;
	
}; // end class NodePoint

 
// define our own point struct to use rather than rely on
// something like POINT that requires windef.h
typedef struct _point_struct
{
	int x;
	int y;
} _point;
 
}; // end namespace FFLLDatatypes

using namespace FFLLDatatypes ;
 
// Class:  FFLLBase
//
// This is the base class for all FFLL classes. It stores the parent of the object
// so we can get the parent to go up the model hierarchy and holds a string that contains
// an error or warning message.
// Note:  Since each object has it's own msg_text string, to report an error you need to
// propagate it up the classes.  For example if an error occurs in a set the calling
// function should detect the error and set the msg_text for the set's variable, then the caller
// for the variable object should set it for the model object - until it's shown to the user.
//

class  FFLLBase
{

	public:
 
		FFLLBase();	// This has NO body. This (DEFAULT constructor) should not be called so we're explicitly disallowing it
		FFLLBase(void* _parent);
 
 		const wchar_t* get_msg_text() const;
 
		void set_msg_text(int msg_id) const;
		void set_msg_text(const std::wstring _text) const;
		void set_msg_text(const wchar_t* _text = NULL) const;

	protected:

		void* get_parent() const;
		const wchar_t* load_string(int str_id) const;

	private:

		// vars are 'mutable' so we can write an error in a func that's declared as 'const'.
		// This enables us to be able to write out errors when appropriate and still declare
		// funcs as 'const' who wouldn't modify anything in the object if no error occured
		mutable bool			error_read;	// indicates if the current error has been read or not.  If it's been
											// read, we can assume it's been handled.
		mutable std::wstring	msg_text;	// error or warning message
		void*					parent;		// points to the parent. 

}; // end class FFLLBase
 
#endif // _FFLLBASE_H
