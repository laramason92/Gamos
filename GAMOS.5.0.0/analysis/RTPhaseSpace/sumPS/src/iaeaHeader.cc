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
#include <iostream>
/*
 * Copyright (C) 2006 International Atomic Energy Agency
 * -----------------------------------------------------------------------------
 * This code uses subroutines developed by Jeffrey V Siebers [jsiebers@vcu.edu]
 * at Virginia Commonwealth University 
 * See Copyright motice and license in utilities.h and utilities.cpp modules
 *------------------------------------------------------------------------------
 * iaea_header.cpp is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * iaea_header.cpp is distributed in the hope that it will be useful,
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
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include "iaeaUtilities.h"
#include "iaeaHeader.h"

int iaea_header_type::read_header ()
{
    char line[MAX_STR_LEN]; 
    
	if(fheader==NULL)
    {
      printf("\n ERROR: Unable to open header file \n"); 
	  return(FAIL);
    }

  // ******************************************************************************
  // 1. PHSP format

	/*********************************************/
	if ( read_block(line,"FILE_TYPE") == FAIL ) 
	{
		printf("\nMandatory keyword FILE_TYPE is not defined in input\n");
		return FAIL;
	}
	else file_type = atoi(line); 

	/*********************************************/
	if ( read_block(line,"CHECKSUM") == FAIL ) 
	{
		printf("\nMandatory keyword CHECKSUM is not defined in input\n");
		return FAIL;
	}
	else checksum = atol(line); 

	/*********************************************/
	if ( read_block(line,"RECORD_LENGTH") == FAIL ) 
	{
		printf("\nMandatory keyword RECORD_LENGTH is not defined in input\n");
		return FAIL;
	}
	else record_length = atoi(line); 

	/*********************************************/
	if ( read_block(line,"BYTE_ORDER") == FAIL ) 
	{
		printf("\nMandatory keyword BYTE_ORDER is not defined in input\n");
		return FAIL;
	}
	else byte_order = atoi(line); 

	/*********************************************/
    if( get_blockname(line,"RECORD_CONTENTS") == FAIL) 
    {
       printf("\nMandatory keyword RECORD_CONTENTS is not defined in input\n");
	   return FAIL; 
    }

	int i;
    for (i=0;i<9;i++)
    {
	  record_contents[i] = 0;
	  if( get_string(fheader,line) == FAIL ) return FAIL;
	  if( *line == SEGMENT_BEG_TOKEN ) break; 
      record_contents[i] = atoi(line); 
    }
	/*********************************************/
    if( get_blockname(line,"RECORD_CONSTANT") == FAIL) 
    {
       printf("\nMandatory keyword RECORD_CONSTANT is not defined in input\n");
	   return FAIL; 
    }

    for (i=0;i<7;i++)
    {
	  record_constant[i] = 32000.f;
	  if(record_contents[i] > 0) continue;
	  if( get_string(fheader,line) == FAIL ) return FAIL;
	  if( *line == SEGMENT_BEG_TOKEN ) break; 
	  record_constant[i] = (float)atof(line); 
    }

// ******************************************************************************
// 2. Mandatory description of the phsp

	if ( read_block(coordinate_system_description,"COORDINATE_SYSTEM_DESCRIPTION") == FAIL)	
	{
		printf("\nMandatory keyword COORDINATE_SYSTEM_DESCRIPTION is not defined in input\n");
		return FAIL;
	}

  if(file_type == 1) // For event generators
  {
    /*********************************************/
	if ( read_block(line,"INPUT_FILE_FOR_EVENT_GENERATOR") == FAIL ) 
	{
		printf("\nMandatory keyword INPUT_FILE_FOR_EVENT_GENERATOR is not defined in input\n");
		return FAIL;
	}
  }

  if(file_type == 0) // for phsp files
  {
	/*********************************************/
	if ( read_block(line,"ORIG_HISTORIES") == FAIL ) 
	{
		printf("\nMandatory keyword ORIG_HISTORIES is not defined in input\n");
		return FAIL;
	}
	else orig_histories = atol(line);  

	/*********************************************/
	if ( read_block(line,"PARTICLES") == FAIL ) 
	{
		printf("\nMandatory keyword PARTICLES is not defined in input\n");
		return FAIL;
	}
	else nParticles = atol(line); 

	/*********************************************/
	nPhotons = nElectrons = nPositrons = nNeutrons = nProtons = 0; 
	if ( read_block(line,"PHOTONS") == OK ) nPhotons = atol(line);
	if ( read_block(line,"ELECTRONS") == OK ) nElectrons = atol(line);
	if ( read_block(line,"POSITRONS") == OK ) nPositrons = atol(line);
	if ( read_block(line,"NEUTRONS") == OK ) nNeutrons = atol(line);
	if ( read_block(line,"PROTONS") == OK ) nProtons = atol(line);
  }
