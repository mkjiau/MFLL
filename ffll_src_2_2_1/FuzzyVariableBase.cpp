//
// File:	FuzzyVariableBase.cpp
//
// Purpose:	This file contains ancestor code for the FuzzyVariable classes
//
// Copyright © 1999-2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
// 

#include "FuzzyVariableBase.h"
#include "FuzzySetBase.h"
#include "FuzzyModelBase.h"
#include "MemberFuncBase.h"

#include <fstream>
#include <limits.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

#include "debug.h"

#endif

// init static variables to a reasonable value. We add one to each array count to
// account for the fact that we start counting at zero.
// For example if we only had 5 elements in the array and the variable range was 0 to 5,
// we'd expect each element to be 1 'x' value, but then the 'x' value at values[4] (the
// fifth element) would be 4 and we EXPECT 5.The values array would look like:
// 
//		values[0] = 0
//		values[1] = 1
//		values[2] = 2
//		values[3] = 3
//		values[4] = 4
//
// if each "step" was 1. So we need to add 1 element to the array to get the results we want.


ValuesArrCountType FuzzyVariableBase::x_array_count = 201;
ValuesArrCountType FuzzyVariableBase::x_array_max_idx = FuzzyVariableBase::x_array_count - 1;
DOMType FuzzyVariableBase::dom_array_count = 101;
DOMType FuzzyVariableBase::dom_array_max_idx = FuzzyVariableBase::dom_array_count - 1;
 

//
// Function: FuzzyVariableBase()
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
// Date:	6/10/99
// 
// Modification History
// Author		Date		Modification
// ------		----		------------
//
//

FuzzyVariableBase::FuzzyVariableBase(FuzzyModelBase* _parent) :  FFLLBase(_parent) 
{
 	num_of_sets = 0;
	rule_index = 0;
 	index = -1;

	sets = NULL; 
 
	// set left/right shoulder
	left_x = 0;
	right_x = FuzzyVariableBase::x_array_max_idx;

	set_id(L""); // init identifier
 
	idx_multiplier = 0; // it should NEVER be 0 so by init'ing to 0 we'll know if there's a problem sooner
 
} // end FuzzyVariableBase::FuzzyVariableBase()

//
// Function: ~FuzzyVariableBase()
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
// Date:	6/10/99
// 
// Modification History
// Author		Date		Modification
// ------		----		------------
//
//
FuzzyVariableBase::~FuzzyVariableBase()
{
 
	delete_all_sets();

} // end FuzzyVariableBase::~FuzzyVariableBase()



 

//
// Function:	set_ramp()
// 
// Purpose:		Change the ramp status of one of the variable's sets.
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
//		int set_idx				-	index of the set to change the ramp for
//
// Returns:
//
//		
//
// Author:	Michael Zarozinski
// Date:	7/99
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
 
void FuzzyVariableBase::set_ramp(int left_right_ind, int hi_lo_ind, int set_idx )
{
 	sets[set_idx]->set_ramp(hi_lo_ind, left_right_ind);

} // end FuzzyVariableBase::set_ramp()

 


//
// Function:	is_set_id_unique
// 
// Purpose:		Checks if the set name passed in is unique for this variable
//
// Arguments:
//
//		const wchar_t*	set_id	- set name to check
//		int				set_idx	- index for the set the id belongs to
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
bool FuzzyVariableBase::is_set_id_unique(const wchar_t* set_id, int set_idx) const
{
 	FuzzySetBase*	set;	// set we're checking

	// go through all the OTHER sets and make sure there is no match

	for (int i = 0; i < get_num_of_sets(); i++)
		{
		if (i == set_idx)
			continue;	// skip this set
 
		set = get_set(i);  

		assert(set != NULL);
 
		// case insensitive comparison
		if (wcsicmp(set->get_id(), set_id) == 0)
			{
			set_msg_text(ERR_SET_NON_UNIQUE_ID);  
			return false;
			}

		} // end loop through sets

	// if we got here the id IS unique...
	return true;

} // end  is_set_id_unique()

