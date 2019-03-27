//
// File:	FuzzyModelBase.cpp
//
// Purpose:	Implementation of the FuzzyModelBase class.  This class represents
//			the rules associated with a set of variables
//
// Copyright ?1999-2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//

#include "FuzzyModelBase.h"
#include "FuzzySetBase.h"
#include "FuzzyOutVariable.h"
#include "RuleArray.h"
#include "DefuzzVarObj.h"

//#include <fstream> // ??? moved to .h
#include <time.h>
#include <math.h>

#ifdef _DEBUG  
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;

#include "debug.h"

#endif

//
// Function:	~FuzzyModelBase()
// 
// Purpose:		Destructor for FuzzyModelBase
//
// Arguments:
//
//				none
//
// Returns:
//
//				nothing
//
// Author:	Michael Zarozinski
// Date:	5/00
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
FuzzyModelBase::~FuzzyModelBase()
{
	// remove variables and perform any clean up
	delete_vars();

	if (rules)
		{
		delete rules;
		rules = NULL;
		}

}; // end FuzzyModelBase::~FuzzyModelBase()

 

//
// Function:	FuzzyModelBase()
// 
// Purpose:		Default constructor for FuzzyModelBase class
//
// Arguments:
//
//			none
//
// Returns:
//
//			nothing
//
// Author:	Michael Zarozinski
// Date:	5/00
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
FuzzyModelBase::FuzzyModelBase() : FFLLBase(NULL)
{

#ifdef TEST_MEM_LEAK_REPORTING
	char *mem_leak = new char[43];
#endif

	input_var_count = 0; 

  	rules = NULL;  
	input_var_arr = NULL;
	output_var = NULL;

	model_name = ""; // clear out file name
 
	// default to MIN inference
 	inference_method = INFERENCE_OPERATION_MIN;
 
} // end FuzzyModelBase::FuzzyModelBase()


//
// Function:	delete_vars()
// 
// Purpose:		Delete all the variables for the model and clean
//				up the memory they use.
//
// Arguments:
//
//			none
//
// Returns:
//
//			void
//
// Author:	Michael Zarozinski
// Date:	5/00
//	
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
void FuzzyModelBase::delete_vars()
{

	// if we have input variables, delete them
	if (input_var_arr)
		{
		for (int i = 0; i < input_var_count; i++)
			{
			if (input_var_arr[i])
				{
				delete input_var_arr[i];
				input_var_arr[i] = NULL;
				}
			} // end loop through input vars
		delete[] input_var_arr;
		input_var_arr = NULL;
		}

	// if we have an output variable, delete it
	if (output_var)
		{
		delete output_var;
		output_var = NULL;
		}

} // end FuzzyModelBase::delete_vars()
 
//
// Function:	add_input_variable()
// 
// Purpose:		Create and add an input variable to this model. This automatically
//				renames the identifier if it conflicts with an existing one.
//
// Arguments:
//
//			const wchar_t*	_name				-	name of the variable we're adding. Note this can be NULL
//													in which case it'll be named "Variable X" AND the name passed
//													in may not be the final name because variable names MUST be
//													unique within the model
//			RealType		start_x				-	Starting (left most) 'x' value for the variable
//			RealType		end_x				-	Ending (right most) 'x' value for the variable
//			bool			create_unique_id	-	if true, alter the identifier (if needed) to make it unique) default = true
//
// Returns:
//
//		0 if success
//		non-zero if failure
//
// Author:	Michael Zarozinski
// Date:	5/00
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
int FuzzyModelBase::add_input_variable(const wchar_t* _name, RealType start_x, RealType end_x, bool create_unique_id /* = true */)
{ 
 	FuzzyVariableBase*	var = new_variable();
   
	if (var->init(_name, start_x, end_x, create_unique_id))
		{
		delete var;
		return -1;
		}

	add_input_var_to_list(var);
 
	return 0;
 
} // end FuzzyModelBase::add_input_variable()

//
// Function:	add_output_variable()
// 
// Purpose:		Create and add an output variable to this model. This automatically
//				renames the identifier if it conflicts with an existing one.
//
// Arguments:
//
//			const wchar_t*	_name				-	name of the variable we're adding. Note this can be NULL
//													in which case it'll be named "Variable X" AND the name passed
//													in may not be the final name because variable names MUST be
//													unique within the model
//			RealType		start_x				-	Starting (left most) 'x' value for the variable
//			RealType		end_x				-	Ending (right most) 'x' value for the variable
//			bool			create_unique_id	-	if true, alter the identifier (if needed) to make it unique) default = true
//
// Returns:
//
//		0 if success
//		non-zero if failure
//
// Author:	Michael Zarozinski
// Date:	5/00
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
int FuzzyModelBase::add_output_variable(const wchar_t* _name, RealType start_x, RealType end_x, bool create_unique_id /* = true */)
{
	// return error if we already have an output variable
	if (output_var)
		{
		set_msg_text(ERR_OUT_VAR_EXISTS);
		return -1;
		}
  
 	output_var = new_output_variable();

  	if (output_var->init(_name, start_x, end_x, create_unique_id))
		{
		delete output_var;
		output_var = NULL;
		return -1;
		}
  
	calc_rule_index_wrapper();

	return 0;

} // end FuzzyModelBase::add_output_variable()
	

//
// Function:	delete_variable()
// 
// Purpose:		This functions removes a single variable
//				from the model and adjusts memory and rules appropriately.
//				This is very different (despite the naming) from delete_vars().
//				That is called during the destruction of the model, this is used
//				to remove a specific variable from the model.
//
// Arguments:
//
//		int _var_idx - index of the variable to remove
//
// Returns:
//
//		0 on success
//		non-zero on failure
//
// Author:	Michael Zarozinski
// Date:	5/00
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//	
int FuzzyModelBase::delete_variable(int _var_idx )
{
	int		i, j;			// counter
	int		new_mem_size;	// size of new rules array

	// don't allow del of output var...

	FuzzyVariableBase*	var= get_var(_var_idx);	// var we're dealing with
 
	// don't allow removal of output variable
	if (var->is_output())
		{
		set_msg_text(ERR_CANT_DEL_OUTPUT_VAR);
		return -1;
		}

	int new_var_count = input_var_count - 1;
 	// need to create shrunken memory
	FuzzyVariableBase** tmp_var;
	
	if (new_var_count)
		tmp_var = new FuzzyVariableBase*[new_var_count];  
	else
		tmp_var = NULL; // no vars!

	if (!tmp_var && new_var_count)
		{
		// problem allocating memory
		// NOTE: in MSVC new returns NULL if there's not enough memory. If this is ported
		// to a diff platform new may throw a std::bad_alloc exception it it can't alloc the memory.
 		set_msg_text(ERR_ALLOC_MEM);
		assert(tmp_var != NULL);
		return -1;

		} // end if error allocating memory

	// copy the old memory to the new mem..
	j = 0;
	for (i = 0; i < input_var_count; i++)
		{
		if (i == _var_idx)
			{
			// this is the var we want to delete...
			delete input_var_arr[i];
			input_var_arr[i] = NULL;
			continue;	 
			}

		tmp_var[j] = input_var_arr[i];
		// reset the index of each var...
		tmp_var[j]->set_index(j);
		j++;

		} // end loop through input variables

	input_var_count--;  // increment the number of variables we have
	
	// set the old mem to the new mem...

	delete[] input_var_arr; // free old mem

	input_var_arr = tmp_var;

	// calculate how big the rules array needs to be...
	new_mem_size = calc_num_of_rules();
 
	if (new_mem_size)
		{
		// this will free mem, re-allocate it, and clear it cuz
		// deleting a var deletes all rules...
		rules->alloc(new_mem_size);

		// re-calc at the variable index factors...
		calc_rule_index_wrapper();
		}

	return 0;

} // end FuzzyModelBase::delete_variable()
 
