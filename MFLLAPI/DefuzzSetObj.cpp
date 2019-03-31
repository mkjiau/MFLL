//
// File:	DefuzzSetObj.cpp	
//
// Purpose:	Base class for Defuzzification methods.  Decendents of this object are part
//			of each set in the output variable and contains all logic to calculate the
//			defuzzification
//
// Copyright © 2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//
 

#include "DefuzzSetObj.h"
#include "FuzzyOutSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

#include "debug.h"

#endif

//
// Function:	DefuzzSetObj()
// 
// Purpose:		Constructor
//
// Arguments:
//
//		FuzzyOutSet* par - set that this is part of
//
// Returns:
//
//		none
//
// Author:	Michael Zarozinski
// Date:	6/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//
DefuzzSetObj::DefuzzSetObj(FuzzyOutSet* par) : FFLLBase(par)
{
	// nothing to do 
}

//
// Function:	~DefuzzSetObj()
// 
// Purpose:		Destructor
//
// Arguments:
//
//		none
//
// Returns:
//
//		none
//
// Author:	Michael Zarozinski
// Date:	6/01
// 
// Modification History
// Author	Date		Modification
// ------	----		------------
//
//

DefuzzSetObj::~DefuzzSetObj()
{
	// nothing to do
}

/////////////////////////////////////////////////////////////////////
////////// Trivial Functions That Don't Require Headers /////////////
/////////////////////////////////////////////////////////////////////   
		
FuzzyOutSet* DefuzzSetObj::get_parent() const 
{
	return static_cast<FuzzyOutSet*>(FFLLBase::get_parent());
};
 