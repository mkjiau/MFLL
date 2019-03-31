//
// File:	DefuzzSetObj.h
//
// Purpose:	This is the base class for the defuzzification sets  
//
// Copyright © 1999-2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//

#if !defined(AFX_DEFUZZSETOBJ_H__B251E1B3_3315_4D8B_9EF5_5C9B6052E4CF__INCLUDED_)
#define AFX_DEFUZZSETOBJ_H__B251E1B3_3315_4D8B_9EF5_5C9B6052E4CF__INCLUDED_


#include "FFLLBase.h"
class FuzzyOutSet;

//
// Class:	DefuzzSetObj
//
// Abstract base class for defuzzifcation set objects.
//

class DefuzzSetObj : virtual public FFLLBase  
{
	////////////////////////////////////////
	////////// Member Functions ////////////
	////////////////////////////////////////

	public:
		// constructor/destructor funcs
		DefuzzSetObj();// No function body for this. Explicitly disallow auto-creation of it by the compiler
		DefuzzSetObj(FuzzyOutSet* par);
		virtual ~DefuzzSetObj();
 
		// get functions
		FuzzyOutSet* get_parent() const ;
		virtual int get_defuzz_type() const = 0;

		// misc functions
 		virtual void calc(void) = 0;

}; // end class DefuzzSetObj

#else

class DefuzzSetObj; // already included

#endif // !defined(AFX_DEFUZZSETOBJ_H__B251E1B3_3315_4D8B_9EF5_5C9B6052E4CF__INCLUDED_)