//
// Function:	set_left_x()
// 
// Purpose:		Set the left value for this variable. Note that the left value IS
//				allowed to be greater than the right value. The only restriction is
//				that they can NOT be the same.
//
// Arguments:
//
//		RealType value - value to set left_x to
//
// Returns:
//
//		0 - set value 
//		-1 - problem setting value 
//
// Author:	Michael Zarozinski
// Date:	7/99
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
 
int FuzzyVariableBase::set_left_x(RealType value)
{
 
	// make sure value != right_x
	if (value == right_x)
		{
		set_msg_text(ERR_SAME_LEFT_RIGHT_VALS);
		return -1;
		}

	left_x = value; 
 
	// re-calc multiplier cuz it's affected by this
	calc_idx_multiplier();
 
	return 0;

} // end FuzzyVariableBase::set_left_x()
 
//
// Function:	set_right_x()
// 
// Purpose:		Set the right value for this variable. Note that the left value IS
//				allowed to be greater than the right value. The only restriction is
//				that they can NOT be the same.
//
// Arguments:
//
//		RealType value - value to set right_x to
//
// Returns:
//
//		0 - set value 
//		-1 - problem setting value 
//
// Author:	Michael Zarozinski
// Date:	7/99
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
int FuzzyVariableBase::set_right_x(RealType value)
{
	// make sure avlue != left_x
	if (value == left_x)
		{
		set_msg_text(ERR_SAME_LEFT_RIGHT_VALS);
		return -1;
		}

	right_x = value; 

	// re-calc multiplier cuz it's affected by this
	calc_idx_multiplier();

	return 0;

} // end FuzzyVariableBase::set_right_x()
 
//
// Function:	calc_idx_multiplier()
// 
// Purpose:		Calculate the index multiplier value for this variable.
//				NOTE: there is not 'set' for the multiplier - it's always calculated
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
// Date:	8/99
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
// 
void FuzzyVariableBase::calc_idx_multiplier()
{ 
 	idx_multiplier =(right_x - left_x) /((RealType)( FuzzyVariableBase::x_array_max_idx));
	
}; // end FuzzyVariableBase::calc_idx_multiplier()
 

 
//
// Function:	delete_set()
// 
// Purpose:		Deletes a set from the variable and adjusts the sets[]
//				array appropriately.
//
// Arguments:
//
//		int _set_idx - index of the set to delete
//
// Returns:
//
//		0 - success
//		non-zero - failure
//
// Author:	Michael Zarozinski
// Date:	8/99
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
int FuzzyVariableBase::delete_set(int _set_idx)
{
	// if we don't have any sets, just return
	if (sets == NULL)
		return 0; // nothing to do.

	FuzzySetBase**	tmp_sets; // temp array to hold sets
	
	// if we're deleting the last set, don't alloc any memory
	if (num_of_sets - 1 == 0)
		tmp_sets = NULL;
	else
		tmp_sets = new FuzzySetBase*[num_of_sets - 1];	// create temp array

	// copy the old memory to the temp mem...
	int j = 0;
	for (int i = 0; i < num_of_sets; i++)
		{
		// don't copy the set we're deleting
		if (i == _set_idx)
			{
			// free the memory for the set we're deleting
			delete sets[i] ;
			sets[i] = NULL;
			}
		else
			{
			tmp_sets[j] = (FuzzySetBase*)sets[i];
			tmp_sets[j]->set_index(j);
			// set the index
			j++;
			}
		
		} // end loop through sets

	// decrement the set count
	num_of_sets--;

	// free old memory...
	delete[] sets;

	// assign the new array of sets to the sets[] member variable
 	sets = tmp_sets;
	
	return 0; 

} // end FuzzyVariableBase::delete_set()


//
// Function: delete_all_sets()
// 
// Purpose:	clean up the memory allocated for sets
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
// Date:	6/10/99
// 
// Modification History
// Author		Date		Modification
// ------		----		------------
//
//
 
