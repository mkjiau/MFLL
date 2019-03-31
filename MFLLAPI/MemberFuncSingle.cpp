//
// File:	MemberFuncSingle.cpp
//
// Purpose:	Implementation of the Singelton membership function
//
// Copyright © 1999-2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//
 
#include "MemberFuncSingle.h"
#include "FuzzyModelBase.h"
#include "FuzzySetBase.h"
#include "FuzzyVariableBase.h"

#ifdef _DEBUG  
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;

#include "debug.h"

#endif


//
// Function:	MemberFuncSingle()
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
MemberFuncSingle::MemberFuncSingle(FuzzySetBase* _parent) : MemberFuncBase(_parent), FFLLBase(_parent)
{
	// create the nodes array
	alloc_nodes( get_node_count() );

} // MemberFuncSingle::MemberFuncSingle()
 

//
// Function:	~MemberFuncSingle()
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

MemberFuncSingle::~MemberFuncSingle() 
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
void MemberFuncSingle::set_node(int idx, int x, int y, bool validate /* = true */)
{
 	nodes[idx].x = x;
 
	if (idx == 0)
		nodes[idx].y = 0;

} // end MemberFuncSingle::set_node()

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

void MemberFuncSingle::init_nodes(int mid_pt_x, int term_width)
{
	nodes[0].x = mid_pt_x;  

} // end MemberFuncSingle::set_all_anchors()

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
void MemberFuncSingle::calc()
{
  	// zero out beyond the bookends...
	clear_values( );
  
	set_value(nodes[0].x,FuzzyVariableBase::get_dom_array_max_idx());
  
} // end MemberFuncSingle::calc()

  

//
// Function:	save_to_fcl_file()
// 
// Purpose:		Virtual func to write out the FCL (Fuzzy Control Language) for this membership function.
//				This is the "membership_function" part of the set's information which
//				has the format (as defined by the IEC 61131-7 International Standard):
//
//					TERM term_name:= membership_function;
//
//				Singletons are special so just the x value is written out.
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
void MemberFuncSingle::save_to_fcl_file(std::ofstream& file_contents)
{
 	RealType fvalue = get_parent()->convert_idx_to_value(get_node_x(0));

	char  val[6];
	int precision ;	// how many decimal places to show  

	if (fmod(fvalue, 1) == 0)
		precision = 0;
	else
		precision = 2;

 	// trim 'x' value to the specified precision
	sprintf( val, "%.*lf", precision, fvalue );

 	file_contents << val << ";";

} // end MemberFuncSingle::save_to_file()

/////////////////////////////////////////////////////////////////////
////////// Trivial Functions That Don't Require Headers /////////////
/////////////////////////////////////////////////////////////////////

void MemberFuncSingle::expand(int x_delta ) 
{
	// empty
} 
void MemberFuncSingle::shrink(int x_delta ) 
{
	// empty
} 

 
void MemberFuncSingle::set_ramp(int hi_lo_ind, int left_right_ind)
{
	ramp = MemberFuncBase::RAMP_NA; // N/A for this member func
}; 
// the "mid" x is sepecific to the shape... the start/end x funcs
// are in the ancestor
int MemberFuncSingle::get_center_x(void) const 
{
	return nodes[0].x;
};

int MemberFuncSingle::get_func_type() const 
{
	return MemberFuncBase::SINGLETON;
};
int MemberFuncSingle::get_node_count() const 
{ return 1;
};
