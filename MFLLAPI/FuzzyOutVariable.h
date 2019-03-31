//
// File:	FuzzyOutVariable.h
//
// Purpose:	Interface for the FuzzyOutVariable class.   
//
// Copyright © 1999-2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//

#if !defined(AFX_FUZZYOUTVARIABLE_H__71AD4A4D_2C78_414A_B0A5_244D15E0D0A8__INCLUDED_)
#define AFX_FUZZYOUTVARIABLE_H__71AD4A4D_2C78_414A_B0A5_244D15E0D0A8__INCLUDED_
 
#include "FuzzyVariableBase.h"
class FuzzyModelBase;
class FuzzyOutSet;
class DefuzzVarObj;
 
//
// Class:	FuzzyOutVariable
//
// This class is for a model's output variable. It's basedo n FuzzyVariableBase
// and holds information to deal with defuzzification (a process that only applys to
// output variables) and makes sure that the output variable contains FuzzyOutSet objects
// as opposed to the FuzzySetBase that the input variables use.
//
class   FuzzyOutVariable : virtual public FuzzyVariableBase  
{
	////////////////////////////////////////
	////////// Member Functions ////////////
	////////////////////////////////////////

	public:

		// construct/destruct functions
 		FuzzyOutVariable(); // No function body for this. Explicitly disallow auto-creation of it by the compiler
		FuzzyOutVariable(FuzzyModelBase* _parent);
		virtual ~FuzzyOutVariable();
		virtual int init(const wchar_t* _id, RealType _left_x, RealType _right_x, bool create_unique_id = true);
		virtual int init(const wchar_t* _id, bool create_unique_id = true);

		// get functions
		int get_defuzz_method();
		int get_composition_method();
		static const char* get_fcl_block_start() ;
		static const char* get_fcl_block_end() ;
		FuzzyOutSet* get_set(int idx) const; 

		// set functions
		int set_defuzz_method(int type);
		virtual int set_composition_method(int method);

		// misc functions
		FuzzySetBase* new_set();
		virtual bool is_output() const;
		virtual RealType calc_output_value(DOMType* out_set_dom_arr ) const;  
		virtual RealType convert_idx_to_value(int idx) const;

	////////////////////////////////////////
	////////// Class Variables /////////////
	////////////////////////////////////////

	public:

		enum COMPOSITION_OPERATION { COMPOSITION_OPERATION_MIN, COMPOSITION_OPERATION_MAX };	// composition is the type of operation to apply to the output set.  When we evaluate the rules, we
													// may have more than one DOM for a single output set.  
													//
													// For (a trivial) example consider a system with 2 input variables and one output
													// variable we may have rules:
													//
													// if var1.set1 and var2.set1 then out.set1
													// if var1.set1 and var2.set2 then out.set1
													//
													// Assume the values:
													//		var1.set1 = .5
													//		var2.set1 = .25
													//		var2.set2 = .75
													//
													// After applying the rules we have 2 values for out.set1:  .25 and .5 (assuming we
													// take the MIN when "anding" the inputs together)
													//
													// We must combine those values via a "composition subprocess" to come up with a single
													// value for the output set.
													// if we use MIN (same as logical AND) we get a value of .25
													// if we use MAX (same as logical OR) we get a value of .5
													//


	protected:

		DefuzzVarObj*	defuzz_obj;	// defuzzification object - this holds the functions to calc the defuzzified
									// output value.  Each FuzzyOutSet holds defuzzification specific information
									// that the defuzz object can use

	private:

 		int composition_method;	// composition method for this model

}; // end class FuzzyOutVariable

#else

class FuzzyOutVariable; // already included

#endif // !defined(AFX_FUZZYOUTVARIABLE_H__71AD4A4D_2C78_414A_B0A5_244D15E0D0A8__INCLUDED_)