void FuzzyVariableBase::delete_all_sets()
{
 	// if we don't have any sets, just return
	if (sets == NULL)
		return; // nothing to do.

	// loop through all the sets and free memory
	for (int i = 0; i < num_of_sets; i++)
		{
		delete sets[i] ;
		sets[i] = NULL;
		} // end loop through sets

	num_of_sets = 0;

	// free old memory...
	delete[] sets;
 	sets = NULL;

} // end FuzzyVariableBase::delete_all_sets()
 

//
// Function:	set_id()
// 
// Purpose:		Set the identifier for a variable or set. This version
//				takes an ASCII string, converts it to wide characters and
//				calls the wide character version of this function
//
// Arguments:
//
//		const char*	_id		-	identifier to set
//		int			set_idx	-	index of the set to change the id for, if -1 (default), change
//								the id for the variable
//
// Returns:
//	
//		0 - success
//		non-zero - non-unique id	
//
// Author:	Michael Zarozinski
// Date:	8/99
// 
// Modification History
// Author		Date		Modification
// ------		----		------------
// Michael Z	4/03		changed to use convert_to_wide_char() for compatiblity
//
int FuzzyVariableBase::set_id(const char* _id, int set_idx /* = -1 */)
{
 	// convert from char to wchar_t
	wchar_t* wc = convert_to_wide_char(_id);

	int ret_val = set_id(wc, set_idx);  

	// we MUST free mem convert_to_wide_char() created
	delete[] wc;

	return ret_val;
 	
} // end FuzzyVariableBase::set_id()

//
// Function:	set_id()
// 
// Purpose:		Set the identifier for a variable or set. If no set index
//				is passed in set the variable's id.
//				This function will only set the identifier if it's unique
//
// Arguments:
//
//		const wchar_t*	_id		-	identifier to set
//		int				set_idx	-	index of the set to change the id for, if -1 (default), change
//									the id for the variable
//
// Returns:
//	
//		0 - success
//		non-zero - non-unique id	
//
// Author:	Michael Zarozinski
// Date:	8/99
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
int FuzzyVariableBase::set_id(const wchar_t* _id, int set_idx /* = -1 */)
{
	if (_id == NULL || wcslen(_id) == 0)
		{
		id = L"";
		return 0;
		}

	// if a set_idx is passed in, set the name for that set
	if (set_idx >= 0)
		{
		int ret_val = sets[set_idx]->set_id(_id);

		if (ret_val)
			{
			// read the error from the set and set it for 'this'
			set_msg_text(sets[set_idx]->get_msg_text());
 			return ret_val;  
			}

		return 0;
		}

 	FuzzyModelBase* par = get_parent();

	// make sure our id is UNIQUE within the rule block
  	if (par->is_var_id_unique(_id, get_index()) == false)
		{
		// read the error from the parent and set it for 'this'
		set_msg_text(par->get_msg_text());
 		return -1;  
		}
 
	// set the identifier
 	id = _id;

	return 0;

}; // end FuzzyVariableBase::set_id()
 
