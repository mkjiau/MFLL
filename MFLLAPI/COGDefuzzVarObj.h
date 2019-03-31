//
// File:	COGDefuzzVarObj.h
//
// Purpose:	Center of Gravity Defuzzification Method 
//
// Copyright © 1999-2001 Louder Than A Bomb! Software
//
// This file is part of the FFLL (Free Fuzzy Logic Library) project (http://ffll.sourceforge.net)
// It is released under the BSD license, see http://ffll.sourceforge.net/license.txt for the full text.
//
 
#if !defined(AFX_COGDEFUZZVAROBJ_H__23D883BE_7100_4E65_BA78_7CAA820A6B69__INCLUDED_)
#define AFX_COGDEFUZZVAROBJ_H__23D883BE_7100_4E65_BA78_7CAA820A6B69__INCLUDED_
  

#include "DefuzzVarObj.h"
 
class FuzzyOutVariable;
class COGDefuzzSetObj;

// 
// Class:	COGDefuzzVarObj
//
// Variable object for Center of Gravity Defuzzification Method
// a.k.a. Center of Mass, Centroid
//
// To calulate this we treat each output set as a point mass. We then calculate
// the center of gravity for those point masses distributed along the 'x' axis.
//
//
// So the standard view of sets we have:
//
//            
//           /\           /\           /\       
//          /  \         /  \         /  \      
//         /    \       /    \       /    \     
//        /      \     /      \     /      \    
//    ---/--------\---/--------\---/--------\---
//           |            |            |
//           m1           m2           m3
//
//	Can be represented as point masses as:
//
//   --------O------------O------------O------
//	         |            |            |
//	         m1           m2           m3
//
// We then find the center of gravity of the 3 point masses using the formula:
//
//      Sum of the Moments
//      ------------------
//      Sum of the Masses
//
//    - OR - 
//
//         n
//	      ---
//	      \     m   x
//        /      i   i
//        ---
//        i=0
//      -------------------
//         n
//	      ---
//	      \     m    
//        /      i    
//        ---
//        i=0
//
// NOTE: That in the COG classes we refer to "mass" as "area"... it seems
// to be a bit more understandable to use the term "area" as we're dealing
// with stuff on a computer where there is no reall mass.  So in our case:  MASS = AREA.
//
// The "moment" is simply the mass times the distance from the origin.  According
// to Archimedes and the "law of the lever" equilibrium in a system is achieved when the
// algebraic sum of the moments is zero.  
//
// You'll often read about this method (especially when it's refered to
// as the centroid method) being the point at which the set would balance
// in 2 Dimensions on the point of a pencil (think of balancing a piece of
// glass shaped like a triangle on the tip of a pencil).  
// The method we are using reduces everything to point masses, effectively
// making it a 1D problem.
//
// All these forumulas are from my Calc 1 textbook:  Calculus by James F. Hurley, 
// Wadsworth Publishing Company 1987
//

class COGDefuzzVarObj : public DefuzzVarObj  
{
	////////////////////////////////////////
	////////// Member Functions ////////////
	////////////////////////////////////////

	public:
		// constructor/destructor funcs
		COGDefuzzVarObj();// No function body for this. Explicitly disallow auto-creation of it by the compiler
		COGDefuzzVarObj(FuzzyOutVariable* _parent);
		virtual ~COGDefuzzVarObj();

		// get method
		int get_defuzz_type() const ;

		// misc functions
		RealType calc_value(DOMType* out_set_dom_arr );
 
	protected:
 
		// get functions
		COGDefuzzSetObj* get_set_defuzz_obj(int set_idx) const;  

}; // end class COGDefuzzVarObj

#else

class COGDefuzzVarObj;

#endif // !defined(AFX_COGDEFUZZVAROBJ_H__23D883BE_7100_4E65_BA78_7CAA820A6B69__INCLUDED_)
