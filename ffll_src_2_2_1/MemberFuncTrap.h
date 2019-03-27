//
// File:	MemberFuncTrap.h
//
// Purpose:	This class is for the trapezoid shaped member function
//
// Copyright © 1999-2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//

#if !defined(AFX_MemberFuncTrap_H__DE3C07E3_650E_11D3_B77F_906F59C10001__INCLUDED_)
#define AFX_MemberFuncTrap_H__DE3C07E3_650E_11D3_B77F_906F59C10001__INCLUDED_
 
#include "MemberFuncBase.h"
 
// 
// Class:	MemberFuncTrap
//
// This is the trapezoid shaped membership function. It consists of 4 points.
// 

class   MemberFuncTrap :  virtual public MemberFuncBase  
{
 	////////////////////////////////////////
	////////// Member Functions ////////////
	////////////////////////////////////////

	public:

		// constructor/destructor funcs
		MemberFuncTrap(); // No function body for this. Explicitly disallow auto-creation of it by the compiler
		MemberFuncTrap(FuzzySetBase* _parent);
 		virtual ~MemberFuncTrap();

		// get functions
  		virtual int get_center_x(void) const;
 		int get_func_type()  const;
 		int get_node_count() const;

		// set functions
 		void set_node(int idx, int x, int y, bool validate = true);
 		void set_ramp(int hi_lo_ind, int left_right_ind);

		// misc functions
		void calc();
 		virtual void expand(int x_delta );
 		virtual void shrink(int x_delta );


	protected:

		// misc functions
		void init_nodes(int start_x, int term_width);
 
}; // end class MemberFuncTrap

#else

class MemberFuncTrap; // already defined

#endif // !defined(AFX_MemberFuncTrap_H__DE3C07E3_650E_11D3_B77F_906F59C10001__INCLUDED_)
