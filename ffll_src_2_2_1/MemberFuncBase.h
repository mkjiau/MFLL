//
// File:	MemberFuncBase.h
//
// Purpose:	This is the base class all the membership fucntions.  
//
// Copyright © 1999-2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//
 
#if !defined(AFX_MEMBERFUNC_H__A9B71344_61EC_11D3_B77F_906F59C10001__INCLUDED_)
#define AFX_MEMBERFUNC_H__A9B71344_61EC_11D3_B77F_906F59C10001__INCLUDED_
 
#include "FFLLBase.h"
#include <fstream>
#include <math.h>

class FuzzySetBase;
 
// 
// Class:	MemberFuncBase
//
// This is an abstract class that defines the type of "curve" a set has.
//

class  MemberFuncBase : virtual public FFLLBase  
{
 	////////////////////////////////////////
	////////// Member Functions ////////////
	////////////////////////////////////////

	public:

		// constructor/destructor funcs
		virtual ~MemberFuncBase();
 		MemberFuncBase(); // No function body for this. Explicitly disallow auto-creation of it by the compiler
		MemberFuncBase(FuzzySetBase* _parent);
		void init();
		virtual void init_nodes(int start_x, int term_width) = 0;
 
		// get functions
		int get_end_x(void) const; 
		int get_start_x(void) const ;
		int get_node_x(int idx) const ;
		int get_node_y(int idx) const;
		int get_ramp() const;
		NodePoint get_node(int idx) const;
		DOMType get_value(int idx) const ;
		DOMType get_dom(int idx);
		FuzzySetBase* get_parent() const  ;
		RealType get_left_x() const; 
		virtual int get_center_x(void) const;
		virtual int get_node_count() const = 0;
		virtual int get_func_type() const = 0;

		// set functions

		virtual void set_node(int idx, int x, int y, bool validate = true) = 0;
	 	virtual void set_ramp(int hi_lo_ind, int left_right_ind);
		int set_value(int idx, DOMType val);
		int set_value(int idx, RealType val);

		// misc functions

 		void move(int x_delta);
		void move_node(int idx, _point pt ) ;
		virtual void move_node(int idx, int x, int y ) ;
		virtual void calc() = 0;

  		// save/load functions

		virtual void save_to_fcl_file(std::ofstream& file_contents);
 		virtual void expand(int x_delta) = 0;
 		virtual void shrink(int x_delta) = 0;
 
	protected:

		// misc functions

		virtual int alloc_values_array();
		virtual void dealloc_values_array();
 		int alloc_nodes(int node_count);
		void clear_values();

	////////////////////////////////////////
	////////// Class Variables /////////////
	////////////////////////////////////////

	public:
		enum TYPE {S_CURVE, TRIANGLE, TRAPEZOID, SINGLETON};
		enum RAMP {RAMP_NONE, RAMP_LEFT, RAMP_RIGHT, RAMP_NA}; // NA is not applicable for singleton member funcs

	protected:
 
 		NodePoint* nodes;	// array of "node" points for the curve
		int			ramp;	/* indicates if this term is a ramp. It can be one of the
							** RAMP enum types defined in this class. A ramp is a membership
							** function that has the 'y' axis as one of it's sides.
							** For example a normal trapezoid memberhip function would look like this:
							**                                       ___
							**              Normal Trapezoid:       /   \
							**                                       ___
							**              Left Ramp Trapezoid:    |   \ 
							**                                       ___
							**              Right Ramp Trapezoid:   /   |  
							*/
		DOMType*	values;	// This points to an array that holds the 'y' values for this term.
							// It has FuzzyVariableBase::x_array_count elements

}; // end class MemberFuncBase

#else

class MemberFuncBase; // already included

#endif // !defined(AFX_MEMBERFUNC_H__A9B71344_61EC_11D3_B77F_906F59C10001__INCLUDED_)
