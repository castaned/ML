/*
Simple macro showing how to access branches from the delphes output root file,
loop over events, and plot simple quantities such as the jet pt and the di-electron invariant
mass.

root -l examples/Example1.C'("delphes_output.root")'
*/

#ifdef __CLING__
R__LOAD_LIBRARY(libDelphes.so)
#include "classes/DelphesClasses.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"
#endif

//------------------------------------------------------------------------------

void ZtoEE_Delphes(const char *inputFile)
{
  gSystem->Load("libDelphes");

  // Create chain of root trees
  TChain chain("Delphes");
  chain.Add(inputFile);

  // Create object of class ExRootTreeReader
  ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
  Long64_t numberOfEntries = treeReader->GetEntries();

  TClonesArray *branchElectron = treeReader->UseBranch("Electron");
    
  // Book histograms
  TH1 *histMass = new TH1F("mass", "Invariant Mass (e_{1}e_{2})", 100, 40.0, 140.0);
  TH1 *histPT1 = new TH1F("pt1", "e_{1}", 80, 0.0, 100.0);
  TH1 *histPT2 = new TH1F("pt2", "e_{2}", 80, 0.0, 70.0);

  // Loop over all events
  for(Int_t entry = 0; entry < numberOfEntries; ++entry)
  {
    // Load selected branches with data from specified event
    treeReader->ReadEntry(entry);

    Electron *e1, *e2;

    // If event contains at least 2 muons
    if(branchElectron->GetEntries() > 1)
    {
      // Take first two electrons
      e1 = (Electron *) branchElectron->At(0);
      e2 = (Electron  *) branchElectron->At(1);

      // Plot their invariant mass
      histMass->Fill(((e1->P4()) + (e2->P4())).M());
      histPT1 -> Fill(e1 -> PT);
      histPT2 -> Fill(e2 -> PT);
    }


  }

  // Show resulting histograms
  TFile *f = new TFile("mass.root", "recreate");
  TCanvas *c1 = new TCanvas("c1", "c1");
  histMass-> GetXaxis()-> SetTitle("Mass [GeV]");
  histMass-> GetYaxis()-> SetTitle("Entries");
  histMass-> GetXaxis()-> SetTitleSize(0.05);
  histMass-> GetYaxis()-> SetTitleSize(0.05);  
  histMass->Draw();
  histMass -> Write();
  f -> Close();
  c1 -> SaveAs("mass.png");
  
  TCanvas *c2 = new TCanvas("c3", "c3");
  histPT1-> GetXaxis()->SetTitle("PT [GeV]");
  histPT1-> GetYaxis()->SetTitle("Entries");
  histPT1-> GetXaxis()-> SetTitleSize(0.05);
  histPT1-> GetYaxis()-> SetTitleSize(0.05);  
  histPT1 ->Draw();
  c2 -> SaveAs("pt1.png");
  
  TCanvas *c3 = new TCanvas("c3", "c3");
  histPT2-> GetXaxis()->SetTitle("PT [GeV]");
  histPT2-> GetYaxis()->SetTitle("Entries");
  histPT2-> GetXaxis()-> SetTitleSize(0.05);
  histPT2-> GetYaxis()-> SetTitleSize(0.05);  
  histPT2 ->Draw();
  c3 -> SaveAs("pt2.png");
  

}

