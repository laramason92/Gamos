/*
 * Copyright (C) 2006 International Atomic Energy Agency
 *
 * iaea_header.h is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * iaea_header.h is distributed in the hope that it will be useful,
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
#include "iaeaRecord.h"

#ifndef IAEA_HEADER
#define IAEA_HEADER

/* *********************************************************************** */
// defines

#define ULONG unsigned long //long // it could be defined as __int64

#define SEGMENT_BEG_TOKEN '$'
#define SEGMENT_END_TOKEN ':'
#define OK     0
#define FAIL  -1

#ifndef MAX_STR_LEN
  #define MAX_STR_LEN 120   /* maximum length of a string */
#endif
#define MAX_NUMB_LINES 30   /* maximum number of lines in a block */

struct iaea_header_type
{
  FILE *fheader;

  // ******************************************************************************
  // 1. PHSP format
  
  int file_type;		    // 0 = phsp file ;  1 = phsp generator 

  int byte_order;		    // as defined by get_byte_order routine

  int record_contents[9];   // record_contents[i] = 1 or 0 (variable or constant)
                            // correspond to the following logical variables :
                            //             ix,iy,iz,iu.iv,iw;
                            //       iweight,iextrafloat,iextralong;  

  float record_constant[7]; // if record_contents[i<7] = 0
                            // then record_constant[i] contents the constant value
                            // extra floats and longs are always variable
                            // so no need to store them
  int record_length;
  //  record_length = 1 +                                     (particle)
  //                  4 + 									  (energy)
  //                  SUM(i=0;i<3) {record_contents[i]*4}	  (ix,iy,iz)
  //                  SUM(i=3;i<6) {record_contents[i]*4}	  (iu,iv,iw)
  //                  record_contents[6]*4 + 				  (iweigth)
  //                  record_contents[7]*4 + 				  (iextrafloat)
  //				  record_contents[8]*4 +				  (iextralong)
  long checksum;

  // ******************************************************************************
  // 2. Mandatory description of the phsp
  
  char coordinate_system_description[MAX_STR_LEN*MAX_NUMB_LINES+1];

  // Counters for phsp file
  ULONG orig_histories;  
  ULONG nParticles;

  ULONG nPhotons;
  ULONG nElectrons;
  ULONG nPositrons;
  ULONG nNeutrons;
  ULONG nProtons;

  // Event generator input file
  char input_file_for_event_generator[MAX_STR_LEN*MAX_NUMB_LINES+1];
  
  // ******************************************************************************
  // 3. Mandatory additional information
  
  unsigned int iaea_index;                                 // Agency ID

  char title[MAX_STR_LEN*MAX_NUMB_LINES+1];
  
  char machine_type[MAX_STR_LEN*MAX_NUMB_LINES+1];
  
  char MC_code_and_version[MAX_STR_LEN*MAX_NUMB_LINES+1];
  
  float global_photon_energy_cutoff;
  
  float global_particle_energy_cutoff;
  
  char transport_parameters[MAX_STR_LEN*MAX_NUMB_LINES+1];

  // ******************************************************************************
  // 4. Optional description
  
  char beam_name[MAX_STR_LEN*MAX_NUMB_LINES+1];
  
  char field_size[MAX_STR_LEN*MAX_NUMB_LINES+1];
  
  char nominal_SSD[MAX_STR_LEN*MAX_NUMB_LINES+1];
  
  char variance_reduction_techniques[MAX_STR_LEN*MAX_NUMB_LINES+1];
  
  char initial_source_description[MAX_STR_LEN*MAX_NUMB_LINES+1];

  // Documentation sub-section
  char MC_input_filename[MAX_STR_LEN*MAX_NUMB_LINES+1];
  
  // Assumed to be the preferred citation
  char published_reference[MAX_STR_LEN*MAX_NUMB_LINES+1]; 

  char authors[MAX_STR_LEN*MAX_NUMB_LINES+1];
  
  char institution[MAX_STR_LEN*MAX_NUMB_LINES+1];
  
  char link_validation[MAX_STR_LEN*MAX_NUMB_LINES+1];

  char additional_notes[MAX_STR_LEN*MAX_NUMB_LINES+1];

  // ******************************************************************************
  // 5. Optional statistical information
  float averageKineticEnergy[MAX_NUM_PARTICLES];
  float sumParticleWeight[MAX_NUM_PARTICLES];
  float maximumKineticEnergy[MAX_NUM_PARTICLES];
  float minimumKineticEnergy[MAX_NUM_PARTICLES];
  float minimumX, maximumX;
  float minimumY, maximumY;
  float minimumZ, maximumZ;  
  float minimumWeight[MAX_NUM_PARTICLES];
  float maximumWeight[MAX_NUM_PARTICLES];  

// CLASS FUNCTIONS

public:
        int read_header();
        int write_header();
	int print_header();
	int set_record_contents(iaea_record_type *p_iaea_record);
	int get_record_contents(iaea_record_type *p_iaea_record);
    
	void initialize_counters();
	void update_counters(iaea_record_type *p_iaea_record);
	
	int write_sample_header();

private:
	int read_block(char *lineread,char *blockname);
	int get_block(char *lineread);
	int get_blockname(char *line, char *blockname);
	void write_blockname(char *blockname);

	int check_byte_order();
	void print_statistics();
};

#endif
