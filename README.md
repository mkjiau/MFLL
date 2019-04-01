## MultiCharts Fuzzy Logic Library (MFLL)
Extended from C/C++ library of [FFLL](http://ffll.sourceforge.net/), MFLL allows you to apply [fuzzy control engine](https://en.wikipedia.org/wiki/Fuzzy_logic) for your trading strategy design in MultiCharts. You will write [Fuzzy Control Language](http://ffll.sourceforge.net/fcl.htm) (FCL) with PL/EL during strategy development. 


### How to use MFLL?

Go to `Installation` folder, then follow the instructions as below:

1. Download `MFLLAPI.dll` and `MFLL_MC.pla`
2. Put .dll in `C:\Program Files\TS Support\MultiCharts64`
3. Click `.pla` to import PL/EL-wrapped APIs for use
4. Done. Get started with MFLL.

*You also can download the source codes and compile MFLLAPI.dll yourself to be compatiable with your MultiCharts.*



#### Write FCL Directly in PL/EL
###### Multi-API Formation
```pas
vars: fcl("");

fcl = Text(
"FUNCTION_BLOCK", NewLine,
NewLine,
"VAR_INPUT", NewLine,
"RSI	REAL; (* RANGE(0 .. 100) *)", NewLine,
"Bias	REAL; (* RANGE(-100 .. 100) *)", NewLine,
"END_VAR", NewLine,
NewLine,
"VAR_OUTPUT", NewLine,
"Action	REAL; (* RANGE(-2 .. 2) *)", NewLine,
"END_VAR", NewLine,
NewLine,
"FUZZIFY RSI", NewLine,
"TERM Small  := (0, 0) (0, 1) (50, 0) ;", NewLine,
"TERM Middle := (10, 0) (50, 1) (70, 0) ;", NewLine,
"TERM Large  := (30, 0) (100, 1) (100, 0) ;", NewLine,
"END_FUZZIFY", NewLine,
NewLine,
"FUZZIFY Bias", NewLine, 
"TERM Negative  := (-100, 0) (-100, 1) (50, 0) ;", NewLine,
"TERM Zero := (-80, 0) (-35, 1) (5, 1) (50, 0) ;", NewLine,
"TERM Positive  := (-50, 0) (100, 1) (100, 0) ;", NewLine,
"END_FUZZIFY", NewLine,
NewLine,
"FUZZIFY Action", NewLine,
"TERM Short   := -1 ;", NewLine,
"TERM Neutral := 0 ;", NewLine,
"TERM Long    := 1 ;", NewLine,
"END_FUZZIFY", NewLine,
NewLine,
"DEFUZZIFY valve", NewLine,
"METHOD: MoM;", NewLine,
"END_DEFUZZIFY", NewLine,
NewLine,
"RULEBLOCK first", NewLine,
"AND:MIN;", NewLine,
"ACCUM:MAX;", NewLine,
"RULE 0: IF Large AND Positive THEN Long;", NewLine,
"RULE 1: IF Large AND Zero THEN Long;", NewLine,
"RULE 2: IF Large AND Negative THEN Long ;", NewLine,
"RULE 3: IF Middle AND Positive THEN Long ;", NewLine,
"RULE 4: IF Middle AND Zero THEN Long ;", NewLine,
"RULE 5: IF Middle AND Negative THEN Short ;", NewLine,
"RULE 6: IF Small AND Positive THEN Neutral;", NewLine,
"RULE 7: IF Small AND Zero THEN Short ;", NewLine,
"RULE 8: IF Small AND Negative THEN Short ;", NewLine, 
"END_RULEBLOCK", NewLine,
NewLine,
"END_FUNCTION_BLOCK", NewLine
);


{ Configure Fuzzy Control Engine }

vars: model(-1), child(-1);

if model = -1 then begin 
   model = MFLLNewModel();
   MFLLLoadFCLString(model, fcl);
   child = MFLLNewChild(model);
end;

MFLLSetValue(model,child,0, RSI(C,14)); // input RSI indicator (built-in)
MFLLSetValue(model,child,1, Bias(C,30)); // input Bias indicator (custom)


{ Trading Signal }

vars: Action(0);

Action = MFLLGetOutputValue(model,child);

if MP <> 1 and Action = 1 then buy ("fuzzy-LE") next bar at market; 
if MP <>-1 and Action =-1 then sellshort ("fuzzy-SE") next bar at market;

if Action = 0 then begin
   if MP > 0 then sell ("fuzzy-LX") next bar at market;
   if MP < 0 then buytocover ("fuzzy-SX") next bar at market;
end;
```
###### Single-API Formation
```pas
vars: fcl("");

fcl = Text(
"FUNCTION_BLOCK", NewLine,
NewLine,
"VAR_INPUT", NewLine,
"RSI	REAL; (* RANGE(0 .. 100) *)", NewLine,
"Bias	REAL; (* RANGE(-100 .. 100) *)", NewLine,
"END_VAR", NewLine,
NewLine,
"VAR_OUTPUT", NewLine,
"Action	REAL; (* RANGE(-2 .. 2) *)", NewLine,
"END_VAR", NewLine,
NewLine,
"FUZZIFY RSI", NewLine,
"TERM Small  := (0, 0) (0, 1) (50, 0) ;", NewLine,
"TERM Middle := (10, 0) (50, 1) (70, 0) ;", NewLine,
"TERM Large  := (30, 0) (100, 1) (100, 0) ;", NewLine,
"END_FUZZIFY", NewLine,
NewLine,
"FUZZIFY Bias", NewLine, 
"TERM Negative  := (-100, 0) (-100, 1) (50, 0) ;", NewLine,
"TERM Zero := (-80, 0) (-35, 1) (5, 1) (50, 0) ;", NewLine,
"TERM Positive  := (-50, 0) (100, 1) (100, 0) ;", NewLine,
"END_FUZZIFY", NewLine,
NewLine,
"FUZZIFY Action", NewLine,
"TERM Short   := -1 ;", NewLine,
"TERM Neutral := 0 ;", NewLine,
"TERM Long    := 1 ;", NewLine,
"END_FUZZIFY", NewLine,
NewLine,
"DEFUZZIFY valve", NewLine,
"METHOD: MoM;", NewLine,
"END_DEFUZZIFY", NewLine,
NewLine,
"RULEBLOCK first", NewLine,
"AND:MIN;", NewLine,
"ACCUM:MAX;", NewLine,
"RULE 0: IF Large AND Positive THEN Long;", NewLine,
"RULE 1: IF Large AND Zero THEN Long;", NewLine,
"RULE 2: IF Large AND Negative THEN Long ;", NewLine,
"RULE 3: IF Middle AND Positive THEN Long ;", NewLine,
"RULE 4: IF Middle AND Zero THEN Long ;", NewLine,
"RULE 5: IF Middle AND Negative THEN Short ;", NewLine,
"RULE 6: IF Small AND Positive THEN Neutral;", NewLine,
"RULE 7: IF Small AND Zero THEN Short ;", NewLine,
"RULE 8: IF Small AND Negative THEN Short ;", NewLine, 
"END_RULEBLOCK", NewLine,
NewLine,
"END_FUNCTION_BLOCK", NewLine
);


Array: double FuzzyInputs[2](0.0);

FuzzyInputs[0] = RSI(C,14); // input RSI indicator (built-in)
FuzzyInputs[1] = Bias(C,30); // input Bias indicator (custom)


{ Trading Signal }

vars: Action(0);

Action = MFLLFuzzyInference(fcl, FuzzyInputs);

if MP <> 1 and Action = 1 then buy ("fuzzy-LE") next bar at market; 
if MP <>-1 and Action =-1 then sellshort ("fuzzy-SE") next bar at market;

if Action = 0 then begin
   if MP > 0 then sell ("fuzzy-LX") next bar at market;
   if MP < 0 then buytocover ("fuzzy-SX") next bar at market;
end;

```

#### Load FCL file into PL/EL
###### Multi-API Formation
```pas
{ Configure Fuzzy Control Engine }

vars: model(-1), child(-1);

if model = -1 then begin 
   model = MFLLNewModel();
   MFLLLoadFCLFile(model, "PATH\TO\FCL_FILE");
   child = MFLLNewChild(model);
end;

MFLLSetValue(model,child,0, RSI(C,14)); // input RSI indicator (built-in)
MFLLSetValue(model,child,1, Bias(C,30)); // input Bias indicator (custom)


{ Trading Signal }

vars: Action(0);

Action = MFLLGetOutputValue(model,child);

if MP <> 1 and Action = 1 then buy ("fuzzy-LE") next bar at market; 
if MP <>-1 and Action =-1 then sellshort ("fuzzy-SE") next bar at market;

if Action = 0 then begin
   if MP > 0 then sell ("fuzzy-LX") next bar at market;
   if MP < 0 then buytocover ("fuzzy-SX") next bar at market;
end;
```
###### Single-API Formation
```pas
Array: double FuzzyInputs[2](0.0);

FuzzyInputs[0] = RSI(C,14); // input RSI indicator (built-in)
FuzzyInputs[1] = Bias(C,30); // input Bias indicator (custom)


{ Trading Signal }

vars: Action(0);

Action = MFLLFuzzyInferenceByFile("PATH\TO\FCL_FILE", FuzzyInputs);

if MP <> 1 and Action = 1 then buy ("fuzzy-LE") next bar at market; 
if MP <>-1 and Action =-1 then sellshort ("fuzzy-SE") next bar at market;

if Action = 0 then begin
   if MP > 0 then sell ("fuzzy-LX") next bar at market;
   if MP < 0 then buytocover ("fuzzy-SX") next bar at market;
end;
```
see `Examples` folder to find an example of `rsi-and-bias.fcl` for `FCL_FILE` that's just a text file written in FCL.

Copyright
---
© 2019-present Ming-Kai Jiau.