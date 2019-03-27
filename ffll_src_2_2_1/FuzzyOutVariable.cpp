
//
// File:	FuzzyOutVariable.cpp
//
// Purpose:	This file contains  code for the FuzzyOutVariable class
//
// Copyright © 1999-2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//
  
#include "FuzzyModelBase.h"
#include "FuzzySetBase.h"
#include "FuzzyOutVariable.h"
#include "COGDefuzzVarObj.h"
#include "MOMDefuzzVarObj.h"
#include "FuzzyOutSet.h"

 
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

#include "debug.h"

#endif

//
// Function: FuzzyOutVariable()
// 
// Purpose:	default constructor 
// 
// Arguments:
//
//		FuzzyModelBase* _parent - model that this variable is part of
// 
// Returns:
//
//		nothing
// 
// Author:	Michael Zarozinski	
// Date:	08/05/01
// 
// Modification History
// Author		Date		Modification
// ------		----		------------
//
//

FuzzyOutVariable::FuzzyOutVariable(FuzzyModelBase* _parent) : FuzzyVariableBase(_parent), FFLLBase(_parent) 
{
	defuzz_obj = NULL;

	// default to MAX composition
	composition_method =  COMPOSITION_OPERATION_MAX;

	// default to Center of Gravity defuzzification
	set_defuzz_method( DefuzzVarObj::DEFUZZ_COG);
 
}; // end FuzzyOutVariable::FuzzyOutVariable()

//
// Function: ~FuzzyOutVariable()
// 
// Purpose:	default destructor 
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
// Date:	08/05/01
// 
// Modification History
// Author		Date		Modification
// ------		----		------------
//
//
FuzzyOutVariable::~FuzzyOutVariable()
{
	if (defuzz_obj)
		delete defuzz_obj;

}; // FuzzyOutVariable::~FuzzyOutVariable()


