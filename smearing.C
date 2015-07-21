#include "TH1F.h"
#include "TBox.h"
#include <vector>
using namespace std;

const Int_t N = 100000;
vector<double> energy_spectrum;
TH1F *EnergyMC;
TH1F *QuenchEnergyMC;

int smearing(){  
  string Isotope ="alpha_v6";
  string Time="_Feb11PM";
  TFile *file = new TFile("/darkside/users/hqian/TMBfraction/results/tmb_alpha_v6_Feb11AM.root");
  //  QuenchEnergyMC = (TH1F*) file->Get("nv_alpha_qenergy_hist");
  //  EnergyMC = (TH1F*) file->Get("nv_alpha_energy_hist");
  QuenchEnergyMC = (TH1F*) file->Get("nv_thermal_long_eqch_hist");
  EnergyMC = (TH1F*) file->Get("nv_thermal_long_eqch_hist");
  //#define Start_Fit
#ifdef Start_Fit 
  const  Int_t npars = 6;
  //  TH1F *Ba133_Energy = new TH1F("Ba133_Energy","Ba133 Energy Spectrum ;Energy [PE];Counts",500,0,2000);
  EnergyMC->Rebin(5);
  QuenchEnergyMC->Rebin(2);
  TF1* Energy = new TF1("Energy",Single,0,500,npars);
  TF1* QuenchEnergy = new TF1("QuenchEnergy",Full,0,500,npars);
  Double_t kB               = 0.00835;
  Double_t baseline_mean    = 0.386;    
  Double_t ly_mean          = 0.55;      
  Double_t baseline_var     = 1.;
  Double_t spe_var          = 0.14;     
  Double_t ly_var           = 0.00642;  
  Double_t amplitude        = 0;
  Double_t binwidth         = QuenchEnergyMC->GetBinWidth(1);
  Double_t constant         = 1.;      
  //  Double_t params[npars]={baseline_mean,ly_mean,baseline_var,spe_var,ly_var,amplitude,binwidth,constant};
  Double_t params[npars]={kB,baseline_mean,ly_mean,baseline_var,spe_var,ly_var};
  Energy->SetParameters(params);
  Energy->SetNpx(3000);
  Energy->SetLineColor(kRed);
  Energy->SetTitle("Smearing Energy");
  QuenchEnergy->SetParameters(params);
  QuenchEnergy->SetNpx(3000);
  QuenchEnergy->GetXaxis()->SetTitle("Energy [P.E.]");
  QuenchEnergy->GetYaxis()->SetTitle("Counts");
  QuenchEnergy->SetTitle("Smearing Quenching Energy");

  /*  TLegend *legene = new TLegend(0.3,0.75,0.9,0.9);
  legene->AddEntry(QuenchEnergy,"Full Energy Spectrum after smearing","l");
  legene->AddEntry(Energy,"Single Electron Scattering Energy Spectrum after smearing","l");
  */
  TBox *box1 = new TBox(1000,0,2000,1400);
  box1->SetLineColor(4);
  box1->SetLineWidth(1);
  //  box1->SetFillColor(4);
  TArrow ar1(3000,1200,5000,1200,0.05,"<");
  ar1.SetLineColor(6);
  ar1.SetLineWidth(4);
  
  TCanvas *c1 = new TCanvas("c1","Energy Spectrum after smearing",800,400);
  c1->Divide(2,1);
  c1->cd(1);
  gPad->SetLogy();
  EnergyMC->Draw();
  c1->cd(2);
  Energy->Draw();
  box1->Draw();
  //  ar1.Draw();

  TCanvas *c2 = new TCanvas("c2","Quench Energy Spectrum after smearing",800,400);
  c2->Divide(2,1);
  c2->cd(1);
  gPad->SetLogy();
  QuenchEnergyMC->Draw();
  c2->cd(2);
  QuenchEnergy->Draw();

  /* TCanvas *c3 = new TCanvas("c3","Energy Spectrum from smearing",600,400);
  QuenchEnergy->Draw();
  Energy->Draw("same");
  legene->Draw();
  TPaveText *L=new TPaveText(0.5,0.45,0.95,0.75,"BRNDC");
  L->SetTextAlign(12);
  L->SetFillColor(15);
  //  L->SetTextSize(2);
  L->AddText("Parameters  Value");
  L->AddLine(0.0,0.85,1,0.85);
  L->AddText("Light Yield Mean=6.99 [P.E./keV]");
  L->AddText("SPE Var=-1");
  L->AddText("Light Yield Var=0");
  L->AddText("Baseline Mean=0 [PE]");
  L->AddText("Baseline Var=347 [PE^2]");
  //  L->Draw();
  */
  string outdir = "/darkside/users/hqian/TMBfraction/results/";
  string output = outdir + "Smearing_Energy_Spectrum_"+Isotope+Time+".root";
  TFile f(output.c_str(), "RECREATE");
  EnergyMC->Write();
  Energy->Write();
  QuenchEnergyMC->Write();
  QuenchEnergy->Write();
  c1->Write();
  c2->Write();
  // c3->Write();
  f.Write();
  f.Close();

#else  
  TFile *gammadata = new TFile("/home/hqian/montecarlo/g4ds10/Linux-g++/GammaCalibration/norm_gamma_hists.root");
  TH1F *Ba133_twopulses_s1;
  TH1F *Ba133_npulses_s1;
  Ba133_twopulses_s1 = (TH1F*) gammadata->Get("Run10025-10036_two_pulse_s1");
  Ba133_npulses_s1 = (TH1F*) gammadata->Get("Run10025-10036_n_pulse_s1");
  Ba133_twopulses_s1->Rebin();
  Ba133_npulses_s1->Rebin();
  Ba133_twopulses_s1->SetAxisRange(0,3500);
  Ba133_npulses_s1->SetAxisRange(0,3500);
  //  Ba133_twopulses_s1->Sumw2();
  //  Ba133_npulses_s1->Sumw2();
  Double_t HistIntegralsum = Ba133_twopulses_s1->Integral();
  Double_t start = 0;
  Double_t end = 3500;
  const Int_t npars = 8;
  Double_t new_par[npars];
  Double_t new_par_error[npars];
  string par0 = "Baseline Mean [p.e]";
  string par1 = "LY Mean [PE/keV]";
  string par2 = "Baseline Var";
  string par3 = "Rel SPE Var.";
  string par4 = "Rel LY Var.";
  string par5 = "Ba133 Amplitude";
  string par6 = "Bin Width";
  string par7 = "Constant";
  TF1* FitBa133 = new TF1("FitBa133",Ba133,start,end,npars);
  TF1* FitBa133_full = new TF1("FitBa133_full",Ba133_Full,start,end,npars);
  FitBa133->SetParNames(par0.c_str(),par1.c_str(),par2.c_str(),par3.c_str(),par4.c_str(),par5.c_str(),par6.c_str(),par7.c_str());
  FitBa133_full->SetParNames(par0.c_str(),par1.c_str(),par2.c_str(),par3.c_str(),par4.c_str(),par5.c_str(),par6.c_str(),par7.c_str());
  Double_t basel_mean       = 12;    
  Double_t ly_mean          = 7;      
  Double_t basel_var        = 133;    
  Double_t spe_var          = 1e-9;     
  Double_t ly_var           = 4.4e-4;  
  Double_t ba133ampl         = 681;//HistIntegralsum; 
  Double_t binw             = Ba133_twopulses_s1->GetBinWidth(1);
  Double_t constant         = 0;      
  double p0 = 0.266;
  double p1 = 0.492;
  double p2 = 0.000159;
  double p3 = -0.0347;
  double p4 = 0.768;
  double p5 = 0.613;
  double p6 = -0.0565;
  double p7 = -7.868e-6;
  //Fit two pulses 
  FitBa133->SetParameter(0, basel_mean);
  FitBa133->SetParameter(1, ly_mean);
  FitBa133->SetParameter(2, basel_var);
  FitBa133->SetParameter(3, spe_var);
  FitBa133->SetParameter(4, ly_var);
  FitBa133->SetParameter(5, ba133ampl);
  FitBa133->FixParameter(6, binw);
  FitBa133->SetParameter(7, constant);
  /*  FitBa133->SetParameter(8, p0);
  FitBa133->SetParameter(9, p1);
  FitBa133->SetParameter(10, p2);
  FitBa133->SetParameter(11, p3);
  FitBa133->SetParameter(12, p4);
  FitBa133->SetParameter(13, p5);
  FitBa133->SetParameter(14, p6);
  FitBa133->SetParameter(15, p7);
  */
  FitBa133->SetParLimits(0,0,5000);
  FitBa133->SetParLimits(1,0,10);
  FitBa133->SetParLimits(2,0,5000);
  // FitBa133->SetParLimits(3,0,5000);
  FitBa133->SetParLimits(4,0,1);
  FitBa133->SetParLimits(5,0,5000);
  FitBa133->SetParLimits(7,0,10); 
  /*
#ifndef Fit_Full
  //Fit Ba133 Full spectrum
  FitBa133_full->SetParameter(0, basel_mean);
  FitBa133_full->SetParameter(1, ly_mean);
  FitBa133_full->SetParameter(2, basel_var);
  FitBa133_full->SetParameter(3, spe_var);
  FitBa133_full->SetParameter(4, ly_var);
  FitBa133_full->SetParameter(5, ba133ampl);
  FitBa133_full->FixParameter(6, binw);
  FitBa133_full->SetParameter(7, constant);
  FitBa133_full->SetParLimits(0,0,5000);
  FitBa133_full->SetParLimits(1,0,10);
  FitBa133_full->SetParLimits(2,0,5000);
  // FitBa133_full->SetParLimits(3,0,5000);
  FitBa133_full->SetParLimits(4,0,1);
  FitBa133_full->SetParLimits(5,0,5000);
  FitBa133_full->SetParLimits(7,0,10); 
  FitBa133_full->SetNpx(3000);
  FitBa133_full->SetLineColor(kRed);
  Ba133_npulses_s1->Fit(FitBa133_full,"RV");
#endif
  */
  FitBa133->SetNpx(3000);
  FitBa133->SetLineColor(kRed);
  Ba133_twopulses_s1->Fit(FitBa133,"RV");

  TCanvas *c1 = new TCanvas("c1","Energy Spectrum from MC",600,400);
  Ba133_EnergyMC->Draw();
  
  TCanvas *c2 = new TCanvas("c2","Energy Spectrum with Fit",800,600);
  // Ba133_npulses_s1->Draw();
  Ba133_twopulses_s1->Draw();

  TCanvas *c3 = new TCanvas("c3","Fit Function",800,600);  
  FitBa133->Draw();

  string outdir = "/home/hqian/montecarlo/g4ds10/Linux-g++/GammaCalibration/";
  string output = outdir + "Energy_Spectrum_"+Isotope+Time+".root";
  TFile f(output.c_str(), "RECREATE");
  Ba133_EnergyMC->Write();
  Ba133_twopulses_s1->Write();
  Ba133_QuenchEnergyMC->Write();
  Ba133_npulses_s1->Write();
  FitBa133->Write();
  FitBa133_full->Write();
  c1->Write();
  c2->Write();
  c3->Write();
  f.Write();
  f.Close();

#endif
 
  return 1;
}

