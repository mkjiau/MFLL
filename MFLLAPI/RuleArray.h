
//
// File:	RuleArray.h
//
// Purpose:	Interface for the RuleArray class.  This class represents
//			the rules associated with a set of variables.
//
// Copyright © 1999-2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//


#if !defined(AFX_RULEARRAY_H__0BBBA8C1_3467_11D3_B77E_709159C10001__INCLUDED_)
#define AFX_RULEARRAY_H__0BBBA8C1_3467_11D3_B77E_709159C10001__INCLUDED_
 
#include "FFLLBase.h"
class FuzzyModelBase;
 
// 
// Class:	RuleArray
//
// This class holds the rules array for the model
//

class  RuleArray  :  virtual public FFLLBase  
{
 
 	////////////////////////////////////////
	////////// Member Functions ////////////
	////////////////////////////////////////

	public:

		// constructor/destructor funcs
		RuleArray() ; // No function body for this. Explicitly disallow auto-creation of it by the compiler
		RuleArray(FuzzyModelBase* _parent) ; 
		virtual ~RuleArray();

		// get funcs
		int get_max() const ;
 		RuleArrayType get_rule(int index) const ;
		const char* get_model_name() const;
		FuzzyModelBase* get_parent(void) const;

		// set funcs
		int set(RuleArrayType* source, int size);

		// misc funcs
		virtual void free_memory();
		virtual int alloc(int size);	 
 		void clear(); 
 		void add_rule(int index, RuleArrayType output_set);
		void remove_rule(int index);
		bool no_rules() const ;

	////////////////////////////////////////
	////////// Class Variables /////////////
	////////////////////////////////////////

	protected:
		RuleArrayType*	rules;		// array of rules
 		int				max;		// max number of rules we've allocated space for
	
		
}; // end class RuleArray  

#else

class RuleArray; // already defined

#endif // !defined(AFX_RULEARRAY_H__0BBBA8C1_3467_11D3_B77E_709159C10001__INCLUDED_)