//
// Function:	calc_rule_components(
// 
// Purpose:		Determines which sets comprise a rule.
//
// Arguments:
//
//		int		rule_index		- index of the rule to get the components for
//		int*	set_idx_array	-		Array with N elements where N = the number of input variables.
//										Each element corresponds to an input variable and the set in that
//										variable that is involved in this rule is stored in that element.
//
// Returns:
//
//		void
//
// Author:	Michael Zarozinski
// Date:	5/00
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
void FuzzyModelBase::calc_rule_components(int rule_index, int* set_idx_array) const
{
 	int		i;		// counter
	int		accum = 0;	// accumulator 
	int last_in_var_idx = input_var_count - 1; // special value to let us know we're at the LAST array element

	// deal with special cases (first and last var) up front for speed considerations...
	set_idx_array[0] = rule_index/get_rule_index(0);
	set_idx_array[last_in_var_idx] = rule_index % input_var_arr[last_in_var_idx]->get_num_of_sets();

 	// loop through the rest of the vars...
 
	// below is a slightly faster (or at least fewer assemby instruction) version of:
	//
	//	for (i = 1; i < last_in_var_idx; i++)
	//		{
	//		accum += set_idx_array[i-1] * get_rule_index(i-1);
	//		set_idx_array[i] = (rule_index - accum)/get_rule_index(i);
	//		}  
	// I'll leave that version of the algorithm to help people figure out what I'm doing below

	i = 0;
	while (i < last_in_var_idx )
		{
		accum += set_idx_array[i] * get_rule_index(i);
		i++;
 		set_idx_array[i] = (rule_index - accum)/get_rule_index(i);
		} // end loop through vars
  
} // end FuzzyModelBase::calc_rule_components()

  
//
// Function:	calc_rule_index_wrapper()
// 
// Purpose:		Calculates the rule_index value for all the variables
//				and the variable's sets. See FuzzySetBase.h for a detailed
//				explaination on how "rule_index" is used
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
// Date:	6/00
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
void FuzzyModelBase::calc_rule_index_wrapper(void)
{ 
 	int		factor;		// factor we've calculated

	// go through all the input variables...
	for (int i = 0; i < input_var_count; i++)
		{
		// get the rule index for this variable

		// 'i' is the index of the variable we want to find the rule_index for, BUT we incrment it by
		// one because we do NOT want to include that variable in the calculation. rule_index only
		// needs to take into account the variables after the one we are calculating for.
	 
		factor = calc_rule_index(i + 1);

		// set the variable's rule_index
		input_var_arr[i]->set_rule_index(factor);

		// now that we have the factor for the variable... 
		// set the rule_index value for each set
		for (int j = 0; j < input_var_arr[i]->get_num_of_sets(); j++)
			{
 			input_var_arr[i]->set_rule_index(factor * j, j);
			} // end loop through sets

		} // end loop through variables

 
} // end FuzzyModelBase::calc_rule_index_wrapper()

 
//
// Function:	calc_rule_index()
// 
// Purpose:		Recursive function that calculates the rule_index for a variable.
//				See FuzzySetBase.h for a detailed explaination rule_index and how it's used
//
// Arguments:
//
//		int var_idx - index of the variable to calc the rule_index for
//
// Returns:
//
//		int - the rule_index for the variable
//
// Author:	Michael Zarozinski
// Date:	5/00
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
int FuzzyModelBase::calc_rule_index(int var_idx)
{
 
	// last var... we're done recursing
	if (var_idx >= input_var_count)
		{
		return 1;
		}
 
	int	num_of_sets = input_var_arr[var_idx]->get_num_of_sets();

	// call ourselves recursively cuz we need to find the values for the variables below
 
	return(num_of_sets * calc_rule_index(var_idx + 1));

} // end  FuzzyModelBase::calc_rule_index()


//
// Function:	add_set()
// 
// Purpose:		Insert a set into the variable associated with the index
//				passed in. This also adjusts the rules array appropriately.
//				NOTE: that a COPY of the set passed in is inserted into 
//				the var so the caller must free the object if it dynamically allocated it.
//
// Arguments:
//
//		int					_var_idx	- index of the variable to insert the set into
//		const FuzzySetBase* _set		- set to insert a copy of
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
int FuzzyModelBase::add_set(int _var_idx,  const FuzzySetBase* _set) 
{
 	RuleArrayType*		new_mem = NULL;		// pointer to the new memory
	int					new_mem_size;		// size of the new memory we're allocating
  
	FuzzyVariableBase* var = get_var(_var_idx); 

	// _set is NOT modified in add_set, so we can NOT rely on it's index value or
	// anything. add_set() adds the set so we must use that for any furthur information
	// we need.

	int ret_val = var->add_set(_set);  
	
	if (ret_val)
		return ret_val;

	FuzzySetBase* new_set = var->get_set(var->get_num_of_sets() - 1);

	calc_rule_index_wrapper(); // re-calc rule_index values now that we have a new set

	// if this is an output var we don't need to assign any more memory
	if (var->is_output())
		return 0;

	// if we got to this point, we're dealing with an input variable

	// calc the size of the new memory block
	new_mem_size = calc_num_of_rules();
 
	if (new_mem_size == 0)
		return 0; // nothing more to do...

	// allocate and init the new memory for rules...
	new_mem = new RuleArrayType[new_mem_size];;
	memset(new_mem, NO_RULE, new_mem_size * sizeof(RuleArrayType));

	assert(new_mem != NULL);
 
	// IF we have rules to copy...

	if (rules->get_max())
		{
 		// rather than stick the new memory in the middle of the existing mem we'll just
		// copy the needed data...
		int* set_idx_array = new int[get_input_var_count()]; // array of the sets involved in the current rule
  
		int	rule_index = 0;	// index for the rule into the OLD memory
		
 		for (int new_mem_idx = 0; new_mem_idx < new_mem_size; new_mem_idx++)
			{
			// we've added the set so this gets the rule components for the
			// rule index with the new set added
			calc_rule_components(new_mem_idx, set_idx_array);

			// if this index involves the NEW set leave it as NO_RULE
			if (set_idx_array[_var_idx] == new_set->get_index())
 				continue;
 
			// copy the rule from the OLD array to the NEW array
 				
			new_mem[new_mem_idx] = rules->get_rule(rule_index);

			// increment the index into the OLD rules...
			rule_index++;

			} // end looop through new mem

		delete[] set_idx_array;
		} // end if we have rules
 
	// now set the rules to the new rule array with the old rules copied in
	rules->set(new_mem, new_mem_size);

	// free the new rules we allocated
	delete[] new_mem;
 
	return 0;

} // end FuzzyModelBase::add_set()
 
 
//
// Function:	delete_set()
// 
// Purpose:		Delete a set from the variable associated with the index
//				passed in. This also adjusts the rules array appropriately.
//
// Arguments:
//
//		int					_var_idx	- index of the variable to remove the set from
//		int					_setr_idx	- index of the set to remove from the variable associated with _var_idx
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
int FuzzyModelBase::delete_set(int _var_idx, int _set_idx)
{
 	RuleArrayType*	new_mem = NULL;		// pointer to the new memory
	int					new_mem_size;		// size of the new memory we're allocating
  	int ret_val;

	FuzzyVariableBase* var = get_var(_var_idx);  

	assert(var != NULL);

	// if this is an output var we don't need to remove any memory, but we
	// need to adjust existing rules
	if (var->is_output())
		{
		// go through all rules and remove any references to this rule
		// AND if any rule is GREATER than the one we're deleting, decrement
		// it to adjust for the removed rule

		RuleArrayType out_set;	// output set in rules array
		
		int num_rules = rules->get_max();

		for (int rule_index = 0; rule_index < num_rules; rule_index++)
			{
			out_set = rules->get_rule(rule_index);
			if (out_set == _set_idx)
				rules->add_rule(rule_index, NO_RULE);
			else if (out_set > _set_idx && out_set != NO_RULE)
				rules->add_rule(rule_index, out_set - 1);

			} // end loop through rules

		// now do the actual delete...
		return var->delete_set(_set_idx); 
 
		} // end if output var

	// if we got to this point, we're dealing with an input var

	// calculate the new size of the memory. NOTE we'd normally use calc_num_of_rules() here
	// but we haven't deleted the set yet so that would give us the wrong value
	int _num_sets;
	new_mem_size = 1; // init to 1 so we don't mult by 0
	for (int i = 0; i < input_var_count; i++) 	// don't include the outupt var
		{
		// make sure we don't multply by 0...
		_num_sets = input_var_arr[i]->get_num_of_sets();
		if (i == _var_idx)
			_num_sets--;	// SUBTRACT for the set we're deleting
//		if (_num_sets) // OK to mult by 0 if ANY of the inputs is 0 there can't be any rules!
			new_mem_size *= _num_sets;

		} // end loop through input variables
 
	// allocate and init the new memory...
 	new_mem = new RuleArrayType[new_mem_size];

	if (!new_mem)
		{
 		set_msg_text(ERR_ALLOC_MEM);
 		assert(new_mem != NULL);
		return -1;
		} // end if error allocating memory

	memset(new_mem, NO_RULE, new_mem_size * sizeof(RuleArrayType));
	
	// copy all the rules that do NOT include the set being deleted
	int new_rule_index = 0;

	int* set_idx_array = new int[get_input_var_count()]; // array of the sets involved in the current rule

	for (int rule_index = 0; rule_index < rules->get_max(); rule_index++)
		{
		// get the sets that make up this rule

 	 	calc_rule_components(rule_index, set_idx_array);

		// if the rule does NOT involve the set we're going to delete, copy it

		if (set_idx_array[_var_idx] == _set_idx)
			continue;

		// set that new index to the rule that should be there
		new_mem[new_rule_index] = rules->get_rule(rule_index);

		new_rule_index++; // increment NEW rule index

		} // end loop through rules

	delete[] set_idx_array;

 	// now copy the new memory into the old rules block...
	rules->set(new_mem, new_mem_size);

	delete[] new_mem;	// free the memory we allocated

	// now do the actual delete...
	ret_val = var->delete_set(_set_idx); 

	// re-calc at the variable index factors...
	calc_rule_index_wrapper();

	return ret_val;

} // end FuzzyModelBase::delete_set()

 
//
// Function:	is_var_id_unique
// 
// Purpose:		Checks if the variable name passed in is unique for this model
//
// Arguments:
//
//		const wchar_t*	_id			- variable name to check
//		int				_var_idx	- index for the variable the id belongs to
//
// Returns:
//
//		true - id is unique
//		false - id is NOT unique
//
// Author:	Michael Zarozinski
// Date:	6/00
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
bool FuzzyModelBase::is_var_id_unique(const wchar_t* _id, int _var_idx) const
	{
	FuzzyVariableBase* var;	// pointer to variable

	// strip off any leading/trailing spaces...

	// make sure our id is UNIQUE within the rule block
 	for (int i = 0; i < input_var_count; i++)
		{
		if (i == _var_idx)
			continue;	// don't check the var the id is for

		var = get_var(i);

		// do a case insensitive comparison
		if (wcsicmp(var->get_id(), _id) == 0)
			{
			// NOT unique
			set_msg_text( load_string(ERR_VAR_NON_UNIQUE_ID));
			return false;
			}

		} // end loop through vars

	// check ouput var - UNLESS the id is FOR an output var
	if (_var_idx != OUTPUT_IDX && output_var)
		{
		var = get_var(OUTPUT_IDX);

		// do a case insensitive comparison
		if (wcsicmp(var->get_id(), _id) == 0)
			{
			// NOT unique
			set_msg_text(ERR_VAR_NON_UNIQUE_ID);
			return false;
			}

		} // end if not output var

	return true;

} // end FuzzyModelBase::is_var_id_unique()
 

