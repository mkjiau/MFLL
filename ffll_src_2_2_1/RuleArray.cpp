
//
// File:	RuleArray.cpp
//
// Purpose:	Implementation of the RuleArray class.  This class represents
//			the rules associated with a set of variables.
//
// Copyright © 1999-2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//
 
#include "RuleArray.h"
#include "FuzzyModelBase.h"
 
#ifdef _DEBUG  
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;

#include "debug.h"

#endif

//
// Function: RuleArray()
// 
// Purpose:	default constructor 
// 
// Arguments:
//
//		FuzzyModelBase* _parent - model that these rules belong to
// 
// Returns:
//
//		nothing
// 
// Author:	Michael Zarozinski	
// Date:	6/99
// 
// Modification History
// Author		Date		Modification
// ------		----		------------
//
//
RuleArray::RuleArray(FuzzyModelBase* _parent) :  FFLLBase(_parent)
{
	rules = NULL; 
	max = 0;

}; // end RuleArray::RuleArray()


//
// Function: ~RuleArray()
// 
// Purpose:	default destructor 
// 
// Arguments:
//
//		 none
//
// Returns:
//
//		nothing
// 
// Author:	Michael Zarozinski	
// Date:	6/99
// 
// Modification History
// Author		Date		Modification
// ------		----		------------
//
//
RuleArray::~RuleArray()
{
	free_memory();
};

//
// Function:	alloc()
// 
// Purpose:		This function allocates memory for the rule array
// 
// Arguments:
//
//			int	size	-	how many rules to allocate space for
// 
// Returns:
//
//			0 - success
//			non-zero - failure
//
// Author:	Michael Zarozinski
// Date:	7/8/99
// 
// Modification History
// Author		Date		Modification
// ------		----		------------
//
int RuleArray::alloc(int size) 
{
 
	// if we have rules, free the memory
	if (rules != NULL)
		{
		free_memory();
		}

	// 0 size is OK - we may not have any terms yet
	if (size <= 0)
		{
		max = 0;
		return 0;
		} 

	rules = new RuleArrayType[size]; 

	if (rules == NULL)
		{
		assert(rules != NULL);
		set_msg_text(ERR_ALLOC_MEM);
		return -1;
		}

	// set the size of the array
	max = size;

	// init the memory...
	clear();

	return 0;

} // end RuleArray::alloc()

//
// Function:	set()
// 
// Purpose:		This function sets the rules array equal to the array passed in.
// 
// Arguments:
//
//			RuleArrayType*	source	-	source data to copy
//			int				size	-	how many rules to copy.  NOTE this is NOT the total size
//										of the memory, it's the number of elements in the rules array
// 
// Returns:
//
//			0 - success
//			non-zero - failure
//
// Author:	Michael Zarozinski
// Date:	7/8/99
// 
// Modification History
// Author		Date		Modification
// ------		----		------------
//
 
int RuleArray::set(RuleArrayType* source, int size) 
{

	// allocate the rules (this deletes any existing memory)
	alloc(size);

	// copy the rules passed in
	memcpy(rules, source, sizeof(RuleArrayType) * size);
 	
	return 0;

} // end RuleArray::set()


//
// Function:	no_rules()
// 
// Purpose:		This function tells us if we have any rules that have been
//				defined for this model.
// 
// Arguments:
//
//			none
// 
// Returns:
//
//			true - if no rules are defined
//			false - if we have at least one rule
//
// Author:	Michael Zarozinski
// Date:	7/8/99
// 
// Modification History
// Author		Date		Modification
// ------		----		------------
//
bool RuleArray::no_rules() const 
{
	if (rules == NULL)
		return true;

	// loop through and find out if there are ANY rules - we need this
	// so we don't ask stuipid questions like "this will delete all rules" when
	// there are NO rules
	for (int i = 0; i < max; i++)
		{
		if (rules[i] != NO_RULE)
			return  false;
		}

	return true;
	
}; // end RuleArray::no_rules()

/////////////////////////////////////////////////////////////////////
////////// Trivial Functions That Don't Require Headers /////////////
/////////////////////////////////////////////////////////////////////

void RuleArray::clear()
{
	memset(rules, NO_RULE, max * sizeof(RuleArrayType));

}; // end RuleArray::clear()

void RuleArray::free_memory()
{
	if (rules != NULL) 
		delete[] rules;

	max = 0; 
	rules = NULL;
};

RuleArrayType RuleArray::get_rule(int index) const 
{
	assert(index < max);

	return(rules[index]);
}; 

void RuleArray::add_rule(int index, RuleArrayType output_set)
{
	assert(index < max);

	rules[index] = (RuleArrayType)(output_set);
}; 

int RuleArray::get_max() const
{
	return max;
};

void RuleArray::remove_rule(int index)
{
	assert(index < max);

	rules[index] = NO_RULE;
};

FuzzyModelBase* RuleArray::get_parent(void) const
{
	return static_cast< FuzzyModelBase*>(FFLLBase::get_parent());
};
 
const char* RuleArray::get_model_name() const
{
	FuzzyModelBase* par = get_parent();

	if (!par)
		return NULL;

	return par->get_model_name();

};