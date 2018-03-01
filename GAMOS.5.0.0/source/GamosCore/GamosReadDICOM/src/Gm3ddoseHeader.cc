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
#include "Gm3ddoseHeader.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//-----------------------------------------------------------------------
G4bool Gm3ddoseHeader::operator==(const Gm3ddoseHeader& right) const
{
  G4bool equ = 1;
  if( fNoVoxelX != right.GetNoVoxelX() ||
    fNoVoxelX != right.GetNoVoxelX() ||
    fNoVoxelX != right.GetNoVoxelX() ||
    fVoxelLimitsX != right.GetVoxelLimitsX() ||
    fVoxelLimitsX != right.GetVoxelLimitsX() ||
    fVoxelLimitsX != right.GetVoxelLimitsX() ) {
    equ = 0;
  }
  return equ;

}


//-----------------------------------------------------------------------
G4bool Gm3ddoseHeader::operator!=(const Gm3ddoseHeader& right) const
{ 
  return !( *this == right );
}

//-----------------------------------------------------------------------
Gm3ddoseHeader::Gm3ddoseHeader(const GmSqdoseHeader& sqdh)
{
  //  G4cout << " reading header file " << G4endl;
  fNoEvent = sqdh.GetNumberOfEvents();
  G4cout << " N events " << fNoEvent << G4endl;

  fNoVoxelX = sqdh.GetNoVoxelX();
  fNoVoxelY = sqdh.GetNoVoxelY();
  fNoVoxelZ = sqdh.GetNoVoxelZ();

  fVoxelLimitsX = sqdh.GetVoxelLimitsX();
  fVoxelLimitsY = sqdh.GetVoxelLimitsY();
  fVoxelLimitsZ = sqdh.GetVoxelLimitsZ();

}

//-----------------------------------------------------------------------
void Gm3ddoseHeader::Read( GmFileIn& fin )
{
  //  G4cout << " reading header file " << G4endl;
  std::vector<G4String> wl;
  fin.GetWordsInLine(wl);
  if( wl.size() != 1 ){
    G4Exception("Gm3ddoseHeader::Read"," first line must have 1 words: number of events",FatalErrorInArgument,G4String("It has "+GmGenUtils::itoa(wl.size())).c_str());
  }
  fNoEvent = (unsigned long long)(GmGenUtils::GetValue( wl[0] ));
  G4cout << " N events " << fNoEvent << G4endl;

  fin.GetWordsInLine(wl);
  if( wl.size() != 3 ){
    G4Exception("Gm3ddoseHeader::Read"," second line must have 3 words: number of voxels",FatalErrorInArgument,G4String("It has "+GmGenUtils::itoa(wl.size())).c_str());
  }

  fNoVoxelX = GmGenUtils::GetInteger( wl[0] );
  fNoVoxelY = GmGenUtils::GetInteger( wl[1] );
  fNoVoxelZ = GmGenUtils::GetInteger( wl[2] );

  fin.GetWordsInLine(wl);
  if( wl.size() != fNoVoxelX+1 ){
    G4Exception("Gm3ddoseHeader::Read",G4String(" third line must have number of words = " + GmGenUtils::itoa(fNoVoxelX)).c_str(),FatalErrorInArgument,G4String("It has "+GmGenUtils::itoa(wl.size())).c_str());
  }
  for( size_t ii = 0; ii < fNoVoxelX+1; ii++ ){    
    fVoxelLimitsX.push_back( GmGenUtils::GetValue(wl[ii]) );
  }

  fin.GetWordsInLine(wl);
  if( wl.size() != fNoVoxelY+1 ){
    G4Exception("Gm3ddoseHeader::Read",G4String(" fourth line must have number of words = " + GmGenUtils::itoa(fNoVoxelY)).c_str(),FatalErrorInArgument,G4String("It has "+GmGenUtils::itoa(wl.size())).c_str());
  } 
  for( size_t ii = 0; ii < fNoVoxelY+1; ii++ ){    
    fVoxelLimitsY.push_back( GmGenUtils::GetValue(wl[ii]) );
  }

  fin.GetWordsInLine(wl);
  if( wl.size() != fNoVoxelZ+1 ){
    G4Exception("Gm3ddoseHeader::Read",G4String(" fifth line must have number of words = " + GmGenUtils::itoa(fNoVoxelZ)).c_str(),FatalErrorInArgument,G4String("It has "+GmGenUtils::itoa(wl.size())).c_str());
  }
  for( size_t ii = 0; ii < fNoVoxelZ+1; ii++ ){    
    fVoxelLimitsZ.push_back( GmGenUtils::GetValue(wl[ii]) );
  }

}


//-----------------------------------------------------------------------
std::ostream& operator<<(std::ostream &out, 
			 const Gm3ddoseHeader &dh)
{

  out << dh.GetNumberOfEvents() << G4endl;
  //  G4cout << "DOSE N EV " << dh.GetNumberOfEvents() << G4endl;

  out << dh.GetNoVoxelX() << " " << dh.GetNoVoxelY() << " " << dh.GetNoVoxelZ() << G4endl;
  std::vector<float>::const_iterator ite;

  std::vector<float> vlim = dh.GetVoxelLimitsX();
  //  out << " size x " << dh.GetVoxelLimitsX().size() << G4endl;
  for( ite = vlim.begin(); ite != vlim.end(); ite++ ){
    out << *ite << " ";
  }
  out << G4endl;
 
  vlim = dh.GetVoxelLimitsY();
  for( ite = vlim.begin(); ite != vlim.end(); ite++ ){
    out << *ite << " ";
  }
  out << G4endl;
  
  vlim = dh.GetVoxelLimitsZ();
  for( ite = vlim.begin(); ite != vlim.end(); ite++ ){
    out << *ite << " ";
  }
  out << G4endl;

  return out;

}