//
// Function:	get_msg_textA()
// 
// Purpose:		This fucntion gets the msg_text (from FFLLBase) and 
//				returns the ascii version of it. 
//
// Arguments:
//
//			none
// 
// Returns:
//
//			const char* - ansii version of msg_text
//
//
// Author:	Michael Zarozinski
// Date:	9/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
const char* FuzzyModelBase::get_msg_textA()
{
	ascii_err_msg = "";	// clear out error message

	char* aid = convert_to_ascii(get_msg_text());

	if (aid == NULL)
		return NULL;

	ascii_err_msg = aid;

	delete[] aid;

 	return ascii_err_msg.c_str();
	
} // end get_msg_textA()


//
// Function:	calc_output()
// 
// Purpose:		Calculates the defuzzified output value for the model
//
// Arguments:
//
//		short*		var_idx_arr		-	Array that holds the current index value 
//										for each input var
//		DOMType*	out_set_dom_arr -	Array that holds the DOM value for each
//										set in the output variable
//
// Returns:
//
//		RealType - the output value, FLT_MIN if no ouput set is active
//
// Author:	Michael Zarozinski
// Date:	
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
RealType FuzzyModelBase::calc_output(short* var_idx_arr, DOMType* out_set_dom_arr )  
{
	calc_active_output_level_wrapper(var_idx_arr, out_set_dom_arr);
 
	if (!output_var)
		return FLT_MIN;	// don't have an output var yet!

	return output_var->calc_output_value(out_set_dom_arr);

} // end FuzzyModelBase::calc_output()
 


//
// Function:	validate_fcl_identifier()
// 
// Purpose:		Validate that the identifier is in valid FCL format. The IEC 61131-7 
//				specifies identifiers as :
//			
//					identifier
//						{alpha}{alphanum}*(\[{alphanum}\])?
//						(\.{alpha}{alphanum}*(\[{alphanum}\])?)* |
//						{alpha}{alphanum}*(\[({digits}..{digits})(,({digits}..{digits}))*\])
// 
//					where
//
//						alpha ::= [a-zA-Z_]
//						alphanum ::= [a-zA-Z_0-9]
//
//				You'll note that this does not handle wild characters.
//
//				This currently only checks for spaces or an identifier that starts with
//				a number. This could be made much more robust.
//
//				If there's something wrong with the identifier the problem is written out in
//				a comment.
//
// Arguments:
//
//		std::ofstream&	file_contents	-	file we're writing to
//		std::string		identifier		-	identifier to check
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
void FuzzyModelBase::validate_fcl_identifier(std::ofstream& file_contents, std::string identifier)
{
 
 	bool add_comment = false;

	// if there's a space in the identifier - flag an error
	if (identifier.find(" ") != -1)
		{
		file_contents << "\t(* Identifier should not have any spaces, they were replaced with underscores ";
		add_comment = true;
		}

	if (static_cast<int>(identifier[0]) < 9)
		{
		if (add_comment)
			file_contents << "or start with a digit ";
		else
			file_contents << "\t(* Identifier should not start with a digit ";
		add_comment = true;
		}

	if (add_comment)
		file_contents << "*)";


} // end FuzzyModelBase::validate_fcl_identifier()
  


//
// Function:	calc_active_output_level_wrapper()
// 
// Purpose:		Wrapper function that calculates the active output level
//				for each output set.
//
// Arguments:
//
//		short*		var_idx_arr		-	Array that holds the current index value 
//										for each input var
//		DOMType*	out_set_dom_arr -	Array that we're writing to. It holds the DOM value for each
//										set in the output variable
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
void FuzzyModelBase::calc_active_output_level_wrapper(short* var_idx_arr, DOMType* out_set_dom_arr )   
{
  	if (!output_var)
		return;	// don't have an output var yet!

	// zero out the dom arrays...
	for (int i = 0; i < output_var->get_num_of_sets(); i++)
		{
		out_set_dom_arr[i] = 0;  
		}

	// call recursive func to calc each sets DOM...
  	calc_active_output_level(0, 0, 0, var_idx_arr, out_set_dom_arr);
 
} // end FuzzyModelBase::calc_active_output_level_wrapper()


