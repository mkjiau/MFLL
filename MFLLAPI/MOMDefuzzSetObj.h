//
// File:	MOMDefuzzSetObj.h
//
// Purpose:	Set class for Mean of Maximum defuzzification Method.
// 			See MOMDefuzzVarObj for details on how this method works.
//
// Copyright © 2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//

#ifndef _MOMDefuzzSetObj_H
#define _MOMDefuzzSetObj_H

#include "DefuzzVarObj.h"
#include "DefuzzSetObj.h"
class FuzzyOutSet;

//
// Class:	MOMDefuzzSetObj
//
// Mean of Maximum defuzzifcation set object. This object holds the index of the
// set for this method. See MOMDefuzzVarObj.h for details on this defuzz method.
//
class   MOMDefuzzSetObj : public DefuzzSetObj
{
	////////////////////////////////////////
	////////// Member Functions ////////////
	////////////////////////////////////////

	public:
		// constructor/destructor funcs
		MOMDefuzzSetObj();// No function body for this. Explicitly disallow auto-creation of it by the compiler
		MOMDefuzzSetObj(FuzzyOutSet* par) ;
 		virtual ~MOMDefuzzSetObj(); 

 		// get functions
		RealType get_mean_value() const;
 		int get_defuzz_type() const;

		// set functions
		void set_mean_value(RealType mean);
 
		// misc functions
 		void calc(void);

	////////////////////////////////////////
	////////// Class Variables /////////////
	////////////////////////////////////////
	
	private:

  		RealType mean_value;	// 'x' value for the mean

}; // end class MOMDefuzzSetObj
 
#else

class MOMDefuzzSetObj;
  
#endif // _MOMDefuzzSetObj_H
