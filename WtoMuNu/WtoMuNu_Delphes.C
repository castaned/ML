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

void WtoMuNu_Delphes(const char *inputFile)
{
  gSystem->Load("libDelphes");

  // Create chain of root trees
  TChain chain("Delphes");
  chain.Add(inputFile);

  // Create object of class ExRootTreeReader
  ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
  Long64_t numberOfEntries = treeReader->GetEntries();

  TClonesArray *branchMuon = treeReader->UseBranch("Muon");
  TClonesArray *branchJet = treeReader->UseBranch("Jet");
  TClonesArray *branchMET = treeReader->UseBranch("MissingET");
      
  // Book histograms
  TH1 *histMass = new TH1F("mass", "Invariant Mass of Jets", 80, 0.0, 1000.0);
  TH1 *histPTmuon = new TH1F("pt", "Muon PT", 60, 0.0, 225.0);
  TH1 *histMissET = new TH1F("miss", "Missing Energy", 40, 0.0, 160.0);
  
  // Loop over all events
  for(Int_t entry = 0; entry < numberOfEntries; ++entry)
  {
    // Load selected branches with data from specified event
    treeReader->ReadEntry(entry);

    Muon *mu;
    Jet *jet1, *jet2;
    MissingET *nu;
    
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
    if(branchMuon->GetEntries() == 1 && branchJet->GetEntries() > 1)
    {
      mu = (Muon *) branchMuon -> At(0);
      jet1 = (Jet *) branchJet -> At(0);
      jet2 = (Jet *) branchJet -> At(1);
      nu = (MissingET *) branchMET -> At(0);

      // Plot jets invariant mass, muon PT, and MET.
      histMass-> Fill(((jet1->P4()) + (jet2->P4())).M());
      histPTmuon -> Fill(mu -> PT);
      histMissET -> Fill(nu -> MET);
      
      if(mu->PT > 100 && nu->MET > 60 && ((jet1->P4()) + (jet2->P4())).M() > 80)
      {
        cout << "Entry: " << entry << endl;
      }
    }
  }

  TCanvas *c1 = new TCanvas("c1", "c1");
  histMass-> GetXaxis()-> SetTitle("Mass [GeV]");
  histMass-> GetYaxis()-> SetTitle("Entries");
  histMass-> GetXaxis()-> SetTitleSize(0.05);
  histMass-> GetYaxis()-> SetTitleSize(0.05);  
  histMass-> Draw();
  c1 -> SaveAs("01_mass.png");

  TCanvas *c2 = new TCanvas("c2", "c2");
  histPTmuon-> GetXaxis()-> SetTitle("PT [GeV]");
  histPTmuon-> GetYaxis()-> SetTitle("Entries");
  histPTmuon-> GetXaxis()-> SetTitleSize(0.05);
  histPTmuon-> GetYaxis()-> SetTitleSize(0.05);  
  histPTmuon-> Draw();
  c2 -> SaveAs("02_pt.png");
  
  TCanvas *c3 = new TCanvas("c3", "c3");
  histMissET-> GetXaxis()-> SetTitle("Missing ET [GeV]");
  histMissET-> GetYaxis()-> SetTitle("Entries");
  histMissET-> GetXaxis()-> SetTitleSize(0.05);
  histMissET-> GetYaxis()-> SetTitleSize(0.05);  
  histMissET-> Draw();
  c3 -> SaveAs("03_met.png");  
}