// ******************************************************************************
// 3. Mandatory additional information
	if ( read_block(line,"IAEA_INDEX") == FAIL ) 
	{
		printf("\nMandatory keyword IAEA_INDEX is not defined in input\n");
		return FAIL;
	}
	else iaea_index = atoi(line); 

	/*********************************************/
	if ( read_block(title,"TITLE") == FAIL ) 
	{
		printf("\nMandatory keyword TITLE is not defined in input\n");
		return FAIL;
	}

	/*********************************************/
	if ( read_block(machine_type,"MACHINE_TYPE") == FAIL) 
	{
		printf("\nMandatory keyword MACHINE_TYPE is not defined in input\n");
		return FAIL;
	}
 
	/*********************************************/
	if ( read_block(MC_code_and_version,"MONTE_CARLO_CODE_VERSION") == FAIL ) 
	{
		printf("\nMandatory keyword MONTE_CARLO_CODE_VERSION is not defined in input\n");
		return FAIL;
	}

	/*********************************************/
	if ( read_block(line,"GLOBAL_PHOTON_ENERGY_CUTOFF") == FAIL ) 
	{
		printf("\nMandatory keyword GLOBAL_PHOTON_ENERGY_CUTOFF is not defined in input\n");
		return FAIL;
	}
	else global_photon_energy_cutoff = (float)atof(line); 

	/*********************************************/
	if ( read_block(line,"GLOBAL_PARTICLE_ENERGY_CUTOFF") == FAIL ) 
	{
		printf("\nMandatory keyword GLOBAL_PARTICLE_ENERGY_CUTOFF is not defined in input\n");
		return FAIL;
	}
	else global_photon_energy_cutoff = (float)atof(line); 

	/*********************************************/
	if ( read_block(transport_parameters,"TRANSPORT_PARAMETERS") == FAIL ) 
	{
		printf("\nMandatory keyword TRANSPORT_PARAMETERS is not defined in input\n");
		return FAIL;
	}

// ******************************************************************************
// 4. Optional description

	if ( read_block(beam_name,"BEAM_NAME") == FAIL ) 
		printf("ERROR reading BEAM_NAME\n");
	if ( read_block(field_size,"FIELD_SIZE") == FAIL ) 
		printf("ERROR reading FIELD_SIZE\n");
	if ( read_block(nominal_SSD,"NOMINAL_SSD") == FAIL ) 
		printf("ERROR reading NOMINAL_SSD\n");
	if ( read_block(variance_reduction_techniques,
		            "VARIANCE_REDUCTION_TECHNIQUES") == FAIL ) 
		printf("VARIANCE_REDUCTION_TECHNIQUES\n");
	if ( read_block(initial_source_description,"INITIAL_SOURCE_DESCRIPTION") == FAIL ) 
		printf("INITIAL_SOURCE_DESCRIPTION:\n");
  
	// Documentation sub-section
	/*********************************************/
	if ( read_block(MC_input_filename,"MC_INPUT_FILENAME") == FAIL ) 
		printf("MC_INPUT_FILENAME\n");
	if ( read_block(published_reference,"PUBLISHED_REFERENCE") == FAIL ) 
		printf("PUBLISHED_REFERENCE\n");
	if ( read_block(authors,"AUTHORS") == FAIL ) printf("AUTHORS\n");
	if ( read_block(institution,"INSTITUTION") == FAIL ) printf("INSTITUTION\n");
	if ( read_block(link_validation,"LINK_VALIDATION") == FAIL ) 
		printf("LINK_VALIDATION\n");
	if ( read_block(additional_notes,"ADDITIONAL_NOTES") == FAIL ) 
		printf("ADDITIONAL_NOTES\n");

