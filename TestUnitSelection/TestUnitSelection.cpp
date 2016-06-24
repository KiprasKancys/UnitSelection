// TestUnitSelection.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "fv2id.h"
#include "UnitSel.h"
#include <time.h>
// JUST FOR TESTING PURPOSE
#include <iostream>


// UnitSelection.cpp : Defines the entry point for the console application.
//

////#ifdef _DEBUG
////#define new DEBUG_NEW
////#endif


#define MAX_ROWS 100*1024
#define MAX_UNITS 256

// The one and only application object

////CWinApp theApp;

#include <string>
using namespace std;


////typedef int (CALLBACK* LPFNDLLFUNCINIT)(char*);
////typedef int (CALLBACK* LPFNDLLFUNCREAD)(unsigned short[],unsigned short[],int,unsigned int[],unsigned int*);
//typedef int (CALLBACK* LPFNDLLFUNCREAD)(unsigned short);

void spausdinti_loga(char* pranesimas)
{
	FILE *stream;
	stream = fopen("TextNorm_log.txt", "a+");
	fprintf(stream, "%s\n", pranesimas);
	printf("%s\n", pranesimas);
	fclose(stream);
}

void PrintTime(string info_str)
{
	clock_t t = clock();
	printf ("%d clicks (%f seconds): %s\r\n",t,((float)t)/CLOCKS_PER_SEC, info_str.c_str());
}



unsigned short unitsDB_ID[10*1024*1024];
int n_total_DBunits = 0;
int getDBFromFile(char * dataBaseFileName)
{
	FILE * pFile;
	pFile = fopen (dataBaseFileName , "r");
	if (pFile == NULL) return -1;//perror ("Error opening file");

	unsigned short PAUSE_UNIT_ID;
	PAUSE_UNIT_ID = fv2id("_ _");

	string tmpBuffer;
	
	char tmpCharBuffer[1024];
	
	while ( fgets (tmpCharBuffer , 1024 , pFile) != NULL )
	{
		string tmpStringBuffer(tmpCharBuffer);

		unitsDB_ID[n_total_DBunits] = PAUSE_UNIT_ID;
		n_total_DBunits++;

		size_t cursor = 0;
		size_t sep = 0;

		if (tmpStringBuffer.find("|") != string::npos) {
			size_t n = count(tmpStringBuffer.begin(), tmpStringBuffer.end(), '|');
			for (int i = 0; i < n + 1; i++)
			{
				sep = tmpStringBuffer.find("|", sep + 1);
				string s = tmpStringBuffer.substr(cursor, sep - cursor);
				cursor = sep + 1;

				removeCharsFromString(s, "+-");
				unitsDB_ID[n_total_DBunits] = fv2id((char*)(s.c_str()));

				n_total_DBunits++;
			}
		}

		unitsDB_ID[n_total_DBunits] = PAUSE_UNIT_ID;
		n_total_DBunits++;

	}
	
	fclose (pFile);

	return 0;
}


int DEFAULT_NULL = -1;

struct unit {
	int id = DEFAULT_NULL;
	int pointer = DEFAULT_NULL;
	int value = DEFAULT_NULL;
	unsigned int sum = 0;
};