//
// Function:	add_input_var_to_list()
// 
// Purpose:		Add the variable passed in to the list of input variables and
//				adjust the rules as needed.
//				Note that this does not COPY the var passed in, it sets the
//				input variable array to point to the var passed in.
//
// Arguments:
//
//		FuzzyVariableBase* var - variable to insert
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
void FuzzyModelBase::add_input_var_to_list(FuzzyVariableBase* var )
{
	// need to create new memory then we'll copy the old to it...
	FuzzyVariableBase** tmp_var = new FuzzyVariableBase*[input_var_count + 1]; // new mem

	// copy the old memory to the new mem..
	for (int i = 0; i < input_var_count; i++)
		tmp_var[i] = input_var_arr[i];

	// add this variable to the variable array....

 	var->set_index(input_var_count);
	tmp_var[input_var_count] = var; 

 	// set the index in var so we know where we are in the array

	input_var_count++;  // increment the number of variables we have
	
	// set the old mem to the new mem...

	delete[] input_var_arr; // free old mem

	input_var_arr = tmp_var;

	// if there are no sets for the new var... no need to expand memory
	if (var->get_num_of_sets() == 0)
		{
		// clear rules before going on...
		rules->clear();
		return;
		}

	// calc the size of the new memory block
	// NOTE:  we've already added the set so the num_of_sets has been incremented
 	int	 new_mem_size = calc_num_of_rules();

	// if there's any var that has NO sets... new_mem_size will be 0... this is ok,
	// cuz that means we CAN'T have any rules yet!
	if (new_mem_size == 0)
		return; // nothing more to do...

	// anytime we add a new variable (even if we're copying an existing one) we
	// loose all the rules so we just call alloc

	rules->alloc(new_mem_size);
	
 	// re-calc at the variable index factors...
 
	calc_rule_index_wrapper();
 
} // end FuzzyModelBase::add_input_var_to_list()
 

//
// Function:	calc_active_output_level()
// 
// Purpose:		Recursive function that calculates the DOMs for the output sets.
//
// Arguments:
//
//		int			var_num				-	variable we're checking the DOMs for
//		DOMType		activation_level	-	current activation level 
//		int			rule_index			-	index for the rule we're checking DOMs for
//		short*		var_idx_arr			-	Array that holds the current index value 
//											for each input var
//		DOMType*	out_set_dom_arr		-	Array that holds the DOM value for each
//											set in the output variable
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
void FuzzyModelBase::calc_active_output_level(int var_num, DOMType activation_level, int rule_index, short* var_idx_arr , DOMType* out_set_dom_arr )   
{

  	DOMType	old_activation_level;	// need to save the old actiavation
									// level in case we have more than 
									// one active set in this var
			
	DOMType		set_dom;		// var to hold the current set's dom.

	// if the 'var_num' is greater than the number of input variables - set it to the OUTPUT var,
	// that will end the recursing through this function

	if (var_num >= input_var_count)
		var_num = OUTPUT_IDX;

	// get the variable...
	const FuzzyVariableBase* var = get_var(var_num);
	assert(var != NULL);

	// loop through the sets for this var...
	int num_of_sets = var->get_num_of_sets();
  
	// loop through each set in this variable
	for (int i = 0; i < num_of_sets; i++)
		{
 
		if (!(var->is_output())) 
			{
			// input variable...

  			set_dom = var->get_dom(i, (var_idx_arr[var_num])); 
 			
			// if this set is not active - skip it...

			if (set_dom == 0) 		
				continue;

 			// we need to deal with this set, save the activation level...
	  		old_activation_level = activation_level;

			// if this is the FIRST time through, set the activation level
			if (var_num == 0)
				activation_level = set_dom;

			// set the activation level to the current set's level dependent on the inference method
 			if ((inference_method == INFERENCE_OPERATION_MIN) && (set_dom < activation_level) )
				activation_level = set_dom;
			else if ((inference_method == INFERENCE_OPERATION_MAX) && (set_dom > activation_level))
				activation_level = set_dom;


			// get the set we're dealing with for the recursive call
			FuzzySetBase* set = var->get_set(i);

			// call recursively, incrementing the variable index by one and adding
			// this set's rule_index to the one passed in.
			calc_active_output_level(var_num + 1, activation_level, rule_index + set->get_rule_index(), var_idx_arr, out_set_dom_arr);  

			}
		else
			{
			// we're dealing with the output variable

			// we need to deal with this set, save the activation level...
	  		old_activation_level = activation_level;
 
			// if there is a rule for the rule_index passed in, set the output set's DOM
			if (rule_exists(rule_index))
				{
				// SUB 1 from activation level cuz that's from 0 to MAX_DOM and
				// we're setting an INDEX
 
				set_output_dom(out_set_dom_arr, rules->get_rule(rule_index), activation_level - 1);
				}
			
			} // end if output var

		// reset the activation level...  
 	 	activation_level = old_activation_level;
 
		} // end loop through sets

} // end FuzzyModelBase::calc_active_output_level()


//
// Function:	save_to_fcl_file()
// 
// Purpose:		Main function to save the model in the IEC 61131-7 International Standard 
//				FCL (Fuzzy Control Language)
//
// Arguments:
//
//		const char* file_name - path and file name to save the information to.
//
// Returns:
//
//		0 - success
//		non-zero - failure 
//
// Author:	Michael Zarozinski
// Date:	9/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
// 

int FuzzyModelBase::save_to_fcl_file(const char* file_name)
{
	int				i;				// counter
 	std::ofstream	file_contents;	// content of the file
 
	// open the file
	file_contents.open(file_name); 

	if (!(file_contents.is_open()))
		{
		set_msg_text(ERR_OPENING_FILE); // could be a little more robuts here
		return -1;
		}

	time_t tmp_time;	// time structure
	char date[100];		// temp string

	time(&tmp_time);
	struct tm *newtime = localtime( &tmp_time );  // Convert time to "struct tm" form 
 
	// asctime returns EXACTLY 26 characters... the last 2 of which are a new line and null
	// we don't want them, so only print the first 24 chars...
	sprintf(date, "%.24s", asctime( newtime ) );

	file_contents << "(* FCL File Created From FFLL Model: " << date << " *)\n\n";

	// start the FCL block
 	file_contents << get_fcl_block_start() << "\n\n";

	// start the input variable block
	file_contents << FuzzyVariableBase::get_fcl_block_start() << "\n";

	// save input variables
 	for (i = 0; i < input_var_count; i++)
  	  	input_var_arr[i]->save_var_to_fcl_file(file_contents); 

	// end the input variable block
	file_contents << FuzzyVariableBase::get_fcl_block_end() << "\n\n";

 	// save output var
	file_contents << FuzzyOutVariable::get_fcl_block_start() << "\n";
	output_var->save_var_to_fcl_file(file_contents);
	file_contents << FuzzyOutVariable::get_fcl_block_end() << "\n\n";

	// write input vars' out sets...
	for (i = 0; i < input_var_count; i++)
 	  	input_var_arr[i]->save_sets_to_fcl_file(file_contents); 

	// write out the output var's sets
	output_var->save_sets_to_fcl_file(file_contents);

	// write out the rules
 	save_rules_to_fcl_file(file_contents);

	// end the FCL block
	file_contents << get_fcl_block_end() <<  "\n\n";

	// close the file
	file_contents.close();

	return 0;
 
} // end FuzzyModelBase::save_to_fcl_file( )
 


//
// Function:	save_rules_to_fcl_file()
// 
// Purpose:		Save rules to a file according to the IEC 61131-7 International Standard 
//				FCL (Fuzzy Control Language)
//
// Arguments:
//
//		std::ofstream& file_contents - file we're writing to in stream form
//
// Returns:
//
//		void
//
// Author:	Michael Zarozinski
// Date:	9/01
// 
// Modification History
// Author		Date		Modification
// ------		----		------------
// Michael Z	05/02		Updating so writing out rules adhears to the FCL standard
//							more closely
//
// 
 