// ******************************************************************************
// 5. Statistical information
	/*********************************************/
    if( get_blockname(line,"STATISTICAL_INFORMATION_PARTICLES") == OK) 
    {
	  for(i=0;i<MAX_NUM_PARTICLES;i++) 
	  {
		  if( get_string(fheader,line) == FAIL ) return FAIL;
		  if( *line == SEGMENT_BEG_TOKEN ) break;
		  
		  if( (i == 0) && (nPhotons == 0 ) ) continue;
		  if( (i == 1) && (nElectrons == 0 ) ) continue;
		  if( (i == 2) && (nPositrons == 0 ) ) continue;
		  if( (i == 3) && (nNeutrons == 0 ) ) continue;
		  if( (i == 4) && (nProtons == 0 ) ) continue;

		  // -------------------------------------------------------
		  // The fragment below replaces buggy sscanf() function
		  int index0 =0, index1 =0, len = strlen(line), icnt =0;
		  float fbuff[6];
		  do 
		  {   if(advance(line, &index1, len) != OK) break; // Finding first number
			  fbuff[icnt++] = (float)atof(line+index1);
              if(advance(line, &index0, len) != OK) break; // Skipping spaces
		  }	while (icnt < 6);
		  // -------------------------------------------------------
		  sumParticleWeight[i] = fbuff[0];
		  minimumWeight[i]	   = fbuff[1];
	      maximumWeight[i]     = fbuff[2];
          averageKineticEnergy[i] = fbuff[3];
		  minimumKineticEnergy[i] = fbuff[4];
		  maximumKineticEnergy[i] = fbuff[5];
	  }
    }			 

    if( get_blockname(line,"STATISTICAL_INFORMATION_GEOMETRY") == OK) 
	{
   	  minimumX = minimumY = minimumZ = 32000.f;
	  maximumX = maximumY = maximumZ = 0.f;

	  for(i=0;i<3;i++) 
	  {
		if(record_contents[i] == 1) 
		{
			if( get_string(fheader,line) == FAIL ) return FAIL;
			if( *line == SEGMENT_BEG_TOKEN ) break;

   		    // -------------------------------------------------------
		    // The fragment below replaces buggy sscanf() function
		    int index0 =0, index1 =0, len = strlen(line), icnt =0;
		    float fbuff[2];
		    do 
		    {   if(advance(line, &index1, len) != OK) break; // Finding first number
			    fbuff[icnt++] = (float)atof(line+index1);
                if(advance(line, &index0, len) != OK) break; // Skipping spaces
		    }	while (icnt < 2);
		    // -------------------------------------------------------

			if( i == 0) {minimumX = fbuff[0]; maximumX  = fbuff[1];}
			if( i == 1) {minimumY = fbuff[0]; maximumY  = fbuff[1];}
			if( i == 2) {minimumZ = fbuff[0]; maximumZ  = fbuff[1];}
		}
	  }
	}

	std::cout << " HEADER RETURN OK " <<std::endl;
    return(OK);  
}

void iaea_header_type::write_blockname(char *blockname)
{
  fprintf(fheader,"%c%s%c\n",SEGMENT_BEG_TOKEN,blockname,SEGMENT_END_TOKEN);
}

int iaea_header_type::get_blockname(char *line, char *blockname)
{
  char *begptr, *endptr;

  // printf("                       Reading block: %s ...\n",blockname);

  if(fheader==NULL)
  {
    printf("\n ERROR: Opening header file to Get Block \n"); return(FAIL);
  }
  rewind(fheader) ;

  while( get_string(fheader,line) == OK )
  {	
    if( *line == SEGMENT_BEG_TOKEN ) 
      {
         begptr = line + 1 ;
		 // printf("                       Read line %s ...\n",line);
         endptr = strchr(begptr,SEGMENT_END_TOKEN) ;
         if( endptr != NULL ) {
           *endptr = '\0' ;
		   // printf("                       Comparing %s ...\n",begptr);
           if( strcmp(begptr,blockname) == 0 ) return(OK) ;
         }
      }
  }
  return(FAIL) ;
}

int iaea_header_type::get_block(char *lineread)
{
	int read = FAIL, count = 0;

	char line[MAX_STR_LEN]; 

	strcpy (lineread,""); // Deleting lineread contents
	while( get_string(fheader,line) == OK )
    {
	  if( *line == SEGMENT_BEG_TOKEN ) break; 
	  strcat(lineread+count*MAX_NUMB_LINES,line); count++;
	  read = OK;
    }
	return (read);
}

int iaea_header_type::read_block(char *lineread,char *blockname)
{
    char line[MAX_STR_LEN]; 
    if( get_blockname(line,blockname) != OK) return FAIL; 
	if( get_block(lineread) != OK) return FAIL; 
	return OK;	
}

int iaea_header_type::set_record_contents(iaea_record_type *p_iaea_record)
{
   int i;
   for(i=0;i<9;i++) record_contents[i] = 0;          
   for(i=0;i<7;i++)	record_constant[i] = 32000.f;          

   if(p_iaea_record->ix > 0) record_contents[0] = 1;         
   else record_constant[0] = p_iaea_record->x;

   if(p_iaea_record->iy > 0) record_contents[1] = 1;         
   else record_constant[1] = p_iaea_record->y;

   if(p_iaea_record->iz > 0) record_contents[2] = 1;         
   else record_constant[2] = p_iaea_record->z;

   if(p_iaea_record->iu > 0) record_contents[3] = 1;         
   else record_constant[3] = p_iaea_record->u;

   if(p_iaea_record->iv > 0) record_contents[4] = 1;    
   else record_constant[4] = p_iaea_record->v;

   if(p_iaea_record->iw > 0) record_contents[5] = 1; 
   else record_constant[5] = p_iaea_record->w;

   if(p_iaea_record->iweight > 0) record_contents[6] = 1;         
   else record_constant[6] = p_iaea_record->weight;

   if(p_iaea_record->iextrafloat>0) record_contents[7] = p_iaea_record->iextrafloat;
   if(p_iaea_record->iextralong>0) record_contents[8] = p_iaea_record->iextralong;

   record_length = 5; // To consider for particle type (1 byte) and energy (4 bytes)
   for(i=0;i<8;i++) record_length += record_contents[i]*4;          
   record_length -= 4; // 4 bytes substracted as w is not stored, just his sign
   record_length += record_contents[8]*sizeof(ULONG);
   if(record_length > 0) return OK;
   else
   {
	   printf("\nRECORD LENGTH IS ZERO, CHECK HEADER BLOCK RECORD_CONTENTS\n");
	   return FAIL;
   }
}

