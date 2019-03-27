The files in this directory are to compile FFLL using the GNU C++ compiler.

The file makefile.win, is the makefile that was created from the Dev-C++ IDE 
(available free at http://www.bloodshed.net).

The file  ffll_gcc.dev, is the "project" file for the Dev-C++ IDE.

The makefile should serve as a good jumping off point for whatever version
of GCC and OS you are using. NOTE: you'll have to change the paths in the 
LIBS and INCS declarations.

NOTE: In many GCC implementations you'll have to uncomment the following line in <string>:

 typedef basic_string <wchar_t> wstring;

