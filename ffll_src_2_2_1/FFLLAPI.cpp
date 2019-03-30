//
// File:	FFLLAPI.cpp
//
// Purpose:	API for FFLL
//
// Copyright ?2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//

// include files
#include "FFLLAPI.h"
#include "FFLLBase.h"
#include "FuzzyModelBase.h"
#include "FuzzyOutVariable.h"
#include "MemberFuncBase.h"
#include <vector>
#include <list>
#include <windows.h>

class ModelContainer;	// forward declaration

// local function
ModelContainer* get_model(int idx);

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Declare the classes here, rather than in a .h so everything is in one place
// 
// Class:	ModelChild
//
// This class represents a child of the FFLL model. Each child maintains
// an array of indexes into the values[] array for each input variable and
// an array of DOM (Degree of Membership) for the sets in the output variable.
// This allows each child to be thread-safe and can pass this information to 
// the FuzzyModelBase object to perform calcuations and get the defuzzified value.
//
// NOTE: This class currently is NOT thread-safe.
//

class ModelChild
{
	// everything is public cuz these are only used in this file.
	public:
		ModelChild(const FuzzyModelBase* model)
			{
			assert(model != NULL);  

			int num_vars = model->get_input_var_count();
			int num_out_sets = model->get_num_of_sets(OUTPUT_IDX);

			out_set_dom_arr = new DOMType[num_out_sets];
		 	var_idx_arr = new short[num_vars];

			int i;	// counter

			for (i = 0; i < num_vars; i++)
			  	var_idx_arr[i] = 0;  
 
			for (i = 0; i < num_out_sets; i++)
 			  	out_set_dom_arr[i] = 0; 
 
			}; // end constructor
		virtual ~ModelChild()
			{
 			if (var_idx_arr)
		 		delete[] var_idx_arr;
			if (out_set_dom_arr)
				delete[] out_set_dom_arr;
			};

		DOMType	*out_set_dom_arr;	// array of that holds the DOM for each set in the output variable
		short  *var_idx_arr;		// array that holds the index into the values[] array for each input variable
	
}; // end class ModelChild
 
// 
// Class:	ModelContainer
//
// This is a "container" for a model. It holds a pointer to a FuzzyModelBase
// object and a list of children that can use the model. Each model can have multiple
// children because model constains information that is common to all children and does
// not change. 
//


class ModelContainer
{
	// everything is public cuz these are only used in this file.
	public:
		ModelContainer()
			{
			model = NULL;
			}	
		
		void init()
			{
			model = new FuzzyModelBase();
			model->init();
			};

	     // Copy constructor
        ModelContainer(const ModelContainer& obj) 
			{
			*this = obj; 
			};

        // Assignment operator
        ModelContainer& operator=(const ModelContainer& obj)
			{
			model = obj.model;

			return *this;
			};  
	
		virtual ~ModelContainer()
			{
			if (model)
				{
				delete model;
				model = NULL;
				}

			// free the memory we allocated for the children

			std::vector<ModelChild*>::iterator it = child_list.begin();
			for (int i=0; it < child_list.end(); ++i, it++)
				{
				delete child_list[i];
				}
 
			}; // end destructor

	 	std::vector<ModelChild*> child_list;	// list of the children for this fuzzy model
		FuzzyModelBase* model;					// model this container holds

}; // end class ModelContainer

 
// using a vector array to hold open models. It holds the actual object 
// (as opposed to a pointer to it) so the destructor is called when the program ends
// and we don't have to require the user to call a "close_model" type function to free memory.

// 4/03 changed to LIST to avoid destructor calls when inserting models.
std::list<ModelContainer > model_list; // list of the fuzzy models being used

//
// Function:	ffll_new_child()
// 
// Purpose:		Create a new child for the model passed in.
//
// Arguments:	
//
//		int		model_idx	- index of the model 
//
// Returns:
//
//		The index of the child for the model
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
int WIN_FFLL_API ffll_new_child(int model_idx)
{
	ModelContainer* container = get_model(model_idx);

	assert(container->model != NULL); // make sure you call ffll_load_fcl_file() before this func.

	ModelChild* child = new ModelChild(container->model);
 
	container->child_list.push_back(child);

	return container->child_list.size() - 1;
 
}; // end ffll_get_child()