void FuzzyModelBase::save_rules_to_fcl_file(std::ofstream& file_contents) const
{
	int i;	// counter

	// start the 'ruleblock'
	file_contents << "RULEBLOCK first\n";	// name 'first' cuz we have no better name

	// print out inference method

	int inference = get_inference_method();

 	file_contents << "\t";

	if (inference == FuzzyModelBase::INFERENCE_OPERATION_MIN)  
		file_contents << "AND:MIN";
	else if (inference == FuzzyModelBase::INFERENCE_OPERATION_MAX) 
		file_contents << "OR:MAX";

	file_contents << ";\n";

	// print out composition method

	int composition = get_composition_method();
	
	file_contents << "\tACCU:";

  	if (composition == FuzzyOutVariable::COMPOSITION_OPERATION_MIN)  
		file_contents << "BSUM";
	else if (composition == FuzzyOutVariable::COMPOSITION_OPERATION_MAX) 
		file_contents << "MAX";

	file_contents << ";\n";

	// list the rules, if a rule does NOT exist we'll put a comment in to clearly indicate
	// that a rule is missing

	// create an array for each variable to hold the sets, we'll do it dynamically rather
	// than use an STL container cuz we know the sizes before we start

	std::string** var_sets;
	std::string* var_name; // names of the variables (saved so we only have to get them
							// and convert them once)

	var_sets = new std::string*[input_var_count + 1];	// add one for output var
	var_name = new std::string[input_var_count + 1];	// add one for output var

	// create mem for each var...
	int num_of_sets;
	FuzzyVariableBase* var;

	// loop through input vars, we'll save each set's ID into the var_sets array
	// 05/02 - modifying so we write out what the FCL standard says we should:
	//		subcondition ::= (‘NOT???variable_name ‘IS?[‘NOT'] ) term_name ?? | ( variable_name ‘IS?[‘NOT’] term_name ) 
	// NOTE: right now (5/02) we still don't support the 'NOT' option
	// as opposed to the way we were doing it which was just:
	//		subcondition ::= term_name
	// which assumes the term_name part of each rule is specified in the order 
	// that the variables are declared so it "knows" which variable_name the 
	// term_name belongs to.

	for (i = 0; i < input_var_count; i++)
		{
		var = get_var(i);

		// get the variable's names
		char* var_name_aid = convert_to_ascii(var->get_id(), '_');

		var_name[i] = var_name_aid;

		delete[] var_name_aid;

		num_of_sets = var->get_num_of_sets();

		var_sets[i] = new std::string[num_of_sets];

		// loop through this variable's sets
		for (int j = 0; j < num_of_sets; j++)
			{
			FuzzySetBase* set = var->get_set(j);

			char* aid = convert_to_ascii(set->get_id(), '_');

			var_sets[i][j] = aid;

			delete[] aid;

			} // end loop through sets

		} // end loop through vars

	// add output var's sets...
	var = get_var(OUTPUT_IDX);
	num_of_sets = var->get_num_of_sets();

	// save output var's name

	// NOTE: we use the 'i' counter from above
	char* var_name_aid = convert_to_ascii(var->get_id(), '_');

	var_name[i] = var_name_aid;

	delete[] var_name_aid;

	// now get each set's name...
	var_sets[i] = new std::string[num_of_sets];

	for (int j = 0; j < num_of_sets; j++)
		{
 		char* aid = convert_to_ascii(var->get_id(j), '_');
		
		var_sets[i][j] = aid;

		delete[] aid;

		} // end loop through sets

 	// loop through rules and write them out...

	RuleArrayType rule;	// rule we're dealing with
	int*	set_idx_array = new int[input_var_count];
	char	tmp[4];
 	for (i = 0; i < get_num_of_rules(); i++)
		{
		rule = rules->get_rule(i);

		file_contents << "\t";

		if (rule == NO_RULE)
			{
			// converting int to string cuz MSVC reports a memory leak if we don't and
			// use '<< i <<' below. I'm sure this is another "false leak" that MSVC is
			// prone to reporting when STL objects are used.
			sprintf(tmp, "%d", i);		 
 	 		file_contents << "(* RULE " << tmp << " : No Rule Specified *)";
 
			}
		else
			{
			// calculate the components of the rule...
			calc_rule_components(i, set_idx_array);

			sprintf(tmp, "%d", i);		 
 						
			file_contents << "RULE " << tmp << ": IF ";
 
			// print out the IDs of the sets involved in this rule
			int j;

			for (j = 0; j < input_var_count; j++)
				{
				// write out the subconditions in the form:
				// ( variable_name ‘IS?term_name )

				var = get_var(j);

	 			file_contents << "(" << var_name[j]  << " IS ";
				file_contents << var_sets[j][set_idx_array[j]] << ")";

				if (j < input_var_count - 1)
					file_contents << " AND ";
				} // end loop through vars

	 		file_contents << " THEN ";
			
 			file_contents << "(" << var_name[j]  << " IS ";
			file_contents << var_sets[j][rule] << ")";
 
			} // define rule

		file_contents << ";\n";

		} // end loop through rules
 
	file_contents << "END_RULEBLOCK\n\n"; 

	// free all the memory we allocated
	delete[] set_idx_array;

	for (i = 0; i < input_var_count + 1; i++)
		{
		delete[] var_sets[i];
		}

	delete[] var_sets;

	delete[] var_name;

} // end FuzzyModelBase::save_rules_to_file()


//
// Function:	load_from_fcl_file()
// 
// Purpose:		Main function to read in an FCL file and create a model.
//				NOTE: It is a BRAIN-DEAD parser. 
// Arguments:
//
//		const char* file_name - file to read
//
// Returns:
//
//		0 - success
//		non-zero - failure
//
// Author:	Michael Zarozinski
// Date:	9/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
// 

int FuzzyModelBase::load_from_fcl_file(const char* file_name)
{
	// NOTE: in some cases MSVC reports this is a memory leak,
	// as stated in _Visual C++ Developers Journal_ (March 2000) p. 50
	// when using STL false memory leaks can be reported because
	// iostream objects are constructed before main() starts and destructed
	// after mian() exits so the memory leak detection will report a false leak

 	std::ifstream file_contents(file_name);

 	if (!(file_contents.is_open()))
		{
		set_msg_text(ERR_OPENING_FILE); // could be a little more robuts here
		return -1;
		}

	// load input variables
	if (load_vars_from_fcl_file(file_contents))
		return -1;	// error is written to msg_txt in the called func
 
	// load the output variable
 	if (load_vars_from_fcl_file(file_contents, true))  
		return -1;	// error is written to msg_txt in the called func

	// load sets for each var
	for (int i = 0; i < input_var_count; i++)
		{
 	  	if (input_var_arr[i]->load_sets_from_fcl_file(file_contents))
			{
			// get the message text and set it for the model
			set_msg_text(input_var_arr[i]->get_msg_text());
			return -1;
			}
		} // end loop through input vars

	if (output_var)
		{
		if (output_var->load_sets_from_fcl_file(file_contents))
			{
			// get the message text and set it for the model
			set_msg_text(output_var->get_msg_text());
			return -1;
			}
		} // end if output var

	if (load_rules_from_fcl_file(file_contents))
		return -1;	// error is written to msg_txt in the called func

	if (load_defuzz_block_from_fcl_file(file_contents))
		return -1;	// error is written to msg_txt in the called func
 
	return 0;


} // end FuzzyModelBase::load_from_fcl_file()


//
// Function:	load_vars_from_fcl_file()
// 
// Purpose:		Loads the variables from the FCL file and creates them.
//
// Arguments:
//
//		std::ifstream&	file_contnts	- file to read from
//		bool			output_ind		-	indicates if we're reading the output variable
//
// Returns:
//
//		0 - success
//		non-zero - failure
//
// Author:	Michael Zarozinski
// Date:	9/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//	

