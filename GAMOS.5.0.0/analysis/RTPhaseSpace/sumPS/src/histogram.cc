//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  GAMOS software  is  copyright of the Copyright  Holders  of *
// * the GAMOS Collaboration.  It is provided  under  the  terms  and *
// * conditions of the GAMOS Software License,  included in the  file *
// * LICENSE and available at  http://fismed.ciemat.es/GAMOS/license .*
// * These include a list of copyright holders.                       *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GAMOS collaboration.                       *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the GAMOS Software license.           *
// ********************************************************************
//
/*==================================================================
                                                                 
      PLOTPHSP HISTOGRAM LIBRARY  
 
      Juan Diego Soler Pulido
      email: juandiego.soler@ciemat.es
      Madrid, Spain
      December 2006

 =================================================================*/

#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <string>
#include <math.h>
using namespace std;

#include "histogram.h"

histogram::histogram(float min, float max, float size, int tp){

  float aux;
  type = tp;
  aux = (max-min)/size;
  bins = (int) aux;
  bins += 1;
  v1min = min;
  v1max = max;
  bin_size = size;
  vect1 = new float[(bins)];
  vect2 = new float[(bins)];

}

histogram::~histogram(){}

void histogram::fill_histogram(){
  int count;
  for(count=0;count<bins;count++){
    vect1[count] = v1min+count*bin_size;
    vect2[count] = 0;
     }  

}

void histogram::putin_histogram(float value){

  int count;
  for(count=0;count<bins;count++){
    if(value >= vect1[count] && value < vect1[(count+1)]){
      vect2[count] += 1;
    }
    else if(count == (bins-1) && value >= vect1[count]){
      vect2[count] += 1;
    }
  }  

}

void histogram::print_histogram(){

  int count;
  for(count=0;count<bins;count++){
    cout << vect1[count] << " " << vect2[count] <<endl;
    }  

}

void histogram::plot_histogram(char filename[80]){

  FILE* gnuplotfile;
  gnuplotfile = popen("gnuplot -persist","w");

  if(type == 1){
  fprintf(gnuplotfile,"set title 'Weight Histogram'\n");
  fprintf(gnuplotfile,"set xlabel 'Weight'\n");
  fprintf(gnuplotfile,"set ylabel 'Counts'\n");
  }
  else if(type == 2){
  fprintf(gnuplotfile,"set title 'Energy Histogram'\n");
  fprintf(gnuplotfile,"set xlabel 'Energy (MeV)'\n");
  fprintf(gnuplotfile,"set ylabel 'Counts'\n");
  }

  fprintf(gnuplotfile,"plot '%s' using 1:2 with steps \n",filename);
  fflush(gnuplotfile);
  fclose(gnuplotfile);

}

void histogram::save_histogram(char filename[80]){
  
  int count;
  char outputhis[81]={'\0'};
  FILE* histogramfile;
  histogramfile = fopen(filename,"w");
  
  for(count=0;count<bins;count++){

    fprintf(histogramfile,"%f\t%f\n",vect1[count],vect2[count]); 
    } 

  fclose(histogramfile);

}
