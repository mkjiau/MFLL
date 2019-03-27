FFLL API Readme
---------------

Version: 2.2.1
Release Date: 05/1/03


Release History
----------------

May 1, 2003 v2.2.1

* change to COGDefuzzSetObj.cpp that was causing slightly incorrect output values
* updates to FFLLAPI.cpp to use std::list instead of std::vector to avoid destructors getting called before they should
* Added:
     #define swprintf _snwprintf
for Windows platforms to be more compatible with non-windows platforms 

August 1, 2002 v2.1.2

This version of the FFLL library has been modified to compile using the GCC compiler.
The the /GCC subdirectory there is a makefile (makefile.win) that was created from 
the Dev-C++ IDE (available free at http://www.bloodshed.net) using the GCC compiler that
came with it. The makefile should serve as a good jumping off point for whatever version
of GCC and OS you are using. NOTE: you'll have to change the paths in the LIBS and INCS declarations.

NOTE: In many GCC implementations you'll have to uncomment the following line in <string>:

 typedef basic_string <wchar_t> wstring;

The parsing of FCL files has been modified to adhear more closely to the IEC 61131-7 standard
when specifying rules. See http://ffll.sourceforge.net/ffll_and_fcl.htm for more details.

--------------------------------------------------------------------------------
 
March 21, 2002 v2.1.1

 Minor update so correct ACCU keyword is used. Older versions used ACCUM. FFLL will still
 read in FCL files that have ACCUM for backwards compatibility. 

--------------------------------------------------------------------------------
 
February 28, 2002 v2.1.0

 Removed __declspec(dllexport) from all classes. This means if you're using the DLL version 
of  FFLL the classes cannot be accessed directly. This was done to make the code cleaner.
Modified project to eliminate (significant) external dependencies. This makes the dll a bit 
bigger but does not require the distribution of any other "supporting" dlls. 

--------------------------------------------------------------------------------
 
October 2001 v2.0.0

 FFLL code is submitted to be included in the AI Game Programming Wisdom book. 
The code that is included in the CD of that book is the initial check-in into 
the FFLL project's CVS tree. 
