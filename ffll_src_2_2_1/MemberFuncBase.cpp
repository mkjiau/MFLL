//
// File:	MemberFuncBase.cpp
//
// Purpose:	This is the base class for the membership functions
//
// Copyright © 1999-2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//

// include files
#include "FuzzySetBase.h"
#include "MemberFuncBase.h"
#include "FuzzyModelBase.h"
#include "FuzzyVariableBase.h"

#ifdef  _DEBUG 
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

#include "debug.h"

#endif

//
// Function:	MemberFuncBase()
// 
// Purpose:		Constructor
//
// Arguments:
//
//		FuzzySetBase* _parent - Set this member function is part of
//
// Returns:
//
//		none
//
// Author:	Michael Zarozinski
// Date:	5/99
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
MemberFuncBase::MemberFuncBase(FuzzySetBase* _parent) : FFLLBase(_parent)  
{
 
	// NOTE: don't call init() from here cuz it's virtual and it calls virtual functions which
	// aren't called the way we want from the constructor

	nodes = NULL;
  
	values = NULL; 

	ramp = MemberFuncBase::RAMP_NONE; // not a ramp (at this point)

} // end MemberFuncBase::MemberFuncBase()


//
// Function:	~MemberFuncBase()
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
// Date:	5/99
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
MemberFuncBase::~MemberFuncBase(void)
{
	if (nodes != NULL)
		delete[] nodes;

 	dealloc_values_array();

	nodes = NULL;

} // end MemberFuncBase::~MemberFuncBase()

//
// Function:	init()
// 
// Purpose:		Initialization for the member func that is specific
//				to the type of member function. This is a VIRTUAL 
//				function and as such must be called AFTER the object
//				is created (virtual funcs can not be called from
//				the constructor).
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
 void MemberFuncBase::init( )
{
	// allocate the values array
	alloc_values_array();
 
	// zero out values...
	clear_values( );

} // end MemberFuncBase::init();



//
// Function:	set_ramp()
// 
// Purpose:		Set the ramp status for this member func. This is a VIRTUAL func
//				so member funcs like the Singleton can override this.
//				A ramp is a membership function that has the 'y' axis
//				as one of it's sides. For example a normal trapezoid memberhip
//				function would look like this:
//                                       ___
//              Normal Trapezoid:       /   \
//                                       ___
//              Left Ramp Trapezoid:    |   \ 
//                                       ___
//              Right Ramp Trapezoid:   /   |  
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
// Date:	7/99
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
 
void  MemberFuncBase::set_ramp(int hi_lo_ind, int left_right_ind)
{
	// set the type of ramp for this term
 	if (hi_lo_ind == 0)
		ramp = MemberFuncBase::RAMP_NONE;
	else
		{
		if (left_right_ind)
			ramp = MemberFuncBase::RAMP_LEFT;
		else
			ramp = MemberFuncBase::RAMP_RIGHT;
		}

	calc(); // re-calc 'y' values
 
} // end MemberFuncBase::set_ramp()

 
//
// Function:	move()
// 
// Purpose:		Moves ALL the nodes for the member func by the delta passed in.
//
// Arguments:
//
//		int x_delta - how many 'x' elements (in the values[] array) to move each node by 
//
// Returns:
//
//		void
//
// Author:	Michael Zarozinski
// Date:	7/99
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
void MemberFuncBase::move(int x_delta) 
{

	if (x_delta == 0)
		return; // nothing to do!

	// move all the nodes... 

	// we need to be careful about the order we move them in.
	// if they are being move to the right (positive offset) we need to
	// move the last point and work our way to the first point so we don't
	// cross any points if the offset is large.
	// likewize if we're moving to the left (negative offset) move them in
	// order (first to last)

	int start, end, increment;	// values that determine how we loop
 	int node_count = get_node_count();

	if (x_delta < 0)
		{
		// moving to the left
		start = 0;
		end = node_count;
		increment = 1;

		// adjust the delta so we don't "collapse" the term
		
		if (nodes[0].x + x_delta < 0)
			x_delta = -nodes[0].x;

		} // end if offset negative
	else
		{
		// offset is positive
		start = node_count - 1;
		end = -1;
		increment = -1;
		if (nodes[node_count - 1].x + x_delta > FuzzyVariableBase::get_x_array_max_idx())
			x_delta = ( FuzzyVariableBase::get_x_array_max_idx()) - nodes[node_count - 1].x;

		} // end if offset positive

	// now move them all (note the 'y' direction is invariant when moving all nodes at once)

	for (int i = start; i != end; i += increment)
		{
  		move_node(i, nodes[i].x + x_delta, nodes[i].y );

		} // end loop through nodes

} // end MemberFuncBase::move()