//
// Function:	add_set()
// 
// Purpose:		Insert a set into this variable.  NOTE: that a COPY of the
//				set passed in is inserted into the var so the caller must
//				free the object if it dynamically allocated it.
//
// Arguments:
//
//		const FuzzySetBase* _new_set - set to insert a copy of
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
int FuzzyVariableBase::add_set(const FuzzySetBase* _new_set)
{ 
 
	// allocate temp memory which is the current number of sets PLUS 1
	FuzzySetBase**	tmp_sets = new FuzzySetBase*[num_of_sets + 1];

	// copy the old memory to the temp mem...
	for (int i = 0; i < num_of_sets; i++)
		{
 		tmp_sets[i] = (FuzzySetBase*)sets[i];
		}
 
 	tmp_sets[num_of_sets] =  new_set(); // insert empty set 

	tmp_sets[num_of_sets]->copy(*_new_set); // copy the set we passed in

	// ensure that the set id is unique... if the user changed the id we
	// performed a check PRIOR to calling this, so it'll be unique, if
	// we get there through cut/copy/paste or something else we 
	// may need to change the name...
 
	std::wstring tmp_name; // temp name
 		 
	std::wstring set_name = tmp_sets[num_of_sets]->get_id();

 	while (!(is_set_id_unique(set_name.c_str(),  get_num_of_sets())))
		{
		// save the name
		tmp_name = set_name;

		// just keep adding "Copy of" until we get a unique id
		set_name = load_string(STR_COPY_OF);
		set_name += L" " + tmp_name;

		// clear the message text...
		set_msg_text();

		} // end wile set is NOT unique
   
	tmp_sets[num_of_sets]->set_id(set_name.c_str());

	 // set the set index...
	tmp_sets[num_of_sets]->set_index(num_of_sets);

	tmp_sets[num_of_sets]->calc();

	num_of_sets++;	// increment the number of sets

	// free old memory...
	delete[] sets;
 
	// copy new mem to old
	sets = tmp_sets;
  
	return 0;

} // end FuzzyVariableBase::add_set()
 
 
//
// Function:	set_x_array_count()
// 
// Purpose:		Set the value for x_array_count. This also sets the
//				x_array_max_idx value appropriately. Note: this is a static
//				function so it can't call non-static member funcs which
//				means the caller needs to make sure the idx_multiplier
//				variables are re-calculated.
//
// Arguments:
//
//		int _count - value to set x_array_count to.
//
// Returns:
//
//		0 - success
//		-1 - invalid value passed in, set x_array_count to max allowed by datatype
//
// Author:	Michael Zarozinski
// Date:	5/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//

int FuzzyVariableBase::set_x_array_count(int _count)
{
	// if they're trying to set over max, set to max and return -1
	if (_count > USHRT_MAX)
		{
		x_array_count = USHRT_MAX;
		x_array_max_idx = x_array_count - 1;
		return -1;
		}
	
	x_array_count = _count;
	x_array_max_idx = x_array_count - 1;
 
	return 0;

}; // end FuzzyVariableBase::set_x_array_count()


//
// Function:	set_dom_array_count()
// 
// Purpose:		Set the value for dom_array_count. This also sets the
//				dom_array_max_idx value appropriately.
//
// Arguments:
//
//		int _count - value to set dom_array_count to.
//
// Returns:
//
//		0 - success
//		-1 - invalid value passed in, set dom_array_count to max allowed by datatype
//
// Author:	Michael Zarozinski
// Date:	5/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//

int FuzzyVariableBase::set_dom_array_count(int _count)
{
	// if they're trying to set over max, set to max and return -1
	if (_count > UCHAR_MAX)
		{
		dom_array_count = UCHAR_MAX;
		dom_array_max_idx = x_array_count - 1;
		return -1;
		}
	
	dom_array_count = _count;
	dom_array_max_idx = dom_array_count - 1;
 
	return 0;	

}; // end FuzzyVariableBase::set_dom_array_count()


//
// Function:	save_var_to_fcl_file()
// 
// Purpose:		Write out the FCL (Fuzzy Control Language) for this variable. This is written
//				inside a FUNCTION_BLOCK section and has the format (as defined by the
//				IEC 61131-7 International Standard):
//
//					VAR_INPUT Input parameter declaration
//						variable_name: data_type; (* RANGE *)
//						....
//					END_VAR
//
//				NOTE: we augment the standard by writing out the variable's range in a comment
//				after the semi-colon. Without this we won't know the left/right values for the
//				varaible when a FCL file is read in.
//
// Arguments:
//
//		std::ofstream& file_contents - STL stream to write out to a file
//
/// Returns:
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
void FuzzyVariableBase::save_var_to_fcl_file(std::ofstream& file_contents)
{
	// get the ascii version of the id...
	char* aid = convert_to_ascii(get_id());
	 
	// store the ORIGINAL version BEFORE we replace any spaces so we can report on it
	std::string tmp = aid;

	delete[] aid;

	// now get the version with spaces replaced by underscores
	aid = convert_to_ascii(get_id(), '_');

	// NOTE: the IEC 61131-7 does not specify a way to set the range of a varaible, so 
	// we write them out in a range comment

	file_contents << "\t" << aid <<  "\tREAL; (* RANGE(" << get_left_x() << " .. " << get_right_x() << ") *) ";

	delete[] aid;

	FuzzyModelBase::validate_fcl_identifier(file_contents, tmp);

	file_contents << "\n";

} // end FuzzyVariableBase::save_var_to_fcl_file()


