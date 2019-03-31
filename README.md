## MultiCharts Fuzzy Logic Library (MFLL)
Extended from C/C++ library of [FFLL](http://ffll.sourceforge.net/), MFLL allows you to apply [fuzzy control engine](https://en.wikipedia.org/wiki/Fuzzy_logic) for your trading strategy design in MultiCharts. You will write [Fuzzy Control Language](http://ffll.sourceforge.net/fcl.htm) (FCL) with PL/EL during strategy development. 


### How to use MFLL?

Go to `Installation` folder, then follow the instructions as below:

Step 1. Download `MFLLAPI.dll` and `MFLL_MC.pla`
Step 2. Put .dll in `C:\Program Files\TS Support\MultiCharts64`
Step 3. Click `.pla` to import PL/SL-wrapped APIs for use
Step 4. Done. Get started with MFLL.

*You also can download the source codes and compile MFLLAPI.dll yourself to be compatiable with your MultiCharts.*



#### Write FCL Directly in PL/EL
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

Array: double FuzzyInputs[2](0.0);
FuzzyInputs[0] = 5.; // Our_Health
FuzzyInputs[1] = 5.; // Enemy_Health
print(MFLLFuzzyInference(fcl, FuzzyInputs));
```

#### Load FCL file into PL/EL
```pas
Array: double FuzzyInputs[2](0.0);
FuzzyInputs[0] = 5.; 
FuzzyInputs[1] = 5.;
print(MFLLFuzzyInferenceByFile("PATH\TO\FCL_FILE", FuzzyInputs));
```
see `Examples` folder to find an example of `FCL_FILE` that's just a text file written in FCL.

Copyright
---
© 2019-present Ming-Kai Jiau.