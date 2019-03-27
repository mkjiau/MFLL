//
// File:	MOMDefuzzVarObj.cpp	
//
// Purpose:	Variable object for Mean of Maximum Defuzzification method
//
// Copyright © 2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//
// 
#include "MOMDefuzzVarObj.h"
#include "MOMDefuzzSetObj.h"
#include "FuzzyOutSet.h"
#include "FuzzyOutVariable.h"
 
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

#include "debug.h"

#endif

//
// Function:	MOMDefuzzVarObj()
// 
// Purpose:		Constructor
//
// Arguments:
//
//		FuzzyOutVariable* par - variable that this is part of
//
// Returns:
//
//		none
//
// Author:	Michael Zarozinski
// Date:	6/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//

MOMDefuzzVarObj::MOMDefuzzVarObj(FuzzyOutVariable* _parent): DefuzzVarObj(_parent), FFLLBase(_parent)
{
	// nothing to do
}
//
// Function:	~MOMDefuzzVarObj()
// 
// Purpose:		Destructor
//
// Arguments:
//
//		none
//
// Returns:
//
//		none
//
// Author:	Michael Zarozinski
// Date:	6/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//

MOMDefuzzVarObj::~MOMDefuzzVarObj()
{
	// nothing to do
}


//
// Function:	calc_value()
// 
// Purpose:		Calculate the defuzzified output value (using the MOM 
//				defuzzification method) for the output variable.
//
// Arguments:	
//
//		DOMType* out_set_dom_arr -	Array that holds the DOM value for each
//									set in the output variable
//
// Returns:
//
//		RealType - the defuzzified output value. FLT_MIN is returned if no output sets are active
//
// Author:	Michael Zarozinski
// Date:	8/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
RealType MOMDefuzzVarObj::calc_value(DOMType* out_set_dom_arr  )
{
	FuzzyOutVariable*	parent;		// pointer to parent
	int					num_of_sets;// number of sets
 	int					i;			// counter
 	MOMDefuzzSetObj*	defuzz;		// defuzzification object for the set
	MOMDefuzzSetObj*	winning_defuzz = NULL;		//defuzzification object for the WINNING set
	FuzzyOutSet*		set;		// set to deal with
 	DOMType				mom_max;	// max MOM from sets
	DOMType				mom_idx;	// MOM index for the DOM
	int					set_idx = -1;// winning set idx

	parent  = get_parent();
	assert(parent);

	num_of_sets = parent->get_num_of_sets();

	mom_max = 0;

	// find the highest DOM for the output sets
	for (i = 0; i < num_of_sets; ++i)
		{
		set = parent->get_set(i);

		defuzz = dynamic_cast<MOMDefuzzSetObj*>(set->get_defuzz_obj());  

  		mom_idx = out_set_dom_arr[i];  
		if (mom_idx == 255)
			mom_idx = 0;
 
		// if mom_idx is greater, save it
		if (mom_max < mom_idx)
			{
			mom_max = mom_idx;
			winning_defuzz = defuzz;
			}

		} // end loop through sets

	if (!winning_defuzz)
		{
		// no output set value to FLT_MIN - the special value that
		// ensures we know that there is no output
 		return FLT_MIN;	// don't div by 0... just return
		}
 
	return (winning_defuzz->get_mean_value( ));

} // end MOMDefuzzVarObj::calc_value()


//
// Function:	get_set_defuzz_obj()
// 
// Purpose:		Return the defuzzification object of the correct type
//
// Arguments:
//
//		int set_idx - index of the set to get the defuzzifiation object for
//
// Returns:
//
//		MOMDefuzzSetObj* - defuzzification object for the set
//
// Author:	Michael Zarozinski
// Date:	8/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
MOMDefuzzSetObj* MOMDefuzzVarObj::get_set_defuzz_obj(int set_idx) const
{
	FuzzyOutVariable* parent = get_parent();
	FuzzyOutSet* set = parent->get_set(set_idx);

	MOMDefuzzSetObj* tmp_obj = dynamic_cast<MOMDefuzzSetObj*>(set->get_defuzz_obj()); 
	
	return tmp_obj;

} // end MOMDefuzzVarObj::get_set_defuzz_obj()

/////////////////////////////////////////////////////////////////////
////////// Trivial Functions That Don't Require Headers /////////////
/////////////////////////////////////////////////////////////////////

int MOMDefuzzVarObj::get_defuzz_type() const 
{ 
	return DefuzzVarObj::DEFUZZ_MOM; 
};
 