/*
 * Copyright (c) 2012 Pijus Kasparaitis
 * File:    UnitSel.h
 * Purpose: Unit Selection.
 *          Interface of the DLL
 * Author:  Pijus Kasparaitis
 * Email :  pkasparaitis@yahoo.com
 *
 * 2012 06 30
 */
#include <windows.h>
//#include <afx.h>

#ifndef __UNITSEL_H__
#define __UNITSEL_H__

//typedef int (*PROCIC) (char *);
//typedef int (*PROCISI)(short *,int);
//typedef int (CALLBACK* LPFNDLLFUNCINIT)(/*CString*/ char *);
//typedef int (CALLBACK* LPFNDLLFUNCREAD)(unsigned short[],unsigned short[],int,unsigned int[],unsigned int*);
typedef int (*PROCBIC)(char *);
//typedef int (*PROCBISSIII)(unsigned short[],int,unsigned short[],unsigned int[]);

//extern PROCIC		init_LUSS;
//extern PROCIC		write_LUSS;
//extern PROCISI		read_LUSS;
//extern LPFNDLLFUNCINIT functionInit;
//extern LPFNDLLFUNCREAD functionRead;
extern PROCBIC initUnitSel;
//extern PROCBISSIII selectUnits;


BOOL loadUnitSel(char*);
void unloadUnitSel();

#endif