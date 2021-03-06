#include <sstream>
#include <vector>
#include <iostream> 
#include "TFile.h"
#include "THStack.h"
#include "TTree.h"
#include "TStyle.h"
#include "TColor.h"
#include "TCanvas.h"
#include "TLatex.h"
#include "TH2F.h"
#include "TRint.h"
#include "TNtuple.h"
#include "TF1.h"
#include "TH1F.h"
#include "TLatex.h"
#include "TBox.h"

using namespace std;
string Time = "_Mar26PM";

#define save_plots

double pos1 = 150.;
double pos2 = 310.;
double pos3 = 0.;
double pos4 = 50.;

//TRint* theApp;
void Alpha_Slave_diff()
//void main()
{
  //theApp = new TRint("App",NULL,NULL,NULL,0);  
  gStyle->SetOptStat(0);
  THStack *after_ene_norm = new THStack("after_ene_norm","Veto Neutron First Energy;Energy [PE]");
  THStack *first_ene_norm = new THStack("first_ene_norm","Veto Gamma First Energy;Energy [PE]");
  THStack *prompt_ene_norm = new THStack("prompt_ene_norm","Veto Gamma Prompt Energy;Energy [PE]");
  THStack *coincidence_ene_norm = new THStack("coincidence_ene_norm","Veto Gamma Coincidence Energy;Energy [PE]");
  THStack *ene_norm = new THStack("ene_norm","Veto Gamma Energy;Energy [PE]");

  TFile* attach = new TFile("/darkside/users/hqian/AmBe10Hz_Calibration/AmBe10Hz_1097411009_slave_Mar26PM.root");
   TFile* away = new TFile("/darkside/users/hqian/AmBe10Hz_Calibration/AmBe10Hz_1113111148_slave_Mar26PM.root");
  //  TFile* away = new TFile("/darkside/users/hqian/AmBe10Hz_Calibration/AwaySelfTriggerResults/AmBe10Hz_11039_away_Feb8PM.root");
    
  //  TFile* attach = new TFile("/darkside/users/hqian/AmBe10Hz_Calibration/FermiDSTSlaveResults/AmBe10Hz_1097411009_slave_Mar28PM.root");
  //  TFile* away = new TFile("/darkside/users/hqian/AmBe10Hz_Calibration/FermiDSTSlaveResults/AmBe10Hz_1113111148_slave_Mar28PM.root");

  TH1F* attach_after_ene_hist = (TH1F*) attach->Get("nv_neutron_first_small_ene_subtracted_hist");
  TH1F* attach_first_ene_hist = (TH1F*) attach->Get("nv_gamma_first_small_ene_subtracted_hist");
  TH1F* attach_prompt_ene_hist = (TH1F*) attach->Get("nv_gamma_prompt_small_ene_subtracted_hist");
  TH1F* attach_coincidence_ene_hist = (TH1F*) attach->Get("nv_gamma_coincidence_small_ene_subtracted_hist");
  TH1F* attach_ene_hist = (TH1F*) attach->Get("nv_gamma_coincidence_ene_hist");
  TNtuple* attach_nv_ntuple = (TNtuple*) attach->Get("nv_ntuple");
  TNtuple* attach_nv_after_ntuple = (TNtuple*) attach->Get("nv_gamma_after_ntuple");

  TH1F* away_after_ene_hist = (TH1F*) away->Get("nv_neutron_first_small_ene_subtracted_hist");
  TH1F* away_first_ene_hist = (TH1F*) away->Get("nv_gamma_first_small_ene_subtracted_hist");
  TH1F* away_prompt_ene_hist = (TH1F*) away->Get("nv_gamma_prompt_small_ene_subtracted_hist");
  TH1F* away_coincidence_ene_hist = (TH1F*) away->Get("nv_gamma_coincidence_small_ene_subtracted_hist");
  TH1F* away_ene_hist = (TH1F*) away->Get("nv_gamma_coincidence_ene_hist");
  TNtuple* away_nv_ntuple = (TNtuple*) away->Get("nv_ntuple");
  TNtuple* away_nv_after_ntuple = (TNtuple*) away->Get("nv_gamma_after_ntuple");
 
  TObjArray attach_list(0);
  TObjArray away_list(0);

  //  double height = attach_after_ene_hist->GetMaximum();
  double height = 45;
  TBox *box1 = new TBox(pos3,0,pos4,height);
  box1->SetLineColor(2);
  box1->SetLineWidth(1);

  TBox *box2 = new TBox(pos1,0,pos2,height);
  box2->SetLineColor(3);
  box2->SetLineWidth(1);

  //  attach_after_ene_hist->Integral(attach_after_ene_hist->FindBin(pos3),attach_after_hist->FindBin(pos4));
  // attach_after_ene_hist->Integral(attach_after_ene_hist->FindBin(pos3),attach_after_hist->FindBin(pos2));

  //  attach_after_ene_hist->Rebin(2);
  //  attach_after_ene_hist->Sumw2(); 
  //  away_after_ene_hist->Sumw2();
  int after_bin1 = attach_after_ene_hist->FindBin(pos1);
  int after_bin2 = attach_after_ene_hist->FindBin(pos2);
  attach_after_ene_hist->SetTitle("Veto Neutron First Energy");
  away_after_ene_hist->SetTitle("Veto Neutron First Energy");
  double attach_after_ene_sum = attach_after_ene_hist->Integral(after_bin1,after_bin2);
  double away_after_ene_sum = away_after_ene_hist->Integral(after_bin1,after_bin2);
  attach_after_ene_hist->Scale(away_after_ene_sum/attach_after_ene_sum);
  //  away_after_ene_hist->Scale(attach_after_ene_sum/away_after_ene_sum);

  //  attach_first_ene_hist->Rebin(2);
  //  attach_first_ene_hist->Sumw2(); 
  //  away_first_ene_hist->Sumw2();
  int first_bin1 = attach_first_ene_hist->FindBin(pos1);
  int first_bin2 = attach_first_ene_hist->FindBin(pos2);
  double attach_first_ene_sum = attach_first_ene_hist->Integral(first_bin1,first_bin2);
  double away_first_ene_sum = away_first_ene_hist->Integral(first_bin1,first_bin2);
  attach_first_ene_hist->Scale(away_first_ene_sum/attach_first_ene_sum);
  //  away_first_ene_hist->Scale(attach_first_ene_sum/away_first_ene_sum);

  //  attach_prompt_ene_hist->Rebin(2);
  //  attach_prompt_ene_hist->Sumw2(); 
  //  away_prompt_ene_hist->Sumw2();
  int prompt_bin1 = attach_prompt_ene_hist->FindBin(pos1);
  int prompt_bin2 = attach_prompt_ene_hist->FindBin(pos2);
  double attach_prompt_ene_sum = attach_prompt_ene_hist->Integral(prompt_bin1,prompt_bin2);
  double away_prompt_ene_sum = away_prompt_ene_hist->Integral(prompt_bin1,prompt_bin2);
  attach_prompt_ene_hist->Scale(away_prompt_ene_sum/attach_prompt_ene_sum);
  //  away_prompt_ene_hist->Scale(attach_prompt_ene_sum/away_prompt_ene_sum);

  //  attach_coincidence_ene_hist->Rebin(2);
  //  attach_coincidence_ene_hist->Sumw2(); 
  //  away_coincidence_ene_hist->Sumw2();
  int coincidence_bin1 = attach_coincidence_ene_hist->FindBin(pos1);
  int coincidence_bin2 = attach_coincidence_ene_hist->FindBin(pos2);
  double attach_coincidence_ene_sum = attach_coincidence_ene_hist->Integral(coincidence_bin1,coincidence_bin2);
  double away_coincidence_ene_sum = away_coincidence_ene_hist->Integral(coincidence_bin1,coincidence_bin2);
  attach_coincidence_ene_hist->Scale(away_coincidence_ene_sum/attach_coincidence_ene_sum);
  //  away_coincidence_ene_hist->Scale(attach_coincidence_ene_sum/away_coincidence_ene_sum);

  //  attach_ene_hist->Rebin(2);
  //  attach_ene_hist->Sumw2(); 
  //  away_ene_hist->Sumw2();
  int ene_bin1 = attach_ene_hist->FindBin(pos1);
  int ene_bin2 = attach_ene_hist->FindBin(pos2);
  double attach_ene_sum = attach_ene_hist->Integral(ene_bin1,ene_bin2);
  double away_ene_sum = away_ene_hist->Integral(ene_bin1,ene_bin2);
  attach_ene_hist->Scale(away_ene_sum/attach_ene_sum);
  //  away_ene_hist->Scale(attach_ene_sum/away_ene_sum);

  TH1F* attach_time_hist = new TH1F("attach_time_hist","Attach Veto Neutron Time",1000,0,14.e+4);  
  TH1F* away_time_hist = new TH1F("away_time_hist","Away Veto Neutron Time",1000,0,14.e+4);  
  attach_nv_ntuple->Draw("time>>attach_time_hist");
  away_nv_ntuple->Draw("time>>away_time_hist");

  TH1F* attach_capture_time_hist = new TH1F("attach_capture_time_hist","Attach Veto Neutron Capture Time",100,2.e+4,6.e+4);  
  TH1F* away_capture_time_hist = new TH1F("away_capture_time_hist","Away Veto Neutron Capture Time",100,2.e+4,6.e+4);  
  attach_nv_after_ntuple->Draw("time>>attach_capture_time_hist");
  away_nv_after_ntuple->Draw("time>>away_capture_time_hist");
  attach_capture_time_hist->Sumw2();
  away_capture_time_hist->Sumw2();
  
  double low = 25000.;
  double  up = 60000.;
  double par1 = 2.e+4;
  double par2 = 1.;
  double par3 = 1.;
  TLatex tex;
  TF1* attach_capture_time = new TF1("attach_capture_time",attach_time_fit,low,up,3);
  attach_capture_time->SetParNames("Capture Time [ns]");
  attach_capture_time->SetParameter(0,par1);
  attach_capture_time->SetParameter(1,par2);
  attach_capture_time->SetParameter(2,par3);
  attach_capture_time->SetLineColor(kRed);
  attach_capture_time_hist->Fit(attach_capture_time,"RV");
  double attach_time_temp = attach_capture_time->GetParameter(0) * 1.e+3;
  stringstream oss3;
  oss3<<"capture_time="<<attach_time_temp<<"us";
  tex.DrawLatex(41000,1.e-3,oss3.str().c_str());
  oss3.str("");

  TF1* away_capture_time = new TF1("away_capture_time",away_time_fit,low,up,3);
  away_capture_time->SetParNames("Capture Time [ns]");
  away_capture_time->SetParameter(0,par1);
  away_capture_time->SetParameter(1,par2);
  away_capture_time->SetParameter(2,par3);
  away_capture_time->SetLineColor(kRed);
  away_capture_time_hist->Fit(away_capture_time,"RV");
  double away_time_temp = away_capture_time->GetParameter(0) * 1.e+3;
  stringstream oss4;
  oss4<<"capture_time="<<away_time_temp<<"us";
  tex.DrawLatex(41000,1.e-3,oss4.str().c_str());
  oss4.str("");

  //  nv_first_qenergy_hist->SetLineWidth(3);
  vector<int> linecolor = Colors();
  attach_after_ene_hist->SetLineColor(linecolor.at(0));
  attach_first_ene_hist->SetLineColor(linecolor.at(0));
  attach_prompt_ene_hist->SetLineColor(linecolor.at(0));
  attach_coincidence_ene_hist->SetLineColor(linecolor.at(0));
  attach_ene_hist->SetLineColor(linecolor.at(0));
  attach_time_hist->SetLineColor(linecolor.at(0));

  away_after_ene_hist->SetLineColor(linecolor.at(1));
  away_first_ene_hist->SetLineColor(linecolor.at(1));
  away_prompt_ene_hist->SetLineColor(linecolor.at(1));
  away_coincidence_ene_hist->SetLineColor(linecolor.at(1));
  away_ene_hist->SetLineColor(linecolor.at(1));
  away_time_hist->SetLineColor(linecolor.at(1));

  after_ene_norm->Add(attach_after_ene_hist);
  first_ene_norm->Add(attach_first_ene_hist);
  prompt_ene_norm->Add(attach_prompt_ene_hist);
  coincidence_ene_norm->Add(attach_coincidence_ene_hist);
  ene_norm->Add(attach_ene_hist);

  after_ene_norm->Add(away_after_ene_hist);
  first_ene_norm->Add(away_first_ene_hist);
  prompt_ene_norm->Add(away_prompt_ene_hist);
  coincidence_ene_norm->Add(away_coincidence_ene_hist);
  ene_norm->Add(away_ene_hist);

  attach_list.Add(attach_after_ene_hist);
  attach_list.Add(attach_first_ene_hist);
  attach_list.Add(attach_prompt_ene_hist);
  attach_list.Add(attach_coincidence_ene_hist);
  attach_list.Add(attach_ene_hist);
  attach_list.Add(attach_time_hist);

  away_list.Add(away_after_ene_hist);
  away_list.Add(away_first_ene_hist);
  away_list.Add(away_prompt_ene_hist);
  away_list.Add(away_coincidence_ene_hist);
  away_list.Add(away_ene_hist);
  away_list.Add(away_time_hist);

  vector<double> attach_ratio, away_ratio;
  Branch_Ratio(attach_list,attach_ratio);
  Branch_Ratio(away_list,away_ratio);
  for(size_t i=0; i<attach_ratio.size(); i++)
    {
      cout<<"ratio "<<i<<"  "<<attach_ratio.at(i)<<"\t"<<away_ratio.at(i)<<endl;
    }
  
  //  string entryname[2] = {"attach to cryotat","away from cryostat"}; 
  string entryname[2] = {"attach to cryotat,0.6g/L PPO","attch to cryostat,1.5g/L PPO"}; 
  TLegend *leg = new TLegend(0.47,0.7,0.87,0.9);
  leg->AddEntry(attach_after_ene_hist,entryname[0].c_str(),"l");
  leg->AddEntry(away_after_ene_hist,entryname[1].c_str(),"l");
  
  TCanvas* c1 = new TCanvas("c1","After Energy",600,400);
  //  gPad->SetLogy();
  after_ene_norm->Draw("nostack");
  leg->Draw();
  box1->Draw();
  box2->Draw();

  TCanvas* c2 = new TCanvas("c2","First Energy",600,400);
  //  gPad->SetLogy();
  first_ene_norm->Draw("nostack");
  leg->Draw();
  box1->Draw();
  box2->Draw();

  TCanvas* c3 = new TCanvas("c3","Prompt Energy",600,400);
  //  gPad->SetLogy();
  prompt_ene_norm->Draw("nostack");
  leg->Draw();
  box1->Draw();
  box2->Draw();

  TCanvas* c4 = new TCanvas("c4","Coincidence Energy",600,400);
  //  gPad->SetLogy();
  coincidence_ene_norm->Draw("nostack");
  leg->Draw();
  box1->Draw();
  box2->Draw();

  TCanvas* c5 = new TCanvas("c5","Veto Energy",600,400);
  gPad->SetLogy();
  ene_norm->Draw("nostack");
  leg->Draw();

  TCanvas* c6 = new TCanvas("c6","Attach Time",600,400);
  gPad->SetLogy();
  attach_time_hist->Draw();

  TCanvas* c7 = new TCanvas("c7","Away Time",600,400);
  gPad->SetLogy();
  away_time_hist->Draw();

  TCanvas* c8 = new TCanvas("c8","Attach Capture Time",600,400);
  gPad->SetLogy();
  attach_capture_time_hist->Draw();

  TCanvas* c9 = new TCanvas("c9","Away Capture Time",600,400);
  gPad->SetLogy();
  away_capture_time_hist->Draw();
  
     
  string plotdir="/darkside/users/hqian/AmBe10Hz_Calibration/Plots/";
  string ext = "png";
#ifdef save_plots
  c1->SaveAs(Form("%safter_ene%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c2->SaveAs(Form("%sfirst_ene%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c3->SaveAs(Form("%sprompt_ene%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c4->SaveAs(Form("%scoincidence_ene%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c5->SaveAs(Form("%sene%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c6->SaveAs(Form("%sattach_time%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c7->SaveAs(Form("%saway_time%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c8->SaveAs(Form("%sattach_capture_time%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c9->SaveAs(Form("%saway_capture_time%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));

#endif

  string dirname="/darkside/users/hqian/AmBe10Hz_Calibration/";
  string output1=dirname+"SlavediffAlpha"+Time+".root"; 
  TFile out1(output1.c_str(),"RECREATE");
  attach_list.Write();
  away_list.Write();
  
  c1->Write();
  c2->Write();
  c3->Write();
  c4->Write();
  c5->Write();
  c6->Write();
  c7->Write();
  c8->Write();
  c9->Write();

  out1.Write();
  out1.Close();
}

double attach_time_fit(double* x, double* params)
{
  double q = x[0];
  return TMath::Exp(-q/params[0]+params[1])+params[2];
}

double away_time_fit(double* x, double* params)
{
  double q = x[0];
  return TMath::Exp(-q/params[0]+params[1])+params[2];
}

double double_time_fit(double* x, double* params)
{
  double q = x[0];
  return TMath::Exp(-q/params[0]+params[1]) + TMath::Exp(-q/params[2]);
}

string Readdatafile(int i)
{
  string dirname="/darkside/users/hqian/TMBfraction/results/";
  string middle="tmb_alpha";
  string last=".root";
  string filename;
  stringstream oss;
  if(i==0)
    filename=dirname+middle+Time+last;
  else {
    oss<<i;
    filename=dirname+middle+"_v"+oss.str()+Time+last;
    oss.str("");
  }
  cout<<"Processing Data file: "<<filename<<endl;
  return filename;
}

void Branch_Ratio(TObjArray Hlist, vector<double> &ratio)
{
  for(int i=0; i<= Hlist.GetLast(); i++)
    {
      temphist = dynamic_cast<TH1F*>(Hlist.At(i));
  double br1 = temphist->Integral(temphist->FindBin(pos3),temphist->FindBin(pos4));
  double br2 = temphist->Integral(temphist->FindBin(pos3),temphist->FindBin(pos2));
  ratio.push_back(1.0*br1/br2);
}

}

vector<int> Colors()
{
  vector<int> color;
  color.push_back(TColor::GetColor("#5A1DE8"));
  color.push_back(TColor::GetColor("#B26618"));
  color.push_back(TColor::GetColor("#FB78FF"));
  color.push_back(TColor::GetColor("#AE3EFF"));
  color.push_back(TColor::GetColor("#B24F18"));
  color.push_back(TColor::GetColor("#2FDAFF"));      
  color.push_back(TColor::GetColor("#FF0000"));
  color.push_back(TColor::GetColor("#5A1DE8"));
  color.push_back(TColor::GetColor("#1CFFDF"));
  color.push_back(TColor::GetColor("#1D537F"));
  color.push_back(TColor::GetColor("#000000"));
  color.push_back(TColor::GetColor("#4A067F"));
  color.push_back(TColor::GetColor("#FF379F"));
  color.push_back(TColor::GetColor("#25DCFF"));
  color.push_back(TColor::GetColor("#92B20B"));
  color.push_back(TColor::GetColor("#59FF49"));
  color.push_back(TColor::GetColor("#E8A60C"));
  color.push_back(TColor::GetColor("#79FFFF"));
  color.push_back(TColor::GetColor("#11FF8F"));
  color.push_back(TColor::GetColor("#09B23E"));
  color.push_back(TColor::GetColor("#62B21D"));
  
  return color;
}


  /* 

  THStack *nv_remain_time_norm = new THStack("nv_remain_time_norm","Remain Time;Time [ns]");
  THStack *nv_thermal_time_norm = new THStack("nv_thermal_time_norm","Thermal Time;Time [ns]");
  THStack *nv_residual_time_norm = new THStack("nv_residual_time_norm","Residual Time;Time [ns]");
  THStack *nv_first_time_norm = new THStack("nv_first_time_norm","First Time;Time [ns]");

  THStack *nv_remain_qenergy_norm = new THStack("nv_remain_qenergy_norm","Remain Quench Energy;Energy [PE]");
  THStack *nv_thermal_qenergy_norm = new THStack("nv_thermal_qenergy_norm","Thermal Quench Energy;Energy [PE]");
  THStack *nv_residual_qenergy_norm = new THStack("nv_residual_qenergy_norm","Residual Quench Energy;Energy [PE]");
  THStack *nv_first_qenergy_norm = new THStack("nv_first_qenergy_norm","First Quench Energy;Energy [PE]");



 THStack *nv_pdf_fine_hist_norm = new THStack("nv_pdf_fine_hist_norm","Cumulative distribution function for [-100ns,500ns] window;Energy [keVee];Probability");
  THStack *nv_ER_energy_hist_norm = new THStack("nv_ER_energy_hist_norm","Energy Spectrum for [1.5us,1ms] window;Energy [keVee]");
  THStack *nv_ER_pdf_hist_norm = new THStack("nv_ER_pdf_hist_norm","Cumulative distribution function for [1.5us,1ms] window;Energy [keVee];Probability");
  THStack *nv_ER_pdf_fine_hist_norm = new THStack("nv_ER_pdf_fine_hist_norm","Cumulative distribution function for [1.5us,1ms] window;Energy [keVee];Probability");  
  TObjArray nv_ER_pdf_list(0);
  TObjArray nv_ER_pdf_fine_list(0);
  TH1F* nv_pdf_fine_hist = (TH1F*) t->Get("nv_pdf_fine_hist");
  TH1F* nv_ER_energy_hist = (TH1F*) t->Get("nv_ER_energy_hist");
  TH1F* nv_ER_pdf_hist = (TH1F*) t->Get("nv_ER_pdf_hist");
  TH1F* nv_ER_pdf_fine_hist = (TH1F*) t->Get("nv_ER_pdf_fine_hist");        
  nv_pdf_hist->SetLineColor(color);
  nv_pdf_fine_hist->SetLineColor(color);
  nv_ER_pdf_hist->SetLineColor(color);
  nv_ER_pdf_fine_hist->SetLineColor(color);
  nv_ER_energy_hist->SetLineColor(color);      
  nv_ER_energy_hist_norm->Add(nv_ER_energy_hist);
  nv_pdf_hist_norm->Add(nv_pdf_hist);
  nv_pdf_fine_hist_norm->Add(nv_pdf_fine_hist);
  nv_ER_pdf_hist_norm->Add(nv_ER_pdf_hist);
  nv_ER_pdf_fine_hist_norm->Add(nv_ER_pdf_fine_hist);
  nv_pdf_fine_list.Add(nv_pdf_fine_hist);
  nv_ER_pdf_list.Add(nv_ER_pdf_hist);
  nv_ER_pdf_fine_list.Add(nv_ER_pdf_fine_hist);
  TCanvas* c3 = new TCanvas("c3","pdf",600,400);
  nv_pdf_fine_hist_norm->Draw("nostack");
  leg->Draw();

  TCanvas* c4 = new TCanvas("c4","ER pdf",600,400);
  nv_ER_pdf_fine_hist_norm->Draw("nostack");
  leg2->Draw();
//  L2->Draw();

  TCanvas* c5 = new TCanvas("c5","energy",600,400);
  nv_energy_hist_norm->Draw("nostack");
  leg->Draw();

  TCanvas* c6 = new TCanvas("c6","energy",600,400);
  nv_ER_energy_hist_norm->Draw("nostack");
  leg2->Draw();

  TCanvas* c7 = new TCanvas("c7","Total",1200,800);
  c7->Divide(2,2);
  c7->cd(1);
  gPad->SetLogy();
  nv_energy_hist_norm->Draw("nostack");
  leg->Draw();
  c7->cd(2);
  gPad->SetLogy();
  nv_ER_energy_hist_norm->Draw("nostack");
  leg2->Draw();
  c7->cd(3);
  gPad->SetLogy();
  nv_pdf_hist_norm->Draw("nostack");
  leg->Draw();
  c7->cd(4);
  gPad->SetLogy();
  nv_ER_pdf_hist_norm->Draw("nostack");
  leg2->Draw();  

  TCanvas* c11 = new TCanvas("c11","remain time",1000,600);
  c11->Divide(2,2);
  TCanvas* c12 = new TCanvas("c12","thermal time",1000,600);
  c12->Divide(2,2);
  TCanvas* c13 = new TCanvas("c13","residual time",1000,600);
  c13->Divide(2,2);
  TCanvas* c14 = new TCanvas("c14","first time",1000,600);
  c14->Divide(2,2);

  TCanvas* c20 = new TCanvas("c20","NV last alphaEnergy",1000,600);
  c20->Divide(2,2);

  TCanvas* c21 = new TCanvas("c21","remain energy",1000,600);
  c21->Divide(2,2);
  TCanvas* c22 = new TCanvas("c22","thermal energy",1000,600);
  c22->Divide(2,2);
  TCanvas* c23 = new TCanvas("c23","residual energy",1000,600);
  c23->Divide(2,2);
  TCanvas* c24 = new TCanvas("c24","first energy",1000,600);
  c24->Divide(2,2);

  TCanvas* c31 = new TCanvas("c31","remain time vs energy",1000,600);
  c31->Divide(2,2);
  TCanvas* c32 = new TCanvas("c32","thermal time vs energy",1000,600);
  c32->Divide(2,2);
  TCanvas* c33 = new TCanvas("c33","residual time vs energy",1000,600);
  c33->Divide(2,2);
  TCanvas* c34 = new TCanvas("c34","first time vs energy",1000,600);
  c34->Divide(2,2);

  TObjArray tpc_gamma_energy_list(0);
  TObjArray nv_remain_time_list(0);
  TObjArray nv_thermal_time_list(0);
  TObjArray nv_residual_time_list(0);
  TObjArray nv_first_time_list(0);
  TObjArray nv_remain_qenergy_list(0);
  TObjArray nv_thermal_qenergy_list(0);
  TObjArray nv_residual_qenergy_list(0);

  nv_residual_time_hist->Rebin(4);
  nv_alpha_qenergy_hist->Rebin(10);
  nv_residual_qenergy_hist->Rebin(4);
  nv_first_time_hist->Rebin(4);

  nv_alpha_qenergy_hist->SetLineColor(color);
  nv_remain_time_hist->SetLineColor(color);
  nv_thermal_time_hist->SetLineColor(color);
  nv_residual_time_hist->SetLineColor(color);
  nv_first_time_hist->SetLineColor(color);
  
  nv_remain_qenergy_hist->SetLineColor(color);
  nv_thermal_qenergy_hist->SetLineColor(color);
  nv_residual_qenergy_hist->SetLineColor(color);
  nv_first_qenergy_hist->SetLineColor(color);
  
  tpc_gamma_energy_hist->SetLineWidth(3);
  nv_alpha_qenergy_hist->SetLineWidth(3);
  nv_remain_time_hist->SetLineWidth(3);
  nv_thermal_time_hist->SetLineWidth(3);
  nv_residual_time_hist->SetLineWidth(3);
  nv_first_time_hist->SetLineWidth(3);
  
  nv_remain_qenergy_hist->SetLineWidth(3);
  nv_thermal_qenergy_hist->SetLineWidth(3);
  nv_residual_qenergy_hist->SetLineWidth(3);


      c11->cd(i+1);
      gPad->SetLogy();
      nv_remain_time_hist->Draw();
      leg1->Draw();

      c12->cd(i+1);
      gPad->SetLogy();
      nv_thermal_time_hist->Draw();
      leg1->Draw();

      c13->cd(i+1);
      gPad->SetLogy();
      nv_residual_time_hist->Draw();
      leg1->Draw();

      c14->cd(i+1);
      gPad->SetLogy();
      nv_first_time_hist->Draw();
      leg1->Draw();

      c20->cd(i+1);
      gPad->SetLogy();
      nv_alpha_qenergy_hist->Draw();
      leg1->Draw();

      c21->cd(i+1);
      gPad->SetLogy();
      nv_remain_qenergy_hist->Draw();
      leg1->Draw();

      c22->cd(i+1);
      gPad->SetLogy();
      nv_thermal_qenergy_hist->Draw();
      leg1->Draw();

      c23->cd(i+1);
      gPad->SetLogy();
      nv_residual_qenergy_hist->Draw();
      leg1->Draw();

      c24->cd(i+1);
      gPad->SetLogy();
      nv_first_qenergy_hist->Draw();
      leg1->Draw();

      c32->cd(i+1);
      //      gPad->SetLogy();
      nv_thermal_eof_qenergy_hist->Draw("colz");
      leg1->Draw();

      c33->cd(i+1);
      gPad->SetLogy();
      nv_residual_eof_qenergy_hist->Draw("colz");
      leg1->Draw();

      c34->cd(i+1);
      gPad->SetLogy();
      nv_first_eof_qenergy_hist->Draw("colz");
      leg1->Draw();

      tpc_gamma_energy_norm->Add(tpc_gamma_energy_hist);
      nv_remain_time_norm->Add(nv_remain_time_hist);
      nv_thermal_time_norm->Add(nv_thermal_time_hist);
      nv_residual_time_norm->Add(nv_residual_time_hist);
      nv_first_time_norm->Add(nv_first_time_hist);

      nv_alpha_qenergy_norm->Add(nv_alpha_qenergy_hist);
      nv_remain_qenergy_norm->Add(nv_remain_qenergy_hist);
      nv_thermal_qenergy_norm->Add(nv_thermal_qenergy_hist);
      nv_residual_qenergy_norm->Add(nv_residual_qenergy_hist);


#ifdef Obj
      tpc_gamma_energy_list.Add(tpc_gamma_energy_hist);
      nv_remain_time_list.Add(nv_remain_time_hist);
      nv_thermal_time_list.Add(nv_thermal_time_hist);
      nv_residual_time_list.Add(nv_residual_time_hist);
      nv_first_time_list.Add(nv_first_time_hist);

      nv_remain_qenergy_list.Add(nv_remain_qenergy_hist);
      nv_thermal_qenergy_list.Add(nv_thermal_qenergy_hist);
      nv_residual_qenergy_list.Add(nv_residual_qenergy_hist);

#endif
      i=i+3;
    }
    c31->Close();
  c32->Close();
  c33->Close();
  c34->Close();

  c10->cd(4);
  gPad->SetLogy();
  tpc_gamma_energy_norm->Draw("nostack");
  leg->Draw();

  c11->cd(4);
  gPad->SetLogy();
  nv_remain_time_norm->Draw("nostack");
  leg->Draw();

  c12->cd(4);
  gPad->SetLogy();
  nv_thermal_time_norm->Draw("nostack");
  leg->Draw();
  
  c13->cd(4);
  gPad->SetLogy();
  nv_residual_time_norm->Draw("nostack");
  leg->Draw();
  
  c14->cd(4);
  gPad->SetLogy();
  nv_first_time_norm->Draw("nostack");
  leg->Draw();

  c20->cd(4);
  gPad->SetLogy();
  nv_alpha_qenergy_norm->Draw("nostack");
  leg->Draw();
  
  c21->cd(4);
  gPad->SetLogy();
  nv_remain_qenergy_norm->Draw("nostack");
  leg->Draw();

  c22->cd(4);
  gPad->SetLogy();
  nv_thermal_qenergy_norm->Draw("nostack");
  leg->Draw();
  
  c23->cd(4);
  gPad->SetLogy();
  nv_residual_qenergy_norm->Draw("nostack");
  leg->Draw();

  /*  TCanvas* c1 = new TCanvas("c1","",600,400);
  c1->SetLogy();
  nv_remain_time_norm->Draw("nostack");
  leg->Draw();

  TCanvas* c2 = new TCanvas("c2","",600,400);
  c2->SetLogy();
  nv_residual_time_norm->Draw("nostack");
  leg->Draw();

  TCanvas* c3 = new TCanvas("c3","",600,400);
  c3->SetLogy();
  nv_first_time_norm->Draw("nostack");
  leg->Draw();

void Calculate_Probability(TH1F* nv_pdf_hist, string label, const int N, double ene[], double* prob) 
{
  for(int i=0; i<N; i++)
    {
      int bin =  nv_pdf_hist->GetXaxis()->FindBin(ene[i]);
      prob[i] = nv_pdf_hist->GetBinContent(bin);
      cout<<"For "<<label<<", Probability corresponding to Energy "<<ene[i]<<" is "<<prob[i]<<endl;
    }
}

void Calculate_Probability(TH1F* nv_pdf_hist, double fraction, const int N, double ene[]) 
{
  for(int i=0; i<N; i++)
    {
      int bin =  nv_pdf_hist->GetXaxis()->FindBin(ene[i]);
      double prob = nv_pdf_hist->GetBinContent(bin);
      cout<<"For TMB concentration "<<fraction<<", Probability corresponding to Energy "<<ene[i]<<" is "<<prob*100.<<endl;
    }
}

  c10->SaveAs(Form("tpc_gamma_energy_list_Jan22PM.png"));
  c11->SaveAs(Form("nv_remain_time_list_Jan22PM.png"));
  c12->SaveAs(Form("nv_thermal_time_list_Jan22PM.png"));
  c13->SaveAs(Form("nv_residual_time_list_Jan22PM.png"));
  c14->SaveAs(Form("nv_first_time_list_Jan22PM.png"));
  c20->SaveAs(Form("nv_alpha_qenergy_list_Jan22PM.png"));
  c21->SaveAs(Form("nv_remain_qenergy_list_Jan22PM.png"));
  c22->SaveAs(Form("nv_thermal_qenergy_list_Jan22PM.png"));
  c23->SaveAs(Form("nv_residual_qenergy_list_Jan22PM.png"));
  c24->SaveAs(Form("nv_first_qenergy_list_Jan22PM.png"));
  c31->SaveAs(Form("nv_remain_eof_qenergy_list_Jan22PM.png"));
  c32->SaveAs(Form("nv_thermal_eof_qenergy_list_Jan22PM.png"));
  c33->SaveAs(Form("nv_residual_eof_qenergy_list_Jan22PM.png"));
  c34->SaveAs(Form("nv_first_eof_qenergy_list_Jan22PM.png"));
  */
