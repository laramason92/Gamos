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
#ifndef __EGSPhspParticle_H_
#define __EGSPhspParticle_H_

#include <string>

#define SEGMENT_BEG_TOKEN '$'
#define SEGMENT_END_TOKEN ':'
#define OK     0
#define FAIL  -1
#define MAX_NPART 5 // 1 photons
                    // 2 electrons
                    // 3 positrons
                    // 4 neutrons
                    // 5 protons

class EGSPhspParticle {
public:
  EGSPhspParticle(FILE* file, std::string mode);
  ~EGSPhspParticle();
  long theParticleNumber;
  unsigned int theLatch;
  float theCharge;
  float theEnergy;
  float thePosX, thePosY, thePosZ;
  float theDirU, theDirV, theDirW;
  float theWeight;
  int theNPass;
  // char muse;
  long theFilePointer;
  //  int eof;
  //  vector<long> theLongs;
  //  vector<float> theFloats;
  
  //  int rc_x, rc_y, rc_z, rc_u, rc_v, rc_w, rc_wght, rc_flts, rc_lngs;
  int CheckIntegrity(int i);
  int RefreshParticle(FILE* file, long number);
  //  void write_EGSPhspParticle(FILE* outphspfile, long number);
  float theXmin, theXmax, theYmin, theYmax;
  float theDeltaX, theTotalX, theDeltaY, theTotalY;

private:
};

#endif
