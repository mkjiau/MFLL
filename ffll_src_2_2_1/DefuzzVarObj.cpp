//
// File:	DefuzzVarObj.h	
//
// Purpose:	Base class for Defuzzification methods.  Decendents of this object are
//			of the output variable and contains all logic to calculate the
//			defuzzification
//
// Copyright © 2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//
 
// include files
#include "DefuzzVarObj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

#include "debug.h"

#endif


//
// Function:	DefuzzVarObj()
// 
// Purpose:		Constructor for COGDefuzzVarObj
//
// Arguments:	
//
//		FuzzyOutVariable* _parent - Output variable this object is part of
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
 
DefuzzVarObj::DefuzzVarObj(FuzzyOutVariable* _parent) : FFLLBase(_parent)
{
	// empty
}

//
// Function:	~DefuzzVarObj()
// 
// Purpose:		Destructor for COGDefuzzVarObj
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
// Date:	8/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
DefuzzVarObj::~DefuzzVarObj()
{
	// empty

}; // end DefuzzVarObj::~DefuzzVarObj()
 
/////////////////////////////////////////////////////////////////////
////////// Trivial Functions That Don't Require Headers /////////////
/////////////////////////////////////////////////////////////////////  

FuzzyOutVariable* DefuzzVarObj::get_parent() const
{ 
	return static_cast<FuzzyOutVariable*>(FFLLBase::get_parent());
};
