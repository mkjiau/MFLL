
//
// File:	FuzzyOutVariableBase.h
//
// Purpose:	The FuzzyOutVariableBase class is for output variables in a model
//
// Copyright © 1999-2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//
#if !defined(AFX_FUZZYOUTVARIABLEBASE_H__27AAF6DA_5610_4EF9_B5A6_447EF2BD5869__INCLUDED_)
#define AFX_FUZZYOUTVARIABLEBASE_H__27AAF6DA_5610_4EF9_B5A6_447EF2BD5869__INCLUDED_


#include "FuzzyVariableBase.h"

// 
// Class:	FuzzyOutVariableBase
//
// Class that has the output variable information. Each variable has a list of 
// sets that define the membership functions for the variable.
//

class FuzzyOutVariableBase : public FuzzyVariableBase  
{
	public:
		FuzzyOutVariableBase();
		virtual ~FuzzyOutVariableBase();

}; // end class FuzzyOutVariableBase

#endif // !defined(AFX_FUZZYOUTVARIABLEBASE_H__27AAF6DA_5610_4EF9_B5A6_447EF2BD5869__INCLUDED_)