//// COMPUTING QUENCHING ////////////////////////////////////////////////////////////////////////////////////////////                
double scint_e_quenching (double e_keV, double* params) {       // Calculation of quenching function Q(E)                            
  double kB = params[0];
  double A1 = -0.6292-0.2181*log(kB);
  double A2 = -0.3057-0.1024*log(kB);
  double A3 = -0.0673-0.03353*log(kB);
  double A4 = 0.009876+0.002276*log(kB);
  double A5 = -0.2814-0.09964*log(kB);
  double A6 = -0.09962-0.0376*log(kB);
  // Values obtained from logarithmic fit (see kBFinder.cc)                                                                          
  // During tests used C14QuenchingParams.h                                                                                          
  // The ionization quench factor in liquid-scintillation counting standardizations                                                  
  // Malonda, Carles                                                                                                                 
  // Applied Radiation and Isotopes 51 (1999) 183-188                                                                                

  return ( (A1 + A2*log(e_keV) + A3*log(e_keV)*log(e_keV) + A4*log(e_keV)*log(e_keV)*log(e_keV))/(1 + A5*log(e_keV) + A6*log(e_keV)*log(e_keV) + A4*log(e_keV)*log(e_keV)*log(e_keV)) );

}

double e_quenching(double e_keV,double* params){
  double alpha = 0.21;
  double p0 = params[8];// 0.266;
  double p1 = params[9];//0.492;
  double p2 = params[10];//0.000159;
  double p3 = params[11];//-0.0347;
  double p4 = params[12];//0.768;
  double p5 = params[13];//0.613;
  double p6 = params[14];//-0.0565;
  double p7 = params[15];// -7.868e-6;
  double Field = 200;
  double ReE = p0*(1-p1*exp(p2*Field+p6*e_keV))*exp(p3*pow(e_keV,Field))+p5+p7*Field;
  return (ReE+0.21)/1.21;  
}