//
// Function:	save_sets_to_fcl_file()
// 
// Purpose:		Write out the FCL (Fuzzy Control Language) for the sets in this variable
//				as defined by the IEC 61131-7 International Standard:
//
//					FUZZIFY variable_name
//						TERM term_name:= membership_function;
//						....
//					END_FUZZIFY
//
// Arguments:
//
//		std::ofstream& file_contents - STL stream to write out to a file
//
/// Returns:
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
void FuzzyVariableBase::save_sets_to_fcl_file(std::ofstream& file_contents)
{
 	FuzzySetBase*	set;	// set we're dealing with
 
 	// get the ascii version of the id and replace any spaces
	// with an underscore...
	char* aid = convert_to_ascii(get_id(), '_');
 
	file_contents << "FUZZIFY " << aid << "\n";

	delete[] aid;

	// loop through the sets
	for (int i = 0; i < num_of_sets; i++)
		{
		set = get_set(i);
 
		set->save_to_fcl_file(file_contents);

		char* aid_set = convert_to_ascii(set->get_id());

		FuzzyModelBase::validate_fcl_identifier(file_contents, aid_set);

		delete[] aid_set;

		file_contents << "\n";

		} // end loop through sets

	file_contents << "END_FUZZIFY\n\n";

} // end FuzzyVariableBase::save_sets_to_file()

