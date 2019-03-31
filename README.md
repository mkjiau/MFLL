## MultiCharts Fuzzy Logic Library (MFLL)
Extended from C/C++ library of [FFLL](http://ffll.sourceforge.net/), MFLL allows you to apply [fuzzy control engine](https://en.wikipedia.org/wiki/Fuzzy_logic) for your trading strategy design in MultiCharts. You will write [Fuzzy Control Language](http://ffll.sourceforge.net/fcl.htm) with PL/EL during strategy development. 


### How to use MFLL?

Download the source codes and compile as MFLLAPI.dll (x86 or x64) that's compatiable with MultiCharts.

#### Write FCL Directly in PL/EL
API
```c
double WIN_FFLL_API MFLLFuzzyInference(LPSTR fcl_str, double* crisp_inputs, long input_size);
```
PL/EL
```pas
vars: fcl("");

fcl = Text(
"FUNCTION_BLOCK", NewLine,
NewLine,
"VAR_INPUT", NewLine,
	"Our_Health	REAL; (* RANGE(0 .. 100) *)", NewLine,
	"Enemy_Health	REAL; (* RANGE(0 .. 100) *)", NewLine,
"END_VAR", NewLine,
NewLine,
"VAR_OUTPUT", NewLine,
	"Aggressiveness	REAL; (* RANGE(0 .. 4) *)", NewLine,
"END_VAR", NewLine,
NewLine,
"FUZZIFY Our_Health", NewLine,
	"TERM Near_Death := (0, 0) (0, 1) (50, 0) ;", NewLine,
	"TERM Good := (14, 0) (50, 1) (83, 0) ;", NewLine,
	"TERM Excellent := (50, 0) (100, 1) (100, 0) ;", NewLine,
"END_FUZZIFY", NewLine,
NewLine,
"FUZZIFY Enemy_Health", NewLine,
	"TERM Near_Death := (0, 0) (0, 1) (50, 0) ;", NewLine,
	"TERM Good := (14, 0) (50, 1) (83, 0) ;", NewLine,
	"TERM Excellent := (50, 0) (100, 1) (100, 0) ;", NewLine,
"END_FUZZIFY", NewLine,
NewLine,
"FUZZIFY Aggressiveness", NewLine,
	"TERM Run_Away := 1 ;", NewLine,
	"TERM Fight_Defensively := 2 ;", NewLine,
	"TERM All_Out_Attack := 3 ;", NewLine,
"END_FUZZIFY", NewLine,
NewLine,
"DEFUZZIFY valve", NewLine,
	"METHOD: MoM;", NewLine,
"END_DEFUZZIFY", NewLine,
NewLine,
"RULEBLOCK first", NewLine,
	"AND:MIN;", NewLine,
	"ACCUM:MAX;", NewLine,
	"RULE 0: IF Good AND Good THEN Fight_Defensively;", NewLine,
	"RULE 1: IF Good AND Excellent THEN Fight_Defensively;", NewLine,
	"RULE 2: IF Good AND Near_Death THEN All_Out_Attack;", NewLine,
	"RULE 3: IF Excellent AND Good THEN All_Out_Attack;", NewLine,
	"RULE 4: IF Excellent AND Excellent THEN Fight_Defensively;", NewLine,
	"RULE 5: IF Excellent AND Near_Death THEN All_Out_Attack;", NewLine,
	"RULE 6: IF Near_Death AND Good THEN Run_Away;", NewLine,
	"RULE 7: IF Near_Death AND Excellent THEN Run_Away;", NewLine,
	"RULE 8: IF Near_Death AND Near_Death THEN Fight_Defensively;", NewLine,
"END_RULEBLOCK", NewLine,
NewLine,
"END_FUNCTION_BLOCK", NewLine
);


DefineDLLFunc: "PATH\TO\MFLLAPI.dll",  double, "MFLLFuzzyInference", LPSTR, lpdouble, long; 

Array: double FuzzyInputs[2](0.0);
FuzzyInputs[0] = 5.; FuzzyInputs[1] = 5.;;
print(MFLLFuzzyInference(fcl, &FuzzyInputs[0], 2));
```

#### Load FCL file into PL/EL

API
```c
double WIN_FFLL_API MFLLFuzzyInferenceByFile(LPSTR fcl_file, double* crisp_inputs, long input_size);
```
PL/EL
```pas
DefineDLLFunc: "PATH\TO\MFLLAPI.dll", double, "MFLLFuzzyInferenceByFile", LPSTR, lpdouble, long; 

Array: double FuzzyInputs[2](0.0);
FuzzyInputs[0] = 5.; FuzzyInputs[1] = 5.;;
print(MFLLFuzzyInferenceByFile("PATH\TO\FCL_FILE", &FuzzyInputs[0], 2));
```

Copyright
---
© 2019-present Ming-Kai Jiau.