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

#include "TMath.h"

//------------------------------------------------------------------------------

void Jet_Delphes(const char *inputFile)
{
  gSystem->Load("libDelphes");

  // Create chain of root trees
  TChain chain("Delphes");
  chain.Add(inputFile);

  // Create object of class ExRootTreeReader
  ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
  Long64_t numberOfEntries = treeReader->GetEntries();

  TClonesArray *branchJet = treeReader->UseBranch("Jet");
        
  // Book histograms
  TH1 *histPT   = new TH1F("pt", "PT", 60, 0.0, 300.0);
  TH1 *histEta  = new TH1F("eta", "Eta", 60, -4.0, 4.0);
  TH1 *histPhi  = new TH1F("phi", "Phi", 60, -3.14, 3.14);
  TH1 *histMass = new TH1F("mass", "Mass", 60, 0.0, 40.0);
  
  // Loop over all events
  for(Int_t entry = 0; entry < numberOfEntries; ++entry)
  {
    // Load selected branches with data from specified event
    treeReader->ReadEntry(entry);

    Jet *jet;
    
    // Check if jets are sorted in decreasing order 
    /*
    for(Int_t k=0; k < branchJet->GetEntries(); ++k)
    {
      jet1 = (Jet *) branchJet -> At(k);
      cout << "Entry: " << entry << "\tJet PT: " << (jet1 -> PT) << endl;
    }
    */
    // YES they are!

    // If event contains at least 2 jets and 1 muon
    jet = (Jet *) branchJet -> At(0);

    // Plot jets invariant mass, muon PT, and MET.
    histPT  -> Fill(jet -> PT);
    histEta -> Fill(jet -> Eta);
    histPhi -> Fill(jet -> Phi);
    histMass-> Fill(jet -> Mass);
  }

  TCanvas *c1 = new TCanvas("c1", "c1");
  histMass-> GetXaxis()-> SetTitle("Mass [GeV]");
  histMass-> GetYaxis()-> SetTitle("Entries");
  histMass-> GetXaxis()-> SetTitleSize(0.05);
  histMass-> GetYaxis()-> SetTitleSize(0.05);  
  histMass-> Draw();
  c1 -> SaveAs("01_mass.png");

  TCanvas *c2 = new TCanvas("c2", "c2");
  histPT-> GetXaxis()-> SetTitle("PT [GeV]");
  histPT-> GetYaxis()-> SetTitle("Entries");
  histPT-> GetXaxis()-> SetTitleSize(0.05);
  histPT-> GetYaxis()-> SetTitleSize(0.05);  
  histPT-> Draw();
  c2 -> SaveAs("02_pt.png");
  
  TCanvas *c3 = new TCanvas("c3", "c3");
  histEta-> GetXaxis()-> SetTitle("Eta [GeV]");
  histEta-> GetYaxis()-> SetTitle("Entries");
  histEta-> GetXaxis()-> SetTitleSize(0.05);
  histEta-> GetYaxis()-> SetTitleSize(0.05);  
  histEta-> Draw();
  c3 -> SaveAs("03_eta.png");
  
  TCanvas *c4 = new TCanvas("c4", "c4");
  histPhi-> GetXaxis()-> SetTitle("Phi [GeV]");
  histPhi-> GetYaxis()-> SetTitle("Entries");
  histPhi-> GetXaxis()-> SetTitleSize(0.05);
  histPhi-> GetYaxis()-> SetTitleSize(0.05);  
  histPhi-> Draw();
  c4 -> SaveAs("04_phi.png");
}