//
// Function:	save_to_fcl_file()
// 
// Purpose:		Virtual func to write out the FCL (Fuzzy Control Language) for this membership function.
//				This is the "membership_function" part of the set's information which
//				has the format (as defined by the IEC 61131-7 International Standard):
//
//					TERM term_name:= membership_function;
//
//				Each node is written out as "(<x value>, <y value>)" where the values
//				may or may not have a decimal point. NOTE that singletons are special 
//				and can be written out as just the x value. But we don't need to worry
//				about most of that here cuz this is a VIRTUAL func so if it's a singleton
//				that version will deal with it explicitly.
//
// Arguments:
//
//		std::ofstream& file_contents - STL stream to write out to a file
//
// Returns:
//
//		void
//
// Author:	Michael Zarozinski
// Date:	9/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//

void MemberFuncBase::save_to_fcl_file(std::ofstream& file_contents)
{
 
	char  val[6];	// temp string to hold the string version of a number
	int precision ;	// how many decimal places to show  

 	// list the points...
	for (int i = 0; i < get_node_count(); i++)
		{
		// convert the index in the values array to an 'x' value
 		RealType fvalue = get_parent()->convert_idx_to_value(get_node_x(i));

		// if there is a decimal point for this, display only 2 decimal places,
		// else don't display any (this just makes the FCL file easier to read)
		if (fmod(fvalue, 1) == 0)
			precision = 0;
		else
			precision = 2;

 		// trim 'x' value to the specified precision
		sprintf( val, "%.*lf", precision, fvalue );
 
 		file_contents << "(" <<  val << ", ";
 
		// convert to 'y' coordinates
		fvalue = static_cast<RealType>(get_node_y(i)) / static_cast<RealType>(FuzzyVariableBase::get_dom_array_max_idx());

		if (fmod(fvalue, 1) == 0)
			precision = 0;
		else
			precision = 2;

		sprintf( val, "%.*lf", precision, fvalue );
		
		file_contents << val << ") ";

		} // end loop through nodes

	// add ending semi-colon
	file_contents << ";";

} // end MemberFuncBase::save_to_file()

/////////////////////////////////////////////////////////////////////
////////// Trivial Functions That Don't Require Headers /////////////
/////////////////////////////////////////////////////////////////////

int MemberFuncBase::alloc_values_array()
{
	values = new DOMType[ FuzzyVariableBase::get_x_array_count()];

	if (values == NULL)
		{
		set_msg_text(ERR_ALLOC_MEM);
		return -1;
		}

	return 0;

} // end MemberFuncBase::alloc_values_array()

void MemberFuncBase::dealloc_values_array()
{
 	if (values != NULL)
		delete[] values;

	values = NULL;

} // end MemberFuncBase::dealloc_values_array()

void MemberFuncBase::move_node(int idx, _point pt) 
{
	move_node(idx, pt.x, pt.y);	
};

void MemberFuncBase::move_node(int idx, int x, int y)  
{
 	set_node(idx, x, y);

};

void MemberFuncBase::clear_values()
{ 
	memset(values, 0,  FuzzyVariableBase::get_x_array_count() * sizeof(DOMType));
 
};

