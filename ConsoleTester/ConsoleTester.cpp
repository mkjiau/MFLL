//
// File:	AIWisdom.cpp	
//
// Purpose:	Test program for FFLL API
//
// Copyright ?2001 Louder Than A Bomb! Software
//
//
//
#include "stdafx.h"
#include "FFLLAPI.h"	// FFLL API
#include <iostream>	// for i/o functions

#define OUR_HEALTH	0 // our health is 1st variable
#define ENEMY_HEALTH	1 // enemy health is 2nd variable

using namespace std;

int main(int argc, char* argv[])
{
	string myFCL = " \
FUNCTION_BLOCK \n\
\n\
VAR_INPUT \n\
	Our_Health	REAL; (*RANGE(0 .. 100) *) \n\
	Enemy_Health	REAL; (*RANGE(0 .. 100) *) \n\
END_VAR \n\
\n\
VAR_OUTPUT \n\
	Aggressiveness	REAL; (*RANGE(0 .. 4) *) \n\
END_VAR \n\
\n\
FUZZIFY Our_Health \n\
	TERM Near_Death : = (0, 0) (0, 1) (50, 0); \n\
	TERM Good : = (14, 0) (50, 1) (83, 0); \n\
	TERM Excellent : = (50, 0) (100, 1) (100, 0); \n\
END_FUZZIFY \n\
\n\
FUZZIFY Enemy_Health \n\
	TERM Near_Death : = (0, 0) (0, 1) (50, 0); \n\
	TERM Good : = (14, 0) (50, 1) (83, 0); \n\
	TERM Excellent : = (50, 0) (100, 1) (100, 0); \n\
END_FUZZIFY \n\
\n\
FUZZIFY Aggressiveness \n\
	TERM Run_Away : = 1; \n\
	TERM Fight_Defensively : = 2; \n\
	TERM All_Out_Attack : = 3; \n\
END_FUZZIFY \n\
\n\
DEFUZZIFY valve \n\
	METHOD : MoM; \n\
END_DEFUZZIFY \n\
\n\
RULEBLOCK first\n\
	AND:MIN;\n\
	ACCUM:MAX;\n\
	RULE 0: IF Good AND Good THEN Fight_Defensively;\n\
	RULE 1: IF Good AND Excellent THEN Fight_Defensively;\n\
	RULE 2: IF Good AND Near_Death THEN All_Out_Attack;\n\
	RULE 3: IF Excellent AND Good THEN All_Out_Attack;\n\
	RULE 4: IF Excellent AND Excellent THEN Fight_Defensively;\n\
	RULE 5: IF Excellent AND Near_Death THEN All_Out_Attack;\n\
	RULE 6: IF Near_Death AND Good THEN Run_Away;\n\
	RULE 7: IF Near_Death AND Excellent THEN Run_Away;\n\
	RULE 8: IF Near_Death AND Near_Death THEN Fight_Defensively;\n\
END_RULEBLOCK\n\
\n\
END_FUNCTION_BLOCK \n\
";

	float	our_health, enemy_health; // values for input variables
	char	option;	// var for selection of what user wants to do

	cout.setf(ios::fixed);
	cout.precision(2); // only display 2 decimal places

					   // create and load the model
	int model = ffll_new_model();

	int ret_val = (int)ffll_load_fcl_string(model, myFCL.c_str());
	//int ret_val = (int)ffll_load_fcl_file(model, "D:\\Playground\\mc-dlls-dev\\MultiCharts_Dll\\x64\\aiwisdom.fcl");

	if (ret_val < 0)
	{
		//getchar();
		cout << "Error Opening aiwisdom.fcl";
		// make sure the "working directory" in "Project | Settings..."
		// is set to the executable's directory if running from the MSVC IDE
		return 0;
	}

	// create a child for the model...
	int child = ffll_new_child(model);

	while (1)
	{
		cout << "SELECT AN OPTION:\n\tS - set values\n\tQ - quit";
		cout << endl;
		cin >> option;

		if (option == 'Q' || option == 'q')
			break;

		if (option == 'S' || option == 's')
		{
			cout << "Our Health: ";
			cin >> our_health;
			cout << "Enemy's Health: ";
			cin >> enemy_health;
			cout << "Aggressiveness: ";

			// set input variables...

			ffll_set_value(model, child, OUR_HEALTH, our_health);
			ffll_set_value(model, child, ENEMY_HEALTH, enemy_health);

			// get and display the output value
			int output = (int)ffll_get_output_value(model, child);

			switch (output)
			{
			case (1):
				cout << "Run Away!";
				break;

			case (2):
				cout << "Fight Defensively";
				break;

			case (3):

				cout << "All Out Attack!";
				break;

			} // end switch

			cout << endl;
		} // end if option = 's'

	} // end while(1)

	

	return 0;

} // end main()


