
//
// File:	FuzzyOutSet.cpp
//
// Purpose:	This file contains  code for the FuzzyOutSet class
//
// Copyright © 1999-2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
// 

#include "FuzzyOutSet.h"
#include "COGDefuzzSetObj.h"
#include "MOMDefuzzSetObj.h"
#include "FuzzyOutVariable.h"
 

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

#include "debug.h"

#endif

//
// Function:	FuzzyOutSet()
// 
// Purpose:		FuzzyOutSet constructor
//
// Arguments:
//
//		FuzzyVariableBase* par - parent of this set
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
FuzzyOutSet::FuzzyOutSet(FuzzyVariableBase* par) : FuzzySetBase(par), FFLLBase(par) 
{
	defuzz_obj = NULL; 

	//  default defuzzification method to the parent's method
	int type = (dynamic_cast<FuzzyOutVariable*>(par))->get_defuzz_method();

	set_defuzz_method(type);

}; // end FuzzyOutSet::FuzzyOutSet()

//
// Function:	~FuzzyOutSet()
// 
// Purpose:		FuzzyOutSet destructor
//
// Arguments:
//
//		FuzzyVariableBase* par - parent of this set
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
FuzzyOutSet::~FuzzyOutSet()
{
	if (defuzz_obj)
		delete defuzz_obj;

}; // end FuzzyOutSet::~FuzzyOutSet()

//
// Function:	set_defuzz_method()
// 
// Purpose:		Set the defuzzification method for this set. This involves
//				creating the correct DefuzSetObj object for the set.
//
// Arguments:
//
//		int type - defuzzification method
//
// Returns:
//
//		0 - success
//		non-zero - failure
//
// Author:	Michael Zarozinski
// Date:	8/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//		
int FuzzyOutSet::set_defuzz_method(int type)
{
	// free the previous object
	if (defuzz_obj)
		{
		delete defuzz_obj;
		defuzz_obj = NULL;
		}

	// build teh correct type of defuzz object based on the method passed in
	switch(type)
		{
		case DefuzzVarObj::DEFUZZ_COG:

			// create COG memory
			defuzz_obj = new COGDefuzzSetObj(this);
			dynamic_cast<COGDefuzzSetObj*>(defuzz_obj)->init(FuzzyVariableBase::get_dom_array_count());

			break;

		case DefuzzVarObj::DEFUZZ_MOM:

			// create COG memory
			defuzz_obj = new MOMDefuzzSetObj(this);

			break;

		default:
			set_msg_text(ERR_INVALID_DEFUZZ_MTHD);
			return -1;

		} // end switch on type

	if (!defuzz_obj)
		{
		// problem allocating memory
		// NOTE: in MSVC new returns NULL if there's not enough memory. If this is ported
		// to a diff platform new may throw a std::bad_alloc exception it it can't alloc the memory.
 		set_msg_text(ERR_ALLOC_MEM);
		assert(defuzz_obj != NULL);
		return -1;
		} // end if error allocating memory

	// call calc()
	calc();

	return 0;

} // end FuzzyOutSet::set_defuzz_method()

//
// Function:	calc()
// 
// Purpose:		Calculate the defuzzification look-up table (or
//				whatever calc are needed for the specific defuzzification
//				method) for this set.
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
// Date:	8/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//		
void FuzzyOutSet::calc()
{
	// if we don't have a member_func yet, nothing to calculate

	if (!member_func)
		return;

	// call base version
	FuzzySetBase::calc();

	// calc the defuzzifcation stuff
	if (defuzz_obj)
		defuzz_obj->calc();	

}; // end FuzzyOutSet::calc()

//
// Function:	get_defuzz_x()
// 
// Purpose:		This fuction gets the defuzzified 'x' value for a DOM passed in.
//				it's a generic function so the caller does not need to know the
//				defuzzification method used.
//
// Arguments:
//
//		int		dom - 'y' value to get the 'x' value for (default: -1)
//
// Returns:
//
//		RealType - the defuzzified 'x' value for the set, FLT_MIN if something went wrong
//
// Author:	Michael Zarozinski
// Date:	12/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//		
RealType FuzzyOutSet::get_defuzz_x(int dom /* = -1 */)
{
 
	// get the defuzzification method...

	FuzzyOutVariable* parent = get_parent();

	DefuzzSetObj* defuzz_base = get_defuzz_obj();

	int defuzz_type = defuzz_base->get_defuzz_type();

	if (defuzz_type == DefuzzVarObj::DEFUZZ_COG && dom < 0)
		return FLT_MIN; // invalid dom

	if (defuzz_type == DefuzzVarObj::DEFUZZ_COG)
		{
		COGDefuzzSetObj* cog_defuzz = dynamic_cast<COGDefuzzSetObj*>(defuzz_base);
		
		return cog_defuzz->get_defuzz_x(dom); 
 
		} // end if CoG

	if (defuzz_type == DefuzzVarObj::DEFUZZ_MOM)
		{
		MOMDefuzzSetObj* mom_defuzz = dynamic_cast<MOMDefuzzSetObj*>(defuzz_base);

		return mom_defuzz->get_mean_value(); 

		} // end if MoM

	// should never get her, but just in case...
	return FLT_MIN;  

}; // end FuzzyOutSet::get_defuzz_x()
		
 

/////////////////////////////////////////////////////////////////////
////////// Trivial Functions That Don't Require Headers /////////////
/////////////////////////////////////////////////////////////////////

DefuzzSetObj* FuzzyOutSet::get_defuzz_obj() const
{ 
	return defuzz_obj; 
};
	
FuzzyOutVariable* FuzzyOutSet::get_parent() const
{
	return dynamic_cast<FuzzyOutVariable*>(FuzzySetBase::get_parent());
};
