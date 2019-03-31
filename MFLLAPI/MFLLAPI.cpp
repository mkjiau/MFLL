#include "MFLLAPI.h"
#include "FFLLAPI.h"

//
// Function:	MFLLFuzzyInference()
// 
// Purpose:		This function simplifies usage to offer one-stop fuzzy inference output
//				in which it consists of the required APIs from native FFLL library.
//
// Arguments:	
//
//		LPSTR fcl_str - string of core fuzzy engine in Fuzzy Control Language
//		double* crisp_inputs - array of ordered crisp inputs to fuzzy engine
//		long input_size - size of crisp inputs
//
// Returns:
//
//		double output value inferred by the fuzzy engine
//
// Author:	Ming-Kai Jiau
// Date:	2019/03/31
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
// 
double WIN_FFLL_API MFLLFuzzyInference(LPSTR fcl_str, double* crisp_inputs, long input_size)
{
	// create and load the model
	int model = ffll_new_model();

	int ret_val = (int)ffll_load_fcl_string(model, fcl_str);
	//int ret_val = (int)ffll_load_fcl_file(model, "D:\\Playground\\mc-dlls-dev\\MultiCharts_Dll\\x64\\aiwisdom.fcl");

	// create a child for the model...
	int child = ffll_new_child(model);

	// set input variables...
	for (int i = 0; i < input_size; i++)
		ffll_set_value(model, child, i, crisp_inputs[i]);

	// get and display the output value
	double output = ffll_get_output_value(model, child);

	ffll_close_model(model);

	return output;
}


//
// Function:	MFLLFuzzyInferenceByFile()
// 
// Purpose:		Load FCL from file.
//              This function simplifies usage to offer one-stop fuzzy inference output
//				in which it consists of the required APIs from native FFLL library.
//
// Arguments:	
//
//		LPSTR fcl_file - string of FCL file location 
//		double* crisp_inputs - array of ordered crisp inputs to fuzzy engine
//		long input_size - size of crisp inputs
//
// Returns:
//
//		double output value inferred by the fuzzy engine
//
// Author:	Ming-Kai Jiau
// Date:	2019/03/31
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
// 
double WIN_FFLL_API MFLLFuzzyInferenceByFile(LPSTR fcl_file, double* crisp_inputs, long input_size)
{
	// create and load the model
	int model = ffll_new_model();

	//int ret_val = (int)ffll_load_fcl_string(model, fcl_str);
	int ret_val = (int)ffll_load_fcl_file(model, fcl_file);

	// create a child for the model...
	int child = ffll_new_child(model);

	// set input variables...
	for (int i = 0; i < input_size; i++)
		ffll_set_value(model, child, i, crisp_inputs[i]);

	// get and display the output value
	double output = ffll_get_output_value(model, child);

	ffll_close_model(model);

	return output;
}