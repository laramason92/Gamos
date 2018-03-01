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
using namespace std;  // if not commented "cout" instead of  "std::cout" etc...
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <vector>

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
//#include "GamosCore/GamosBase/include/GmAnalysisMgr.hh"


typedef unsigned short SINO_TYPE;  //!!TO DO: read from .hv/.hs !!

unsigned long int read_data(const char* theName, SINO_TYPE* theSino);
void write_sino3D(SINO_TYPE* sino);

// Global variables:

double _PI;
int Ni, Ne, oType=1;
unsigned long int posTOT, sizeTOT; // total file positions

//const char *fileOUTname="projdata_ALL";
//const char *fileINname ="default_sino3D";

string fileOUTname ("projdata_ALL");
string fileINname ("default_sino3D");

SINO_TYPE *sino_total;