//
// Function:	load_sets_from_fcl_file()
// 
// Purpose:		Read in an FCL file and create the sets that are associated
//				with this variable. 
//				NOTE: This is a brain-dead parser, it can be made much more robust.
//
// Arguments:
//
//		std::ifstream& file_contents - FCL file in stream form
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
int FuzzyVariableBase::load_sets_from_fcl_file(std::ifstream& file_contents)
{
  	file_contents.seekg(0);	// go to start of file
 
	// get the ascii version of this variable's id...
	char* aid = convert_to_ascii(get_id());
 
	// search for keyword FUZZIFY followed by id of this var...
	std::string stoken, var_name;
	bool found = false;

	while (!found)
		{

		// read until we get to the start of the variable declaration
		do
			{
			file_contents >> stoken;

			// check for EOF
			if (file_contents.eof())
				{
				set_msg_text(ERR_EOF_READING_SETS);
				delete[] aid;
 				return -1; 
				}

			} while (strcmp(stoken.c_str(), "FUZZIFY") != 0 );
		
		// if we got here we have a FUZZIFY block... check if it's the right var...
		file_contents >> var_name;

	 	if (strcmp(var_name.c_str(), aid) == 0)
 			found = true;
 
		}; // end while !found

	delete[] aid;

	// we found "FUZZIFY <var name>", now loop until we find "END_FUZZIFY" reading
	// the sets which are in the format:
	// 
	//			TERM term_name:= membership_function;
	//
	// Where membership_function ::= ( point i), ( point j), ...
	// and point is either a single value (if singleton) or a list of x/y values (x, y)
	// and the values MAY have a decimal point.

	// read line-by-line to get the sets...

	char line[300];
 	RealType x_point[7], y_point[7];	// x/y points for the set - NOTE: we assume a max of 7 points

	char* set_name;
	char* token;
			
	// inlude ":=" in tokens so we skip the assignment operator
	char seps[]   = " ,()\t\n:=";
 
	while(1)
		{
		// read in the next line
		file_contents.getline(line, 300);

		// check for EOF
		if (file_contents.eof())
			{
			set_msg_text(ERR_EOF_READING_SETS);
 			return -1; 
			}

		token = strtok(line, seps);

		// if token is null... blank line
		if (token == NULL)
			continue;

		if (strcmp(token, "END_FUZZIFY") == 0)
			break;	// we're done

		// validate this is the "term" keyword
		if (stricmp(token, "term") != 0)
			{
			set_msg_text(ERR_INVALID_FILE_FORMAT); // could be a bit more "robuts" in our error reporting!
			return -1;
			}
		
		// get the set name
		set_name = strtok(NULL, seps);

		token = strtok(NULL, seps);
 
		if (token == NULL)
			{
			// ERROR - invalid format
			set_msg_text(ERR_INVALID_FILE_FORMAT); // could be a bit more "robuts" in our error reporting!
			return -1;
			}

		int num_of_points = 0;

		// handle special case of singletons.  They'll be of the form "<value>;"
		// read points until we reach the semicolon

		if (token[strlen(token) - 1] == ';')
			{

			// replace semi with null so we can convert
			token[strlen(token) - 1] = '\0';
			x_point[num_of_points] = atof(token);
			// set 'y' value to max
 			y_point[num_of_points]  = FuzzyVariableBase::get_dom_array_max_idx();

			num_of_points = 1;
			}
		else
			{
			// not a singleton, points are of the form "(<value> , <value>)"

			while (token[0] != ';')  
				{

				x_point[num_of_points] = atof(token);

				token = strtok(NULL, seps);
 
				if (token[0] == ';') 
					{
					if (num_of_points != 0)
						{
						// ERROR - invalid format
						set_msg_text(ERR_INVALID_FILE_FORMAT); // could be a bit more "robuts" in our error reporting!
						return -1;
						}

					// else singleton which is OK

 					y_point[num_of_points]  = 0; 
					num_of_points++;
					continue;
					} // end if ';' found

 				y_point[num_of_points]  = atof(token);

				num_of_points++;

				token = strtok(NULL, seps);

				if (token == NULL)
					{
					// ERROR - invalid format
					set_msg_text(ERR_INVALID_FILE_FORMAT); // could be a bit more "robuts" in our error reporting!
					return -1;
					}

				} // end while still reading values

			} // end if not singleton

		// set the membership func dependent on the number of datapoints....
		int type;	// type of new member func
		switch(num_of_points)
			{
 			case 1:
 				type = MemberFuncBase::SINGLETON;
				break;
			case 3:
				type = MemberFuncBase::TRIANGLE;
				break;
			case 4:
				type = MemberFuncBase::TRAPEZOID;
				break;
			case 7:
				type = MemberFuncBase::S_CURVE;
				break;
			default:
				return -1; // error

			} // end switch on num_of_points

		// create the set, note we put fake values for width and stuff cuz that'll get
		// set when we set the points

		wchar_t* wset_name = convert_to_wide_char(set_name);

	 	FuzzySetBase* set = new_set(wset_name, 0, this, num_of_sets, 0, type);

		delete[] wset_name;

		if (set == NULL)
			return -1; // error is in msg_text
 
		// set up the nodes...

   		// loop points. Note we skip sanity checks set_node() uses
 		for (int i = 0; i < num_of_points  ; i++)
			{
			// convert values to indexes...
			int x = convert_value_to_idx(x_point[i]);
			int y = y_point[i] * FuzzyVariableBase::get_dom_array_max_idx();

			set->set_node(i, x, y, false); // false indicates skip sanity checks

			} // end loop through points

		// need to call ModelBase version to set up the the rule array
		(get_parent())->add_set(index, set);
		
		delete set;

		}; // end while(1) loop

	return 0;

} // end FuzzyVariableBase::load_sets_from_file()
 

//
// Function: calc()
// 
// Purpose:	This function calculates the 'y' values for all the sets
//			associated with this variable
// 
// Arguments:
//
//		int set_idx -	index of the set to calculate for. If -1 (default value)
//						do ALL the sets
//
// Returns:
//
//		void
// 
// Author:	Michael Zarozinski	
// Date:	1/21/99
// 
// Modification History
// Author		Date		Modification
// ------		----		------------
//
//

