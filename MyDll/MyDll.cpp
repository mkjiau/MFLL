// MyDll.cpp : Defines the exported functions for the DLL application.
//

//#import "C:\Program Files\TS Support\MultiCharts64\PLKit.dll" no_namespace
#include "stdafx.h"
#include "MyDll.h"

MCDLL int __stdcall jAdd(int x, int y)
{
	return x + y;
}

MCDLL int __stdcall jSub(int x, int y)
{
	return x - y;
}
