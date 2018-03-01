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
#ifndef Gm3ddoseHeader__HH
#define Gm3ddoseHeader__HH

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include <vector>
#include <fstream>
#include "globals.hh"
class GmFileIn;
#include "GmSqdoseHeader.hh"

class Gm3ddoseHeader 
{
public:
  Gm3ddoseHeader(){};
  Gm3ddoseHeader(const GmSqdoseHeader& sqdh);
  ~Gm3ddoseHeader(){};

  G4bool operator==(const Gm3ddoseHeader& right) const;
  G4bool operator!=(const Gm3ddoseHeader& right) const;
  
  void Read( GmFileIn& fin);
  
  // Get and Set methods
  float GetNumberOfEvents() const {
    return fNoEvent; }
  
  size_t GetNoVoxelX() const {
    return fNoVoxelX; }
  size_t GetNoVoxelY() const {
    return fNoVoxelY; }
  size_t GetNoVoxelZ() const {
    return fNoVoxelZ; }
  std::vector<float> GetVoxelLimitsX() const {
    return fVoxelLimitsX; }
  std::vector<float> GetVoxelLimitsY() const {
    return fVoxelLimitsY; }
  std::vector<float> GetVoxelLimitsZ() const {
    return fVoxelLimitsZ; }

  void SetNumberOfEvents(float nev ) {
    fNoEvent = nev; }
  void SetNoVoxelX( size_t nv ) {
    fNoVoxelX = nv; }
  void SetNoVoxelY( size_t nv ) {
    fNoVoxelY = nv; }
  void SetNoVoxelZ( size_t nv ) {
    fNoVoxelZ = nv; }

  void SetVoxelLimitsX( std::vector<float> vl ) {
    fVoxelLimitsX = vl; }
  void SetVoxelLimitsY( std::vector<float> vl ) {
    fVoxelLimitsY = vl; }
  void SetVoxelLimitsZ( std::vector<float> vl ) {
    fVoxelLimitsZ = vl; }

private:
  float fNoEvent;
  size_t fNoVoxelX,fNoVoxelY,fNoVoxelZ;
  // Number of voxel in x, y and z dimensions.
  std::vector<float> fVoxelLimitsX, fVoxelLimitsY, fVoxelLimitsZ;
  // List of voxel limits
};

std::ostream& operator<<(std::ostream &out, 
                           const Gm3ddoseHeader &right);

#endif
