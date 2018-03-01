#if !defined(__CINT__) || defined(__MAKECINT__)

#include "./compare2.C"
#endif

#include "TFile.h"
#include "TKey.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TPad.h"
#include "TCanvas.h"
#include <iostream>
#include <TStyle.h>
#include "TMarker.h"
#include "TLine.h"
#include "TText.h"

//------------------------------------------------------------------------
void compareAll(const char* filename1, const char* filename2, bool bnorm = 0, bool blogy = 0 ) 
{
  
  theSampleTitle.push_back((char *)("HISTOS1"));
  theSampleTitle.push_back((char *)("HISTOS2"));
 
  InitComparison(filename1, filename2);
  theOptLogy = blogy;

  theGlobalTitle = (char *)("");
  
  TIter ListOfHistograms( theFiles[0]->GetListOfKeys() );
  TKey *HistoKey;

  TH1F *histo1;
  
  int nh = 0;

  while ( (HistoKey = (TKey*)ListOfHistograms()) ) { 
    nh++;
    std::string hisname = HistoKey->GetName();
    //ONLY HISTOGRAMS THAT CONTAIN 'NAME'     if( hisname.find("NAME") == std::string::npos ) continue;
    //ONLY HISTOGRAMS THAT DO NOT CONTAIN 'NAME'     if( hisname.find("NAME") != std::string::npos ) continue;

    TH1* histo = (TH1*)theFiles[0]->Get( HistoKey->GetName() );
    if( histo->GetDimension() == 1 ){
      histo1 = (TH1F*) theFiles[0]->Get( HistoKey->GetName() );
      std::cout << " comparing " << histo->GetName() << std::endl;
      compare2His(histo->GetName(),histo->GetName(),"N entries",histo->GetName(), bnorm);
    }
  }
  
}