int selectUnits(unsigned short unitsRow[], int unitsRowLength, unsigned short unitsDB_ID[], int n_total_DBunits, unsigned int retUnits[], unsigned int *currentCost)
{
	//Creates an array
	const int nubmerOfColumns = unitsRowLength + 1;
	const int nubmerOfUnits = 2048;
	unit **temp;
	temp = new unit*[nubmerOfColumns];
	for (int i = 0; i < nubmerOfColumns; ++i) {
		temp[i] = new unit[nubmerOfUnits];
	}


	int tempFirstIter;
	int previousLen = 1;
	int storage = 0;
	bool notFound;
	int cost = 100;

	for (tempFirstIter = 1;  tempFirstIter < nubmerOfColumns; tempFirstIter++) {

		previousLen = storage;
				
		int tempSecIter = 0;
		for (int k = 0; k < n_total_DBunits; k++) {

			if (unitsRow[tempFirstIter-1] == unitsDB_ID[k])
			{
				temp[tempFirstIter][tempSecIter].value = k;
				temp[tempFirstIter][tempSecIter].id = tempSecIter;

				int previousIter = 0;
				notFound = true;

				while (notFound && previousIter < previousLen) {
					if (k - 1 == temp[tempFirstIter - 1][previousIter].value) {
						temp[tempFirstIter][tempSecIter].pointer = temp[tempFirstIter - 1][previousIter].id;
						temp[tempFirstIter][tempSecIter].sum += temp[tempFirstIter - 1][previousIter].sum + 0;
						notFound = false;
					} else {
						previousIter++;
					}
				}

				if (notFound) {
					previousIter = 0;
					temp[tempFirstIter][tempSecIter].sum = temp[tempFirstIter - 1][previousIter].sum;
					temp[tempFirstIter][tempSecIter].pointer = temp[tempFirstIter - 1][previousIter].id;
					while (previousIter < previousLen) {
						if (temp[tempFirstIter][tempSecIter].sum > temp[tempFirstIter - 1][previousIter].sum) {
							temp[tempFirstIter][tempSecIter].pointer = temp[tempFirstIter - 1][previousIter].id;
							temp[tempFirstIter][tempSecIter].sum = temp[tempFirstIter - 1][previousIter].sum;
						} else {
							previousIter++;
						}
					}
					temp[tempFirstIter][tempSecIter].sum += cost;
				}
				//cout << "F " << tempFirstIter << ", S " << tempSecIter << ", uRow " << unitsRow[tempFirstIter] << ", k " << k << ", suma " << temp[tempFirstIter][tempSecIter].sum <<", pointer " << temp[tempFirstIter][tempSecIter].pointer
				//	<< ", id " << temp[tempFirstIter][tempSecIter].id << ", value " << temp[tempFirstIter][tempSecIter].value << ", dif " << FonV[unitsDB_ID[k]].fv  << endl;
				tempSecIter++;
				storage = tempSecIter;
			}
		}
	}



	//Find min
	tempFirstIter--;
	int minID = temp[tempFirstIter][0].id;
	int min = temp[tempFirstIter][0].sum;
	int j = 1;
	while (temp[tempFirstIter][j].id != DEFAULT_NULL) {
		if (min > temp[tempFirstIter][j].sum) 
		{
			minID = temp[tempFirstIter][j].id;
			min = temp[tempFirstIter][j].sum;
		}
		j++;
	}

	//Step by step using pointers
	int id = minID;
	*currentCost = temp[tempFirstIter][id].sum;

	while (tempFirstIter != DEFAULT_NULL) {
		retUnits[tempFirstIter-1] = temp[tempFirstIter][id].value;
		id = temp[tempFirstIter][id].pointer;
		tempFirstIter--;
	}

	//Cleaning
	for (int i = 0; i < nubmerOfColumns; i++) {
		delete[] temp[i];
	}

	delete[] temp;
	
	return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//int main(int argc, char* argv[])
int main()
//int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	
	FILE *stream;
	stream = fopen("output.txt", "w+");
		
	int nRetCode = 0;


	//if(loadUnitSel("unitselection.dll") == FALSE)
	//{
	//	spausdinti_loga("klaida pakraunant unitselection.dll");
	//	return -1;//hr = -1;
	//}
	
		PrintTime("Start");				
		//CStdioFile file;
		//FILE * file = fopen(argv[2], "r");
		FILE * file = fopen("tr_5d_s10.txt", "r");
		//if (file.Open(argv[2], CFile::modeRead))
		if (file != NULL)
		{
			char buffer[256];
			unsigned short **unitsRows;//[1024][128];
			unsigned short **unitsRowsNextSeparators;//[1024][128];
			int *unitsRowsLength;//[1024];

			int i;
			unitsRows = new unsigned short*[MAX_ROWS];
			unitsRowsNextSeparators = new unsigned short*[MAX_ROWS];
			for (i = 0; i < MAX_ROWS; i++)
			{
				unitsRows[i] = new unsigned short[MAX_UNITS];
				unitsRowsNextSeparators[i] = new unsigned short[MAX_UNITS];
			}
			unitsRowsLength = new int[MAX_ROWS];

			int total_rows = 0;

			//while (file.ReadString(buffer))
			while (fgets(buffer, MAX_UNITS, file) != NULL)
			{
				unitsRowsLength[total_rows] = trText2UnitList(buffer, unitsRows[total_rows], unitsRowsNextSeparators[total_rows]);
				//buffer.ReleaseBuffer();
				total_rows++;
			}
			//file.Close();
			fclose(file);	

			PrintTime("Input data preparation finished");

			//if (initUnitSel(argv[1]) < 0)
			//if (initUnitSel(".\\Regina\\") < 0)  YRA KLAIDU SUSIDEDANT SVORIUS
			if (0 < 0)
				spausdinti_loga("Error in initUnitSel\n");
			else
			{
				unsigned int totalCost = 0;
				unsigned int currentCost = 0;
				unsigned int retUnits[MAX_UNITS];

				unsigned int res_neigh_units = 0;
				unsigned int res_tot_neigh_units = 0;
				unsigned int res_tot_non_neigh_units = 0;
				unsigned int res_tot_seq_num = 0;
				unsigned int res_tot_units = 0;
				unsigned int res_tot_non_zero_rows = 0;
				unsigned int word_sep_num = 0;
				unsigned int syl_sep_num = 0;
				unsigned int no_sep_num = 0;


				char laikKat[200];
				//strcpy(laikKat, argv[1]);
				strcpy(laikKat, ".\\Regina\\");
				strcat(laikKat, "test_db_fon.txt");
				getDBFromFile(laikKat);

				unsigned int same_left_unit = 0;
				unsigned int different_left_unit = 0;
				unsigned int same_right_unit = 0;
				unsigned int different_right_unit = 0;
				
				PrintTime("Init finished");
				
				for (int r = 0; r < total_rows; r++)
				{
					//cout << r << endl;
					int ret = selectUnits(unitsRows[r], unitsRowsLength[r], unitsDB_ID, n_total_DBunits, retUnits, &currentCost);
					
					totalCost += currentCost;
					
					if (ret < 0)
						printf("Error on row %d\n", r);
					else
					{
						if (currentCost > 0)
						{
							res_tot_non_zero_rows++;
							res_tot_units+=unitsRowsLength[r];
						}
						printf("(cost=%d):", currentCost);
						for (int f = 0; f < unitsRowsLength[r]; f++)
						{
							printf("[%d:%s]", retUnits[f], FonV[unitsRows[r][f]]);//[80203:a]

							fprintf(stream, "[%d:%s]\n", retUnits[f], FonV[unitsRows[r][f]]);


							if (currentCost > 0)
							{
								if ((f > 0) && (retUnits[f] != (retUnits[f-1]+1)))
								{
									if ((retUnits[f] > 0)&&(unitsRows[r][f-1] == unitsDB_ID[retUnits[f]-1])) same_left_unit++;
									else different_left_unit++;
								}
								if ((f < (unitsRowsLength[r]-1)) && (retUnits[f] != (retUnits[f+1]-1)))
								{
									if ((retUnits[f] < (n_total_DBunits-1))&&(unitsRows[r][f+1] == unitsDB_ID[retUnits[f]+1])) same_right_unit++;
									else different_right_unit++;
								}

								if (f > 0)
								{
									if (retUnits[f] == (retUnits[f-1]+1))
										res_tot_neigh_units++;
									else
									{
										if (unitsRowsNextSeparators[r][f-1] == '+') word_sep_num++;
										else if (unitsRowsNextSeparators[r][f-1] == '-') syl_sep_num++;
										else no_sep_num++;

										res_tot_non_neigh_units++;
										res_tot_seq_num++;
									}
								}
								else
									res_tot_seq_num++;
							}

						}
						printf("\r\n");
					}
				}
				printf("totalCost=%d\n", totalCost);

				printf("res_tot_neigh_units=%d, res_tot_non_neigh_units=%d, res_tot_seq_num=%d, res_tot_units=%d, res_tot_non_zero_rows=%d, word_sep_num=%d, syl_sep_num=%d, no_sep_num=%d, same_left_unit=%d, different_left_unit=%d, same_right_unit=%d, different_right_unit=%d\r\n", 
						res_tot_neigh_units, res_tot_non_neigh_units, res_tot_seq_num, res_tot_units, res_tot_non_zero_rows, word_sep_num, syl_sep_num, no_sep_num, same_left_unit, different_left_unit, same_right_unit, different_right_unit);

			}
			PrintTime("Finish");

			for (i = 0; i < MAX_ROWS; i++)
			{
				delete[] unitsRows[i];
				delete[] unitsRowsNextSeparators[i];
			}					
			delete[] unitsRows;
			delete[] unitsRowsNextSeparators;
			delete[] unitsRowsLength;

		}
		else 
		{
			spausdinti_loga("Klaida pakraunant test faila");
		}
				
		unloadUnitSel();
		//FreeLibrary(dll_handle);
		//dll_handle = NULL;
		cin.get();

		fclose(stream);

	return nRetCode;
}