void FuzzyVariableBase::calc(int set_idx /* = -1 */)
{
	if (set_idx >= 0)
		{
		sets[set_idx]->calc();
		}
	else
		{
 		for (int i = 0; i < num_of_sets; i++)
 			sets[i]->calc();
		}
 
} // end FuzzyVariableBase::calc(void)
 


//
// Function: convert_idx_to_value()
// 
// Purpose:	This function converts an index (into the values[] array) to 
//			an 'x' value between left_x and right_x for this variable
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
// Date:	1/99
// 
// Modification History
// Author		Date		Modification
// ------		----		------------
// Michael Z	10/23/01	Added sanity checks
//
//

RealType FuzzyVariableBase::convert_idx_to_value(int idx) const
{
	assert(idx >= 0);
	assert(idx <= FuzzyVariableBase::get_x_array_count());

	// make sure index is within range
	if (idx < 0)
		idx = 0;
	if (idx > FuzzyVariableBase::get_x_array_max_idx())
		idx = FuzzyVariableBase::get_x_array_max_idx();

	return left_x + (idx * get_idx_multiplier());
};

//
// Function: convert_value_to_idx()
// 
// Purpose:	This function converts the 'x' value passed in to
//			an index (into the values[] array) 
// 
// Arguments:
//
//		RealType - x value to convert
//
// Returns:
//
//		ValuesArrCountType idx -	index for the value
// 
// Author:	Michael Zarozinski	
// Date:	1/99
// 
// Modification History
// Author		Date		Modification
// ------		----		------------
// Michael Z	10/23/01	Added sanity checks
//
//

ValuesArrCountType FuzzyVariableBase::convert_value_to_idx(RealType value) const
{
	// add .5 so any floating point values get rounded correctly when converting to int
	int idx = ((value -  get_left_x()) / get_idx_multiplier()) + .5;

	// make sure index is within range
	if (idx < 0)
		idx = 0;
	if (idx > FuzzyVariableBase::get_x_array_max_idx())
		idx = FuzzyVariableBase::get_x_array_max_idx();

	return idx;
};


//
// Function: init()
// 
// Purpose:	Perform common variable initialization
// 
// Arguments:
//
//		const wchar_t*	_id					-	identifier for the variable
//		RealType		_left_x				-	left x value
//		RealType		_right_x			-	right x value
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
		
int FuzzyVariableBase::init(const wchar_t* _id, RealType _left_x, RealType _right_x, bool create_unique_id /* = true */)
{
	if (set_left_x(_left_x))
		return -1;

	if (set_right_x(_right_x))
		return -1;

	if (init(_id, create_unique_id)) // virtual func to set in/out var appropriately
		return -1;

	calc();

	return 0;

} // end FuzzyVariableBase::init()


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
		
int FuzzyVariableBase::init(const wchar_t* _id, bool create_unique_id /* = true */)
{
	int name_len;	// length of the name

	if (_id == NULL)
		name_len = 30;
	else
		name_len = wcslen(_id) + 5;

	wchar_t* tmp_name = new wchar_t[name_len]; // temp var to hold var name

	// if no name is passed in name it "Variable 0"
 	if (_id == NULL)
		swprintf(tmp_name, name_len, L"%ls 1", load_string(STR_VARIABLE));    
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

 		int counter = 1; // value to add to identifier to make it unique

		while (set_id(tmp_name))
			{
			// rename variable...
			if (_id == NULL)
				swprintf(tmp_name, name_len, L"%ls %d", load_string(STR_VARIABLE), counter  );  
 			else
				swprintf(tmp_name, name_len, L"%ls %d", _id, counter );  

			counter++;
 
			// clear message text (should say "var not unique" right now)
			set_msg_text();
			
			} // end while !unique id

		}// end if rename var if not unique

	// free memory
	delete[] tmp_name;

	return ret_value;

} // end FuzzyVariableBase::init()