int iaea_header_type::get_record_contents(iaea_record_type *p_iaea_record)
{
   int i;

   p_iaea_record->ix = 0;
   if(record_contents[0] == 1) p_iaea_record->ix = 1;         
   else p_iaea_record->x = record_constant[0];

   p_iaea_record->iy = 0;
   if(record_contents[1] == 1) p_iaea_record->iy = 1;         
   else p_iaea_record->y = record_constant[1];

   p_iaea_record->iz = 0;
   if(record_contents[2] == 1) p_iaea_record->iz = 1;         
   else p_iaea_record->z = record_constant[2];

   p_iaea_record->iu = 0;
   if(record_contents[3] == 1) p_iaea_record->iu = 1;         
   else p_iaea_record->u = record_constant[3];

   p_iaea_record->iv = 0;
   if(record_contents[4] == 1) p_iaea_record->iv = 1;         
   else p_iaea_record->v = record_constant[4];

   p_iaea_record->iw = 0;
   if(record_contents[5] == 1) p_iaea_record->iw = 1;         
   else p_iaea_record->w = record_constant[5];

   p_iaea_record->iweight = 0;
   if(record_contents[6] == 1) p_iaea_record->iweight = 1;         
   else p_iaea_record->weight = record_constant[6];

   p_iaea_record->iextrafloat = 0;
   if(record_contents[7] > 0) p_iaea_record->iextrafloat = record_contents[7];         
   
   p_iaea_record->iextralong = 0;
   if(record_contents[8] > 0) p_iaea_record->iextralong = record_contents[8];         

   record_length = 5; // To consider for particle type (1 bytes) and energy (4 bytes)
   for(i=0;i<8;i++) record_length += record_contents[i]*4; // 4 bytes variables         
   record_length -= 4; // 4 bytes substracted as w is not stored, just his sign
   record_length += record_contents[8]*sizeof(ULONG);
   if(record_length > 0) return OK;
   else
   {
	   printf("\nWRONG DEFINED HEADER BLOCK RECORD_CONTENTS\n");
	   return FAIL;
   }
}

void iaea_header_type::initialize_counters()
{
  nParticles = orig_histories = 0;
  nPhotons = nElectrons = nPositrons = nNeutrons = nProtons = 0;

  for(int i=0;i<MAX_NUM_PARTICLES;i++) 
  {
	  sumParticleWeight[i] = 0.f;
	  maximumKineticEnergy[i] = 0.f;
	  minimumKineticEnergy[i] = 32000.f;
	  averageKineticEnergy[i] = 0.f;
	  minimumWeight[i] = 32000.f;
	  maximumWeight[i] = 0.f;
  }
  minimumX = minimumY = minimumZ = 32000.f;
  maximumX = maximumY = maximumZ = 0.f;


}

