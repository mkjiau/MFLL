
//
// File:	FuzzySetBase.h
//
// Purpose:	This is the base class for the Fuzzy sets.  It is the ancestor of
//			all set classes
//
// Copyright © 1999-2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//
 
#if !defined(AFX_FuzzySetBase_H__F5CADA04_27CC_11D3_B77E_B09059C10001__INCLUDED_)
#define AFX_FuzzySetBase_H__F5CADA04_27CC_11D3_B77E_B09059C10001__INCLUDED_
 
class MemberFuncBase;
class FuzzyVariableBase;
class FuzzyModelBase;

#include "FFLLBase.h"

// 
// Class:	FuzzySetBase
//
// This class is the base class for all sets. It's used (without desendents) in 
// the output variables. It contains a pointer to a member func class that holds
// the specific information for the curve that defines the set.
//

class   FuzzySetBase : virtual  public FFLLBase  
{ 
	////////////////////////////////////////
	////////// Member Functions ////////////
	////////////////////////////////////////
	
	public:

		// constructor/destructor funcs
		FuzzySetBase(); // No function body for this. Explicitly disallow auto-creation of it by the compiler
	 	FuzzySetBase(FuzzyVariableBase* par);
		virtual ~FuzzySetBase();
		int init(const wchar_t* n, int mid_pt_x, short idx, int set_width, int type );
 		virtual int copy(const FuzzySetBase& copy_from);
 	 	virtual FuzzySetBase& operator=(const FuzzySetBase& copy_from);

		// get functions
 
		const char* get_model_name() const;
		const wchar_t* get_id(void) const;
		NodePoint get_node(int idx);
		int get_node_count() const ;
		int get_var_index() const;
		int get_end_x(void) const;
		int get_start_x(void) const;
		int get_func_type() const;
 		int get_ramp();
		int get_node_x(int node_idx) const;
		int get_rule_index() const;
		RealType get_idx_multiplier() const;
		RealType  get_left_x() const; 
		void set_index(int _idx);
		DOMType get_value(int idx) const;
		DOMType get_index() const;
		virtual DOMType get_dom(int idx) const;

		// Set Functions
 
		int set_id(const wchar_t* _id, bool allow_dup = false);
		void set_ramp(int hi_lo_ind, int left_right_ind);
		void set_node(int idx, int x, int y, bool validate = false);
		void set_member_func(void* new_func);
		void set_rule_index(int idx);

		// misc funcs

		bool is_output() const ;
		RealType FuzzySetBase::convert_idx_to_value(int idx) const;
		virtual int new_member_func(int type, int start_x, int width);
 		virtual int new_member_func(int type);
 		void move_node(int anchor_idx, _point pt);
  		void move_node(int anchor_idx, int x, int y);
  		void move(int x_delta);
 		virtual void calc();
 		virtual void expand(int x_delta);
		virtual void shrink(int x_delta);
   
		// save/load functions
 
		void save_to_fcl_file(std::ofstream& file_contents);
 
 	protected:

		// get funcs
 		FuzzyModelBase* get_rule_block(); 

		// misc funcs
 		FuzzyVariableBase* get_parent() const;
 		bool is_set_id_unique() const;

 
	////////////////////////////////////////
	////////// Class Variables /////////////
	////////////////////////////////////////

	protected:
 		MemberFuncBase*	member_func;	// member function for this set. MemberFuncBase is an abstract
										// class.

	private:

		std::wstring	id;			// name of the variable 
		DOMType			index;		// idex for this set within it's variable
		int				rule_index;	// this is the offset into memory for this set.  it's
									// used to speed access to the rules.
									// this value is ADDED to the index of the set in the
									// preceeding variable.
									// For example, if we have a 3x3x3 system (3 input variables 
									// of 3 sets each) there would be 27 total rules.
									// The rule_indexes would be:
									// Variable 1:
									//		set 1:	0
									//		set 2:	9
									//		set 3: 18
									// Variable 2:
									//		set 1: 0
									//		set 2: 3
									//		set 3: 6
									// Variable 3:
									//		set 1: 0
									//		set 2: 1
									//		set 3: 2
									//
									// so the rule index corresponding to [2][1][1] = 18 + 3 + 1 = 22
									// NOTE: remember indexes start at 0 so index [2] = set 3!
									//
									// This means that everytime we calculate a rule index we have
									// to incurr (n - 1) additions.  This isn't too bad considering 
									// the alternative.  We made this consession to save on memory,
									// since we're using 'unsigned short' each rule element only takes
									// up one byte.  There are many other ways to store this information
									// which are much more memory intensive.  The absolute fastest/most
									// efficient way would be to access the memory via the [] operator(s)
									// but to do that we'd need to know the number of dimensions at the 
									// time we declare the memory (unsigned short*** array; for a 3d example)
									// but (to my knowledge) there's no way (short of a HUGE 'case' statement)
									// to set this at runtime.

}; // end class FuzzySetBase

#else

class FuzzySetBase; // already defined

#endif // !defined(AFX_FuzzySetBase_H__F5CADA04_27CC_11D3_B77E_B09059C10001__INCLUDED_)