int FuzzyModelBase::load_vars_from_fcl_file(std::ifstream& file_contents, bool output /* = false */)
{
		
	const char* start_token;// starting token (depends on input or output)
	const char* end_token;	// ending token (depends on input or output)

	if (output)
		{
		start_token = FuzzyOutVariable::get_fcl_block_start();
		end_token = FuzzyOutVariable::get_fcl_block_end();
		}
	else
		{
		start_token = FuzzyVariableBase::get_fcl_block_start();
		end_token = FuzzyVariableBase::get_fcl_block_end();
		}

 	file_contents.seekg(0);	// go to start of file

 	std::string token;
 
	do 
		{
		file_contents >> token;
		// if token is empty we reached EOF
		if (file_contents.eof())
			{
			set_msg_text(ERR_EOF_READING_VARS);
 			return -1; 
			}
		}while (strcmp(token.c_str(), start_token) != 0);


  	// parse names...
	// AGAIN... this is BRAIN-DEAD parsing, we're assuming the 
	// input vars have the format:
	//
	// VAR_INPUT  
	//		variable_1_name: REAL;
	//				.
	//				.
	//				.
	//		variable_n_name: REAL;
	// END_VAR
	//
	// Output varaible declaration is:
	//
	// VAR_OUTPUT  
	//		variable_name: REAL;
	// END_VAR
	//
	// NOTE: the IEC 61131-7 does not specify a way to set the range of a varaible, so 
	// we write them out in a range comment

 	char line[300];
	char var_name[50];
	RealType start_val, end_val;	// range for the variable

	// get line-by-line until we reach END_VAR
	file_contents.getline(line, 300);

	while (strncmp(line, end_token, strlen(end_token)) != 0)
		{
		start_val = end_val = FLT_MIN; // init so we know if we have a range for the variable

 		var_name[0] = '\0';

		// defined a var... find "REAL" in it and anything before that is the variable name
		sscanf(line, "%s", var_name);

		if (strlen(var_name) > 0) // deal with lines that are just spaces
			{
 			// find the range of the variable
 			 
			// find "RANGE"
			char* pos = strstr(line, "RANGE(");

			char* num_start;
			char* num_end;
			char* stop_scan; // used for strtod

			if (pos != NULL)
				{
				// found "RANGE"... set position to start of first number

				pos += strlen("RANGE(");

				// found start of 1st number, save position
				num_start = pos;

				// find ".."
				pos = num_end = strstr(pos, "..");
			 
				// get the substring and convert...
				char value[50];
				int num_len = num_end - num_start;
				
				memset(value, '\0', 50);

				strncpy(value, num_start, num_len);

				start_val = strtod(value, &stop_scan);
 
				if (fabs(start_val) == HUGE_VAL)
					{
					// error converting...
					set_msg_text(ERR_VAR_MIN_VALUE);
 					return -1; 
					}

				pos += strlen(".."); // puts us at the start of the next number

				// find the ending ')'
				num_start = pos;

				num_end = strstr(pos, ")");

				memset(value, '\0', 50);

				num_len = num_end - num_start;

				strncpy(value, num_start, num_len);

				end_val = strtod(value, &stop_scan);

				if (fabs(end_val) == HUGE_VAL)
					{
					// error converting...
					set_msg_text(ERR_VAR_MAX_VALUE);
 					return -1; 
					}

				} // end found token RANGE

			int ret_val;	// holds return value

			// convert var_name to wide chars...
			wchar_t* wname = convert_to_wide_char(var_name);

			// create the variable
			if (output)
				{
				ret_val = add_output_variable(wname, start_val, end_val);
				}
			else
				{
				ret_val = add_input_variable(wname, start_val, end_val);
				}

			delete[] wname;
 
			if (ret_val)
				return -1; // error is written in called func

			} // end if line is not null

		// get the next line
		file_contents.getline(line, 300);

		}; // end while not END_VAR

	return 0;

} // end FuzzyModelBase::load_input_vars_from_file()

//
// Function:	load_rules_from_fcl_file()
// 
// Purpose:		Loads the rules from the FCL file and creates them.
//
// Arguments:
//
//		std::ifstream&	file_contnts	- file to read from
//
// Returns:
//
//		0 - success
//		non-zero - failure
//
// Author:	Michael Zarozinski
// Date:	9/01
// 
// Modification History
//	Author		Date		Modification
//	------		----		------------
//	Michael Z	6/02		modifying so we read what the FCL standard says we should:
//								subcondition ::= (‘NOT???variable_name ‘IS?[‘NOT'] ) term_name ?? | ( variable_name ‘IS?[‘NOT’] term_name ) 
//							NOTE: we still don't support the 'NOT' option
//							as opposed to the way we were doing it which was just:
//								subcondition ::= term_name
//							both methods will be supported for backwards compatibility

