//
// File:	MemberFuncSCurve.cpp
//
// Purpose:	Implementation of the MemberFuncSCurve membership function
//
// Copyright © 1999-2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//
 
#include "MemberFuncSCurve.h"
#include "MemberFuncSingle.h"
#include "FuzzyModelBase.h"
#include "FuzzyVariableBase.h"
 

#ifdef _DEBUG  
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;

#include "debug.h"

#endif


//
// Function:	MemberFuncSCurve()
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
MemberFuncSCurve::MemberFuncSCurve(FuzzySetBase* _parent) : MemberFuncBase(_parent), FFLLBase(_parent)
{
	// create the nodes array
	alloc_nodes( get_node_count() );

} // MemberFuncSCurve::MemberFuncSCurve()


 

//
// Function:	~MemberFuncSCurve()
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
MemberFuncSCurve::~MemberFuncSCurve() 
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
void MemberFuncSCurve::set_node(int idx, int x, int y, bool validate /* = true */)
{
	if (!validate)
		{
		nodes[idx].y = y;
		nodes[idx].x = x;
		return;
		}

 	// if the node is 1, 2, 4, or 5 the 'y' direction can be changed.
	if (idx == 0 ||  idx == 6)
 		 y = 0;
 
 	// check the first point...
	if (idx == 0)
		{
		// we're dealing with an end point
		if( x > nodes[1].x)
			nodes[idx].x = nodes[1].x;
		else
			nodes[idx].x =  x;
		}
	else if (idx == 6)
		{ 
		// we're dealing with the last point
		if ( x < nodes[5].x)
			nodes[idx].x = nodes[5].x;
		else
			nodes[idx].x =  x;
		}
	else
		{
		if ( x < nodes[idx - 1].x)
			nodes[idx].x = nodes[idx - 1].x;
		else if ( x > nodes[idx + 1].x)
			nodes[idx].x = nodes[idx + 1].x;
		else
			nodes[idx].x = x;

 		nodes[idx].y = y;
		}
		
} // end MemberFuncSCurve::set_node()

 
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

