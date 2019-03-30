## MultiCharts Fuzzy Logic Library (MFLL)

### Fuzzy Inference Usage in Easy Language
```
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


DefineDLLFunc: "\path\to\ffllapi.dll",  int, "jFuzzyInference", LPSTR, double, double; 
print( NumToStr( jFuzzyInference(fcl, 91., 82.), 0 ) );
```