int FuzzyModelBase::load_rules_from_fcl_file(std::ifstream& file_contents)
{
	// a very brain-dead parser of the RULEBLOCK section, this assumes that we have
	// a line for every rule (even if one does not exist, there's a comment).  It also
	// assumes that the rule number corresponds to the index into the rules array.
	// the safest way is to map the rule conditions to the var/sets and calculate the
	// rule index from that

	// this assumes that the sets involved in the rules are in the same order as
	// the varaibles are declared.  Since there is no restriction that set names
	// must be unique for the whole system we need to make this assumption, short 
	// of using some dot notation (var1.set1) which I don't believe is supported
	// by the IEC standard

	// we have a nice happy little situation here... we don't care about any of the
	// conditions for the rules!  all we care about is the rule # which is the index in
	// the rules array, and the result which we need to map to the index of the output set.
			
	char line[500];

	// create array of output sets...
	FuzzyVariableBase* var = get_var(OUTPUT_IDX);

	int num_sets = 0;

	if (var)
		num_sets = var->get_num_of_sets();

	// create an array of arrays that holds the set IDs for each variable (including output)
	std::string** sets = new std::string*[input_var_count + 1]; // add one for output var

	int i, j;		// counter 

	// loop through the input vars
	for (i = 0; i < input_var_count; i++)
		{
		var = get_var(i);

		num_sets = get_num_of_sets(i);

		// create an array of strings that has 'num_sets' elements
		sets[i] = new std::string[num_sets];

		for (j = 0; j < num_sets; j++)
			{
			// convert the set's ID to ascii and replace any spaces with underscores
 			char* aid = convert_to_ascii(var->get_id(j), '_');
 
			// add it to the array
			sets[i][j] = aid;			

			delete[] aid;

			} // end loop through sets

		} // end loop through input vars

	// add the output variable's sets...
	num_sets = get_num_of_sets(OUTPUT_IDX);
	sets[i] = new std::string[num_sets];

	var = get_var(OUTPUT_IDX);

	for (j = 0; j < num_sets; j++)
		{
 		char* aid = convert_to_ascii(var->get_id(j), '_');
 
		sets[i][j] = aid;			

		delete[] aid;
		} // end loop through output sets

	std::string tmp; // temp string to hold tokens
	// read until we get to the start of the rules declaration

	do
		{
		file_contents >> tmp;
		} while (strcmp(tmp.c_str(), "RULEBLOCK") != 0 );

	file_contents >> tmp; // "eat" the rule block name which isn't significant

	// read in the operator definition (brain-dead as we are, assume it's there!)
	// we read in the whole next line - ASSUMING it's the operator

	line[0] = '\0';
	// skip blank lines
	while (strlen(line) <= 0)
		file_contents.getline(line, 50);

	// parse it...
 	char seps[]   = " :;\t\n";

	// right now we only care about the first part (before the colon)...
	char* operation = strtok(line, seps);

	// right now we only support 'and'
	if (strncmp(operation, "AND", strlen("AND")) == 0)
		{

		}
	else
		{
		set_msg_text(ERR_INVALID_FILE_FORMAT);
		return -1;
		}

	// read in the ACCU method...

	line[0] = '\0';
	// skip blank lines
	while (strlen(line) <= 0)
		file_contents.getline(line, 50);

	char* accum = strtok(line, seps);

	// sanity check
	if (strncmp(accum, "ACCU", strlen("ACCU")) != 0)
		{
		set_msg_text(ERR_INVALID_FILE_FORMAT);
		return -1;
		}

	accum = strtok(NULL, seps); 

	// get the accumm method

	if (strncmp(accum, "BSUM", strlen("BSUM")) == 0)
		{
		set_composition_method(FuzzyOutVariable::COMPOSITION_OPERATION_MIN);
		}
	else if (strncmp(accum, "MAX", strlen("MAX")) == 0)
		{
		set_composition_method(FuzzyOutVariable::COMPOSITION_OPERATION_MAX);
		}
	else
		{
		set_msg_text(ERR_INVALID_FILE_FORMAT);
		return -1;
		}

	// now create the rules, the rule array was allocated when sets were added
	// we want to avoid the line-by-line method cuz rules can span more than one line

	std::string* rule_components = new std::string[input_var_count + 1]; // +1 for output var

	file_contents >> tmp;
			
	bool strict_parsing = false;

	while (strcmp(tmp.c_str(), "END_RULEBLOCK") != 0)
		{
		if (file_contents.eof())
			{
			set_msg_text(ERR_EOF_READING_RULES);
 			return -1;
			}

		// check if comment

		if (strncmp(tmp.c_str(), "(*", sizeof("(*")) == 0)
			{
			// found a comment
			// skip the rest of the line. Note, as this is a brain-dead parser
			// it assumes no multi-line comments!
			file_contents.getline(line, 500);
			} // end if found a comment

		if (strcmp(tmp.c_str(), "RULE") == 0)
			{

			int component_idx = 0;	// which component we're dealing with
			// found a rule

			// get the rule components...
			// look for the first condition, after the "IF"
			do
				{
				file_contents >> tmp;
				} while (strcmp(tmp.c_str(), "IF") != 0);

			// we need to determine if this FCL file has the FFLL "shorthand" of just the term name
			// or if it adhears to the FCL standard...
			// NOTE that whichever method is used, it is ASSUMED that the terms are in the order
			// that the varaiable are declared (again, lazy parsing!)

			// if next token is '(' we're gonna assume we have a FCL file that uses the stricter
			// compliance with the FCL and NOT the FFLL shorthand of just naming the term name.

			file_contents >> tmp;

			if (tmp[0] == '(')
				{
				// use strict FCL parsing

				strict_parsing = true;

				}
			else
				{
				// use FFLL shorthand...
				strict_parsing = false;
				rule_components[component_idx++] = tmp;  // next condition
				file_contents >> tmp;  // next token (could be "AND" or "THEN"
				} // end if FFLL rule shorthand notation

 	//		file_contents >> rule_components[component_idx++];  // next condition

			// loop through the rest until we find THEN
			// NOTE that all these comparisons are case SENSITIVE!
		//	file_contents >> tmp;  // next token (could be "AND" or "THEN"
			while (strcmp(tmp.c_str(), "THEN") != 0)
				{
				if (strict_parsing)
					{
					// look for the token after ' IS '
					do
						{
						file_contents >> tmp;
						} while (strcmp(tmp.c_str(), "IS") != 0);
				
					file_contents >> rule_components[component_idx];  // next condition

					// remove any trailing ')' that may be there
					int paren_pos = rule_components[component_idx].find(")");

					if (paren_pos > 0)	// NOTE: we only check > 0 rather than >= cuz if it's = 0 we have nothing
						(rule_components[component_idx])[paren_pos] = '\0';

					component_idx++;

					file_contents >> tmp; // next token (could be "AND" or "THEN"

					}
			 	else
					{
					file_contents >> rule_components[component_idx++];  // next condition
					file_contents >> tmp; // next token (could be "AND" or "THEN"
					}

 
				} ; // end while != THEN

			// found THEN so next token is the result
			if (strict_parsing)
				{
				// look for the token after ' IS '
				do
					{
					file_contents >> tmp;
					} while (strcmp(tmp.c_str(), "IS") != 0);

				} // end if strict parsing
 
			file_contents >> rule_components[component_idx];
			// make sure the ';' isn't included
			int semi_pos = rule_components[component_idx].find(";");

			if (semi_pos > 0)	// NOTE: we only check > 0 rather than >= cuz if it's = 0 we have nothing
				(rule_components[component_idx])[semi_pos] = '\0';

			// remove any trailing ')' that may be there
			int paren_pos = rule_components[component_idx].find(")");

			if (paren_pos > 0)	// NOTE: we only check > 0 rather than >= cuz if it's = 0 we have nothing
				(rule_components[component_idx])[paren_pos] = '\0';

			// calc the rule index...

			int rule_idx = 0;

			for (i = 0; i < input_var_count; i++)
				{
				// find match between rule_component and set saved
				num_sets = get_num_of_sets(i);
				for (j = 0; j < num_sets; j++)
					{
					if (strcmp((sets[i][j]).c_str(), rule_components[i].c_str()) == 0)
						{
						rule_idx +=  get_rule_index(i, j);
						break;
						}
					} // end loop through sets
				} // end loop through input vars

			// find the output idx (NOTE: we're using the 'i' counter above)
			int out_set_idx = NO_RULE;
			num_sets = get_num_of_sets(OUTPUT_IDX);
			for (j = 0; j < num_sets; j++)
				{
				if (strcmp((sets[i][j]).c_str(), rule_components[i].c_str()) == 0)
					{
					out_set_idx = j;
					break;
					}

				} // end loop through sets

			// set the rule index
			add_rule(rule_idx,  out_set_idx);
			} // end if found a rule
	
		file_contents >> tmp;

		}; // end while not end ruleblock

	// free the memory we allocated

	for (i = 0; i <= input_var_count; i++) // include output var
		{
		delete[] sets[i];
		}
		
	delete[] sets;
	delete[] rule_components;
	
	return 0;

} // end FuzzyModelBase::load_rules_from_file()


//
// Function:	load_defuzz_block_from_fcl_file()
// 
// Purpose:		Loads the defuzzification info from the FCL file and creates them.
//
// Arguments:
//
//		std::ifstream&	file_contnts	- file to read from
//
// Returns:
//
//		0 - success
//		non-zero - failure
//
// Author:	Michael Zarozinski
// Date:	9/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//	

int FuzzyModelBase::load_defuzz_block_from_fcl_file(std::ifstream& file_contents)
{
	int method;				// defuzzification method
 	file_contents.seekg(0);	// go to start of file

  	std::string token;

	// loop until we find the "DEFUZZIFY" keyword
	do 
		{
		file_contents >> token;

		if (file_contents.eof())
			{
			// didn't find a defuzzify block... set defaults
 			set_defuzz_method(DefuzzVarObj::DEFUZZ_COG);
 			return 0;
			}

		}while (strcmp(token.c_str(), "DEFUZZIFY") != 0);

	// eat the next token which is the name of the ouptut variable
	file_contents >> token;

	// now search for 'METHOD'...
	do 
		{
		file_contents >> token;

		// if token is empty we reached EOF
		if (file_contents.eof())
			{
			// didn't find the "METHOD" keyword... set defaults
 			set_defuzz_method(DefuzzVarObj::DEFUZZ_COG);
			return 0;
			}

		}while ( strcmp(token.c_str(), "METHOD:") != 0) ;

	// now get the method...

	file_contents >> token;

	// strip off the semicolon (if it's there)
	int semi_pos = token.find(";");

	if (semi_pos > 0)	// NOTE: we only check > 0 rather than >= cuz if it's = 0 we have nothing
		token[semi_pos] = '\0';

	// find the defuzz method
	// defuzzification_method ::= 'METHOD' ':' 'CoG' | 'CoGS' | 'CoA' | 'LM' | 'RM' | 'MoM' ';'
	// *** NOTE: 'MoM' (Mean of Maximum) is not part of the standard, we added it ***
	// AND we only currently support "CoG" and "MoM"

	if (strcmp(token.c_str(), "CoG") == 0)
		method = DefuzzVarObj::DEFUZZ_COG;
	else if (strcmp(token.c_str(), "MoM") == 0)
		method = DefuzzVarObj::DEFUZZ_MOM;
	else
		{
		// default to Center of Gravity
		method = DefuzzVarObj::DEFUZZ_COG;
		}
	 
	set_defuzz_method(method);

	return 0;

} // end FuzzyModelBase::load_defuzz_block_from_file()