void MemberFuncSCurve::init_nodes(int mid_pt_x, int term_width)
{
	int i;  // counter
 
	// set start point
	nodes[0].x = mid_pt_x - term_width/2;
 	nodes[0].y = 0;
 
	// set mid point
	nodes[3].x = mid_pt_x;
	nodes[3].y = FuzzyVariableBase::get_dom_array_max_idx();

	// set end point
	nodes[6].x = nodes[0].x + term_width;
	nodes[6].y = 0;

	// now use common code to calc the other points...

	init_nodes(nodes[0], nodes[3], nodes[6]);
 
	// the only sanity check we'll do is to make sure we don't go "over" the midpoint

	for (i = 0; i < 3; i++)
		{
		if (nodes[i].x > mid_pt_x)
			nodes[i].x = mid_pt_x;
		}
	for (i = 4; i < 7; i++)
		{
		if (nodes[i].x < mid_pt_x)
			nodes[i].x = mid_pt_x;
		}

	calc(); // for this curve we must calc the curve. 
 
} // end MemberFuncSCurve::init_nodes()


 
//
// Function:	init_nodes()
// 
// Purpose:		Initialized the nodes that can have their 'y' value changed
//				based on the position of the start, mid, and end points
//
// Arguments:
//
//			NodePoint		start_pt	-	start point for the curve
//			NodePoint		mid_pt		-	mid point for the curve
//			NodePoint		end_pt		-	end point for the curve
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
void MemberFuncSCurve::init_nodes(NodePoint start_pt, NodePoint mid_pt, NodePoint end_pt)
{
 	int width = end_pt.x - start_pt.x;

	// we can't use set_node cuz it expects all the nodes to be filled in when it
	// performs validations and we don't have them set yet
	nodes[0] = start_pt;
	nodes[3] = mid_pt;
	nodes[6] = end_pt;

	int quarter = FuzzyVariableBase::get_dom_array_count() / 4;

	// set 'y' values...
	nodes[5].y = nodes[1].y = quarter; // 25% of max
	nodes[2].y = nodes[4].y = quarter * 3; // 75% of max

	// if the anchors line up - we're dealing with a ramp... treat that special
	if (start_pt.x == mid_pt.x)
		{
		// left ramp
  		nodes[1].x = nodes[2].x = nodes[3].x;
 		nodes[4].x = nodes[6].x - width/4;
		nodes[5].x = nodes[6].x - width/8;
		}
	else if (nodes[3].x == nodes[6].x)
		{
		// right ramp
 		nodes[5].x = nodes[4].x;
		nodes[1].x = nodes[0].x + width/8; // use a slight offset so they're not on top of each other
		nodes[2].x = nodes[0].x + width/4;
		}
	else 
		{
 		nodes[1].x = nodes[0].x + width/8; // use a slight offset so they're not on top of each other
		nodes[2].x = nodes[0].x + width/4;

 		nodes[4].x = nodes[6].x - width/4;
		nodes[5].x = nodes[6].x - width/8;
		}

}; // end MemberFuncSCurve::init_nodes()
  
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
void MemberFuncSCurve::calc()
{
 	// clear the values
	clear_values();
  
	// we could use a for loop but this will be faster...
	calc_curve_values(nodes[0], nodes[0], nodes[1], nodes[2]);
	calc_curve_values(nodes[0], nodes[1], nodes[2], nodes[3]);
	calc_curve_values(nodes[1], nodes[2], nodes[3], nodes[4]);
	calc_curve_values(nodes[2], nodes[3], nodes[4], nodes[5]);
	calc_curve_values(nodes[3], nodes[4], nodes[5], nodes[6]);
	calc_curve_values(nodes[4], nodes[5], nodes[6], nodes[6]);
 
} // end MemberFuncSCurve::calc()

//
// Function:	calc_curve_values()
// 
// Purpose:		This function that calculates the 'y' points for the segment
//				of the curve between the points (p2 and p3) passed in. It uses
//				the Catmull-Rom Splines algorithm (see code for specifics)
//
// Arguments:
//
//		NodePoint& p1 - node on curve BEFORE the starting node to calc the points for
//		NodePoint& p2 - starting node to calc the points for
//		NodePoint& p3 - end node to calc the points for
//		NodePoint& p4 - node on curve AFTER the starting node to calc the points for
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

