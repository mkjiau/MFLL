//
// File:	COGDefuzzSetObj.cpp	
//
// Purpose:	Set object for Center of Gravity Defuzzification method
//
// Copyright © 2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//

// include files
#include "COGDefuzzSetObj.h"
#include "FuzzyOutSet.h"
#include "FuzzyOutVariable.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

#include "debug.h"
 
#endif

//
// Function:	COGDefuzzSetObj()
// 
// Purpose:		Constructor
//
// Arguments:
//
//		FuzzyOutSet* par - set that this is part of
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

COGDefuzzSetObj::COGDefuzzSetObj(FuzzyOutSet* par)
: DefuzzSetObj(par), FFLLBase(par)
{
	values = NULL;

}; // end COGDefuzzSetObj::COGDefuzzSetObj()

//
// Function:	~COGDefuzzSetObj()
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

COGDefuzzSetObj::~COGDefuzzSetObj()
{
	if (values)
		delete[] values;

}; // end COGDefuzzSetObj::~COGDefuzzSetObj()

//
// Function:	init()
// 
// Purpose:		Intializes the look up table
//
// Arguments:
//
//		int array_max - how many elements are in the look up table
//
// Returns:
//
//		0 - success
//		non-zero - failure
//
// Author:	Michael Zarozinski
// Date:	6/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//

int COGDefuzzSetObj::init(int array_max)
{
	values = new _cog_struct[array_max];

	if (values == NULL)
		{
 		set_msg_text(ERR_ALLOC_MEM);
		assert(values != NULL);
		return -1;
		} // end if error allocating memory
 
	return 0;

}; // end COGDefuzzSetObj::init()



//
// Function:	calc()
// 
// Purpose:		Calculates the cog under the curve for EVERY Degree of Membership (DOM),
//				creating a lookup table.
//
// Arguments:	
//
//		void
//
// Returns:
//
//		void
//
// Author:	Michael Zarozinski
// Date:	8/01
// 
// Modification History
// Author		Date		Modification
// ------		----		------------
// Michael Z	4/03		removed adding min_x to set_moment()	
//
 

void COGDefuzzSetObj::calc()
{
	int dom_idx, x_idx;		// counters
	int start_idx, end_idx;	// the indexes that start/end the membership
							// function in the values array.
	int	y;					// holds 'y' value for the curve (refered to as DOM
							// everywhere else but that gets confising here cuz
							// we're looping through the doms
	RealType area_sum;		// area sum 
	RealType area;			// area 
	RealType moment_sum;	// sum of the moments

	// get the min val and idx_multiplier for the var...

	FuzzyOutSet* set_base = get_parent();

	RealType idx_mult = set_base->get_idx_multiplier();
	RealType min_x = set_base->get_left_x();
 
	start_idx = set_base->get_start_x();
	end_idx = set_base->get_end_x();

	// 
	// To calc the COG for each DOM, we treat each index in the values[] 
	// array as a point mass.  Where the mass is equal to the area.
	// Remember Calc 1 where you treated each "segment" of the curve
	// you were integrating as a rectangle?  That's what we're doing here,
	// however when we calculate the HEIGHT of the rectangle we're taking
	// the LESSER of the dom_idx or the value in the values[] array.
	//
	//
	// MAX_DOM                       X
	//                              X X
	//                             X   X
	//                            X     X  
	//                           X       X
	//                          X         X 
	//                         X           X
	//                        X             X
	// dom_idx ----------------------------------------
	//                       XXXXXXXXXXXXXXXXX
	//                      XXXXXXXXXXXXXXXXXXX
	//                     XXXXXXXXXXXXXXXXXXXXX
	// DOM 0              XXXXXXXXXXXXXXXXXXXXXXX
	//                    |                     |
 	//                 start_idx            end_idx
	//

	// loop through all the doms
	for (dom_idx = 0; dom_idx < FuzzyVariableBase::get_dom_array_count(); dom_idx++)
		{
		// init area and moment for this dom
		area_sum = 0.0;
		moment_sum = 0.0;

		// go through the DOMs for the curve 
		for (x_idx = start_idx; x_idx <= end_idx; x_idx++)
			{
			// get the DOM for this 'x' value
			y = set_base->get_dom(x_idx);

			// use the LESSER of the 'y' value for this 'x' position or
			// the dom we're checking

 			if (y > dom_idx)
				 area = dom_idx;
			else
				 area = y;

			// add the area to the accumulator
 			area_sum += area;

			// add the moment to the accumulator
			moment_sum += (RealType)x_idx * area;

			} // end loop through curve

		set_area(dom_idx, area_sum);
 
		// since the moment invovles the distance from 0 and we use the x_idx
		// above in our calcs, we need to multiply by the index multiplier
		// to convert an index to a 'x' value.  Note: we could have multiplied
		// above when we calculated each moment but it's faster to just perform
		// ONE multiplication here
		// 4/03 - removing min_x in set_moment, this was causing slight output inconsistencies
		
		set_moment(dom_idx, (moment_sum * idx_mult ));

		} // end loop through DOM

} // end COGDefuzzSetObj::calc()

//
// Function:	get_defuzz_x()
// 
// Purpose:		This fuction gets the defuzzified 'x' value for a DOM passed in.
//
// Arguments:
//
//		int		dom - 'y' value to get the 'x' value for  
//
// Returns:
//
//		RealType - the defuzzified 'x' value for the set
//
// Author:	Michael Zarozinski
// Date:	12/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//	
RealType COGDefuzzSetObj::get_defuzz_x(int dom )
{
  
	if (dom < 0)
		{
		return FLT_MIN;
		}

	RealType area = get_area(dom); 

	if (!area)
		{
		return FLT_MIN;
		}

	RealType  moment =  get_moment(dom);  

	FuzzyOutSet* set_base = get_parent();
 	FuzzyOutVariable* parent = set_base->get_parent();

	RealType left_x = parent->get_left_x();

	// be sure to account for the left x (start of the var)
	return (left_x + (moment / area));


} // end COGDefuzzSetObj::get_defuzz_x()

/////////////////////////////////////////////////////////////////////
////////// Trivial Functions That Don't Require Headers /////////////
/////////////////////////////////////////////////////////////////////  

RealType COGDefuzzSetObj::get_area(int _idx) const
{
	return values[_idx].area;
};
RealType COGDefuzzSetObj::get_moment(int _idx) const
{
	return values[_idx].moment;
};
void COGDefuzzSetObj::set_area(int _idx, RealType val)
{  
	values[_idx].area = val;
};
void COGDefuzzSetObj::set_moment(int _idx, RealType val)
{  
	values[_idx].moment = val;
};
 
int COGDefuzzSetObj::get_defuzz_type() const
{ 
	return DefuzzVarObj::DEFUZZ_COG; 
};

 
