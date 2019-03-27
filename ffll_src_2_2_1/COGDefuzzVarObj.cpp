//
// File:	COGDefuzzVarObj.h	
//
// Purpose:	Variable object for Center of Gravity Defuzzification method
//
// Copyright © 2001 Louder Than A Bomb! Software
//
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//


#include "COGDefuzzVarObj.h"
#include "COGDefuzzSetObj.h"
#include "FuzzyOutSet.h"
#include "FuzzyOutVariable.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

#include "debug.h"
 
#endif
//
// Function:	COGDefuzzVarObj()
// 
// Purpose:		Constructor for COGDefuzzVarObj
//
// Arguments:	
//
//		FuzzyOutVariable* _parent - Output variable this object is part of
//
// Returns:
//
//		nothing
//
// Author:	Michael Zarozinski
// Date:	8/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
COGDefuzzVarObj::COGDefuzzVarObj(FuzzyOutVariable* _parent) : DefuzzVarObj(_parent), FFLLBase(_parent)
{

}


//
// Function:	~COGDefuzzVarObj()
// 
// Purpose:		Destructor for COGDefuzzVarObj
//
// Arguments:	
//
//		none
//
// Returns:
//
//		nothing
//
// Author:	Michael Zarozinski
// Date:	8/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
COGDefuzzVarObj::~COGDefuzzVarObj()
{

}


//
// Function:	calc_value()
// 
// Purpose:		Calculate the defuzzified output value (using the COG 
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
RealType COGDefuzzVarObj::calc_value(DOMType* out_set_dom_arr  )
{
	RealType area_sum = 0.0;		// sum of the area
	RealType moment_sum = 0.0;		// sum of the moments
	RealType divisor = 0.0;		// value to divide cog_sum by
	RealType tmp_area = 0.0;		// tmp var  
	COGDefuzzSetObj*	defuzz;	// defuzzification object for the set
 	int	 cog_idx;				// center of gravity index
 
	FuzzyOutVariable* parent = get_parent();

	int num_of_sets = parent->get_num_of_sets();

	// each set has it's individual COG components (area and moment) calculated
	// for every possible DOM. These calculations treat each set as a point mass.
	// Now we have multiple sets to find the COG for, with each treated as a
	// point mass, we can calculate the COG via the simple formula:
	//
	//		Sum of Moments
	//		---------------
	//		Sum of Mass (Note: mass is same as area)
	//
	// See COGDefuzzVarObj.h for a more detailed expanation of this formula and 
	// how we use it.

	for (int i = 0; i < num_of_sets; ++i)
		{
 		defuzz = get_set_defuzz_obj(i);  
	
		if (defuzz == NULL)
			continue;	// nothing to calc for this set

 		cog_idx = out_set_dom_arr[i]; 
		
		if (cog_idx == 255)
			cog_idx = 0;
	 
		tmp_area =  defuzz->get_area(cog_idx); 

		if (tmp_area) 
			{
			area_sum += tmp_area;
			moment_sum += defuzz->get_moment(cog_idx);  
			divisor++;
			}

		} // end loop thru sets

	if (!divisor)
		{
		// if no output sets were active, return FLT_MIN - the special value that
		// ensures we know that there is no output
 
		return FLT_MIN;	// return so we don't div by 0 below

		} // end if no divisor

	RealType left_x = parent->get_left_x();

	// be sure to account for the left x (start of the var)
	return (left_x + (moment_sum / area_sum));

} // end COGDefuzzVarObj::calc_value()


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
//		COGDefuzzSetObj* - defuzzification object for the set
//
// Author:	Michael Zarozinski
// Date:	8/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//		
COGDefuzzSetObj* COGDefuzzVarObj::get_set_defuzz_obj(int set_idx) const
{
	FuzzyOutVariable* parent = get_parent();
	FuzzyOutSet* set = parent->get_set(set_idx);

	COGDefuzzSetObj* tmp_obj = dynamic_cast<COGDefuzzSetObj*>(set->get_defuzz_obj()); 
	
	return tmp_obj;

} // end COGDefuzzVarObj::get_set_defuzz_obj()

/////////////////////////////////////////////////////////////////////
////////// Trivial Functions That Don't Require Headers /////////////
/////////////////////////////////////////////////////////////////////

int COGDefuzzVarObj::get_defuzz_type() const
{ 
	return DefuzzVarObj::DEFUZZ_COG; 
};

 