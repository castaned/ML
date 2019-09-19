void fitZboson(){

  TCanvas *c1 = new TCanvas("c1","");
  gStyle->SetOptStat(111111);
  TString File = "DoubleMu.root";
  TFile *f = new TFile(File.Data());
  TH1F *h = (TH1F*) f->Get("demo/Zboson2");
  //h->Draw();
  //c1->SaveAs("fitjpsi.pdf");
  // Create RooDataHist

  RooRealVar mass_peak("m0","m0",90, 60,120);
  RooRealVar width("width","width",10, 0,100);
  RooRealVar a0("a0","",0,1000);
  RooRealVar x("x","",40,140);
  x.setBins(1000);
  RooPolynomial pol5("pol5","",x,RooArgList(a0));//construct a polynomial
  RooRealVar coe_bw("coe_bw","",1,0,1);
  RooRealVar coe_pol("coe_pol","",.0,0,1);
  RooBreitWigner bw("bw","Breit-Wigner function fit",x,mass_peak,width);//construct a Breit-Wigner function

  RooDataHist *mass_data=new RooDataHist("Zmass","Z boson's mass",x,h);//convert data to roodatahist
  RooPlot *plot_frame=x.frame(RooFit::Title("signal invariant mass of muon+ muon- and photon"),RooFit::Bins(1000));
  RooAddPdf bwandpol("bwandpol","",bw,pol5,coe_bw);//construct function include background and peak
  bwandpol.fitTo(*mass_data);//fit the curve
  mass_data->plotOn(plot_frame);
  bwandpol.plotOn(plot_frame);
  plot_frame->Draw();

  