//
// Function:	ffll_set_value()
// 
// Purpose:		Set the variable's index for the value passed in.
//
// Arguments:	
//
//		int			model_idx	- index of the model 
//		int			child_idx	- index of the child
//		int			var_idx		- index of the variable
//		RealType	value		- value to set the variable to
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
int WIN_FFLL_API ffll_set_value(int model_idx, int child_idx, int var_idx, double value)
{
	ModelContainer* container = get_model(model_idx);

	ModelChild* child = container->child_list[child_idx];

	// convert value to an index into the values[] array
	ValuesArrCountType idx = container->model->convert_value_to_idx(var_idx, value);
 		
	child->var_idx_arr[var_idx] = idx;

	return 0;
 
}; // end ffll_set_value()

//
// Function:	ffll_get_output_value()
// 
// Purpose:		Gets the defuzzified output value for
//				the model/child pair passed in
//
// Arguments:	
//
//		int	model_idx	- index of the model 
//		int	child_idx	- index of the child
//
// Returns:
//
//		Output value for the model (FLT_MIN is returned if no
//		output sets are active)
//
// Author:	Michael Zarozinski
// Date:	9/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
// 
 
double WIN_FFLL_API ffll_get_output_value(int model_idx, int child_idx)
{
	ModelContainer* container = get_model(model_idx);

	// get the child
	ModelChild* child = container->child_list[child_idx];

	// pass in the input value for each input variable and the array
	// of DOMs for the output sets
	RealType out_val = container->model->calc_output(child->var_idx_arr, child->out_set_dom_arr); 

	return out_val;
 
}; // end ffll_get_output_value()

 
//
// Function:	ffll_load_fcl_file()
// 
// Purpose:		This function initializes the model and
//				loads the FCL file passed in
//
// Arguments:	
//
//		int			model_idx	- index of the model 
//		const char*	file		- file name and path of the file to load
//
// Returns:
//
//		The index of the model on success
//		-1 on error
//
// Author:	Michael Zarozinski
// Date:	9/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
// 
 
int WIN_FFLL_API ffll_load_fcl_file(int model_idx, const char* file)
{
	ModelContainer* container = get_model(model_idx);

	// we have a pointer to the container, so any modifications we
	// make will be in the list. If we used the [] operator here,
	// we'd get a COPY of the model and any changes wouldn't be permanent.

	// perform initialization
	container->init();

	// load the FCL file passed in
	if (container->model->load_from_fcl_file(file))
 		return -1;

	return model_idx;

}; // end ffll_load_fcl_file()


//
// Function:	ffll_load_fcl_string()
// 
// Purpose:		This function initializes the model and
//				loads the FCL file passed in
//
// Arguments:	
//
//		int			model_idx	- index of the model 
//		const char*	fcl_str		- string input of fuzzy control language
//
// Returns:
//
//		The index of the model on success
//		-1 on error
//
// Author:	Ming-Kai Jiau
// Date:	2019/03/30
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
// 

int WIN_FFLL_API ffll_load_fcl_string(int model_idx, const char* fcl_str)
{
	ModelContainer* container = get_model(model_idx);

	// we have a pointer to the container, so any modifications we
	// make will be in the list. If we used the [] operator here,
	// we'd get a COPY of the model and any changes wouldn't be permanent.

	// perform initialization
	container->init();

	// load the FCL file passed in
	if (container->model->load_from_fcl_string(fcl_str))
		return -1;

	return model_idx;

}; // end ffll_load_fcl_string()


//
// Function:	ffll_get_msg_textA()
// 
// Purpose:		Get the ascii character version of the msg_txt
//				for the model.
//
// Arguments:	
//
//		int model_idx - index of the model to get the msg_txt for
//
// Returns:
//
//		const char* - pointer to the msg_txt string
//
// Author:	Michael Zarozinski
// Date:	9/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
// 

