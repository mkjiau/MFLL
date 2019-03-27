//
// File:	COGDefuzz.h	
//
// Purpose:	Center of Gravity Defuzzification method
//
// Author:	Michael Zarozinski
// Date:	07/03/01
//
// Copyright © 2001 Louder Than A Bomb! Software
//
// $Log: COGDefuzz.cpp,v $
// Revision 1.2  2001-12-18 15:34:07-05  michael_zarozinski
// <>
//
// Revision 1.0  2001-12-11 20:43:08-05  michael_zarozinski
// Initial revision
//
// Revision 1.1  2001-08-05 12:37:50-04  michael_zarozinski
// check in before change defuzz obj - right now it's held in the member_func class and that doesn't work very well for things like MOM
//
// Revision 1.0  2001-08-01 21:48:18-04  michael_zarozinski
// Initial revision
//
//
//

// Static variables
static const char *Id = "$Header: C:\\Work\\RCS\\C\\Work\\Source\\Spark\\ClassLib\\SparkDll\\COGDefuzz.cpp,v 1.2 2001-12-18 15:34:07-05 michael_zarozinski Exp michael_zarozinski $";

// include files
#include "COGDefuzz.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

#include "debug.h"

#endif


//
// Function:
// 
// Purpose:
//
// Arguments:
//
//
// 
// Algorithm:
//
//
// 
// Returns:
//
//
//
// Globals:
//
//		none
//
// Author:	Michael Zarozinski
// Date:
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
COGDefuzz::COGDefuzz()
{
	values = NULL;

}; // end COGDefuzz::COGDefuzz()

COGDefuzz::COGDefuzz(int array_max)
{
if (array_max > 0)
	values = new _cog_struct[array_max];
// ???? should be an init() function????

}; // end COGDefuzz::COGDefuzz()

COGDefuzz::~COGDefuzz()
{
	if (values)
		delete[] values;
}; // end COGDefuzz::~COGDefuzz()

int COGDefuzz::get_defuzz_type(){ return DefuzzBase::COG;};

float COGDefuzz::get_cog(int _idx){return values[_idx].cog;};
float COGDefuzz::get_area(int _idx){return values[_idx].area;};
float COGDefuzz::get_moment(int _idx){return values[_idx].moment;};
unsigned char COGDefuzz::get_idx(int _idx){return values[_idx].idx;};

void COGDefuzz::set_cog(int _idx, float val){  values[_idx].cog = val;};
void COGDefuzz::set_area(int _idx, float val){  values[_idx].area = val;};
void COGDefuzz::set_moment(int _idx, float val){  values[_idx].moment = val;};
void COGDefuzz::set_idx(int _idx, unsigned char val){  values[_idx].idx = val;};