//
// File:	FuzzyOutSet.h
//
// Purpose:	This is the base class for the output sets.   
//
// Copyright © 1999-2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//

#if !defined(AFX_FUZZYOUTSET_H__C20D825A_5D54_4AE9_8412_5AE3DD0D721F__INCLUDED_)
#define AFX_FUZZYOUTSET_H__C20D825A_5D54_4AE9_8412_5AE3DD0D721F__INCLUDED_
 
#include "FuzzySetBase.h"
class DefuzzSetObj;
class FuzzyOutVariable;

// 
// Class:	FuzzyOutSet
//
// This class is for the sets in the output variable. It contain a pointer
// to the DefuzzSetObj abstract class. DefuzzSetObj handles all the calcuations
// needed for the defuzzification process.
//
class  FuzzyOutSet : virtual public FuzzySetBase   
{

	////////////////////////////////////////
	////////// Member Functions ////////////
	////////////////////////////////////////

	public:

		FuzzyOutSet(FuzzyVariableBase* par) ;
		virtual ~FuzzyOutSet();
 
		// get functions
		DefuzzSetObj* get_defuzz_obj() const;
		RealType get_defuzz_x(int dom = -1);
 		FuzzyOutVariable* get_parent() const;

		// set functions
		int set_defuzz_method(int type);
 
	private:

		void calc();

	////////////////////////////////////////
	////////// Class Variables /////////////
	////////////////////////////////////////

	protected:

		DefuzzSetObj* defuzz_obj;	// this array holds the Defuzification object.  Right now
									// we're only doing COG so it's hardcoded but later we
									// can just use a common ancestor and have whatever method is
									// appropriate in here.
									// This is just a look up table and is done purely for speed
									// it will be slower while building the model cuz this array will have
									// to be recalculated every time an anchor/handle is moved but that
									// only happens when the output var is moved and we're MUCH more
									// concerned with the speed at run-time not at design time.
 
}; // end class FuzzyOutSet

#else

class FuzzyOutSet; // already defined

#endif // !defined(AFX_FUZZYOUTSET_H__C20D825A_5D54_4AE9_8412_5AE3DD0D721F__INCLUDED_)
