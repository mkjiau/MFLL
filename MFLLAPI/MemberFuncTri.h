//
// File:	MemberFuncTri.h
//
// Purpose:	This class is for the triangle shaped member function
//
// Copyright © 1999-2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//

 
#if !defined(AFX_MEMBERFUNCTRI_H__DE3C07E3_650E_11D3_B77F_906F59C10001__INCLUDED_)
#define AFX_MEMBERFUNCTRI_H__DE3C07E3_650E_11D3_B77F_906F59C10001__INCLUDED_

 
#include "MemberFuncBase.h"
 
// 
// Class:	MemberFuncTri
//
// This is the triangle shaped membership function. It consists of 3 points.
// 
 
class  MemberFuncTri :  virtual public MemberFuncBase  
{
 	////////////////////////////////////////
	////////// Member Functions ////////////
	////////////////////////////////////////

	public:

		// constructor/destructor funcs
		MemberFuncTri(); // No function body for this. Explicitly disallow auto-creation of it by the compiler
		MemberFuncTri(FuzzySetBase* _parent);
  		virtual ~MemberFuncTri();
	
		// set functions
 		void set_ramp(int hi_lo_ind, int left_right_ind);
  		void set_node(int idx, int x, int y, bool validate = true);
 
		// get functions
 		virtual int get_center_x(void) const ;
 		virtual int get_func_type() const;
 		virtual int get_node_count() const;

		// misc functions
		void calc( );
 		virtual void expand(int x_delta );
 		virtual void shrink(int x_delta );

	protected:

		// mics functions
		void init_nodes(int start_x, int term_width);
 
}; // end class MemberFuncTri
 
#else

class MemberFuncTri; // already defined

#endif // !defined(AFX_MEMBERFUNCTRI_H__DE3C07E3_650E_11D3_B77F_906F59C10001__INCLUDED_)
