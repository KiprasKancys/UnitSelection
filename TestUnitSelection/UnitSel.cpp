/*
 * File    : UnitSel.cpp
 * Purpose : Interface with Unit Selection DLL
 * Author  : Pijus Kasparaitis
 * Email   : pkasparaitis@yahoo.com
 * 
 * 2012 06 30 : New Interface, using functions instead of Object 
 *
 * Copyright (c) 2012 Pijus Kasparaitis
 * All rights reserved.
 */
#include "stdafx.h"
#include "UnitSel.h"

//HINSTANCE	hinstDllLUSS;
HMODULE dll_handle;
//PROCIC		init_LUSS;
//PROCIC		write_LUSS;
//PROCISI		read_LUSS;
//LPFNDLLFUNCINIT functionInit;
//LPFNDLLFUNCREAD functionRead;
PROCBIC initUnitSel;
//PROCBISSIII selectUnits;


BOOL loadUnitSel(char *biblioteka)
{
	if (!(dll_handle=LoadLibrary(biblioteka))) //"UnitSelection.dll"
		return FALSE;
	initUnitSel = (PROCBIC)GetProcAddress(dll_handle, "initUnitSel");
//	selectUnits = (PROCBISSIII)GetProcAddress(dll_handle, "selectUnits");

//	init_LUSS			=(PROCIC) GetProcAddress(hinstDllLUSS,"init_LUSS");
//	write_LUSS			=(PROCIC) GetProcAddress(hinstDllLUSS,"write_LUSS");
//	read_LUSS			=(PROCISI) GetProcAddress(hinstDllLUSS,"read_LUSS");

//	if ((!initUnitSel)	|| (!selectUnits))
	if (!initUnitSel)
	{
    	FreeLibrary (dll_handle); 
		dll_handle=NULL;
		return FALSE;
	}
	return TRUE;
}

void unloadUnitSel()
{
	if (dll_handle)
	{
		FreeLibrary (dll_handle);
		dll_handle=NULL;
	}
}