void iaea_header_type::update_counters(iaea_record_type *p_iaea_record)
{
  if (p_iaea_record->x > maximumX )  maximumX = p_iaea_record->x;
  if (p_iaea_record->x < minimumX )  minimumX = p_iaea_record->x;

  if (p_iaea_record->y > maximumY )  maximumY = p_iaea_record->y;
  if (p_iaea_record->y < minimumY )  minimumY = p_iaea_record->y;

  if (p_iaea_record->z > maximumZ )  maximumZ = p_iaea_record->z;
  if (p_iaea_record->z < minimumZ )  minimumZ = p_iaea_record->z;

	
  nParticles++;
  if( p_iaea_record->particle == 1 ) 
	  {
		  nPhotons++;
		  sumParticleWeight[0] +=  p_iaea_record->weight;
		  averageKineticEnergy[0] += p_iaea_record->weight*
			                         fabs(p_iaea_record->energy);
          if (p_iaea_record->weight > maximumWeight[0] )  
	          maximumWeight[0] = p_iaea_record->weight;
          if (p_iaea_record->weight < minimumWeight[0] )  
	          minimumWeight[0] = p_iaea_record->weight;

          if (fabs(p_iaea_record->energy) > maximumKineticEnergy[0] )  
             maximumKineticEnergy[0] = fabs(p_iaea_record->energy);
          if (fabs(p_iaea_record->energy) < minimumKineticEnergy[0] )  
             minimumKineticEnergy[0] = fabs(p_iaea_record->energy);
	  }
  if( p_iaea_record->particle == 2 ) 
	  {
		  nElectrons++; 
		  sumParticleWeight[1] +=  p_iaea_record->weight;
		  averageKineticEnergy[1] += p_iaea_record->weight*
			                         fabs(p_iaea_record->energy);
          if (p_iaea_record->weight > maximumWeight[1] )  
	          maximumWeight[1] = p_iaea_record->weight;
          if (p_iaea_record->weight < minimumWeight[1] )  
	          minimumWeight[1] = p_iaea_record->weight;

          if (fabs(p_iaea_record->energy) > maximumKineticEnergy[1] )  
             maximumKineticEnergy[1] = fabs(p_iaea_record->energy);
          if (fabs(p_iaea_record->energy) < minimumKineticEnergy[1] )  
             minimumKineticEnergy[1] = fabs(p_iaea_record->energy);
	  }
  if( p_iaea_record->particle == 3 ) 
	  {
		  nPositrons++; 
		  sumParticleWeight[2] +=  p_iaea_record->weight;
		  averageKineticEnergy[2] += p_iaea_record->weight*
			                         fabs(p_iaea_record->energy);
          if (p_iaea_record->weight > maximumWeight[2] )  
	          maximumWeight[2] = p_iaea_record->weight;
          if (p_iaea_record->weight < minimumWeight[2] )  
	          minimumWeight[2] = p_iaea_record->weight;

          if (fabs(p_iaea_record->energy) > maximumKineticEnergy[2] )  
             maximumKineticEnergy[2] = fabs(p_iaea_record->energy);
          if (fabs(p_iaea_record->energy) < minimumKineticEnergy[2] )  
             minimumKineticEnergy[2] = fabs(p_iaea_record->energy);

	  }
  if( p_iaea_record->particle == 4 ) 
	  {
		  nNeutrons++ ; 
		  sumParticleWeight[3] +=  p_iaea_record->weight;
		  averageKineticEnergy[3] += p_iaea_record->weight*
			                         fabs(p_iaea_record->energy);
          if (p_iaea_record->weight > maximumWeight[3] )  
	          maximumWeight[3] = p_iaea_record->weight;
          if (p_iaea_record->weight < minimumWeight[3] )  
	          minimumWeight[3] = p_iaea_record->weight;

          if (fabs(p_iaea_record->energy) > maximumKineticEnergy[3] )  
             maximumKineticEnergy[3] = fabs(p_iaea_record->energy);
          if (fabs(p_iaea_record->energy) < minimumKineticEnergy[3] )  
             minimumKineticEnergy[3] = fabs(p_iaea_record->energy);

	  }
  if( p_iaea_record->particle == 5 ) 
	  {
		  nProtons++  ; 
		  sumParticleWeight[4] +=  p_iaea_record->weight;
		  averageKineticEnergy[4] += p_iaea_record->weight*
			                         fabs(p_iaea_record->energy);
          if (p_iaea_record->weight > maximumWeight[4] )  
	          maximumWeight[4] = p_iaea_record->weight;
          if (p_iaea_record->weight < minimumWeight[4] )  
	          minimumWeight[4] = p_iaea_record->weight;

          if (fabs(p_iaea_record->energy) > maximumKineticEnergy[4] )  
             maximumKineticEnergy[4] = fabs(p_iaea_record->energy);
          if (fabs(p_iaea_record->energy) < minimumKineticEnergy[4] )  
             minimumKineticEnergy[4] = fabs(p_iaea_record->energy);
	  }

}

