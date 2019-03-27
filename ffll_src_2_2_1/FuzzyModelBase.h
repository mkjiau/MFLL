//
// File:	FuzzyModelBase.h
//
// Purpose:	Interface for the FuzzyModelBase class.  This class holds
//			all the information for a fuzzy model.
//
// Copyright © 1999-2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//
//
#if !defined(AFX_FuzzyModelBase_H__F9E8F561_B1FA_11D2_B77E_00A855C10000__INCLUDED_)
#define AFX_FuzzyModelBase_H__F9E8F561_B1FA_11D2_B77E_00A855C10000__INCLUDED_
 
 
#include "FFLLBase.h"  



class FuzzyVariableBase;
class FuzzyOutVariable;
class FuzzySetBase;
class RuleArray;
 
// Class:	FuzzyModelBase
//
// Purpose:	This is the "main" class for a FFLL model. It contains the variables
//			and rules that define a model.
//
 
class  FuzzyModelBase : virtual public FFLLBase  
{
	////////////////////////////////////////
	////////// Member Functions ////////////
	////////////////////////////////////////

	public:
 
		// constructors/destructors
		FuzzyModelBase();
		virtual	~FuzzyModelBase();
  		void init();

		// get functions
		int get_defuzz_method() const;
		int get_rule_index(int _var_idx, int _set_idx = -1) const;
		int get_composition_method() const;	 
		DOMType get_dom(int var_idx, int set_idx, int x_position) const;
		const char* get_msg_textA();
		FFLL_INLINE int get_num_of_rules() const;
		FFLL_INLINE int get_num_of_sets(int var_idx) const ;
 		FFLL_INLINE int get_input_var_count() const;
 		FFLL_INLINE int get_inference_method() const ;	 
		FFLL_INLINE RuleArrayType get_rule(int idx) const ;
		FFLL_INLINE const char* get_model_name() const;

		// set functions
 		int set_defuzz_method(int method);
		virtual int set_inference_method(int method);
		virtual int set_composition_method(int method);

		// load file functions
 	 	virtual int load_from_fcl_file(const char* file_name);
		int load(const char* file, short mode, bool from_loder = false);

		// save model functions
		virtual int save_to_fcl_file(const char* file_name);

		// variable/set functions

		virtual int delete_variable(int _var_idx);
		virtual int add_input_variable(const wchar_t* _name = NULL, RealType start_x = 0, RealType end_x = 100, bool create_unique_id = true);
		virtual int add_output_variable(const wchar_t* _name = NULL, RealType start_x = 0, RealType end_x = 100, bool create_unique_id = true);
		bool is_var_id_unique(const wchar_t* _id, int _var_idx) const;
   		virtual int delete_set(int _var_idx, int _set_idx);
  	 	int add_set(int var_idx, const FuzzySetBase* _set);
 
		// rule functions
		FFLL_INLINE bool no_rules() const ;
		FFLL_INLINE RuleArrayType rule_exists(int index) const ;
		FFLL_INLINE void clear_rules();
		virtual void add_rule(int index, RuleArrayType output_set);
		virtual void remove_rule(int index);

		// misc functions
 		void calc_rule_components(int rule_index, int* set_idx_array) const; 
		void calc_rule_index_wrapper(void);
		RealType calc_output(short*  var_idx_arr, DOMType* out_set_dom_arr)  ;
		ValuesArrCountType convert_value_to_idx(int var_idx, RealType value) const; 
 		static void validate_fcl_identifier(std::ofstream& file_contents, std::string identifier);

	protected:

		// get functions
		FuzzyVariableBase* get_var(int idx) const;
		FFLL_INLINE int get_total_var_count() const;

		// set functions
		void set_model_name(const char* _name);
		virtual int set_output_dom(DOMType* out_set_dom_arr, int set_idx, DOMType new_value) ;

		// load file functions
 		int load_vars_from_fcl_file(std::ifstream& file_contents, bool output = false);
		int load_defuzz_block_from_fcl_file(std::ifstream& file_contents);
		int load_rules_from_fcl_file(std::ifstream& file_contents);
  
		// save model functions
 		void save_rules_to_fcl_file(std::ofstream& file_contents) const;

		// variable functions
		virtual FuzzyOutVariable* new_output_variable();
 		virtual FuzzyVariableBase* new_variable();
		void delete_vars(); 
		void add_input_var_to_list(FuzzyVariableBase* var );

		// misc functions
		void calc_active_output_level_wrapper(short* var_idx_arr, DOMType* out_set_dom_arr);
		int calc_rule_index(int var_idx);

	private:

		// get functions
 		const char* get_fcl_block_start() const;
		const char* get_fcl_block_end() const;
 
		// rule functions
		virtual RuleArray* new_rule_array();

 		// misc functions
		void calc_active_output_level(int var_num, DOMType activation_level, int rule_index, short* var_idx_arr, DOMType* out_set_dom_arr )   ;
		int calc_num_of_rules() const;

	////////////////////////////////////////
	////////// Class Variables /////////////
	////////////////////////////////////////
	public:
		// inference operation is the type of operation to perform when combining
		// the "if" parts of the "if-then" rules.  If MIN we take the minimum
		// value of all the anteceedents.  If MAX, we take the max.

		// NOTE: we broke composition (in FuzzyVariableOut) and inference into 2 parts for flexability.  
		// you will often see them combined and refered to as "inference methods"
		// such as MIN-MAX.
		enum INFERENCE_OPERATION { INFERENCE_OPERATION_MIN, INFERENCE_OPERATION_MAX };
 
 
	protected:
  
		FuzzyVariableBase**		input_var_arr;	// array of input variables that make up this rule
		FuzzyOutVariable*		output_var;		// output variable for this rule
 		RuleArray*				rules;			// array of all combinations of rules. 

	private:
		int				inference_method;	// inference method to use for this model
  		int				input_var_count;	// number of input variables that make up this rule
 		std::string		ascii_err_msg;		// string to enable conversion from wide chars to ascii chars	 
 		std::string		model_name;			// name of the flile we've opened

}; // end class FuzzyModelBase

#else

// already included
class FuzzyModelBase;

#endif // !defined(AFX_FuzzyModelBase_H__F9E8F561_B1FA_11D2_B77E_00A855C10000__INCLUDED_)
