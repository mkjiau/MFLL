//
// File:	FuzzySetBase.cpp
//
// Purpose:	This file contains code for the FuzzySetBase class
//
// Copyright © 1999-2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
// 
 
#include "FuzzySetBase.h"
#include "MemberFuncTri.h"
#include "MemberFuncTrap.h"
#include "MemberFuncSCurve.h"
#include "MemberFuncSingle.h"

#include "FuzzyVariableBase.h"
 
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;

#include "debug.h"

#endif
 

//
// Function:	FuzzySetBase()
// 
// Purpose:		Constructor
//
// Arguments:
//
//		FuzzyVariableBase* par - variable that this set is part of
//
// Returns:
//
//		none
//
// Author:	Michael Zarozinski
// Date:	6/99
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
FuzzySetBase::FuzzySetBase(FuzzyVariableBase* par) : FFLLBase(par) 
{	
	rule_index = 0;
	index = 255; 
	id = L"";
	member_func = NULL;
}; 

//
// Function:	~FuzzySetBase()
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
// Date:	6/99
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
FuzzySetBase::~FuzzySetBase()
{
	if (member_func != NULL)
		delete member_func;
};


//
// Function: init()
// 
// Purpose:	Initialize set object
// 
// Arguments:
//
//		wchar_t*		n			-	identifier of the variable
//		short			idx			-	index of this set in the variable
//		int				mid_pt_x	-	middle 'x' point for this set
//		int				set_width	-	width of the set
//		int				type		-	type of membership function for this set. It should be one of
//										the enum values MemberFuncBase::TYPE (default is trangle)
//
// Returns:
//
//		0	- success
//		-1	- error
// 
// Author:	Michael Zarozinski	
// Date:	1/21/99
// 
// Modification History
// Author		Date		Modification
// ------		----		------------
//
//
 
 // 
int FuzzySetBase::init(const wchar_t* n, int mid_pt_x, short idx, int set_width, int type /* =  MemberFuncBase::TRIANGLE*/)
{
  
	// set_id() will verify unique for this variable.
	if (set_id(n))
		return -1; // id was not unique

	// perform some sanity checks on the mid point and width

	int var_width = FuzzyVariableBase::get_x_array_max_idx();

	if (set_width > var_width)
		{
		set_width = var_width / 4;
		mid_pt_x = var_width / 2;
		}

	index = idx;
	rule_index = 0;
 
	int ret_val = new_member_func(type, mid_pt_x, set_width); 

	if (ret_val)
		return ret_val;

	calc();

	return 0;

} // end FuzzySetBase::init()
 


 

//
// Function:	new_member_func()
// 
// Purpose:		Create a membership function for this set and perform some initialization
//
// Arguments:
//
//		int		type	-	type of membership function for this set. It should be one of
//							the enum values MemberFuncBase::TYPE (default is trangle)
//		int		mid_x	-	mid point for this membership function
//		int		width	-	width of the set
//
// Returns:
//
//		0 - success
//		non-zero - failure
//
// Author:	Michael Zarozinski
// Date:	7/99
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
int FuzzySetBase::new_member_func(int type, int mid_x, int width)
{

	// create the object...
	int ret_val = new_member_func(type);

	if (ret_val)
		return ret_val;
	
	// fill in relavent info...
	assert(member_func != NULL);
 	member_func->init_nodes(mid_x, width);
 
	// perform any calculations...
	calc();

	return 0;

} // end FuzzySetBase::new_member_func()

//
// Function:	new_member_func()
// 
// Purpose:		Create the appropriate membership function for this set
//
// Arguments:
//
//		int type -	type of membership function to create. This should be one
//					of the enum TYPE values in MemberFuncBase
//
// Returns:
//
//		0 - success
//		non-zero - failure
//
// Author:	Michael Zarozinski
// Date:	6/00
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
int FuzzySetBase::new_member_func(int type)
{
 
	switch (type)
			{
			case MemberFuncBase::TRIANGLE:

 				member_func = new MemberFuncTri(this);

				break;

			case MemberFuncBase::TRAPEZOID:

				member_func = new MemberFuncTrap(this);
				break;

			case MemberFuncBase::S_CURVE:

				member_func = new MemberFuncSCurve(this);

				break;

			case MemberFuncBase::SINGLETON:
 
				member_func = new MemberFuncSingle(this);
 
				break;

			default:
				// default to a triangle
				member_func = new MemberFuncTri(this);
		
				break;

			} // end switch on member_func_type;
 		
	if (!member_func)
		{
		// problem allocating memory
		// NOTE: in MSVC new returns NULL if there's not enough memory. If this is ported
		// to a diff platform new may throw a std::bad_alloc exception it it can't alloc the memory.
 		set_msg_text(ERR_ALLOC_MEM);
		assert(member_func != NULL);
		return -1;
		} // end if error allocating memory

	member_func->init();	// perform any initialization
	calc();

	return 0;

} // end FuzzySetBase::new_member_func()
 
