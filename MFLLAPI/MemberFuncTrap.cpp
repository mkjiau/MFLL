//
// File:	MemberFuncTrap.cpp
//
// Purpose:	Implementation of the Trapezoid membership function
//
// Copyright © 1999-2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//

#include "MemberFuncTrap.h"
#include "MemberFuncSingle.h"
#include "FuzzyModelBase.h"
#include "FuzzyVariableBase.h"

#ifdef _DEBUG  
#undef THIS_FILE
static char THIS_FILE[]=__FILE__; 

#include "debug.h"

#endif

//
// Function:	MemberFuncTrap()
// 
// Purpose:		Constructor
//
// Arguments:
//
//		FuzzySetBase* _parent - Set this member function is part of
//
// Returns:
//
//		nothing
//
// Author:	Michael Zarozinski
// Date:	8/99
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
// 
MemberFuncTrap::MemberFuncTrap(FuzzySetBase* _parent) : MemberFuncBase(_parent), FFLLBase(_parent)
{
	// create the nodes array
	alloc_nodes( get_node_count() );

} // MemberFuncTrap::MemberFuncTrap()


//
// Function:	~MemberFuncTrap()
// 
// Purpose:		Destructor
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
// Date:	8/99
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
// 

MemberFuncTrap::~MemberFuncTrap() 
{
	// nothing to do, base class(es) handle everything
}


//
// Function: set_node()
// 
// Purpose:	This function moves a point to a new position. It performs
//			checks to make sure the nodes are at valid positions (such as
//			not allowing the last point to be BEFORE the mid point.
// 
// Arguments:
//
//		int			idx			-	index of the point to move
//		int			x			-	x position to move to (in variable coords)
//		int			y			-	x position to move to (in variable coords)
//		bool		validate	-	indicates if we should perorm sanity checks (default is true)
//
// Returns:
//
//		void
// 
// Globals:
//
// Author:	Michael Zarozinski	
// Date:	6/21/99
// 
//	Modification History
//	Author		Date		Modification
//	------		----		------------
//	 
//
void MemberFuncTrap::set_node(int idx, int x, int y, bool validate /* = true */)
{
	if (!validate)
		{
		nodes[idx].y = y;
		nodes[idx].x = x;
		return;
		}

  	// set the 'y' value based on the shape of this curve - we don't
	// allow them to change the 'y' for the triangle shape

	if (idx == 0 || idx == 3)
		nodes[idx].y = 0;
	else
		nodes[idx].y = FuzzyVariableBase::get_dom_array_max_idx();

	// perform some sanity checks on the 'x' position

	if (idx == 1 || idx == 2)
		{ 
		// we're dealing with the plateau
		// make sure we don't overlap	
		if (idx == 1)
			{
			if (x >= nodes[2].x)
				nodes[1].x  = nodes[2].x;
			else if ( x < get_start_x())
				nodes[idx].x = get_start_x();
			else if ( x > get_end_x())
				nodes[idx].x = get_end_x();
			else
				nodes[idx].x = x;
 
			} // end if left plateau point
		else if (idx == 2)
			{
			if (x <= nodes[1].x)
				nodes[2].x  = nodes[1].x;
			else if ( x < get_start_x())
				nodes[idx].x = get_start_x();
			else if ( x > get_end_x())
				nodes[idx].x = get_end_x();
			else 
				nodes[idx].x = x;
	 
			} // end if right plateau point
		} 
	else
		{
		// idx is 0 or 3 (start/end nodes)

		// check the first point...
		if (idx == 0)
			{
			// we're dealing with an end point
			if ( x > nodes[1].x)
				nodes[idx].x = nodes[1].x;
			else
				nodes[idx].x =  x; // overloaded operator=() performs some additional validation for us
			}
		else
			{ 
			// we're dealing with the last point
			if ( x < nodes[2].x)
				nodes[idx].x = nodes[2].x;
			else
				nodes[idx].x =  x; // overloaded operator=() performs some additional validation for us
			}

		} // end if dealing with start or end point
 

} // end MemberFuncTrap::set_anchor()

 
//
// Function:	init_nodes()
// 
// Purpose:		Initialized all the nodes to reasonable values depending on the 
//				mid-point and width passed in.
//
// Arguments:
//
//			int		mid_pt_x	-	mid point of the member func
//			int		width		-	how wide this set is
// 
// Returns:
//
//			none
//
// Author:	Michael Zarozinski
// Date:	6/21/99
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//

void MemberFuncTrap::init_nodes(int mid_pt_x, int term_width)
{
	int node_count = get_node_count();

	// if start x is too close to the end move it...

	if (mid_pt_x + term_width/2 >  FuzzyVariableBase::get_x_array_count())
		mid_pt_x = FuzzyVariableBase::get_x_array_max_idx() - term_width/2;

 	nodes[0].x = mid_pt_x - term_width/2;
	nodes[0].y = 0;

	nodes[1].x = nodes[0].x + term_width/(node_count - 1);
	nodes[1].y = FuzzyVariableBase::get_dom_array_max_idx();

	nodes[2].x = nodes[1].x + term_width/(node_count - 1);
	nodes[2].y = FuzzyVariableBase::get_dom_array_max_idx();

	nodes[3].x = nodes[0].x + term_width ;
	nodes[3].y = 0;

	calc(); // re-calc values array

} // end MemberFuncTrap::set_all_anchors()