//
// Function:	set_output_dom()
// 
// Purpose:		Set the output set dom according to the composition method
//
// Arguments:
//
//		DOMType*	out_set_dom_arr -	Array that holds the DOM value for each
//										set in the output variable
//		int			set_idx			-	index of the set we're setting the DOM for
//		DOMType		new_value		-	DOM value
//
// Returns:
//
//		0 - success
//		non-zero - failure
//
// Author:	Michael Zarozinski
// Date:	9/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
// 
 
int FuzzyModelBase::set_output_dom(DOMType* out_set_dom_arr, int set_idx, DOMType new_value)  
{
	bool set_value = false;	// indicates if we need to set value
	int composition_method = output_var->get_composition_method();

	if (set_idx >= output_var->get_num_of_sets())
		{
		assert(set_idx < output_var->get_num_of_sets());
		return -1;
		}

	// set the value dependent on the COMPOSITION method...
	DOMType current_dom = out_set_dom_arr[set_idx];

	// if dom passed in is 0 or current dom is 0 update.  This is needed so we can zero it out, besides if
	// we're passing in a zero the set is NOT active so we WANT it to be zero
 	if (current_dom == 0)
		set_value = true;
	else
		{
		if ((composition_method == FuzzyOutVariable::COMPOSITION_OPERATION_MIN) && ( new_value < current_dom) )
			set_value = true;
		else if ((composition_method == FuzzyOutVariable::COMPOSITION_OPERATION_MAX) && ( new_value > current_dom))
			set_value = true;
		}

	if (set_value)
		{
		if (new_value != 255 && new_value > FuzzyVariableBase::get_dom_array_max_idx())
			assert(0);

		if (new_value < 0)
			new_value = 0;

		out_set_dom_arr[set_idx] = new_value; 

		} // end if we need to set the DOM
 				
	return 0;

} // end FuzzyModelBase::set_output_dom()


/////////////////////////////////////////////////////////////////////
////////// Trivial Functions That Don't Require Headers /////////////
/////////////////////////////////////////////////////////////////////
 
FFLL_INLINE int FuzzyModelBase::get_input_var_count(void) const
{
	return input_var_count;

}  // end FuzzyModelBase::get_input_var_count()
 
FFLL_INLINE int FuzzyModelBase::get_total_var_count() const
{
	int count = get_input_var_count();

	if (output_var)
		count++;

	return count;

}; // end FuzzyModelBase::get_total_var_count()

FFLL_INLINE int FuzzyModelBase::get_num_of_rules() const
{
	if (rules)
		return rules->get_max();
	else
		return 0;

}; // end FuzzyModelBase::get_num_of_rules()


int FuzzyModelBase::get_rule_index(int _var_idx, int _set_idx /* = -1 */) const
{
	// if _set_id is not passed - return the index for the VAR - this
	// is used to reverse engineer the rule components from an index into the rule array
	if (_set_idx == -1)
		{
		// if last var, return 1... we're done recursing
		if (_var_idx == input_var_count - 1)
			return 1;

		return input_var_arr[_var_idx]->get_rule_index();
		} 
	else
		{
		return input_var_arr[_var_idx]->get_rule_index(_set_idx);
		}

}; // end FuzzyModelBase::get_rule_index()

int FuzzyModelBase::calc_num_of_rules() const 
{
	int num_of_rules = 1; // init to 1 so we don't mult by 0

	for (int i = 0; i < input_var_count; i++)
		{
		// multiply by 0 is ok, cuz if one variable has no
		// sets we can't have any rules.
 		num_of_rules *= input_var_arr[i]->get_num_of_sets();
		} // end loop through sets
 
	return num_of_rules;

} // end FuzzyModelBase::calc_num_of_rules()

FFLL_INLINE int FuzzyModelBase::get_num_of_sets(int var_idx) const
{
 
	const FuzzyVariableBase* var =  get_var(var_idx);
 
	assert(var != NULL);

	return var->get_num_of_sets();

}; // end FuzzyModelBase::get_num_of_sets()

 
void FuzzyModelBase::add_rule(int index, RuleArrayType output_set)
{
	rules->add_rule(index, output_set); 
};

void FuzzyModelBase::remove_rule(int index)
{	
	rules->remove_rule(index); 
};


void FuzzyModelBase::init()
{
	if (rules)
		delete rules;

	rules =  new_rule_array();
};

RuleArray* FuzzyModelBase::new_rule_array()
{
	return new RuleArray(this);
};  
 
FFLL_INLINE RuleArrayType FuzzyModelBase::rule_exists(int index) const 
{
	return (((rules->get_rule(index) == NO_RULE) ? 0 : 1));
};  

FFLL_INLINE RuleArrayType FuzzyModelBase::get_rule(int idx) const  
{
	return rules->get_rule(idx);
};

FFLL_INLINE bool FuzzyModelBase::no_rules() const
{ 
	return rules->no_rules();
};

FFLL_INLINE void FuzzyModelBase::clear_rules() 
{ 
	rules->clear();
};

FFLL_INLINE	const char* FuzzyModelBase::get_model_name() const
{
	return model_name.c_str();
}; 
const char* FuzzyModelBase::get_fcl_block_start() const 
{
	return "FUNCTION_BLOCK";
};		

const char* FuzzyModelBase::get_fcl_block_end() const 
{
	return "END_FUNCTION_BLOCK"; 
};
void FuzzyModelBase::set_model_name(const char* _name) 
{
	model_name = _name;
};

DOMType FuzzyModelBase::get_dom(int var_idx, int set_idx, int x_position) const
{
	const FuzzyVariableBase* var = get_var(var_idx);
 	return var->get_dom(set_idx, x_position);
} 

FuzzyVariableBase* FuzzyModelBase::new_variable()
{
 	return new FuzzyVariableBase(this);
}
 
FuzzyOutVariable* FuzzyModelBase::new_output_variable()
{
	return new FuzzyOutVariable(this);
}  

ValuesArrCountType FuzzyModelBase::convert_value_to_idx(int var_idx, RealType value) const
{

	FuzzyVariableBase* var =  get_var(var_idx);
	return var->convert_value_to_idx(value);

}  

int FuzzyModelBase::get_defuzz_method() const
{
	if (output_var)
		return output_var->get_defuzz_method();
	else
		return -1;

}  


int FuzzyModelBase::get_composition_method() const
{
	if (!output_var)
		return -1;

	return output_var->get_composition_method();
} 

int FuzzyModelBase::set_composition_method(int method)
{
	if (!output_var)
		return -1;

	return output_var->set_composition_method(method);

} 

		
int FuzzyModelBase::set_defuzz_method(int method) 
{
	if (!output_var)
		return -1;
	return output_var->set_defuzz_method(method);
};

FFLL_INLINE int FuzzyModelBase::get_inference_method() const 
{
	return inference_method; 
} 

int FuzzyModelBase::set_inference_method(int method)
{
	// validate

	switch (method)
		{
		case INFERENCE_OPERATION_MIN:
		case INFERENCE_OPERATION_MAX:

			inference_method = method;
			break;

		default:
			set_msg_text(ERR_INVALID_INFERENCE_MTHD);
			return -1;	// invalid method  
	
		} // end switch on method

	return 0;

} // end FuzzyModelBase::set_inference_method

FuzzyVariableBase* FuzzyModelBase::get_var(int idx) const
{
	if (idx == OUTPUT_IDX) // output var
		return output_var;
	else
		return(((input_var_count == 0) || (idx > input_var_count)) ? NULL : input_var_arr[idx]);
};
 