//
// Function:	set_id()
// 
// Purpose:		Set the identifier for this set, making sure it's unique
//				if we don't allow duplicated ids.
//
// Arguments:
//
//		const wchar_t*	_id			-	identifier for the set
//		bool			allow_dup	-	if true, allow duplicate identifiers within the variable (default is false)
//
// Returns:
//
//		0  - set id
//		-1 - identifier is not unique
//
// Author:	Michael Zarozinski
// Date:	
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
int FuzzySetBase::set_id(const wchar_t* _id, bool allow_dup /* = false*/)
{
	// if parent is null just assign the id
 
	// only check if the id is unique if we don't allow duplicates
	if (!allow_dup)
		{
		FuzzyVariableBase* par = get_parent();

		assert(par != NULL);
	
		if (!(par->is_set_id_unique(_id, index)))
			{
			// read the error from the parent and set it for 'this'
			set_msg_text(par->get_msg_text());
 			return -1;  
			}		
		} // if don't allow duplicates

	id = _id;	// only set if unique
	return 0;

}; // end FuzzySetBase::set_id() 

 
//
// Function:	copy()
// 
// Purpose:		Copy all values from the set passed in to this set
//
// Arguments:
//
//		const FuzzySetBase& copy_from - set to copy information from
//
// Returns:
//
//		0 - success
//		non-zero - failure	
//
// Author:	Michael Zarozinski
// Date:	5/00
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//

int FuzzySetBase::copy(const FuzzySetBase& copy_from)
{ 
	// set this set's index to be the next for the variable we're part of
	index = get_parent()->get_num_of_sets();

	// don't copy rule_index - that's calculated
 
 	set_id(copy_from.get_id(), true); // true allows us to have a duplicate id 

	// create the member function

	int ret_val = new_member_func(copy_from.get_func_type());

	if (ret_val)
		return ret_val;
 
	// initialize all the nodes to reasonable values before moving to the specific
	// values. This helps get around any problems we may have with validation when we
	// call set_node() below.
 	member_func->init_nodes(copy_from.member_func->get_center_x(), 
		copy_from.member_func->get_end_x() - copy_from.member_func->get_start_x());
 
	// copy the nodes...
	for (int i = 0; i < copy_from.get_node_count(); i++)
		{
		NodePoint node = copy_from.member_func->get_node(i);
 		member_func->set_node (i, node.x,  node.y);
		} // end loop through nodes

	return 0;

} // end  FuzzySetBase::copy()


//
// Function:	save_to_fcl_file()
// 
// Purpose:		Write out the FCL (Fuzzy Control Language) for this set. This is written
//				inside a FUZZIFY block and has the format (as defined by the
//				IEC 61131-7 International Standard):
//
//					TERM term_name:= membership_function;
//
//				Where membership_function is the point that define the membership function curve.
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

void FuzzySetBase::save_to_fcl_file(std::ofstream& file_contents)
{
	// we save all points.  The FCL allows for assumptions for ramps so you
	// don't have to specifiy start/end points.  The IEC 61131-7 says:
	// If the value of a linguistic variable is less than the first base point in the look-up table, all
	// values below the first point in the look-up table shall have the same membership degree as	
	// defined at the first point.

	// convert id to ascii version replacing any spaces with underscores
 	char* aid = convert_to_ascii(get_id(), '_');
 
	file_contents << "\tTERM " << aid << " := ";

	delete[] aid;

	member_func->save_to_fcl_file(file_contents);

} // end FuzzySetBase::save_to_fcl_file()

/////////////////////////////////////////////////////////////////////
////////// Trivial Functions That Don't Require Headers /////////////
/////////////////////////////////////////////////////////////////////

