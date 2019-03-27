//
// File:	FFLLBase.cpp
//
// Purpose:	Base class for all FFLL files.
//
// Copyright ?1998-2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//

#include "FFLLBase.h"
#include "FuzzyVariableBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

#include "debug.h"

#endif
 
// define literals, warning and error messages  

wchar_t* literals[] = 
	{ 
	L"New",
	L"Variable",
	L"Set",
	L"Output",
	L"Copy of"
	};

wchar_t* errors[] = 
	{ 
	L"Variable Identifier Must Be Unique",
	L"Set Identifier Must Be Unique Within The Variable",
	L"The Variable's Minimum And Maximum Values Can Not Be The Same",
	L"Invalid File Format",
	L"Can Not Remove Output Variable",
	L"Reached End Of File Before Reading Variables",
	L"Reached End Of File Before Reading Rules",
	L"Reached End Of File Before Reading Sets",
	L"Model Already Has An Output Variable",
	L"Error Allocating Memory",
	L"Invalid Defuzzification Method",       
	L"Invalid Composition Method",
	L"Invalid Inference Method",
	L"Error Opening File",
	L"Error Reading Variable Minimum Value",
	L"Error Reading Variable Maximum Value"
	};
wchar_t* warnings[] = 
	{ 
	NULL
	};
 
//
// Function:	FFLLBase()
// 
// Purpose:		FFLLBase constructor
//
// Arguments:
//
//		void* parent -	pointer to the parent of this object. For example if this is the
//						base for a variable, the parent will be the model.
//
// Returns:
//
//		nothing
//
// Author:	Michael Zarozinski
// Date:	5/00
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
FFLLBase::FFLLBase(void* _parent)
{
	parent = _parent;
	error_read = false;

}; // end FFLLBase::FFLLBase()
  
//
// Function:	convert_to_ascii()
// 
// Purpose:	Utility function to convert from wide characters to ascii
//			characters. The caller MUST free the memory allocated
//
// Arguments:
//
//		const wchar_t*	wstr			-	wide character string to convert
//		const char*		replace_space	 -	string to replace spaces with (optional)
//
// Returns:
//
//		char* - converted string
//
// Author:	Michael Zarozinski
// Date:	9/01/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
char* convert_to_ascii(const wchar_t* wstr, char replace_space /* = -1 */)
{
#ifdef TEST_MEM_LEAK_REPORTING
	char *mem_leak = new char[33];
#endif
	if (wstr == NULL)
		return NULL;

 	// get the ascii version of the id...
	char* astr = new char[wcslen(wstr) + 10];

 	sprintf(astr, "%S", wstr);

	// if a 'replace_space' character was passed, replace any spaces with that char
	// replace any spaces with underscores

	if (replace_space >= 0)
		{
		int len = strlen(astr);

		while (len)
			{
			if (astr[len] == ' ')
				astr[len] = replace_space;
			len--;
			}
		} // end if replace space

	return astr;

} // end convert_to_ascii()
 
  
//
// Function:	convert_to_wide_char()
// 
// Purpose:	Utility function to convert from ascii to wide characters
//			The caller MUST free the memory allocated
//
// Arguments:
//
//		const char*	wstr -	ascii string to convert
//
// Returns:
//
//		wchar_t* - converted string
//
// Author:	Michael Zarozinski
// Date:	9/01/01
// 
// Modification History
// Author		Date		Modification
// ------		----		------------
// Michael Z	6/17/02		modified to use mbstwcs() rather than swprintf() cuz of
//							problems with compiling on some systems with glib.
//
wchar_t* convert_to_wide_char(const char* astr)
{
	if (astr == NULL)
		return NULL;

 	// get the ascii version of the id...
	int wchar_len = strlen(astr) + 1;
	wchar_t* wstr = new wchar_t[wchar_len];

 	mbstowcs(wstr, astr, wchar_len);
  
	return wstr;

} // end convert_to_wide_char()

//
// Function:	get_msg_text()
// 
// Purpose:		Return the msg_text variable.
//
// Arguments:
//
//		none
//
// Returns:
//
//		const wchar_t* - the message text
//
// Author:	Michael Zarozinski
// Date:	8/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
const wchar_t* FFLLBase::get_msg_text() const
{
	// since we're returning a pointer to the string in msg_text we can't just
	// clear out msg_text. So we use the "read" flag that indicates
	// that this error has been read at some point.

	// if we've already read this error, return null
	if (error_read)
		return NULL;

	error_read = true;
 	return((msg_text == L"") ? NULL : msg_text.c_str());

}; // end FFLLBase::get_msg_text()

//
// Function:	set_msg_text()
// 
// Purpose:		Set the msg_text string so a human-readable error can be reported.
//
// Arguments:
//
//		int msg_id -	identifier of the message. This is converted to text via the
//						load_string() function.
//
// Returns:
//
//		void
//
// Author:	Michael Zarozinski
// Date:	8/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
void FFLLBase::set_msg_text(int msg_id) const
{
 	// vars are mutable so we can modify them event though this func is 'const'
	error_read = false;
	msg_text = load_string(msg_id);

}; // end FFLLBase::set_msg_text()



//
// Function:	set_msg_text()
// 
// Purpose:		Set the msg_text string so a human-readable error can be reported.
//
// Arguments:
//
//		const std::wstring _text - text to set msg_text to
//
// Returns:
//
//		void
//
// Author:	Michael Zarozinski
// Date:	8/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//	

