//
// File:	COGDefuzzSetObj.h
//
// Purpose:	Mean of Maximum defuzzification Method.
//
// Copyright © 2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//

#ifndef _COGDefuzzSetObj_H
#define _COGDefuzzSetObj_H

#include "DefuzzVarObj.h"
#include "DefuzzSetObj.h"
class FuzzyOutSet;
 
// 
// Class:	COGDefuzzSetObj
//
// Defuzz set object for Center of Gravity defuzz method.
// See COGDefuzzVarObj.h for details on this defuzz method
//

class   COGDefuzzSetObj : public DefuzzSetObj
{
	////////////////////////////////////////
	////////// Member Functions ////////////
	////////////////////////////////////////

	public:
		// constructor/destructor funcs
		COGDefuzzSetObj();// No function body for this. Explicitly disallow auto-creation of it by the compiler
		COGDefuzzSetObj(FuzzyOutSet* par) ;
		virtual ~COGDefuzzSetObj(); 
		int init(int array_max);

		// get functions
		int get_defuzz_type() const;
 		RealType get_area(int _idx) const;
		RealType get_moment(int _idx) const;
		RealType get_defuzz_x(int dom);

		// set functions
 		void set_area(int _idx, RealType val);
		void set_moment(int _idx, RealType val);
 
		// misc functions
		void calc(void);

	////////////////////////////////////////
	////////// Class Variables /////////////
	////////////////////////////////////////
	private:

		// struct to hold COG values for each set.  The set will have
		// an array of these, one for each DOM of the set.  We pre-calc
		// the COG values for each DOM.  This greatly speeds the process
		// cuz all we need to do is know the 'x' value for this set and
		// we have the COG for the set already calculated.

		typedef struct _cog_struct_
			{
 			RealType			area;	// area for this COG
			RealType			moment;	// area * cog
			} _cog_struct;

	protected:

		_cog_struct* values;	// array of COG values
	


}; // end class COGDefuzzSetObj
 
#else

class COGDefuzzSetObj;
  
#endif // _COGDefuzzSetObj_H
