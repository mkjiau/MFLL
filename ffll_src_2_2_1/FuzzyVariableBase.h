
//
// File:	FuzzyVariableBase.h
//
// Purpose:	The FuzzyVariableBase class is the ancestor of all Fuzzy Variable classes
//
// Copyright © 1999-2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//

#if !defined(AFX_FUZZYVARIABLEBASE_H__F5CADA07_27CC_11D3_B77E_B09059C10001__INCLUDED_)
#define AFX_FUZZYVARIABLEBASE_H__F5CADA07_27CC_11D3_B77E_B09059C10001__INCLUDED_
 
#include "FFLLBase.h"

class FuzzySetBase;
class FuzzyModelBase;
class FuzzyModelIPC;
 

// 
// Class:	FuzzyVariableBase
//
// Class that has the variable information. Each variable has a list of 
// sets that define the membership functions for the variable.
//

class  FuzzyVariableBase :  virtual public FFLLBase  
{
	////////////////////////////////////////
	////////// Member Functions ////////////
	////////////////////////////////////////
 
 	public:

		// constructor/destructor funcs
		FuzzyVariableBase(FuzzyModelBase* _parent);
		FuzzyVariableBase(); // No function body for this. Explicitly disallow auto-creation of it by the compiler
		virtual ~FuzzyVariableBase();
		virtual int init(const wchar_t* _id, RealType _left_x, RealType _right_x, bool create_unique_id = true);
		virtual int init(const wchar_t* _id, bool create_unique_id = true);

		// set functions
		static int set_x_array_count(int _count);
		static int set_dom_array_count(int _count);
		void set_rule_index(int _rule_index, int set_idx = -1);
		virtual int set_left_x(RealType value);
		virtual int set_right_x(RealType value);
		virtual int set_id(const char* _id, int set_idx = -1);
		virtual void set_ramp(int left_right_ind, int hi_lo_ind, int set_idx  );
		int set_id(const wchar_t* _id, int set_idx = -1);
		FFLL_INLINE void set_index(int idx);
  		
		// get functions
		static ValuesArrCountType get_x_array_count();
		static ValuesArrCountType get_x_array_max_idx();
		static DOMType get_dom_array_count();
		static DOMType get_dom_array_max_idx();
 		RealType get_left_x() const ;
		RealType get_right_x() const ;
		RealType get_idx_multiplier() const;
		virtual DOMType get_dom(int set_idx, int x_position = -1) const ;
 		const char* get_model_name() const;
		int get_rule_index(int _set_idx = -1) const ;
		int get_num_of_sets(void) const ;
 		FFLL_INLINE short get_index() const;
		FuzzySetBase* get_set(int idx) const;  
  		FuzzyModelBase* get_parent(void) const;
 		const wchar_t* get_id(int set_idx = -1) const;
  		static const char* get_fcl_block_start() ;
		static const char* get_fcl_block_end() ;

		// set object functions
		virtual FuzzySetBase* new_set(); 
		virtual FuzzySetBase* new_set(wchar_t* n, int start_x, FuzzyVariableBase* par, short index, int term_width, int type);

		// save/load functions
		void save_var_to_fcl_file(std::ofstream& file_contents);
		void save_sets_to_fcl_file(std::ofstream& file_contents);
		int load_sets_from_fcl_file(std::ifstream& file_contents);

		// misc functions
		virtual RealType convert_idx_to_value(int idx) const;
		ValuesArrCountType convert_value_to_idx(RealType value) const;
		void calc_idx_multiplier(); // NOTE: there is not 'set' for the multiplier - it's always calculated
 		virtual bool is_output(void) const;
  		bool is_set_id_unique(const wchar_t* set_id, int set_idx) const;
 		virtual void move_node(int set_idx, int idx, _point pt);
		virtual void move_node(int set_idx, int idx, int x, int y);
		virtual void move(int set_idx, int node_idx, int new_x);
		void calc(int set_idx = -1);
 		virtual int delete_set(int _set_idx);
 		virtual int add_set(const FuzzySetBase* _new_set);
 
	protected:

		void delete_all_sets();
  
	private:

 		static ValuesArrCountType x_array_count;	// how many elements are in the values[] array for the terms
 		static ValuesArrCountType x_array_max_idx;  // max index for values[] array (seperate var so we don't have to remember to subtract one each time we want that value)
		static DOMType dom_array_count;			// how many 'gradations' are in the 'y' direction for the degree						// of membership.  NOTE:  This can NOT be larger than a the largest
												// of membership.  NOTE:  This can NOT be larger than a the largest
												// value that can be stored in a datatype of type DOMType!!!
		static DOMType dom_array_max_idx;		// max index for max_dom
 		FuzzySetBase**	sets;					// Array of the sets (dynamically allocated).  There are "num_of_sets" minus one elements in the array
												// This is a simple array rather than incurring the overhead of a list or some other STL container
		int				num_of_sets;			// How many sets in this variable
		short			index;					// index for this variable in the model
 
		std::wstring	id ;					// idendifier for the set wich is unique for the variable its part of.  
		RealType		left_x;					// minimum value on the 'x' axis
		RealType		right_x;				// maximum value on the 'x' axis
		RealType		idx_multiplier;			// pre-calculated value of (FuzzyVariableBase::x_array_count/(max_value - min_value). This
												// is how many 'x' values are in each element in the values[] array (part of the MemberFuncBase 
												// object).
		int				rule_index;				// this is the starting offset into memory for this variable.
												// It's used to speed access to the rules.
												// *** For an in-depth explaination, see the rule_index var in FuzzySetBase ***

}; // end class FuzzyVariableBase  

#else

class FuzzyVariableBase; // already defined

#endif // !defined(AFX_FUZZYVARIABLEBASE_H__F5CADA07_27CC_11D3_B77E_B09059C10001__INCLUDED_)
