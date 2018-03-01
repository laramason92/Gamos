/*
 * Copyright (C) 2006 International Atomic Energy Agency
 *
 * iaea_record.h is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * iaea_record.h is distributed in the hope that it will be useful,
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

#include <stdio.h>

#ifndef IAEA_RECORD
#define IAEA_RECORD

/* *********************************************************************** */
// defines

// To use additional float or integers
#define NUM_EXTRA_FLOAT 2 // Maximum two extra float stored
#define NUM_EXTRA_LONG  2 // Maximum two extra long stored
#define MAX_NUM_PARTICLES 5 // 1 photons 
                            // 2 electrons
                            // 3 positrons
                            // 4 neutrons
                            // 5 protons

#define ULONG long // it could be defined as __int64

#define OK     0
#define FAIL  -1

/* *********************************************************************** */
// structures

// int *      signed, signed int   System dependent
// unsigned int      *      unsigned      System dependent
//__int8      1      char, signed char    -128 to 127
//__int16     2      short, short int, signed short int -32,768 to 32,767
//__int32     4      signed, signed int   -2,147,483,648 to 2,147,483,647
//__int64     8      none   -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807
//char 1      signed char   -128 to 127
//unsigned char      1      none   0 to 255
//short       2      short int, signed short int -32,768 to 32,767
//unsigned short     2      unsigned short int   0 to 65,535
//long 4      long int, signed long int   -2,147,483,648 to 2,147,483,647
//unsigned long      4      unsigned long int    0 to 4,294,967,295
//enum *      none   Same as int
//float       4      none   3.4E +/- 38 (7 digits)
//double      8      none   1.7E +/- 308 (15 digits)
//long double 10     none   1.2E +/- 4932 (19 digits)

struct iaea_record_type
{
  FILE *p_file;   // phase space file pointer	

  short particle; // mandatory       (photon:1 electron:2 positron:3 neutron:4 proton:5 ...)
  float  energy;  // mandatory
  
  short IsNewHistory;           // coded as sign of energy
  
  float x;       int ix;       
  float y;	     int iy;
  float z;	     int iz;
  float u;	     int iu;
  float v;	     int iv;
  float w;       int iw;      // sign of w coded as sign of particle type
  float weight;	 int iweight;

  int iextrafloat; 
  int iextralong;  

  float extrafloat[NUM_EXTRA_FLOAT];  // (default two extra float stored)
  long extralong[NUM_EXTRA_LONG];     // (default two extra long stored)

public:
	int read_particle();
	int write_particle();
	int initialize();
};

#endif