void iaea_header_type::print_statistics()
{
   printf("\n *************************************** \n");
   printf("           IAEA PHSP STATISTICS          \n");
   printf(" *************************************** \n");
   printf("\n Original number of histories: %d\n",orig_histories);
   printf(" Total number of particles: %d\n\n",nParticles);

   if(nPhotons>0) 
   {
	   printf(" PHOTONS: %d\n",nPhotons);
	   printf("  <E> = %7.2f,",averageKineticEnergy[0]);
	   printf(" Min.KE = %8.5f, Max.KE = %7.2f\n",minimumKineticEnergy[0],maximumKineticEnergy[0]); 
	   printf("  Total Weight = %15.1f,",sumParticleWeight[0]);
   	   printf(" Min.Weight = %7.5f, Max.Weight = %7.5f\n",minimumWeight[0],maximumWeight[0]); 
	   printf("\n");
   }
   if(nElectrons>0) 
   {
	   printf(" ELECTRONS: %d\n",nElectrons);
  	   printf("  <E> = %7.2f,",averageKineticEnergy[1]);
	   printf(" Min.KE = %8.5f, Max.KE = %7.2f\n",minimumKineticEnergy[1],maximumKineticEnergy[1]); 
	   printf("  Total Weight = %15.1f,",sumParticleWeight[1]);
   	   printf(" Min.Weight = %7.5f, Max.Weight = %7.5f\n",minimumWeight[1],maximumWeight[1]); 
   	   printf("\n");
   }
   if(nPositrons>0) 
   {
  	   printf(" POSITRONS: %d\n",nPositrons);
       printf("  <E> = %7.2f,",averageKineticEnergy[2]);
	   printf(" Min.KE = %8.5f, Max.KE = %7.2f\n",minimumKineticEnergy[2],maximumKineticEnergy[2]); 
	   printf("  Total Weight = %15.1f,",sumParticleWeight[2]);
   	   printf(" Min.Weight = %7.5f, Max.Weight = %7.5f\n",minimumWeight[2],maximumWeight[2]); 
   	   printf("\n");
   }
   if(nNeutrons>0) 
   {
	   printf(" NEUTRONS: %d\n",nNeutrons);
       printf("  <E> = %7.2f,",averageKineticEnergy[3]);
	   printf(" Min.KE = %8.5f, Max.KE = %7.2f\n",minimumKineticEnergy[3],maximumKineticEnergy[3]); 
	   printf("  Total Weight = %15.1f,",sumParticleWeight[3]);
   	   printf(" Min.Weight = %7.5f, Max.Weight = %7.5f\n",minimumWeight[3],maximumWeight[3]); 
   	   printf("\n");
   }
   if(nProtons>0) 
   {
	   printf(" PROTONS: %d \t\t: Sum of Weights %15.4f",nProtons,sumParticleWeight[4]);
	   printf("  <E> = %7.2f,",averageKineticEnergy[4]);
	   printf(" Min.KE = %8.5f, Max.KE = %7.2f\n",minimumKineticEnergy[4],maximumKineticEnergy[4]); 
	   printf("  Total Weight = %15.1f,",sumParticleWeight[4]);
   	   printf(" Min.Weight = %7.5f, Max.Weight = %7.5f\n",minimumWeight[4],maximumWeight[4]); 
   	   printf("\n");
   }
   printf(" GEOMETRY STATISTICS\n");
   if(record_contents[0] == 1) 
	   printf("  %7.3f < X coordinate < %7.3f\n",minimumX,maximumX);
   else printf("  X (constant) = %7.3f",record_constant[0]);
   if(record_contents[1] == 1) 
	   printf("  %7.3f < Y coordinate < %7.3f\n",minimumY,maximumY);
   else printf("  Y (constant) = %7.3f",record_constant[1]);
   if(record_contents[2] == 1) 
	   printf("  %7.3f < Z coordinate < %7.3f\n",minimumZ,maximumZ);
   else printf("  Z (constant) = %7.3f",record_constant[2]);
   printf("\n *************************************** \n\n");
}

int iaea_header_type::check_byte_order()
{
  /* Determine the byte order on this machine */
  float ftest=1.0f; /* assign a float to 1.0 */
  char *pf = (char *) &ftest;
  // printf("\n \t %x %x %x %x", pf[0],pf[1],pf[2],pf[3]);
  if(pf[0] == 0 && pf[3] != 0) 
  { 
	printf("\n\n Byte order: INTEL / ALPHA,LINUX -> LITLE_ENDIAN \n");
    return(LITTLE_ENDIAN);
  }else if(pf[0] != 0 && pf[3] == 0) 
  { 
    printf("\n\n Byte order: OTHER (SGI,SUN-SOLARIS) -> BIG_ENDIAN \n ");
    return(BIG_ENDIAN);
  } 
  else
  {
        printf("\n\n ERROR: indeterminate byte order");
        printf("\n \t %x %x %x %x", pf[0],pf[1],pf[2],pf[3]);
        return(UNKNOWN_ENDIAN);
  }
}