//
// Function:	set_defuzz_method()
// 
// Purpose:		Set the defuzzification method for the output variable (and sets)
// 
// Arguments:
//
//		int type - type of defuzzification. This should be one of the 
//		DefuzzVarObj::DEFUZZ_TYPE enum values
// 
// Returns:
//
//		0 - success
//		non-zero - failure
// 
// Author:	Michael Zarozinski	
// Date:	08/01
// 
// Modification History
// Author		Date		Modification
// ------		----		------------
//
//
int FuzzyOutVariable::set_defuzz_method(int type)
{

	// if we have a defuzz object, delete it cuz we're gonna
	// create one below based on the defuzz method passed in
	if (defuzz_obj)
		{
		// if we already have this type of defuzz, nothing to do...
		if (defuzz_obj->get_defuzz_type() == type)
			return 0; // nothing to do

		// delete the old one...
		delete defuzz_obj;
		defuzz_obj = NULL;

		} // end if we have an existing defuzz object
 
	switch(type)
		{
		case DefuzzVarObj::DEFUZZ_COG:
			
			defuzz_obj = new COGDefuzzVarObj(this);
			break;

		case DefuzzVarObj::DEFUZZ_MOM:

			defuzz_obj = new MOMDefuzzVarObj(this);
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

 	FuzzyOutSet*	set;	// output set

	// loop through all the sets and change the defuzz method 
	for (int i = 0; i < get_num_of_sets(); i++)
		{
		set = get_set(i);

		set->set_defuzz_method(type);

		} // end loop through sets

	return 0;

} // end FuzzyOutVariable::set_defuzz_method()
 

//
// Function:	set_composition_method()
// 
// Purpose:		Set the composition method for the output variable
// 
// Arguments:
//
//		int method - method of composition. This should be one of the 
//		FuzzyOutVariable::COMPOSITION_OPERATION enum values
// 
// Returns:
//
//		0 - success
//		non-zero - failure
// 
// Author:	Michael Zarozinski	
// Date:	08/01
// 
// Modification History
// Author		Date		Modification
// ------		----		------------
//
//

int FuzzyOutVariable::set_composition_method(int method)	
{
	// validate

	switch (method)
		{
		case COMPOSITION_OPERATION_MIN:
		case COMPOSITION_OPERATION_MAX:
			composition_method = method;
			break;

		default:
			// invalid method  
			set_msg_text(ERR_INVALID_COMP_MTHD);
			return -1;	

		} // end switch on method
	
	return 0;

} // end FuzzyOutVariable::set_composition_method

//
// Function:	calc_output_value()
// 
// Purpose:		Calculate the defuzzified value for this output variable
//
// Arguments:
//
//		DOMType* out_set_dom_arr - array of DOM values for each set in the output variable
//
// Returns:
//
//		RealType - the defuzzifed value. FLT_MIN is returned if no output sets are active
//
// Author:	Michael Zarozinski
// Date:	7/00
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
RealType FuzzyOutVariable::calc_output_value(DOMType* out_set_dom_arr ) const
{
 	assert(defuzz_obj);

	RealType out_value = defuzz_obj->calc_value(out_set_dom_arr);

	// Note:  if we don't have an output value FLT_MIN is returned from above func call
	return out_value;

}; // end FuzzyOutVariable::calc_output_value()

 

//
// Function: init()
// 
// Purpose:	Initialize variable identifier and make it unique if applicable
// 
// Arguments:
//
//		const wchar_t*	_id					-	identifier for the variable
//		bool			create_unique_id	-	if true, alter the identifier (if needed) to make it unique) default = true
//
// Returns:
//
//		0 - success
//		non-zero - failure
// 
// Author:	Michael Zarozinski	
// Date:	1/99
// 
// Modification History
// Author		Date		Modification
// ------		----		------------
//
//
		
int FuzzyOutVariable::init(const wchar_t* _id, bool create_unique_id /* = true */)
{
	int name_len;	// length of the name

	if (_id == NULL)
		name_len = 30;
	else
		name_len = wcslen(_id) + 5;

	wchar_t* tmp_name = new wchar_t[name_len]; // temp var to hold var name

	// if no name is passed in name it "Output"
 	if (_id == NULL)
		swprintf(tmp_name, name_len, L"%ls", load_string(STR_OUTPUT));  
	else
		swprintf(tmp_name, name_len, L"%ls", _id );  
 	
 	// set the name.  NOTE: this set_id() performs a check to ensure that the variable name is UNIQUE
	// for this model.  

	int ret_value; // return value

	if (!create_unique_id)
		{
		ret_value = set_id(tmp_name);
		}
	else
		{
		ret_value = 0;

 		int counter = 0; // value to add to identifier to make it unique

		while (set_id(tmp_name))
			{
			// rename variable...
			if (_id == NULL)
				swprintf(tmp_name, name_len, L"%ls %d", load_string(STR_OUTPUT), counter  );  
 			else
				swprintf(tmp_name, name_len, L"%ls %d", _id, counter );  

			counter++;
 
			// clear message text (should say "var not unique" right now)
			set_msg_text(NULL);
			
			} // end while !unique id

		}// end if rename var if not unique

	// free memory
	delete[] tmp_name;

	return ret_value;

} // end FuzzyOutVariable::init()


//
// Function: convert_idx_to_value()
// 
// Purpose:	This function converts an index (into the values[] array) to 
//			an 'x' value between left_x and right_x for this variable
//			NOTE: this is the OUTPUT var version. -1 is a valid "current_x_value"
//			value for it (if no output sets are active) so we handle it special.
// 
// Arguments:
//
//		int idx -	index to convert to a value
//
// Returns:
//
//		RealType - x value for the index passed in
// 
// Author:	Michael Zarozinski	
// Date:	11/28/01
// 
// Modification History
// Author		Date		Modification
// ------		----		------------
//
//

RealType FuzzyOutVariable::convert_idx_to_value(int idx) const
{
	assert(idx >= -1);

	if (idx == -1)
 		return FLT_MIN;
 	else
		return FuzzyVariableBase::convert_idx_to_value(idx);

};

/////////////////////////////////////////////////////////////////////
////////// Trivial Functions That Don't Require Headers /////////////
/////////////////////////////////////////////////////////////////////

FuzzyOutSet* FuzzyOutVariable::get_set(int idx) const
{
	return dynamic_cast<FuzzyOutSet*>(FuzzyVariableBase::get_set(idx));
}  

int FuzzyOutVariable::get_defuzz_method()
{
	if (defuzz_obj)
		return defuzz_obj->get_defuzz_type();
	else
		return -1;
}
	
bool FuzzyOutVariable::is_output() const 
{
	return true;
}; 
int FuzzyOutVariable::get_composition_method()
{ 
	return composition_method;
}
FuzzySetBase* FuzzyOutVariable::new_set()
{
	return new FuzzyOutSet(this);
};
const char* FuzzyOutVariable::get_fcl_block_start()    
{ 
	return "VAR_OUTPUT"; 
};		
const char* FuzzyOutVariable::get_fcl_block_end() 
{ 
	return "END_VAR"; 
};

int FuzzyOutVariable::init(const wchar_t* _id, RealType _left_x, RealType _right_x, bool create_unique_id /* = true */)
{
	// call ancestore
	return FuzzyVariableBase::init(_id, _left_x, _right_x, create_unique_id);
};
