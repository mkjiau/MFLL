//
// File:	MemberFuncSCurve.h
//
// Purpose:	This class is for the S-Curve shaped member function
//
// Copyright © 1999-2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//

#if !defined(AFX_MemberFuncSCurve_H__DE3C07E3_650E_11D3_B77F_906F59C10001__INCLUDED_)
#define AFX_MemberFuncSCurve_H__DE3C07E3_650E_11D3_B77F_906F59C10001__INCLUDED_
 
 
#include "MemberFuncBase.h"
class MemberFuncTrap;
class MemberFuncSingle;
  
// 
// Class:	MemberFuncSCurve
//
// This is the S-Curve shaped membership function. It consists of 7 points.
// This curve is extreemly flexible and allows some if it's nodes 'y' values
// to be altered.
// 

class    MemberFuncSCurve :  virtual public MemberFuncBase  
{
 	////////////////////////////////////////
	////////// Member Functions ////////////
	////////////////////////////////////////

	public:

		// constructor/destructor funcs
		MemberFuncSCurve(); // No function body for this. Explicitly disallow auto-creation of it by the compiler
		MemberFuncSCurve(FuzzySetBase* _parent);
 		virtual ~MemberFuncSCurve();

		// get functions
		virtual int get_center_x(void) const;
		int get_node_count() const ;
	 	int get_func_type() const;

		// set functions

 		void set_node(int idx, int x, int y, bool validate = true);
  		void set_ramp(int hi_lo_ind, int left_right_ind);
 
		// misc functions

 	 	void calc( );
		virtual void expand(int x_delta );
 		virtual void shrink(int x_delta );	

	protected:

		// misc functions
		void init_nodes(int start_x, int term_width);
 		void init_nodes(NodePoint p0, NodePoint p1, NodePoint p2);
 		void calc_curve_values(NodePoint& p1, NodePoint& p2, NodePoint& p3, NodePoint& p4);
 
}; // end class MemberFuncSCurve


#else

class MemberFuncSCurve; // already defined

#endif // !defined(AFX_MemberFuncSCurve_H__DE3C07E3_650E_11D3_B77F_906F59C10001__INCLUDED_)