const char* WIN_FFLL_API ffll_get_msg_textA(int model_idx)
{
	ModelContainer* container = get_model(model_idx);

	return container->model->get_msg_textA();

}; // end ffll_get_msg_textA()


//
// Function:	ffll_get_msg_textW()
// 
// Purpose:		Get the wide character version of the msg_txt
//				for the model.
//
// Arguments:	
//
//		int model_idx - index of the model to get the msg_txt for
//
// Returns:
//
//		const wchar_t* - pointer to the msg_txt string
//
// Author:	Michael Zarozinski
// Date:	9/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
// 
const wchar_t* WIN_FFLL_API ffll_get_msg_textW(int model_idx)
{
	ModelContainer* container = get_model(model_idx);

	return container->model->get_msg_text();
 
}; // end ffll_get_msg_textW()


//
// Function:	ffll_new_model()
// 
// Purpose:		This function creates a new FuzzyModleBase object, inserts
//				it into the managment list and returns the index of the new
//				model into the list
//
// Arguments:	
//
//		none
//
// Returns:
//
//		The index of the model in the list
//
// Author:	Michael Zarozinski
// Date:	9/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
// 
int WIN_FFLL_API ffll_new_model()
{
	// create an empty container and put it at the back
	// of our list. 
	ModelContainer container;  
  
	// add it to the list of open models
	model_list.push_back(container); 
 
	return model_list.size() - 1;

}; // end ffll_new_model()

//
// Function:	get_model()
// 
// Purpose:		Returns a pointer to the model container at
//				the index passed in. This is a LOCAL function
//				and is not exported.
//
// Arguments:	
//
//		int idx - index of the model to return
//
// Returns:
//
//		ModelContainer*  - pointer to the model
//
// Author:	Michael Zarozinski
// Date:	9/01
// 
// Modification History
// Author		Date		Modification
// ------		----		------------
// Michael Z	4/03		Changed to use list iterator
// Michael Z	4/03		Changed the while loop so it returns the correct model
//
ModelContainer* get_model(int idx)
{
	// we have to use an interator (as opposed to just 
	// using model_list[idx]) so we can return a pointer
	// to the container. If we used the [] operator we'd
	// get a copy and writing a "deep copy" for that object
	// would introduce far more places things can go wrong.
   
  	std::list<ModelContainer>::iterator it;
 
	int tmp_idx = idx;

	it = model_list.begin();

	ModelContainer* ret_val = &(*it);

	while(tmp_idx--)
		{
		it++;
		ret_val = &(*it);
		}

	return ret_val;  
				
} // end get_model()
 

//
// Function:	ffll_close_model()
// 
// Purpose:		Closes the model for the index passed in.
//
// Arguments:	
//
//		int		model_idx	- index of the model 
//
// Returns:
//
//		0 - success
//		non-zero - failure
//
// Author:	Michael Zarozinski
// Date:	2/02
// 
// Modification History
//	Author		Date		Modification
//	------		----		------------
//	Michael Z	4/15/03		Ignore null model... 
//  
int WIN_FFLL_API ffll_close_model(int model_idx)
{
	ModelContainer* container = get_model(model_idx);

	if (container->model != NULL)
		return 0;

 	delete container;

	return 0;
 
}; // end ffll_close_model()


int WIN_FFLL_API jFuzzyInference(LPSTR fcl_str, double our_health, double enemy_health)
{
	// create and load the model
	int model = ffll_new_model();

	int ret_val = (int)ffll_load_fcl_string(model, fcl_str);
	//int ret_val = (int)ffll_load_fcl_file(model, "D:\\Playground\\mc-dlls-dev\\MultiCharts_Dll\\x64\\aiwisdom.fcl");


	// create a child for the model...
	int child = ffll_new_child(model);

	// set input variables...
	ffll_set_value(model, child, 0, our_health);
	ffll_set_value(model, child, 1, enemy_health);

	// get and display the output value
	int output = (int)ffll_get_output_value(model, child);


	return output;
}