int iaea_header_type::write_header()
{
  if(fheader==NULL)
  {
      printf("\n ERROR: Opening header file to write \n"); return(FAIL);
  }

  write_blockname("IAEA_INDEX"); 
  fprintf(fheader,"%i   // Test header\n\n",iaea_index);
 
  write_blockname("TITLE");fprintf(fheader,"%s \n\n",title);
 
  write_blockname("FILE_TYPE");fprintf(fheader,"0\n\n"); // phasespace is assumed

  checksum = record_length * nParticles;

  write_blockname("CHECKSUM");fprintf(fheader,"%d \n\n",checksum); 
  
  write_blockname("RECORD_CONTENTS");
  fprintf(fheader,"   %2i     // X is stored ?\n",record_contents[0]);
  fprintf(fheader,"   %2i     // Y is stored ?\n",record_contents[1]);
  fprintf(fheader,"   %2i     // Z is stored ?\n",record_contents[2]);

  fprintf(fheader,"   %2i     // U is stored ?\n",record_contents[3]);
  fprintf(fheader,"   %2i     // V is stored ?\n",record_contents[4]);
  fprintf(fheader,"   %2i     // W is stored ?\n",record_contents[5]);

  fprintf(fheader,"   %2i     // Weight is stored ?\n",record_contents[6]);
  fprintf(fheader,"   %2i     // Extra floats stored ?\n",record_contents[7]);
  fprintf(fheader,"   %2i     // Extra longs stored ?\n",record_contents[8]);

  fprintf(fheader,"\n");
  
  write_blockname("RECORD_CONSTANT");
  if(record_contents[0]==0)
    fprintf(fheader,"   %8.4f     // Constant X\n",record_constant[0]);
  if(record_contents[1]==0)
    fprintf(fheader,"   %8.4f     // Constant Y\n",record_constant[1]);
  if(record_contents[2]==0)
    fprintf(fheader,"   %8.4f     // Constant Z\n",record_constant[2]);
  if(record_contents[3]==0)
    fprintf(fheader,"   %8.5f     // Constant U\n",record_constant[3]);
  if(record_contents[4]==0)
    fprintf(fheader,"   %8.5f     // Constant V\n",record_constant[4]);
  if(record_contents[5]==0)
    fprintf(fheader,"   %8.5f     // Constant W\n",record_constant[5]);
  if(record_contents[6]==0)
    fprintf(fheader,"   %8.4f     // Constant Weight\n",record_constant[6]);

  fprintf(fheader,"\n");
  
  write_blockname("RECORD_LENGTH");fprintf(fheader,"%i\n\n",record_length);

  int byte_order = check_byte_order();
  write_blockname("BYTE_ORDER");fprintf(fheader,"%i\n\n",byte_order);

  write_blockname("ORIG_HISTORIES");fprintf(fheader,"%d\n\n",orig_histories);

  write_blockname("PARTICLES");fprintf(fheader,"%d\n\n",nParticles);

  if(nPhotons>0) { 
	  write_blockname("PHOTONS");fprintf(fheader,"%d\n\n",nPhotons);}
  if(nElectrons>0) {
	  write_blockname("ELECTRONS");fprintf(fheader,"%d\n\n",nElectrons);}
  if(nPositrons>0) {
	  write_blockname("POSITRONS");fprintf(fheader,"%d\n\n",nPositrons);}
  if(nNeutrons>0) {
	  write_blockname("NEUTRONS");fprintf(fheader,"%d\n\n",nNeutrons);}
  if(nProtons>0) {
	  write_blockname("PROTONS");  fprintf(fheader,"%d\n\n",nProtons);}

  write_blockname("TRANSPORT_PARAMETERS"); fprintf(fheader,"\n");

  // 3. Mandatory additional information
  write_blockname("MACHINE_TYPE");  fprintf(fheader,"\n");

  write_blockname("MONTE_CARLO_CODE_VERSION"); fprintf(fheader,"\n");

  write_blockname("GLOBAL_PHOTON_ENERGY_CUTOFF"); fprintf(fheader,"0.001\n");

  write_blockname("GLOBAL_PARTICLE_ENERGY_CUTOFF"); fprintf(fheader,"0.100\n");

  write_blockname("COORDINATE_SYSTEM_DESCRIPTION"); fprintf(fheader,"\n");
  
  // 4. Optional information
  fprintf(fheader,"//  OPTIONAL INFORMATION\n\n");

  write_blockname("BEAM_NAME"); fprintf(fheader,"\n");

  write_blockname("FIELD_SIZE"); fprintf(fheader,"\n");

  write_blockname("NOMINAL_SSD"); fprintf(fheader,"\n");

  write_blockname("MC_INPUT_FILENAME"); fprintf(fheader,"\n");

  write_blockname("VARIANCE_REDUCTION_TECHNIQUES"); fprintf(fheader,"\n");

  write_blockname("INITIAL_SOURCE_DESCRIPTION"); fprintf(fheader,"\n");

  write_blockname("PUBLISHED_REFERENCE"); fprintf(fheader,"\n");

  write_blockname("AUTHORS"); fprintf(fheader,"\n");

  write_blockname("INSTITUTION"); fprintf(fheader,"\n");

  write_blockname("LINK_VALIDATION"); fprintf(fheader,"\n");

  write_blockname("ADDITIONAL_NOTES");
  fprintf(fheader,"%s\n","This is IAEA header as defined in the technical "); 	
  fprintf(fheader,"%s\n","report IAEA(NDS)-0484, Vienna, 2006"); 	

  fprintf(fheader,"\n");
  // 5. Statistical information
  write_blockname("STATISTICAL_INFORMATION_PARTICLES");
  fprintf(fheader,"//    Weight     Wmin    Wmax       <E>       Emin        Emax    Particle\n"); 
  float eaver; char buffer[15];
  for(int i=0;i<MAX_NUM_PARTICLES;i++) 
  { 
	  switch (i) 
	  {
		  case 0:
			  strcpy(buffer," PHOTONS");
			  break;
		  case 1:
			  strcpy(buffer," ELECTRONS");
			  break;
		  case 2:
			  strcpy(buffer," POSITRONS");
			  break;
		  case 3:
			  strcpy(buffer," NEUTRONS");
			  break;
		  case 4:
			  strcpy(buffer," PROTONS");
			  break;
	  }

	  if( (i == 0) && (nPhotons == 0 ) ) continue;
	  if( (i == 1) && (nElectrons == 0 ) ) continue;
	  if( (i == 2) && (nPositrons == 0 ) ) continue;
	  if( (i == 3) && (nNeutrons == 0 ) ) continue;
	  if( (i == 4) && (nProtons == 0 ) ) continue;

	  eaver = 0.f;
	  if(sumParticleWeight[i]>0) { 
		  averageKineticEnergy[i] /= sumParticleWeight[i];
	      eaver = averageKineticEnergy[i];
	  }

	  fprintf(fheader,"  %12.2f %6.5f %6.2f %10.2f    %6.5f  %10.2f  %s\n",
	  sumParticleWeight[i],minimumWeight[i],maximumWeight[i],
	  eaver,minimumKineticEnergy[i],maximumKineticEnergy[i],buffer);
  }
  fprintf(fheader,"\n");

  write_blockname("STATISTICAL_INFORMATION_GEOMETRY");
  if(record_contents[0] == 1) fprintf(fheader," %f  %f\n",minimumX,maximumX);
  if(record_contents[1] == 1) fprintf(fheader," %f  %f\n",minimumY,maximumY);
  if(record_contents[2] == 1) fprintf(fheader," %f  %f\n\n",minimumZ,maximumZ);

  return(OK);

}

