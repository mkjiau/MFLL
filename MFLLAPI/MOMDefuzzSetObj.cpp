//
// File:	MOMDefuzzSetObj.cpp	
//
// Purpose:	Set object for Mean of Maximum Defuzzification method
//
// Copyright © 2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//
 
// include files
#include "MOMDefuzzSetObj.h"
#include "FuzzyOutSet.h"
#include "MemberFuncBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

#include "debug.h"

#endif


//
// Function:	MOMDefuzzSetObj()
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

MOMDefuzzSetObj::MOMDefuzzSetObj(FuzzyOutSet* par)
: DefuzzSetObj(par), FFLLBase(par)
{
	mean_value = -1.0;

}; // end MOMDefuzzSetObj::MOMDefuzzSetObj()
 
//
// Function:	~MOMDefuzzSetObj()
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


MOMDefuzzSetObj::~MOMDefuzzSetObj()
{
 
}; // end MOMDefuzzSetObj::~MOMDefuzzSetObj()


//
// Function:	calc()
// 
// Purpose:		Calculates the max 'x' nodes, their average, then sets the 'x' value
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
// Author	Date		Modification
// ------	----		------------
//
//
 
void MOMDefuzzSetObj::calc()
{
	int		i;				// counters
  	int		x_sum = 0;		// sum of the x values for the nodes with the max 'y'
 	int		max_y = -1;		// max y position
 
	// get the min val and idx_multiplier for the var...

	FuzzySetBase* set_base = get_parent();

	RealType idx_mult = set_base->get_idx_multiplier();
	RealType min_x = set_base->get_left_x();
 
	// this gets a bit complex cuz of the S-Curve where it's possible to have
	// more than 2 nodes all at the max value.  So we'll make this algorithm
	// as generic (read: overkill) as possible.

	// loop through all the nodes and find the max y value and the
	// index of the first and LAST node with that value

	int first_x, last_x;
	int node_count = set_base->get_node_count();

	first_x = last_x = -1;

	NodePoint pt;

	for (i = 0; i < node_count; i++)
		{
		pt = set_base->get_node(i);
		// y = set_base->member_func->get_node_y(i);

 		if (pt.y > max_y)
			{
 			max_y = pt.y;
			}

		} // end loop through nodes

	// now find the first and last nodes that have that 'y' value
	for (i = 0; i < node_count; i++)
		{
		pt = set_base->get_node(i);
 		// if we found a Y value greater than the previous one, 
		// clear out any old information, we have a new maximum
		if (pt.y == max_y)
			{
 			if (first_x == -1)
				{
				// set them both so we can avoid special logic
				// when only one node is at the max y
				first_x = last_x = pt.x; 
				}
			else 
				{
				// set the last node, if theres one furthur down
				// it'll overwrite this one
				last_x = pt.x; 
				}
			} // end if y == max_y

		} // end loop through nodes
 

	// since the moment invovles the distance from 0 and we use the x_idx
	// above in our calcs, we need to multiply by the index multiplier
	// to convert an index to a 'x' value.  Note: we could have multiplied
	// above when we calculated each moment but it's faster to just perform
	// ONE multiplication here
	// We also add the starting x value of the output variable to get
	// it in the correct position relative to the "origin" of the output variable

	set_mean_value(min_x +  ((first_x + (static_cast<RealType>(last_x - first_x)/2.0f)) * idx_mult ));
 
} // end MOMDefuzzSetObj::calc()
 

/////////////////////////////////////////////////////////////////////
////////// Trivial Functions That Don't Require Headers /////////////
/////////////////////////////////////////////////////////////////////
RealType MOMDefuzzSetObj::get_mean_value() const
{
	return mean_value;
};
void MOMDefuzzSetObj::set_mean_value( RealType val)
{  
	mean_value= val;
};
 
int MOMDefuzzSetObj::get_defuzz_type() const 
{ 
	return DefuzzVarObj::DEFUZZ_MOM; 
};

