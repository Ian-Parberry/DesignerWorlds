/// \file main.cpp

//Copyright Ian Parberry, November 2013.
//
//This file is made available under the GNU All-Permissive License.
//
//Copying and distribution of this file, with or without
//modification, are permitted in any medium without royalty
//provided the copyright notice and this notice are preserved.
//This file is offered as-is, without any warranty.

#include <windows.h>
#include <MMSystem.h>
#include <stdio.h>
#include <conio.h>

#include "valuenoise.h"

const int CELLSIZE = 4000; //width of square grid
const int NUMOCTAVES = 8; //number of octaves of 1/f noise
const int ALTITUDE = 4000; //altitude scale value

CDesignerWorld g_cDesignerWorld;

//Height distribution data.
const int POINTCOUNT = 31;
int g_nUtahDistribution[POINTCOUNT] = {
  1, 4, 6, 7, 7, 8, 10, 11, 14, 30, 37, 30, 19, 11, 8, 5, 5, 4, 3, 3, 3, 3, 3, 3, 5, 4, 4, 3, 2, 2, 1
};

/// Print the header for a DEM file.
/// \param output Output file handle.

void printDEMfileHeader(FILE* output){
  if(output == NULL)return; //bail and fail

  fprintf(output, "nrows %d\n", CELLSIZE);
  fprintf(output, "ncols %d\n", CELLSIZE);
  fprintf(output, "xllcenter %0.6f\n", 0.0f);
  fprintf(output, "yllcenter %0.6f\n", 0.0f);
  fprintf(output, "cellsize 5.000000\n");
  fprintf(output, "NODATA_value  -9999\n");
} //printDEMfileHeader

int main(int argc, char *argv[]){ 
  //initialize the random number generator
  int seed = timeGetTime();
  srand(seed);
  printf("Pseudorandom number seed = %d\n", seed);

  //set up designer world
  g_cDesignerWorld.Initialize();
  g_cDesignerWorld.SetValueTable(g_nUtahDistribution, POINTCOUNT);

  //start the DEM file
  char filename[MAX_PATH];
  sprintf_s(filename, "%d.asc", seed);
  FILE* output;
  fopen_s(&output, filename, "wt");
  printDEMfileHeader(output);

  //get random origin
  float x = (float)rand();
  float z = (float)rand();

  //generate and save grid heights to DEM file
  for(int i=0; i<CELLSIZE; i++){
    for(int j=0; j<CELLSIZE; j++)
      fprintf(output, "%0.2f ", ALTITUDE *
        g_cDesignerWorld.GetHeight(x + i/256.0f, z + j/256.0f, 0.5f, 2.0f, NUMOCTAVES));
    fprintf(output, "\n");
    if(i%100 == 0)printf(".");
  } //for

  //shut down and exit
  printf("\n");
  fclose(output);

  printf("Hit Almost Any Key to Exit...\n");
  _getch();
  return 0;
} //main