#include <sstream>
#include <vector>
#include "TF1.h"
#include "TFile.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TLatex.h"
#include "TNtuple.h"

using namespace std;
string Time = "Jan26PM";
const int start = 1097411009;
const int end = 1097411009;
int fitreal_Alpha_Capture()
{
  string inputdir="/darkside/users/hqian/AmBe10Hz_Calibration/";
  //  string inputdir = "/darkside/users/hqian/TMBfraction/results/";
  string middle = "AmBe10Hz_";
  string last = ".root";
  string outputfile;  // = inputdir+"fit"+middle+Time+last;
  // string outputfile = "fit"+middle+Time+last;
  TObjArray nv_alpha_capture_time_list(0);
  double fraction;
  double low, up, par1,par2,par3;
  TCanvas* c1 = new TCanvas("c1","alpha capture time Fit",600,400);
  TLatex tex;

  for(int i=start; i<=end; i++)
    {
      stringstream oss;
      string tag = "";
      if(i>0)
        { oss<<i;
          tag = oss.str()+"_";
        }
      string inputfile = inputdir+middle+tag+Time+last;
      outputfile = inputdir+"fit"+middle+tag+Time+last;
      cout<<"Fitting data file: "<<inputfile<<endl;
      TFile* file = new TFile(inputfile.c_str());
      //      TH1F* nv_alpha_capture_time_hist = (TH1F*) file->Get("nv_gamma_alpha_gps_hist");
      TNtuple *nv_gamma_alpha_ntuple = (TNtuple*) file->Get("nv_gamma_delay_ntuple");
      TH1F* nv_alpha_capture_time_hist = new TH1F("nv_alpha_capture_time_hist","Veto Neutron Capture Time",100,1000,5.e+4);
      nv_gamma_alpha_ntuple->Draw("time>>nv_alpha_capture_time_hist");
      gPad->SetLogy();      
      //      nv_alpha_capture_time_hist->Rebin(10);
      low = 1000.;
      up = 50000.;
      par1 = 2.e+4;
      par2 = 1.;
      par3 = 1.;
      nv_alpha_capture_time_hist->Sumw2();
      TF1* capture_time = new TF1("capture_time",time_fit,low,up,3);
      capture_time->SetParNames("Capture Time [ns]");
      capture_time->SetParameter(0,par1);
      capture_time->SetParameter(1,par2);
      capture_time->SetParameter(2,par3);
      capture_time->SetLineColor(kRed);
      nv_alpha_capture_time_hist->Fit(capture_time,"RV");
      nv_alpha_capture_time_hist->Draw();
      double time_temp = capture_time->GetParameter(0) * 1.e+6;
      fraction = 0.05;
      stringstream oss2;
      oss2<<fraction*100.;
      string hname = "alpha capture time for "+oss2.str()+"% TMB";
      tex.SetTextSize(0.05);
      tex.SetTextAlign(13);
      tex.DrawLatex(35000,2.e-3,"fit=e^{-t/capture\_time+p1}+p2");
      nv_alpha_capture_time_hist->SetTitle(hname.c_str());
      nv_alpha_capture_time_list.Add(nv_alpha_capture_time_hist);
      stringstream oss3;
      oss3<<"capture_time="<<capture_time->GetParameter(0)<<"ns";
      tex.DrawLatex(41000,1.e-3,oss3.str().c_str());
      oss.str("");
    }

  TFile f2D(outputfile.c_str(), "RECREATE");
  nv_alpha_capture_time_hist->Write();
  c1->Write();
  f2D.Write();
  f2D.Close();
  return 1;
}
 
double time_fit(double* x, double* params)
{
  double q = x[0];
  return TMath::Exp(-q/params[0]+params[1])+params[2];
}

double double_time_fit(double* x, double* params)
{
  double q = x[0];
  return TMath::Exp(-q/params[0]+params[1]) + TMath::Exp(-q/params[2]);
}
