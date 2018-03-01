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
/*
 * Copyright (C) 2006 International Atomic Energy Agency
 * -----------------------------------------------------------------------------
 * iaea_record.cpp is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * iaea_record.cpp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *-----------------------------------------------------------------------------
 *
 *   Contact:
 *
 *   Roberto Capote Noy
 *   e-mail: R.CapoteNoy@iaea.org (rcapotenoy@yahoo.com)
 *   International Atomic Energy Agency
 *   Nuclear Data Section, P.O.Box 100
 *   Wagramerstrasse 5, Vienna A-1400, AUSTRIA
 *   Phone: +431-260021713; Fax: +431-26007
 *
 ****************************************************************************/

//#define DEBUG // Comment to avoid printing for every particle write or read

#include <math.h>
#include "iaeaRecord.h"
//#include "../include/iaeaRecord.hh"

int iaea_record_type::initialize()
{
  if(p_file == NULL) {
     fprintf(stderr, "\n ERROR: Failed to open Phase Space file \n");
     return (FAIL);
  }

  // Defines i/o logic and variable quantities to be stored
  // If the value is zero, then corresponding quantity is fixed
  ix = 1;       
  iy = 1;
  iz = 1;
  iu = 1;
  iv = 1;
  iw = 1;
  iweight = 1;
  // Defines a number of EXTRA long variables stored (EGS need 1 for LATCH)
  iextralong = 1;    
  if( iextralong >= NUM_EXTRA_LONG)
  {
     fprintf(stderr, "\n ERROR: Increase NUM_EXTRA_LONG number in iaea_record.h\n");
     return (FAIL);
  }
  // Defines a number of EXTRA float variables stored (EGS could need 1 for ZLAST)
  iextrafloat = 1;     
  if( iextrafloat >= NUM_EXTRA_FLOAT)
  {
     fprintf(stderr, "\n ERROR: Increase NUM_EXTRA_FLOAT number in iaea_record.h\n");
     return (FAIL);
  }

  return (OK);
}

int iaea_record_type::write_particle()
{
  float floatArray[NUM_EXTRA_FLOAT+7];
  long longArray[NUM_EXTRA_LONG];

  char ishort = (char) particle;
  if(w < 0) ishort = -ishort; // Sign of w is stored in particle type
  
  if( fwrite(&ishort, sizeof(char),   1, p_file) != 1)
  {
    fprintf(stderr, "\n ERROR: write_particle: Failed to write particle type\n");
    return (FAIL);;
  }

  int reclength = sizeof(char);

  if(IsNewHistory > 0) energy *= (-1); // New history is signaled by negative energy

  floatArray[0] = energy;
  
  int i = 0;

  if(ix > 0) floatArray[++i] = x;
  if(iy > 0) floatArray[++i] = y;
  if(iz > 0) floatArray[++i] = z;
  if(iu > 0) floatArray[++i] = u;
  if(iv > 0) floatArray[++i] = v;
  if(iweight > 0) floatArray[++i] = weight;

  int j;
  for(j=0;j<iextrafloat;j++) floatArray[++i] = extrafloat[j];

  reclength += (i+1)*sizeof(float);

  if( fwrite(floatArray, sizeof(float), (size_t)(i+1), p_file) != (size_t) (i+1))
  {
     fprintf(stderr, "\n ERROR: write_particle: Failed to write FLOAT phsp data\n");
     return (FAIL);
  }

  if(iextralong > 0) 
  {
     for(j=0;j<iextralong;j++) longArray[j] = extralong[j];
     reclength += iextralong*sizeof(long);
     if( fwrite(longArray, sizeof(long), (size_t)iextralong, p_file) != (size_t)iextralong)
     {
        fprintf(stderr, "\n ERROR: write_particle: Failed to write LONG phsp data\n");
        return (FAIL);
     }
  }

  if(reclength == 0) return(FAIL);

  #ifdef DEBUG
  // charge defined 
  int iaea_charge[MAX_NUM_PARTICLES]={0,-1,+1,0,+1};
  int charge = iaea_charge[particle - 1];

  printf("\n Wrote a particle with a record lenght %d",reclength);
  printf("\n Q %d E %f X %f Y %f Z %f \n\t u %f v %f w %f W %f Part %d \n",
  charge, energy, x, y, z, u, v, w, weight, particle);
  if( iextrafloat > 0) printf(" EXTRA FLOATs:"); 
  for(j=0;j<iextrafloat;j++) printf(" F%i %f",j+1,extrafloat[j]);
  if( iextralong > 0)  printf(" EXTRA LONGs:"); 
  for(j=0;j<iextralong;j++) printf(" L%i %d", j+1,extralong[j]);
  printf("\n"); 
  #endif
    
  return(OK);
}

