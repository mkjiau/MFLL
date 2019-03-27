//
// File:	MOMDefuzzVarObj.h
//
// Purpose:	Variable class for Mean of Maximum defuzzification Method.
//
// Copyright © 2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//


#if !defined(AFX_MOMDefuzzVarObj_H__23D883BE_7100_4E65_BA78_7CAA820A6B69__INCLUDED_)
#define AFX_MOMDefuzzVarObj_H__23D883BE_7100_4E65_BA78_7CAA820A6B69__INCLUDED_

#include "DefuzzVarObj.h"
 
class FuzzyOutVariable;
class MOMDefuzzSetObj;

// 
// Class:	MOMDefuzzVarObj
//
// Variable object for the Mean of Maximum Defuzzification Method
//
// This defuzzification method returns the "MoM" of the set
// with the highest DOM.  The Mean of Max refers to the max
// of the member function's nodes.  For example, in a trianle
// member func the max is always the top point in the triangle,
// so that's the 'x' position returned.  If we're dealing with a 
// trapezoid term that has 2 nodes at the highest DOM, we take
// the average of them (the mean) and use that as the 'x' value.


class MOMDefuzzVarObj : public DefuzzVarObj  
{
	////////////////////////////////////////
	////////// Member Functions ////////////
	////////////////////////////////////////

	public:
		// constructor/destructor funcs
		MOMDefuzzVarObj();// No function body for this. Explicitly disallow auto-creation of it by the compiler
		MOMDefuzzVarObj(FuzzyOutVariable* _parent);
		virtual ~MOMDefuzzVarObj();

		// get functions
		int get_defuzz_type() const ;

		// misc functions
 		RealType calc_value(DOMType* out_set_dom_arr );
 
	protected:

 		// get functions
		MOMDefuzzSetObj* get_set_defuzz_obj(int set_idx) const;

}; // end class MOMDefuzzVarObj

#else

class MOMDefuzzVarObj;

#endif // !defined(AFX_MOMDefuzzVarObj_H__23D883BE_7100_4E65_BA78_7CAA820A6B69__INCLUDED_)
