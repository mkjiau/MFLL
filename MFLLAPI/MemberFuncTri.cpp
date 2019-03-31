//
// File:	MemberFuncTri.cpp
//
// Purpose:	Implementation of the MemberFuncTri membership function
//
// Copyright © 1999-2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//

#include "MemberFuncTri.h"
#include "MemberFuncSingle.h"
#include "FuzzyModelBase.h"
#include "FuzzyVariableBase.h"

#ifdef _DEBUG  
#undef THIS_FILE
static char THIS_FILE[]=__FILE__; 

#include "debug.h"

#endif

//
// Function:	MemberFuncTri()
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
MemberFuncTri::MemberFuncTri(FuzzySetBase* _parent) : MemberFuncBase(_parent), FFLLBase(_parent)
{
	// create the nodes array
	alloc_nodes( get_node_count() );

} // MemberFuncTri::MemberFuncTri()

//
// Function:	~MemberFuncTri()
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
MemberFuncTri::~MemberFuncTri() 
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

void MemberFuncTri::set_node(int idx, int x, int y, bool validate /* = true */)
{
	if (!validate)
		{
		nodes[idx].y = y;
		nodes[idx].x = x;
		return;
		}

 	// set the 'y' value based on the shape of this curve - we don't
	// allow them to change the 'y' for the triangle shape

	if (idx == 0 || idx == 2)
		nodes[idx].y = 0;
	else
		nodes[idx].y = FuzzyVariableBase::get_dom_array_max_idx();

	// perform some sanity checks on the 'x' position

	if (idx == 0)
		{
		// we're dealing with an start point
		if ( x > get_center_x())
	 		nodes[idx].x = get_center_x();
		else
			nodes[idx].x = x; // overloaded operator=() performs some additional validation for us
		}
	else if (idx == 1)
		{
		// we're dealing with the mid point which can't go beyond the end points
		if ( x < get_start_x())
			nodes[idx].x = get_start_x();
		else if ( x > get_end_x())
			nodes[idx].x = get_end_x();
		else
			nodes[idx].x = x; // overloaded operator=() performs some additional validation for us
		} 
	else
		{ 
		// we're dealing with the last point
		if ( x < get_center_x())
			nodes[idx].x = get_center_x();
		else  
			nodes[idx].x = x; // overloaded operator=() performs some additional validation for us
		}

} // end MemberFuncTri::set_node()
  

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

void MemberFuncTri::init_nodes(int mid_pt_x, int width)
{
 	nodes[0].x = mid_pt_x - width/2;
	nodes[0].y = 0;
 
	nodes[1].x = mid_pt_x;
	nodes[1].y = FuzzyVariableBase::get_dom_array_max_idx();  

	nodes[2].x = nodes[1].x + width/2;
	nodes[2].y = 0;

	calc(); // re-calc values array

} // end MemberFuncTri::init_nodes()
 
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
void  MemberFuncTri::set_ramp(int hi_lo_ind, int left_right_ind)
{

	// if we're removing a ramp, move the middle node
 	if (hi_lo_ind == 0)
		{
		// move only the middle term
		nodes[1].x = nodes[0].x + (nodes[2].x - nodes[0].x)/2;
		}
	else
		{
		// we're creating a ramp...
 		if (left_right_ind)
			{
			// make the left the ramp
			nodes[0].x = nodes[1].x = 0;
			}
		else
			{
			// make the right the ramp
			nodes[2].x = nodes[1].x = FuzzyVariableBase::get_x_array_max_idx();
			}
		} // end if making hi ramp

	// call the ancestor to deal with common code...
	MemberFuncBase::set_ramp(hi_lo_ind, left_right_ind);

} // end MemberFuncTri::set_ramp()

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
//  
void MemberFuncTri::calc()
{
	int		i;	// counter
	double	m;	// slope of the curve, this is the 'm' in y = mx + b
 	double	b;	// b for y = mx + b
 
	// clear out the whole values[] array
	clear_values( );

	// calc the slope for the first part of the line...

	double divisor = nodes[1].x - nodes[0].x;
 
	m = 1.0;

	// make sure we don't div by 0...  
	if (divisor )
 		m /= divisor; // this is 1/divisor cuz the 'y' max is 1.0
 
	// fill in the 'y' values for the first part of this member func
 
	for (i = nodes[0].x; i <= nodes[1].x; i++)
		{
		// we "shift" things by subtracting nodes[0].x so the calc of the y = mx + b
		set_value(i, m * (i - nodes[0].x) ); // y = mx + b (b is zero in this case)
		}

	// calc the slope for the 2nd part of the line...

	divisor = nodes[2].x - nodes[1].x;

	m = -1.0;	// slope is negative for this part of the curve

	if (divisor)
 		m /=  divisor;
  	 
	b = 1.0;

	// fill in the 'y' values for the 2nd part of the curve
 	for (i = nodes[1].x; i <= nodes[2].x; i++)
		{
		// we "shift" things by subtracting nodes[1].x so the calc of the y = mx + b
		set_value(i, (m * (i - nodes[1].x)) + b );  
		}

} // end MemberFuncTri::calc()
 
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
void MemberFuncTri::expand(int x_delta )
{
	int delta = abs(x_delta);
 
	move_node(0, nodes[0].x - delta, nodes[0].y);
	move_node(2, nodes[2].x + delta, nodes[2].y);
 
} // end MemberFuncTri::expand()

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
void MemberFuncTri::shrink(int x_delta )
{
	int delta = abs(x_delta);

	move_node(0, nodes[0].x + delta, nodes[0].y);
	move_node(2, nodes[2].x - delta, nodes[2].y);
 
} // end MemberFuncTri::shrink()

/////////////////////////////////////////////////////////////////////
////////// Trivial Functions That Don't Require Headers /////////////
/////////////////////////////////////////////////////////////////////
 
int MemberFuncTri::get_center_x(void) const 
{
	return nodes[1].x;
};
int MemberFuncTri::get_func_type() const 
{
	return MemberFuncBase::TRIANGLE;
};
int MemberFuncTri::get_node_count() const 
{ 
	return 3;
};