int iaea_record_type::read_particle()
{
  float floatArray[NUM_EXTRA_FLOAT+7];
  long longArray[NUM_EXTRA_LONG+7];
  int i,j,iw,reclength;
  char ctmp; 

  if( fread(&ctmp, sizeof(char),   1, p_file) != 1) // particle type is always read
  {
    fprintf(stderr, "\n ERROR: read_particle: Failed to read particle type\n");
    return (FAIL);;
  }
  
  particle = (short) ctmp;

  iw = 1; // getting sign of Z director cosine w
  if(particle < 0) {iw = -1; particle = -particle;}

  reclength = sizeof(char);      // particle type is always read
  unsigned int rec_to_read = 1;    // energy is always read

  if(ix > 0) rec_to_read++;
  if(iy > 0) rec_to_read++;
  if(iz > 0) rec_to_read++;
  if(iu > 0) rec_to_read++;
  if(iv > 0) rec_to_read++;
  if(iweight > 0) rec_to_read++;
  if(iextrafloat>0) rec_to_read += iextrafloat;

  if( fread(floatArray, sizeof(float), rec_to_read, p_file) != rec_to_read)
  {
    fprintf(stderr, "\n ERROR: read_particle: Failed to read FLOATs \n");
    fprintf(stderr, "size %d \n",rec_to_read);
    return (FAIL);
  }
  
  reclength += rec_to_read*sizeof(float);

  
  IsNewHistory = 0;
  if(floatArray[0]<0) IsNewHistory = 1; // like egsnrc   
  energy = fabs(floatArray[0]);

  i = 0;
  if(ix > 0) x = floatArray[++i]; 
  if(iy > 0) y = floatArray[++i];
  if(iz > 0) z = floatArray[++i];
  if(iu > 0) u = floatArray[++i];
  if(iv > 0) v = floatArray[++i];
  if(iweight > 0) weight = floatArray[++i];
  for(j=0;j<iextrafloat;j++) extrafloat[j] = floatArray[++i];

  w = 0.f;
  double aux = (u*u + v*v);
  if (aux<=1.0) w = (float) (iw * sqrt((float)(1.0 - aux)));
  else
  { aux = sqrt((float)aux);
	u /= (float)aux;
	v /= (float)aux;
  }

  if(iextralong > 0) 
  {
     if( fread(longArray, sizeof(long), (size_t)iextralong, p_file) != (size_t)iextralong)
     {
       fprintf(stderr, "\n ERROR: read_particle: Failed to read LONGS\n");
       return (FAIL);
     }
     for(int l=0,j=0;j<iextralong;j++) extralong[j] = longArray[l++];
     reclength += (iextralong)*sizeof(long);
  }

  #ifdef DEBUG
  // charge defined 
  int iaea_charge[MAX_NUM_PARTICLES]={0,-1,+1,0,+1};
  int charge = iaea_charge[particle - 1];

  printf("\n Read a particle with a record lenght %d (New History: %d)",
	         reclength,IsNewHistory);
  printf("\n Q %d E %f X %f Y %f Z %f \n\t u %f v %f w %f W %f Part %d \n",
  charge, energy, x, y, z, u, v, w, weight, particle);
  if( iextrafloat > 0) printf(" EXTRA FLOATs:"); 
  for(j=0;j<iextrafloat;j++) printf(" F%i %f",j+1,extrafloat[j]);
  if( iextralong > 0)  printf(" EXTRA LONGs:"); 
  for(j=0;j<iextralong;j++)  printf(" L%i %d",j+1,extralong[j]);
  printf("\n"); 
  #endif
  return(reclength);
}
