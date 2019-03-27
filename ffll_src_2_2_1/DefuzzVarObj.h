//
// File:	DefuzzVarObj.h
//
// Purpose:	This file contains the defuzzification objects
//
// Copyright © 2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//
 
#ifndef _DEFUZZVAROBJ_H
#define _DEFUZZVAROBJ_H
  
#include "FFLLBase.h"
class FuzzyOutVariable;


//
// Class:	DefuzzVarObj
//
// Abstract base class for defuzzifcation variable objects.
//

class DefuzzVarObj : virtual public FFLLBase  
{
	////////////////////////////////////////
	////////// Member Functions ////////////
	////////////////////////////////////////

	 public:

		// constructor/destructor funcs
		DefuzzVarObj(FuzzyOutVariable* _parent); 
		DefuzzVarObj(); // No function body for this. Explicitly disallow auto-creation of it by the compiler
		virtual ~DefuzzVarObj() ;  

		// get functions
		virtual int get_defuzz_type() const = 0;	

		// misc functions
 		virtual	RealType calc_value(DOMType* out_set_dom_arr ) = 0;

	protected:

		// get functions
		FuzzyOutVariable* get_parent() const;

	////////////////////////////////////////
	////////// Class Variables /////////////
	////////////////////////////////////////

	public:
		// types of defuzzification: Center of Gravity, Mean of Maximum 
		enum DEFUZZ_TYPE { DEFUZZ_COG, DEFUZZ_MOM };

}; // end class DefuzzVarObj
 
#else

class DefuzzVarObj;

#endif // _DEFUZZVAROBJ_H