//
// Function:	set_ramp()
// 
// Purpose:		Virtual function that sets the nodes appropriately
//				depending on the type of ramp (or no ramp) that we want.
//
// Arguments:
//
//		int left_right_ind		-	indicates if we're making a left (1) or right (0) ramp
//		int hi_lo_ind			-	indicates if we're creating (1) or removing (0) ramp
//
// Returns:
//
//		void
//
// Author:	Michael Zarozinski
// Date:	9/99
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
void  MemberFuncTrap::set_ramp(int hi_lo_ind, int left_right_ind)
{

	// initialize the nodes so we have a good starting point
	init_nodes(nodes[0].x + (nodes[3].x - nodes[0].x)/2, nodes[3].x - nodes[0].x);

	// figure out which side we're changing
	if (left_right_ind)
		{
		if (hi_lo_ind == 1)
			{
			// create a ramp
			nodes[1].x = nodes[0].x = 0;
			}
		else
			{
 			// removing a ramp - set the 1st point in the plateau 
			// midway between the 1st anchor and the 2nd point in the plateau
			nodes[1].x = nodes[0].x + (nodes[2].x - nodes[0].x)/2; 
			}
		}
	else
		{
		// right ramp
		if (hi_lo_ind == 1)
			{
			// making a high ramp
			nodes[2].x = nodes[3].x = FuzzyVariableBase::get_x_array_max_idx();
			}
		else
			{
 			// removing a ramp - set the 2nd point in the plateau 
			// midway between the 2nd point in the plateau and the last anchor
			nodes[2].x = nodes[1].x + (nodes[3].x - nodes[1].x)/2; 
			}
		} // end if dealing with 2nd half of function

		// call the ancestor to deal with common code...
	MemberFuncBase::set_ramp(hi_lo_ind, left_right_ind);

} // end MemberFuncTrap::set_ramp()


//
// Function:	calc()
// 
// Purpose:		Virtual function that calculates the 'y' points for this curve
//				 and fills in the values[] array.
//
// Arguments:
//
//		none
//
// Returns:
//
//		void
//
// Author:	Michael Zarozinski
// Date:	5/99
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//

void MemberFuncTrap::calc()
{
	int		i;	// counter
	double	m;	// slope of the curve, this is the 'm' in y = mx + b
 	double	b;	// b for y = mx + b
 
	// clear out the whole values[] array
	clear_values();
 
	m = 1.0; // init slope

	double divisor = nodes[1].x - nodes[0].x;

	// make sure we don't div by 0...  
	if (divisor)
		m /= divisor; // this is 1/divisor cuz the 'y' max is 1.0
 
	// fill in the first part of the curve
	for (i = nodes[0].x; i <= nodes[1].x; i++)
		{
		// we "shift" things by subtracting nodes[0].x so the calc of the y = mx + b
		set_value(i, m * (i - nodes[0].x) ); // y = mx + b (b is zero in this case)
		}

	// fill in the plateau part
	for (i = nodes[1].x; i <= nodes[2].x; i++)
		set_value(i, 1.0f);

	// calc the slope for the 3rd part of the line...
	divisor = nodes[3].x - nodes[2].x;

	m = -1.0;	// slope is negative for this part of the curve

	if (divisor)
 		m /=  divisor;
  	 
	b = 1.0;	// be is the max 'y' value

	// fill in the 3rd part of theline

 	for (i = nodes[2].x; i <= nodes[3].x; i++)
		{
		// we "shift" things by subtracting nodes[2].x so the calc of the y = mx + b
		set_value(i, (m * (i - nodes[2].x)) + b );  
		}

} // end MemberFuncTrap::calc()
 
//
// Function:	expand()
// 
// Purpose:		Uniformly expands this member func around it's center
//
// Arguments:
//
//		int x_delta - how much to expand the member func by		
//
// Returns:
//
//		void
//
// Author:	Michael Zarozinski
// Date:	9/99
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
void MemberFuncTrap::expand(int x_delta )
{
	// call init-nodes rather than move nodes to make it look better...
	init_nodes((nodes[2].x - nodes[1].x)/2, (nodes[3].x - nodes[0].x) + abs(x_delta));
 	
} // end MemberFuncTrap::expand()

//
// Function:	shrink()
// 
// Purpose:		Uniformly shrinks this member func around it's center
//
// Arguments:
//
//		int x_delta - how much to shrink the member func by		
//
// Returns:
//
//		void
//
// Author:	Michael Zarozinski
// Date:	9/99
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
// 
void MemberFuncTrap::shrink(int x_delta )
{
 	// call init-nodes rather than move nodes to make it look better...
	init_nodes((nodes[2].x - nodes[1].x)/2, (nodes[3].x - nodes[0].x) - abs(x_delta));
 	
} // end MemberFuncTrap::shrink()

/////////////////////////////////////////////////////////////////////
////////// Trivial Functions That Don't Require Headers /////////////
/////////////////////////////////////////////////////////////////////
  
int MemberFuncTrap::get_center_x(void) const 
{
	// the "mid" x is sepecific to the shape... the start/end x funcs
	// are in the ancestor	
	return nodes[1].x + ((nodes[2].x - nodes[1].x)/2);
};
int MemberFuncTrap::get_func_type()  const 
{
	return MemberFuncBase::TRAPEZOID;
};
int MemberFuncTrap::get_node_count() const 
{ 
	return 4;
};