int iaea_header_type::print_header ()
{
	printf("IAEA_INDEX: %i\n",iaea_index); 
	printf("TITLE: %s \n",title);

  // ******************************************************************************
  // 1. PHSP format

	/*********************************************/
	if(file_type == 0) printf("FILE TYPE: PHASESPACE \n");
	if(file_type == 1) printf("FILE TYPE: GENERATOR \n");

	printf("CHECKSUM: %d\n",checksum);
    printf("RECORD LENGTH: %i\n",record_length);
    printf("BYTE_ORDER: %i\n",byte_order);

	int i;
    printf("\nRECORD_CONTENTS:\n");
    for (i=0;i<7;i++)
    {
		if(record_contents[i] == 0)
 	      printf(" // Variable %1i is constant\n",i+1);
	}
  
    if(record_contents[7] > 0)
	  printf(" // %1i extra FLOAT variable(s) defined\n",record_contents[7]);
    if(record_contents[8] > 0)
	  printf(" // %1i extra LONG variable(s) defined\n",record_contents[8]);
  
	/*********************************************/
    printf("\nRECORD_CONSTANT:\n");
    for (i=0;i<7;i++)
    {
	  if(record_contents[i] > 0) continue;
	  printf(" %8.4f // Constant variable # %1i\n",record_constant[i],i+1);
    }

// ******************************************************************************
// 2. Mandatory description of the phsp

	printf("\nCOORDINATE_SYSTEM_DESCRIPTION: \n%s\n",
		coordinate_system_description);

	if(file_type == 1) // For event generators
	    printf("INPUT FILE for event generator: %d \n",input_file_for_event_generator);
    if(file_type == 0) // for phsp files
    {
		printf("ORIG_HISTORIES: %d \n",orig_histories);
		printf("PARTICLES: %d \n",nParticles);
		if(nPhotons   > 0) printf("PHOTONS: %d \n",nPhotons);
		if(nElectrons > 0) printf("ELECTRONS: %d \n",nElectrons);
		if(nPositrons > 0) printf("POSITRONS: %d \n",nPositrons);
		if(nNeutrons  > 0) printf("NEUTRONS: %d \n",nNeutrons);
		if(nProtons   > 0) printf("PROTONS: %d \n",nProtons);
	}

// ******************************************************************************
// 3. Mandatory additional information
	/*********************************************/
	printf("MACHINE_TYPE: %s\n",machine_type);
 
	printf("MONTE_CARLO_CODE_VERSION: %s \n",MC_code_and_version);

	printf("GLOBAL_PHOTON_ENERGY_CUTOFF: %8.5f \n",global_photon_energy_cutoff);
	printf("GLOBAL_PARTICLE_ENERGY_CUTOFF: %8.5f \n",global_particle_energy_cutoff);
	printf("\nTRANSPORT_PARAMETERS:\n%s\n",transport_parameters);

// ******************************************************************************
// 4. Optional description

	printf("BEAM_NAME: %s\n",beam_name);
	printf("FIELD_SIZE: %s\n",field_size);
	printf("NOMINAL_SSD: %s\n",nominal_SSD);
	printf("VARIANCE_REDUCTION_TECHNIQUES:\n%s\n",variance_reduction_techniques);
	printf("INITIAL_SOURCE_DESCRIPTION: \n%s\n",initial_source_description);
  
	// Documentation sub-section
	/*********************************************/
	printf("MC_INPUT_FILENAME: %s\n",MC_input_filename);
	printf("PUBLISHED_REFERENCE: \n%s\n",published_reference);
	printf("AUTHORS: \n%s\n",authors);
	printf("INSTITUTION: \n%s\n",institution);
	printf("LINK_VALIDATION: \n%s\n",link_validation);
	printf("ADDITIONAL_NOTES: \n%s\n",additional_notes);

// ******************************************************************************
// 5. Optional statistical information

	print_statistics();

    return(OK);  
}