/////////////////////////////////////////////////////////////////////
////////// Trivial Functions That Don't Require Headers /////////////
/////////////////////////////////////////////////////////////////////
		
void FuzzyVariableBase::move_node(int set_idx, int  idx, _point pt)
{
 	sets[set_idx]->move_node( idx, pt);
};

void FuzzyVariableBase::move_node(int set_idx, int  idx, int x, int y)
{
 	 sets[set_idx]->move_node( idx, x, y);
};
		
FuzzySetBase* FuzzyVariableBase::get_set(int idx) const
{
	return((idx < num_of_sets)? sets[idx] : NULL);
}; 
		
int FuzzyVariableBase::get_num_of_sets(void) const 
{
	return(num_of_sets);
};
		
FFLL_INLINE short FuzzyVariableBase::get_index() const
{
	return index;
};
FFLL_INLINE void FuzzyVariableBase::set_index(int idx)
{ 
	index = static_cast<short>(idx);
}; 
 		
FuzzyModelBase* FuzzyVariableBase::get_parent(void) const
{
	return static_cast< FuzzyModelBase*>(FFLLBase::get_parent());
};
 

const wchar_t* FuzzyVariableBase::get_id(int set_idx /* = -1 */) const
{
	if (set_idx == -1)
		return((id == L"") ? NULL : id.c_str()); 
	else
		{
		FuzzySetBase* set = get_set(set_idx);
		return set->get_id(); 
		}

};  


int FuzzyVariableBase::get_rule_index(int _set_idx /* = -1 */) const
{
	if (_set_idx == -1)
		return rule_index;

	return sets[_set_idx]->get_rule_index();
};  

DOMType FuzzyVariableBase::get_dom(int set_idx, int x_position /* = -1 */) const
{
	if (sets)
		return sets[set_idx]->get_dom(x_position);  
	else
		return 0;
};
		
void FuzzyVariableBase::set_rule_index(int _rule_index, int set_idx /* = -1 */)
{
	if (set_idx == -1)
		rule_index = _rule_index;
	else
		{
		FuzzySetBase* set = get_set(set_idx);
		set->set_rule_index(_rule_index);
		} // end if set rule_index for set

};  
 

   
const char* FuzzyVariableBase::get_model_name() const
{

 	return get_parent()->get_model_name();
};
 
RealType FuzzyVariableBase::get_idx_multiplier() const
{
	return idx_multiplier;
};

void FuzzyVariableBase::move(int set_idx, int node_idx, int new_x)
{
	// need to calc the 'x' delta...
	int x_delta = new_x - sets[set_idx]->get_node_x(node_idx);

	sets[set_idx]->move(x_delta);
};

bool FuzzyVariableBase::is_output() const 
{
	return false;
}; 
		
const char* FuzzyVariableBase::get_fcl_block_start() 
{ 
	return "VAR_INPUT"; 
} 		
const char* FuzzyVariableBase::get_fcl_block_end() 
{ 
	return "END_VAR"; 
} 

RealType FuzzyVariableBase::get_left_x() const 
{	
	return left_x;
}
RealType FuzzyVariableBase::get_right_x() const 
{	
	return right_x;
} 

ValuesArrCountType FuzzyVariableBase::get_x_array_count() 
{	
	return x_array_count;
};
ValuesArrCountType FuzzyVariableBase::get_x_array_max_idx()
{	
	return x_array_max_idx;
};

DOMType FuzzyVariableBase::get_dom_array_count()
{	
	return dom_array_count;
};
DOMType FuzzyVariableBase::get_dom_array_max_idx()
{	
	return 	dom_array_max_idx;
};
 

FuzzySetBase* FuzzyVariableBase::new_set()
{
	return new FuzzySetBase(this);
};
 
FuzzySetBase* FuzzyVariableBase::new_set(wchar_t* n, int mid_pt_x, FuzzyVariableBase* par, short index, int set_width, int type)
{
	FuzzySetBase* tmp = new_set(); // virtual function
	
	tmp->init(n, mid_pt_x, index, set_width, type);

	return tmp;
 
};  