void FFLLBase::set_msg_text(const std::wstring _text) const
{
	set_msg_text(_text.c_str());

}; // end FFLLBase::set_msg_text()


//
// Function:	set_msg_text()
// 
// Purpose:		Set the msg_text string so a human-readable error can be reported.
//
// Arguments:
//
//		const wchar_t* - text to set msg_text to
//
// Returns:
//
//		void
//
// Author:	Michael Zarozinski
// Date:	8/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//	

void FFLLBase::set_msg_text(const wchar_t* _text /* = NULL */) const
{
	// NOTE: Class vars are mutable so we can modify them event though this func is 'const'
	
	// if text is null, we're clearing out any pre-existing error
	if (_text == NULL)
		{
		// set error_read to true so we just get NULL returned if we call get_msg_text()
		error_read = true;
 		msg_text = L"";
		}
 	else
		{
		error_read = false;	
		msg_text = _text;
		}

} // end FFLLBase::set_msg_text()

  

//
// Function:	load_string()
// 
// Purpose:	this function converts a #define to a string.  It's nothing special right now
//			but provides a framework that can be expanded in the future
//
// Arguments:
//
//		int str_id	-	identifier for the string to load
//
// Returns:
//
//		const wchar_t* - string associated with the id passed in
//
// Author:	Michael Zarozinski
// Date:	9/01/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
const wchar_t* FFLLBase::load_string(int str_id) const
{
	// convert to an index....

	if (str_id >= WARNING_BASE)
		{
		str_id -= WARNING_BASE;

		// make sure we don't go beyond the bounds...

		if (str_id >= sizeof(warnings))
			{
			assert(0);
			return L"Message Not Defined";
			}
		else
			return warnings[str_id];
		}

 	if (str_id >= ERROR_BASE)
		{
		str_id -= ERROR_BASE;

		// make sure we don't go beyond the bounds...

		if (str_id >= sizeof(errors))
			{
			assert(0);
			return L"Message Not Defined";
			}
		else
			return errors[str_id];
		}
 
	if (str_id >= LITERAL_BASE)
		{
		str_id -= LITERAL_BASE;

		// make sure we don't go beyond the bounds...

		if (str_id >= sizeof(literals))
			{
			assert(0);
			return L"Message Not Defined";
			}
		else
			return literals[str_id];
		}
 
	assert(0);
	return L"Message Not Defined";

} // FFLLBase::load_string()

/////////////////////////////////////////////////////////////////////
////////// Trivial Functions That Don't Require Headers /////////////
/////////////////////////////////////////////////////////////////////

void* FFLLBase::get_parent() const
{
	// any func that calls this needs to cast to the correct object otherwize
	// the compiler gets very confused
	return(parent);

}; // end FFLLBase::get_parent()
 		
NodeValue::operator int()
{
	return value;
};
int& NodeValue::operator+=(const int& _value)
{
	value += _value; 
	return value;
};
int& NodeValue::operator-=(const int& _value)
{ 
	value -= _value; 
	return value;
};
void NodeValue::operator<<(const int& _value)
{ 
	value = _value;
};
int& NodeValue::operator=(const int& _value)
{
	value = _value; 
	validate();		// virtual func
	return value;
};

// XNodeValue & YNodeValue use their validate() function to make sure values are
// within the appropriate bounds.  The validiation is built-in so we don't have to
// validate the values every time - that would be a pain.
 
int& XNodeValue::operator=(const int& _value)
{
	return NodeValue::operator =(_value);
};
 
void XNodeValue::validate()
{
	if (value < 0)
		value = 0;
	if (value >= FuzzyVariableBase::get_x_array_count())
		value = FuzzyVariableBase::get_x_array_max_idx();
};


int& YNodeValue::operator=(const int& _value)
{
	return NodeValue::operator =(_value);
};

void YNodeValue::validate()
{
	if (value < 0)
		value = 0;
	if (value >= FuzzyVariableBase::get_dom_array_count())
		value = FuzzyVariableBase::get_dom_array_max_idx();
};





static char sDecimalSeparator = '.'; // dot by default
static char sThousantSeparator =','; // comma by default

// call this to format a long to a string
static char*  long2str( char* szBuffer, long lValue )
{
  char *p;                // pointer to traverse string
  char *firstdig;         // pointer to first digit
  char temp;              // temp char
  unsigned digval;        // value of digit
  unsigned long val;

  p = szBuffer;

  if (lValue < 0 ) {
      // negative, so output '-' and negate
     *p++ = '-';
     val = (unsigned long)(-(long)lValue); // make it positive!!
  } else
     val = lValue;

  firstdig = p;    // save pointer to first digit

  int iDecimalPos = 0;

  do {
     iDecimalPos ++;
     if (iDecimalPos != 4)
     {
       digval = (unsigned) (val % 10);
       val /= 10;                     // get next digit
       *p++ = (char) (digval + '0');  // and store the digit
     } else
     {
       *p++ = sThousantSeparator;
       iDecimalPos = 0;
     }
  } while (val > 0);

  //  We now have the digit of the number in the buffer, 
  // but in reverse order.  Thus we reverse them now.

  *p-- = '\0';    // terminate string; p points to last digit

  do {
      temp = *p;
      *p = *firstdig;
      *firstdig = temp;   // swap *p and *firstdig
      --p;
      ++firstdig;         
  } while (firstdig < p); // repeat until halfway

  return szBuffer;
}

