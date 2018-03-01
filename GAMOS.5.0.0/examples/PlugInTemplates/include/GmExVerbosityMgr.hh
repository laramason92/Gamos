#ifndef GmExVerbosityMgr_HH
#define GmExVerbosityMgr_HH

#include "GamosCore/GamosUtils/include/GmVerbosity.hh"
#include "GamosCore/GamosFactories/include/GmVVerbosityMgr.hh"

class GmExVerbosityMgr : public GmVVerbosityMgr
{
public:
  GmExVerbosityMgr();
  virtual ~GmExVerbosityMgr(){};

  virtual void SetFilterLevel( int fl );
  virtual int GetFilterLevel() const;

};

extern GmVerbosity GmExVerb;

#endif