int MemberFuncBase::alloc_nodes(int node_count)
{
	nodes = new NodePoint[node_count];
 
	if (nodes == NULL)
		{
		set_msg_text(ERR_ALLOC_MEM);
		return -1;
		}

	// initialize 
	for (int i = 0; i < node_count; i++) 
 		nodes[i].x = nodes[i].y = 0;

	return 0;
};

int MemberFuncBase::get_start_x(void) const 
{
	return(nodes[0].x);
};
int MemberFuncBase::get_center_x(void) const 
{
	return nodes[0].x + ((nodes[get_node_count() - 1].x - nodes[0].x)/2);
}; 

NodePoint MemberFuncBase::get_node(int idx) const 
{
	return nodes[idx];
};

int MemberFuncBase::get_node_x(int idx) const 
{
	return nodes[idx].x;
};
int MemberFuncBase::get_node_y(int idx) const 
{
	return nodes[idx].y;
};
FuzzySetBase* MemberFuncBase::get_parent() const 
{
	return static_cast<FuzzySetBase*>(FFLLBase::get_parent());
};

int MemberFuncBase::get_ramp() const
{
	return ramp;
};

DOMType MemberFuncBase::get_dom(int idx)
{

	assert(idx >= 0);
	assert(idx < FuzzyVariableBase::get_x_array_count());
 
	// make sure index is within range
	if (idx < 0)
		idx = 0;
	if (idx > FuzzyVariableBase::get_x_array_max_idx())
		idx = FuzzyVariableBase::get_x_array_max_idx();
	
	return values[idx];

};

int MemberFuncBase::get_end_x(void) const
{
	return(nodes[get_node_count() - 1].x);
}  
  
RealType  MemberFuncBase::get_left_x() const 
{
	return get_parent()->get_left_x();
};
	
DOMType MemberFuncBase::get_value(int idx) const 
{ 
	assert(idx >= 0);
	assert(idx < FuzzyVariableBase::get_x_array_count());
		
	// make sure index is within range
	if (idx < 0)
		idx = 0;
	if (idx > FuzzyVariableBase::get_x_array_max_idx())
		idx = FuzzyVariableBase::get_x_array_max_idx();

	return values[idx]; 
};

int MemberFuncBase::set_value(int idx, DOMType val) 
{
	// this value accepts a value between 0 and DOM max
 
	assert(idx >= 0);
	assert(idx < FuzzyVariableBase::get_x_array_count());

	// make sure index is within range
	if (idx < 0)
		idx = 0;
	if (idx > FuzzyVariableBase::get_x_array_max_idx())
		idx = FuzzyVariableBase::get_x_array_max_idx();

	values[idx] = val;
 
	return 0;
};
 
int MemberFuncBase::set_value(int idx, RealType val) 
{
	// this version takes a value from 0 to 1 and maps it to a value between 0 and DOM max

	// mapp the float to the values[] array makeing sure it doen't go over
	// the DOM max

	assert(val >= 0.0f);
	assert(val <= 1.0f);

	// set it if it's out of range
	if (val < 0.0f)
		val = 0;
	if (val > 1.0f)
		val = 1;

	DOMType y = (static_cast<RealType>(FuzzyVariableBase::get_dom_array_max_idx()) * val) + 0.5; // add .5 to account for rounding

	assert(y >= 0);
	assert(y <= FuzzyVariableBase::get_dom_array_max_idx());

	if (y < 0)
		y = 0;
	if (y > FuzzyVariableBase::get_dom_array_max_idx())
		y = FuzzyVariableBase::get_dom_array_max_idx();

	assert(idx >= 0);
	assert(idx < FuzzyVariableBase::get_x_array_count());

	// make sure index is within range
	if (idx < 0)
		idx = 0;
	if (idx > FuzzyVariableBase::get_x_array_max_idx())
		idx = FuzzyVariableBase::get_x_array_max_idx();

	values[idx] = y;
 
	return 0;
};
 