void MemberFuncSCurve::calc_curve_values(NodePoint& p1, NodePoint& p2, NodePoint& p3, NodePoint& p4)
{
	// do some up-front checks to see if we NEED to calc

	if ((p3.x == p2.x) && (p3.y < p2.y))
		return;

	// This code uses the Catmull-Rom Splines algorithm. p0,p1,p2,p3 are points along
	// the curve, however that the curve drawn actually only passes through points p2 and p3.
	// For values of t between 0 and 1 the curve passes through P2 at t=0 and it passes 
	// through P3 at t=1
	//
	//
	//   q(t) = 0.5 * [ t^3 t^2 t 1 ] * [ -1  3 -3  1 ] * [ p1 ]
	//                                  [  2 -5  4 -1 ]   [ p2 ]
	//                                  [ -1  0  1  0 ]   [ p3 ]
	//                                  [  0  2  0  0 ]   [ p4 ]
	//								
	//	This becomes : 
	//
	//  q(t) = 0.5 * ((-p1 + 3*p2 -3*p3 + p4)*t*t*t
	//               + (2*p1 -5*p2 + 4*p3 - p4)*t*t
	//               + (-p1+p3)*t
	//               + 2*p2)
	//
	//  OR
	//
	//	q(t) = 0.5 * (a*t^3 + b*t^2 + c*t)
	//
	// Which we can use to find 'x' and 'y' values using the following equations:
	//
	// Eq 1: x = 0.5 * (ax*t^3 + bx*t^2 + cx*t + dx) 
	// Eq 2: y = 0.5 * (ay*t^3 + by*t^2 + cy*t + dy) 
	//
 
 	double x, y;	// calculated values
 
	double ax, bx, cx, dx;	// constants
	double ay, by, cy, dy;	// constants

	// calc constants up front...
	ax = .5 * (-p1.x + 3*p2.x -3*p3.x + p4.x);
	bx = .5 *(2*p1.x -5*p2.x + 4*p3.x - p4.x);
	cx = .5 *(-p1.x+p3.x);
	dx = p2.x;

	ay = .5 *(-p1.y + 3*p2.y -3*p3.y + p4.y);
	by = .5 *(2*p1.y -5*p2.y + 4*p3.y - p4.y);
	cy = .5 *(-p1.y+p3.y);
	dy = p2.y;

 	int current_idx = p2.x; // current index into values[] array we're calculate the 'y' for
	int next_idx;			// next index in values[] we'll calculate
	float t2;				// calculated t^2 
	bool repeat = false;	// boolean to indicate if we've found the 'y' value

	// the curve goes through p2 at t=0 so plugging t=0 into Eq 2 above
	// leaves us with just dy which is p2.y. 
 
	set_value(current_idx, static_cast<DOMType>(p2.y)); // explicit cast to get over ambiguity
 
	// increment current index by one...
	current_idx++;

	// calculating the 'y' values is not as simple as having a function that we can
	// plug an 'x' value in for and get the 'y'. 
	// I've tried MANY algorithms (including some from _Numerical Recipes in 'C'_
	// (http://www.ulib.org/webRoot/Books/Numerical_Recipes/bookcpdf.html) 
	// which gave us the ability to provide an 'x' value and get the 'y' 
	// but it's way too "wild" a curve, small movement make for large changes. 
	//
 	// We want to have a 'y' value for each 'x' value in the values[] array, so we
	// need to play with the size of the steps we're taking as we go from t=0 to t=1.
 	// For example we may get values such as:
	//
	// x = 8.2 for t = 0.003
	// x = 8.8 for t = 0.004
	// x = 9.1 for t = 0.005
	// x = 9.5 for t = 0.006
	//
	// If we're dealing with index 9 in the values[] array we'll use t = 0.005
	// because it's the first value of 't' we found greater than the index we're dealing with
	// We then calculate the 'y' value for that so values[9] = y(0.005) (where y(t) is
	// Eq 2 above).
	//
	// If the distance between p2 and p3 is large, it's very possible to skip 'x' values.
	// For example we may get:
	// x = 8.9 for t = 0.003
	// x = 10.1 for t = 0.004
	// x = 10.9 for t = 0.005
	// x = 11.6 for t = 0.006
	//
	// there's no value we can use for x = 9, so we need to go back and use a smaller
	// step. We go back in "time" by decrementing 't' by the initial step and use a 
	// smaller step, so we may use a step of 0.005 and get:
	// x = 8.9 for t = 0.003
	// x = 9.5 for t = 0.0035
	// x = 10.1 for t = 0.004
	// x = 11.2 for t = 0.0045
	//
	// So we can set values[9] = y(0.0035) (again, using Eq 2 above)

 	// start with a large step of 0.01
 
 	float step = .01f;

	// NOTE: we don't step in the 'for' statment cuz we're incrementing it below
	for(double t= 0; t <= 1; /* t+=step,*/ current_idx++)
		{ 
		// calc the next index so we know if we've gone past the current 'x' we're dealing with
		next_idx = current_idx + 1;
  
		do	// while(repeat)
			{ 
			repeat = false;

 			do // while (x < current_idx && t <= 1)
				{
	 	 		t += step; // increment 't' by the step value
				
				// calculate t^2
	 			t2 = t*t;

				// calculate the 'x' value for this value of 't'
 	  			x = ax*t*t2 + bx*t2 + cx*t + dx; 
	
				// while we're still within this 'x' value, keep steping
				// so we can get the higest value of 't' for this 'x'

				} while (x < current_idx && t <= 1);

			// When we get here, the 'x' value is >= the current index we're 
			// concerned with. 
			// if (x > current_idx) *AND* (x > next_idx) then we didn't find a value
			// for the current_idx, we'll try a smaller step and progrssively ratchet
			// down the step until we hit all the 'x' values.  

 			if (x > next_idx )
				{
				// set repeat so we go back to the outter 'do-while' loop
	 			repeat = true;

				// step "back in time" by the step we used
 				t -= step;

				// decrease the step
				if (step > .001f)
		 			step -= .001f; // a smallER step
				else
					step -= .0005f; // a very small step

				assert(step > 0);

				} // end if x > next_idx 
	
		} while (repeat);

	// we found a value for current_idx at time 't', now get the 'y' value
	// for that 't' value

  	 y= (ay*t*t2 + by*t2 + cy*t  + dy);

	 if (y < 0)
		 y = 0.0;
	 if (y >= FuzzyVariableBase::get_dom_array_count())
		 y = FuzzyVariableBase::get_dom_array_max_idx();

	 if (current_idx >= FuzzyVariableBase::get_x_array_count())
		 return; // saftey
 
   	set_value(current_idx, static_cast<DOMType>(y)); // explicit cast to get over ambiguity

	} // end for 't' loop

} // end MemberFuncSCurve::calc_curve_values()


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
void  MemberFuncSCurve::set_ramp(int hi_lo_ind, int left_right_ind)
{
	// init the curve so we have a starting point...

	nodes[3].x = nodes[0].x + (nodes[6].x - nodes[0].x)/2;
	init_nodes(nodes[0], nodes[3], nodes[6]);

	// figure out which side we're changing
	if (hi_lo_ind == 0)
		{
		// set the anchor then fit the handles
		nodes[3].x = nodes[0].x + (nodes[6].x - nodes[0].x)/2;
		}
	else
		{
 		if (left_right_ind)
			{
			// make the left the ramp
			nodes[0].x = nodes[1].x = nodes[2].x = nodes[3].x = 0;
			}
		else
			{
			// make the right the ramp
			nodes[3].x = nodes[4].x = nodes[5].x = nodes[6].x   =  FuzzyVariableBase::get_x_array_max_idx();
			}
		} // end if making hi ramp

	init_nodes(nodes[0], nodes[3], nodes[6]);

	// call the ancestor to deal with common code...
	MemberFuncBase::set_ramp(hi_lo_ind, left_right_ind);

} // end MemberFuncSCurve::set_ramp()
 
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
void MemberFuncSCurve::expand(int x_delta )
{
	// call init-nodes rather than move nodes to make it look better...
	init_nodes(nodes[3].x, (nodes[6].x - nodes[0].x) + abs(x_delta));

} // end MemberFuncSCurve::expand()


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
void MemberFuncSCurve::shrink(int x_delta )
{
 	// call init-nodes rather than move nodes to make it look better...
	init_nodes(nodes[3].x, (nodes[6].x - nodes[0].x) - abs(x_delta));

} // end MemberFuncSCurve::shrink()

/////////////////////////////////////////////////////////////////////
////////// Trivial Functions That Don't Require Headers /////////////
/////////////////////////////////////////////////////////////////////
 

int MemberFuncSCurve::get_center_x(void) const 
{
	// the "mid" x is sepecific to the shape... the start/end x funcs
	// are in the ancestor

	return nodes[3].x;
};
int MemberFuncSCurve::get_node_count() const 
{ 
	return 7; 
};
int MemberFuncSCurve::get_func_type() const 
{
	return MemberFuncBase::S_CURVE;
};