double e_quenching(double e_keV){
  double alpha = 0.21;
  double p0 = 0.266;
  double p1 = 0.492;
  double p2 = 0.000159;
  double p3 = -0.0347;
  double p4 = 0.768;
  double p5 = 0.613;
  double p6 = -0.0565;
  double p7 = -7.868e-6;
  double Field = 200;
  double ReE = p0*(1-p1*exp(p2*Field+p6*e_keV))*exp(p3*pow(e_keV,Field))+p5+p7*Field;
  return (ReE+0.21)/1.21;  
}

//// GAUSS RESPONSE FUNCTION ///////////////////////////////////////////////////////////////////////////////     
double response_function (double q, double energy, double* params) {
  double baseline_mean = params[1];
  double ly_mean = params[2];
  double baseline_var = params[3];
  double spe_var = params[4];
  double ly_var = params[5];
  //  double pe = energy*ly_mean*e_quenching(energy);
  double pe = energy*ly_mean;//*scint_e_quenching(energy,params);
  double gaus_mean = pe + baseline_mean;
  double gaus_var = baseline_var + (1 + spe_var)*pe + ly_var*pe*pe;
  double gaus_var_inv = 1.0 / gaus_var;
  double arg = -0.5*(q - gaus_mean)*(q - gaus_mean)*gaus_var_inv;
  return 0.3989422804 * sqrt(gaus_var_inv) * exp(arg);
}

double Single (double* x, double* params){
  int nBins = EnergyMC->GetNbinsX();
  double result = 0;
  double energy, spectrum;
  double q = x[0];
  for (int i=1; i<=nBins; i++)
    { 
      energy = EnergyMC->GetBinCenter(i);
      spectrum = EnergyMC->GetBinContent(i);
      result += spectrum*response_function(q,energy,params); 
    }
  //  return result*params[5]*params[6]+params[7];
   return result;
}

double Full (double* x, double* params){
  int nBins = QuenchEnergyMC->GetNbinsX();
  double result = 0;
  double energy, spectrum;
  double q = x[0];
  for (int i=1; i<=nBins; i++)
    { 
      energy = QuenchEnergyMC->GetBinCenter(i);
      spectrum = QuenchEnergyMC->GetBinContent(i);
      result += spectrum*response_function(q,energy,params);
    }
  //return result*params[5]*params[6]+params[7];
  return result;

}