FuzzyModelBase* FuzzySetBase::get_rule_block()
{
	FuzzyVariableBase* var = get_parent();

	assert(var != NULL);
 
	return (FuzzyModelBase*)(var->get_parent());
}

bool FuzzySetBase::is_set_id_unique() const
{
	// make sure we're part of a variable
	assert(get_parent() != NULL);
	int ret_val = get_parent()->is_set_id_unique(id.data(), index);

	if (ret_val)
		{
		// read the error from the parent and set it for 'this'
		set_msg_text(get_parent()->get_msg_text());
		}

	return (ret_val) ? true : false;
};

bool FuzzySetBase::is_output() const
{
	return get_parent()->is_output();
}
  
RealType  FuzzySetBase::get_left_x() const
{
	return get_parent()->get_left_x();
};
RealType FuzzySetBase::get_idx_multiplier() const
{
	return get_parent()->get_idx_multiplier();
};

const char* FuzzySetBase::get_model_name() const
{
	return get_parent()->get_model_name();	
};
int FuzzySetBase::get_var_index() const
{ 
	return get_parent()->get_index();
};
 
DOMType FuzzySetBase::get_dom(int idx) const
{
	return(member_func->get_value(idx));
};
DOMType FuzzySetBase::get_index() const 
{
	return index;
};
void FuzzySetBase::set_index(int _idx)
{
	index = _idx;
};
int FuzzySetBase::get_node_x(int node_idx) const
{
	return member_func->get_node_x(node_idx);
};
DOMType FuzzySetBase::get_value(int idx) const
{
	return(member_func->get_value(idx));
};
void FuzzySetBase::calc()
{
	if (member_func)
		member_func->calc();
};
		
FuzzyVariableBase* FuzzySetBase::get_parent() const
{
	return static_cast<FuzzyVariableBase*>(FFLLBase::get_parent());
};

const wchar_t* FuzzySetBase::get_id(void) const
{
	return((id == L"") ? NULL :  (id.c_str()));
}; 

int FuzzySetBase::get_end_x(void) const
{
	return member_func->get_end_x();
};


int FuzzySetBase::get_start_x(void) const
{
	return member_func->get_start_x();
};
int FuzzySetBase::get_func_type() const 
{
	return member_func->get_func_type();
};
 
void FuzzySetBase::move_node(int  idx, int x, int y)
{
	member_func->move_node(idx, x, y);
	calc(); 
};
void FuzzySetBase::move_node(int anchor_idx, _point pt)
{
	move_node(anchor_idx, pt.x, pt.y); 
 
};
 void FuzzySetBase::move(int x_delta)
{
	member_func->move(x_delta);
	calc();
};
void FuzzySetBase::set_ramp(int hi_lo_ind, int left_right_ind)
{
	member_func->set_ramp(hi_lo_ind, left_right_ind);
	calc();
};
int FuzzySetBase::get_ramp()
{
	return member_func->get_ramp();
};
 
FuzzySetBase& FuzzySetBase::operator=(const FuzzySetBase& copy_from)
{
	return *this;
};

RealType FuzzySetBase::convert_idx_to_value(int idx) const
{
	assert(idx >= 0);
	assert(idx <= FuzzyVariableBase::get_x_array_count());

	return get_parent()->convert_idx_to_value(idx);

};  

void FuzzySetBase::expand(int x_delta)
{
	member_func->expand(x_delta);
	calc();
}; 
void FuzzySetBase::shrink(int x_delta  )
{
	member_func->shrink(x_delta );
	calc();
}; 
void FuzzySetBase::set_member_func(void* new_func)
{
	 delete member_func; member_func = static_cast<MemberFuncBase*>(new_func);
}; 
void FuzzySetBase::set_rule_index(int idx)
{
	rule_index = idx;
};  
int FuzzySetBase::get_rule_index() const 
{ 
	return rule_index;
};  
int FuzzySetBase::get_node_count() const 
{ 
	return member_func->get_node_count();
};

NodePoint FuzzySetBase::get_node(int idx)
{
	return member_func->get_node(idx);
};
	
void FuzzySetBase::set_node(int idx, int x, int y, bool validate /* = true */)
{ 
	member_func->set_node(idx, x, y, validate);
};
