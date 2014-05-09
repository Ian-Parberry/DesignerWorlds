#include <windows.h>

#include <stdio.h>
#include <conio.h>

int ReadDEMHeader(FILE* input){
  if(input == NULL)return 0; //fail and bail

  int nrows, ncols, nDummy;
  float fDummy;

  fscanf_s(input, "nrows %d\n", &nrows);
  fscanf_s(input, "ncols %d\n", &ncols);
  fscanf_s(input, "xllcenter %f\n", &fDummy);
  fscanf_s(input, "yllcenter %f\n", &fDummy);
  fscanf_s(input, "cellsize %f\n", &fDummy);
  fscanf_s(input, "NODATA_value %d\n", &nDummy);

  return nrows * ncols;
} //ReadDEMHeader


int main(int argc, char *argv[]){
  char filename[] = "..\\1 Data\\12SVH200800.asc";
  const int TABLESIZE = 256;
  int nCount[TABLESIZE];
  for(int i=0; i<TABLESIZE; i++)
    nCount[i] = 0;

  FILE* input;
  fopen_s(&input, filename, "rt");

  int nSize = ReadDEMHeader(input);

  int nMinHt = 99999;
  int nMaxHt = -9999;
  if(input){
    printf("Scanning %s...\n", filename);
    float fSum = 0;
    int n = 0;
    float fHt;
    int nHt;
    for(int i=0; i<nSize; i++){
      fscanf_s(input, "%f", &fHt);
      nHt = (int)(fHt + 0.5f);
      if(nHt > 100){
        fSum += (float)nHt;
        nMinHt = min(nMinHt, nHt);
        nMaxHt = max(nMaxHt, nHt);
        n++;
      } //if
    } //for

    printf("%d points found out of %d\n", n, nSize);
    printf("Lowest Altitude  = %dm\n", nMinHt);
    printf("Average Altitude = %dm\n", (int)(fSum/n));
    printf("Highest Altitude = %dm\n", nMaxHt);
  } //if
  
  printf("Analyzing...\n");
  const int DISTRIBUTIONSIZE = 32;
  const float GRANULARITY = (float)(nMaxHt - nMinHt + 1.0f)/(float)DISTRIBUTIONSIZE;
  fseek(input, 0, SEEK_SET);
  if(input){   
    ReadDEMHeader(input);
    float fHt;
    int nHt;
    for(int i=0; i<nSize; i++){
      fscanf_s(input, "%f", &fHt);
      nHt = (int)(fHt + 0.5f);
      if(nHt > 100.0f)
        nCount[(int)((nHt - nMinHt)/GRANULARITY)]++;
    } //for
    fclose(input);
  } //if

  FILE* output;
  fopen_s(&output, "output.txt", "wt");
  int delta = (int)((float)(nMaxHt - nMinHt + 1)/GRANULARITY);
  if(output){
    for(int i=0; i<delta; i++)
      fprintf(output, "%0.2f\t%d\n", i*GRANULARITY + nMinHt, nCount[i]);
    fclose(output);
  } //if
 
  int nSum = 0;
  for(int i=0; i<=delta; i++)
    nSum += nCount[i];

  int nCount2[TABLESIZE];
  int nTally = 0;
  for(int i=0; i<=delta; i++){
    int count = (int)((float)(TABLESIZE*nCount[i])/nSum);
    nTally += count;
    nCount2[i] = count;
  } //for

  for(int i=0; i<=delta; i++)
    if(nTally < TABLESIZE){
      nCount2[i]++; nTally++;
    } //if

  int min = 0;
  while(nCount2[min] == 0)
    min++;

  int max = delta;
  while(nCount2[max] == 0)
    max--;

  fopen_s(&output, "code.txt", "wt");
  if(output){
    fprintf(output, "const int POINTCOUNT = %d;\n", max - min + 1);
    fprintf(output, "int g_nUtahDistribution[POINTCOUNT] = {\n  ");
    int sum = 0;
    for(int i=min; i<max; i++){
      fprintf(output, "%d, ", nCount2[i]);
      sum += nCount2[i];
    } //for
    fprintf(output, "%d\n};\n", nCount2[max]);
    sum += nCount2[max];

    printf("Checking: %d entries that sum to %d\n", max-min+1, sum);
    fclose(output);
  } //if

  printf("Hit Almost Any Key to Exit...\n");

  _getch();
  return 0;
}