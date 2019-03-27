#pragma once
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>


//# define MCDLL extern "C" __declspec(dllexport)  
# define MCDLL __declspec(dllexport)  

MCDLL int __stdcall jAdd(int x, int y);
MCDLL int __stdcall jSub(int x, int y);
