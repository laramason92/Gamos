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
#include <cstdio>

#ifndef IAEA_RECORD
#define IAEA_RECORD

#include "iaea_utilities.hh"
#include "iaea_config.hh"

/* *********************************************************************** */
// defines

// To use additional float or integers

#ifndef NUM_EXTRA_FLOAT
  #define NUM_EXTRA_FLOAT 10 // Maximum 10 extra float stored
#endif

#ifndef NUM_EXTRA_LONG
  #define NUM_EXTRA_LONG  10 // Maximum 10 extra long stored
#endif

#define MAX_NUM_PARTICLES 5 // 1 photons 
                            // 2 electrons
                            // 3 positrons
                            // 4 neutrons
                            // 5 protons
#define MAX_NUM_SOURCES 30

#define OK     0
#define FAIL  -1

/* *********************************************************************** */
// structures

struct iaea_record_type
{
  FILE *p_file;   // phase space file pointer   

  short particle; // mandatory       (photon:1 electron:2 positron:3 neutron:4 proton:5 ...)
  
  float  energy;  // mandatory
  
  IAEA_I32 IsNewHistory; // coded as sign of energy 
                         //  Type changed from short to IAEA_I32 to store EGS n_stat

  float x;       int ix;       
  float y;       int iy;
  float z;       int iz;
  float u;       int iu;
  float v;       int iv;
  float w;       int iw;      // sign of w coded as sign of code
  float weight;  int iweight;

  short iextrafloat; 
  short iextralong;  

  float extrafloat[NUM_EXTRA_FLOAT];  // (default: no extra float stored)
  IAEA_I32 extralong[NUM_EXTRA_LONG];      // (default: one extra long stored)

public:
      short read_particle();
      short write_particle();
      short initialize();

private:
  int sizeExtraLong;
};

#endif
